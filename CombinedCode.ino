#include <EEPROM.h>
#include <Arduino.h>
#include <Wire.h>
#include "GravityTDS.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#include <SD.h>
#include <SPI.h>
#include <Adafruit_GPS.h>
#include <SoftwareSerial.h>

#define ONE_WIRE_BUS 2
#define TdsSensorPin A1
#define pHSensorPin A0
#define temperatureSensorPin = 2;

GravityTDS gravityTds;

float significantchangeid;
 
unsigned char data[4]={};
char g;

float temperatureCelsius;
float temperatureFahrenheit;
float temperature = temperatureCelsius, tdsValue = 0;
float Ec;
float ph_calibration_value = 21.34;
unsigned long int avgval; 
int buffer_arr[10],temp;

int pressurepin = A8;
int soundsensor = 6;

bool significantchange = false;

const float temp_threshold = 10;
const float tds_threshold = 50;
const float ph_threshold = 1;
const float turbidity_threshold = 50;

float prevTemp = 0;
float prevTds = 0;
float prevPh = 0;
float prevTurbidity = 0;

float distance;
float tempchange = 0;
float tdschange = 0;
float phchange = 0;
float turbiditychange = 0;

const int motorpin1 = 40;
const int motorpin2 = 41;
const int motorpin3 = 42;
const int motorpin4 = 43;
const int motorpin5 = 44;
const int motorpin6 = 45;
const int motorpin7 = 46;
const int motorpin8 = 47;
const int motorpin9 = 48;
const int motorpin10 = 49;
const int rotormotorpin = 50;

const int lightpin = A4;

char logreason;

int chipSelect= 53;
File mySensorData;

SoftwareSerial mySerial2(11,10); // RX, TX 
SoftwareSerial mySerial(3, 2);
Adafruit_GPS GPS(&mySerial);

OneWire oneWire(temperatureSensorPin);
DallasTemperature temperatureSensor(&oneWire);

void setup() {
  gravityTds.setPin(TdsSensorPin);
  gravityTds.setAref(5.0);
  gravityTds.setAdcRange(1024);
  gravityTds.begin(); 
  pinMode(10,OUTPUT);
  SD.begin(chipSelect);
  pinMode(soundsensor, INPUT);
  temperatureSensor.begin();
  pinMode(motorpin1, OUTPUT);
  pinMode(motorpin2, OUTPUT);
  pinMode(motorpin3, OUTPUT);
  pinMode(motorpin4, OUTPUT);
  pinMode(motorpin5, OUTPUT);
  pinMode(motorpin6, OUTPUT);
  pinMode(motorpin7, OUTPUT);
  pinMode(motorpin8, OUTPUT);
  pinMode(motorpin9, OUTPUT);
  pinMode(motorpin10, OUTPUT);
  pinMode(rotormotorpin, OUTPUT);
  significantchangeid = 1;
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
}

