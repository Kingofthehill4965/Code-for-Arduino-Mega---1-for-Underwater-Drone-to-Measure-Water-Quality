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
GravityTDS gravityTds;

float significantchangeid;
 
unsigned char data[4]={};
char c;

float tempC; 
float temperature = tempC,tdsValue = 0;
float Ec;
float calibration_value = 21.34;
int phval = 0; 
unsigned long int avgval; 
int buffer_arr[10],temp;

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

char logreason;

int chipSelect= 53;
File mySensorData;

SoftwareSerial mySerial2(11,10); // RX, TX 
SoftwareSerial mySerial(3, 2);
Adafruit_GPS GPS(&mySerial);

OneWire oneWire(ONE_WIRE_BUS);

DallasTemperature sensors(&oneWire);

void setup() {
  gravityTds.setPin(TdsSensorPin);
  gravityTds.setAref(5.0);
  gravityTds.setAdcRange(1024);
  gravityTds.begin(); 
  pinMode(10,OUTPUT);
  SD.begin(chipSelect);
  sensors.begin();
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
  for(int i=0;i<4;i++){
       data[i]=mySerial2.read();
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
  for(int i=0;i<10;i++){ 
  buffer_arr[i]=analogRead(A0);
    }
    for(int i=0;i<9;i++){
    for(int j=i+1;j<10;j++){
    if(buffer_arr[i]>buffer_arr[j]){
      temp=buffer_arr[i];
      buffer_arr[i]=buffer_arr[j];
      buffer_arr[j]=temp;
    }
  }
}
  avgval=0;
  for(int i=2;i<8;i++)
  avgval+=buffer_arr[i];
  float volt=(float)avgval*5.0/1024/6;
  float ph_act = -5.70 * volt + calibration_value;
  sensors.requestTemperatures();
  float tempC = sensors.getTempCByIndex(0);
  float tempF = sensors.getTempFByIndex(0);
  int sensorValue = analogRead(A2);
  int turbidity =map(sensorValue,0,700,100,0);  
  if(mySensorData){
    mySensorData.println("1");
    mySensorData.print(tdsValue,0);
    mySensorData.println(",");
    mySensorData.print(ph_act);
    mySensorData.println(",");
    mySensorData.print(tempC);
    mySensorData.println(",");
    mySensorData.print(tempF);
    mySensorData.println(",");
    mySensorData.print(turbidity);
    mySensorData.println(",");
    mySensorData.print(Ec);
    mySensorData.print("Distance from riverbed : ");
    mySensorData.print(distance);
    mySensorData.print("cm");
    clearGPS();
    while (!GPS.newNMEAreceived()) {
      c = GPS.read();
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

  tempchange = tempC - prevTemp;
  tdschange = tdsValue - prevTds;
  phchange = ph_act - prevPh;
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
    for (int i=1; i<11; i++){
      if (significantchangeid == i){
        significantchange = false;
        digitalWrite(motorpin1 + i, HIGH);
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
          mySensorData.print(tempC);
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
          mySensorData.print(ph_act);
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
        digitalWrite(motorpin1 + i, LOW);
        significantchangeid = significantchangeid + 1;
        digitalWrite(rotormotorpin, HIGH);
        delay(2500);
        digitalWrite(rotormotorpin, LOW);
        prevTemp = tempC;
        prevTds = tdsValue;
        prevPh = ph_act;
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
    c = GPS.read();
  }
  GPS.parse(GPS.lastNMEA());

  while (!GPS.newNMEAreceived()) {
    c = GPS.read();
  }
  GPS.parse(GPS.lastNMEA());
}
