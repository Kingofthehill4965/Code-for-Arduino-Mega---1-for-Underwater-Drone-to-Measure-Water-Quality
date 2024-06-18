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

float tempC; 
float temperature = tempC,tdsValue = 0;
float Ec;
float calibration_value = 21.34;
int phval = 0; 
unsigned long int avgval; 
int buffer_arr[10],temp;

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
}
delay(10);
}