void loop() {  
  int light = analogRead(lightpin);
  int pressuresensorValue = analogRead(pressurepin);
  float voltage = pressuresensorValue * (5.0 / 1023.0);
  float baselinevoltage = 0;
  float pressure = (voltage - baselinevoltage) * (100/ 4.5 - baselinevoltage);
  for(int a=0;a<4;a++){
       data[a]=mySerial2.read();
  }
  while(mySerial2.read()==0xff);
    mySerial2.flush();
    if(data[0]==0xff){
      int sum;
      sum=(data[0]+data[1]+data[2])&0x00FF;
      if(sum==data[3]){
        distance=(data[1]<<8)+data[2];
        distance=distance / 10;
      }
    }
  mySensorData = SD.open("test.txt",FILE_WRITE);
  gravityTds.setTemperature(temperature);
  gravityTds.update();
  tdsValue = gravityTds.getTdsValue();
  Ec = (tdsValue * 2)/ 1000;
  for(int b=0;b<10;b++){ 
    buffer_arr[b]=pHSensorPin(A0);
  }
  for(int c=0;c<9;c++){
    for(int d=c+1;d<10;d++){
      if(buffer_arr[c]>buffer_arr[d]){
      temp=buffer_arr[c];
      buffer_arr[c]=buffer_arr[d];
      buffer_arr[d]=temp;
      }
    }
  }
  avgval=0;
  for(int e=2;e<8;e++){
    avgval+=buffer_arr[e];
  }
  float volt=(float)avgval*5.0/1024/6;
  float ph = -5.70 * volt + ph_calibration_value;
  temperatureSensor.requestTemperatures();
  temperatureCelsius = sensors.getTempCByIndex(0);
  temperatureFarenheit = sensors.getTempFByIndex(0);
  int sensorValue = analogRead(A2);
  int turbidity =map(sensorValue,0,700,100,0);  
  int data = digitalRead(soundsensor);
  if(mySensorData){
    mySensorData.println("1");
    mySensorData.print(tdsValue,0);
    mySensorData.println(",");
    mySensorData.print(ph);
    mySensorData.println(",");
    mySensorData.print(temperatureCelsius);
    mySensorData.println(",");
    mySensorData.print(temperatureFarenheit);
    mySensorData.println(",");
    mySensorData.print(turbidity);
    mySensorData.println(",");
    mySensorData.print(Ec);
    mySensorData.print("Distance from riverbed : ");
    mySensorData.print(distance);
    mySensorData.print("cm");
    mySensorData.print("Sound (decibels)");
    mySensorData.print(data);
    mySensorData.print("Pressure : ");
    mySensorData.print(pressure);
    mySensorData.print("Light : ");
    mySensorData.print(light);
    clearGPS();
    while (!GPS.newNMEAreceived()) {
      g = GPS.read();
    }
    GPS.parse(GPS.lastNMEA());
    mySensorData.print("Time: ");
    mySensorData.print(GPS.hour, DEC);
    mySensorData.print(':');
    mySensorData.print(GPS.minute, DEC);
    mySensorData.print(':');
    mySensorData.print(GPS.seconds, DEC);
    mySensorData.print('.');
    mySensorData.println(GPS.milliseconds);
    mySensorData.print("Date: ");
    mySensorData.print(GPS.day, DEC);
    mySensorData.print('/');
    mySensorData.print(GPS.month, DEC);
    mySensorData.print("/20");
    mySensorData.println(GPS.year, DEC);
    mySensorData.print("Fix: ");
    mySensorData.print(GPS.fix);
    mySensorData.print(" quality: ");
    mySensorData.println(GPS.fixquality);
    mySensorData.print("Satellites: ");
    mySensorData.println(GPS.satellites);
    if (GPS.fix) {
      mySensorData.print("Location: ");
      mySensorData.print(GPS.latitude, 4);
      mySensorData.print(GPS.lat);
      mySensorData.print(", ");
      mySensorData.print(GPS.longitude, 4);
      mySensorData.println(GPS.lon);
      mySensorData.print("Google Maps location: ");
      mySensorData.print(GPS.latitudeDegrees, 4);
      mySensorData.print(", ");
      mySensorData.println(GPS.longitudeDegrees, 4);
      mySensorData.print("Speed (knots): ");
      mySensorData.println(GPS.speed);
      mySensorData.print("Heading: ");
      mySensorData.println(GPS.angle);
      mySensorData.print("Altitude: ");
      mySensorData.println(GPS.altitude);
    }
    mySensorData.println("-------------------------------------");

  tempchange = temperatureCelsius - prevTemp;
  tdschange = tdsValue - prevTds;
  phchange = ph - prevPh;
  turbiditychange = turbidity - prevTurbidity;
  if (abs(tempchange) > temp_threshold){
    logreason = "Temperature";
    significantchange = true;
  }
  if (abs(tdschange) > tds_threshold){
    logreason = "TDS";
    significantchange = true;
  }
  if (abs(phchange) > ph_threshold){
    logreason = "PH";
    significantchange = true;
  }
  if (abs(turbiditychange) > turbidity_threshold){
    logreason = "Turbidity";
    significantchange = true;
  }
  if (significantchange){
    for (int f=1; f<11; f++){
      if (significantchangeid == f){
        significantchange = false;
        digitalWrite(motorpin1 + f, HIGH);
        delay(15000);
        mySensorData.println("Significant change, ");
        mySensorData.println("id : -");
        mySensorData.print(significantchangeid);
        mySensorData.print("reason : ");
        mySensorData.print(logreason);
        mySensorData.print("Change : ");
        if (logreason == "Temperature"){
          mySensorData.print(tempchange);
          mySensorData.print("Previous : ");
          mySensorData.print(prevTemp);
          mySensorData.print("New : ");
          mySensorData.print(temperatureCelsius);
        }
        if (logreason == "TDS"){
          mySensorData.print(tdschange);
          mySensorData.print("Previous : ");
          mySensorData.print(prevTds);
          mySensorData.print("New : ");
          mySensorData.print(tdsValue);
        }
        if (logreason == "PH"){
          mySensorData.print(phchange);
          mySensorData.print("Previous : ");
          mySensorData.print(prevPh);
          mySensorData.print("New : ");
          mySensorData.print(ph);
        }
        if (logreason == "Turbidity"){
          mySensorData.print(turbiditychange);
          mySensorData.print("Previous : ");
          mySensorData.print(prevTurbidity);
          mySensorData.print("New : ");
          mySensorData.print(turbidity);
        }
        mySensorData.print("Sample id: ");
        mySensorData.print(significantchangeid);
        digitalWrite(motorpin1 + f, LOW);
        significantchangeid = significantchangeid + 1;
        digitalWrite(rotormotorpin, HIGH);
        delay(2500);
        digitalWrite(rotormotorpin, LOW);
        prevTemp = temperatureCelsius;
        prevTds = tdsValue;
        prevPh = ph;
        prevTurbidity = turbidity;
        break;
      }
    }
    if (significantchangeid == 11){
      significantchange = false;
    }
    else {
      mySensorData.print("ERROR 404 : Autonomous Sampling error.");
    }
  }
}
delay(10);
}

void clearGPS() {
  while (!GPS.newNMEAreceived()) {
    g = GPS.read();
  }
  GPS.parse(GPS.lastNMEA());

  while (!GPS.newNMEAreceived()) {
    g = GPS.read();
  }
  GPS.parse(GPS.lastNMEA());
}
