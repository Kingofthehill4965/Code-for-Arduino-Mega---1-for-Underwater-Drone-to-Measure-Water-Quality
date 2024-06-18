// Include necessary libraries
#include <EEPROM.h>
#include <Arduino.h>
#include <Wire.h>
#include "GravityTDS.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#include <SD.h>
#include <SPI.h>
 
// Define pin and variable names
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

// Create OneWire instance and DallasTemperature instance
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// Set up the Arduino
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

// Main loop to read sensor data and write to SD card
void loop() {  
  // Open the file for writing
  mySensorData = SD.open("test.txt",FILE_WRITE);
  
  // Read temperature from temperature sensor
  gravityTds.setTemperature(temperature);
  
  // Update the TDS sensor and get the TDS value
  gravityTds.update();
  tdsValue = gravityTds.getTdsValue();
  
  // Calculate EC value
  Ec = (tdsValue * 2)/ 1000;
  
  // Read analog values from pH and turbidity sensors
  for(int i=0;i<10;i++){ 
    buffer_arr[i]=analogRead(A0);
  }
  
  // Sort the buffer array
  for(int i=0;i<9;i++){
    for(int j=i+1;j<10;j++){
      if(buffer_arr[i]>buffer_arr[j]){
        temp=buffer_arr[i];
        buffer_arr[i]=buffer_arr[j];
        buffer_arr[j]=temp;
      }
    }
  }
  
  // Calculate average value
  avgval=0;
  for(int i=2;i<8;i++)
    avgval+=buffer_arr[i];
  
  // Calculate voltage and pH value
  float volt=(float)avgval*5.0/1024/6;
  float ph_act = -5.70 * volt + calibration_value;
  
  // Read temperature from temperature sensor
  sensors.requestTemperatures();
  float tempC = sensors.getTempCByIndex(0);
  float tempF = sensors.getTempFByIndex(0);
  
  // Read analog value from turbidity sensor and map to value range
  int sensorValue = analogRead(A2);
  int turbidity =map(sensorValue,0,700,100,0);  

  // Write sensor data to SD card
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
  
  // Close the file
  mySensorData.close();
  
  // Delay for stability
  delay(10);
}

