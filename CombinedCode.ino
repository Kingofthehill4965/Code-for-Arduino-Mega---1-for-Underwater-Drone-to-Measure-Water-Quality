#include <EEPROM.h>
#include <Arduino.h>
#include <Wire.h>
#include "GravityTDS.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#include <SD.h>
#include <SPI.h>
 
#define ONE_WIRE_BUS 2
#define TdsSensorPin A1
GravityTDS gravityTds;

float significantchangeid;

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

const int motorPin1 = 40;
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

int chipSelect= 53;
File mySensorData;

OneWire oneWire(ONE_WIRE_BUS);

DallasTemperature sensors(&oneWire);

void setup() {
  Serial.begin(9600);
  gravityTds.setPin(TdsSensorPin);
  gravityTds.setAref(5.0);
  gravityTds.setAdcRange(1024);
  gravityTds.begin(); 
  pinMode(10,OUTPUT);
  SD.begin(chipSelect);
  sensors.begin();
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);
  pinMode(motorPin5, OUTPUT);
  pinMode(motorPin6, OUTPUT);
  pinMode(motorPin7, OUTPUT);
  pinMode(motorPin8, OUTPUT);
  pinMode(motorPin9, OUTPUT);
  pinMode(motorPin10, OUTPUT);
  pinMode(rotormotorpin, OUTPUT);
  significantchangeid = 1;
}

void loop() {  
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
    Serial.println("1");
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
    if (abs(tempC - prevTemp) > temp_threshold){
    significantchange = true;
    prevTemp = tempC;
  }
  if (abs(tdsValue - prevTds) > tds_threshold){
    significantchange = true;
    prevTds = tdsValue;
  }
  if (abs(ph_act - prevPh) > ph_threshold){
    significantchange = true;
    prevPh = ph_act;
  }
  if (abs(turbidity - prevTurbidity) > turbidity_threshold){
    significantchange = true;
    prevTurbidity = turbidity;
  }
  if (significantchange){
    for (int i=1; i<11; i++){
      if (significantchangeid == i){
        significantchange = false;
        digitalWrite(motorPin1 + i, HIGH);
        delay(15000);
        digitalWrite(motorPin1 + i, LOW);
        significantchangeid = significantchangeid + 1;
        digitalWrite(rotormotorpin, HIGH);
        delay(2500);
        digitalWrite(rotormotorpin, LOW);-
        break;
      }
    }
    if (significantchangeid == 11){
      significantchange = false;
    }
    else {
      mySensorData.print("ERROR 404 : Autonomous Sampling error.")
    }
  }
}
delay(10);
}
