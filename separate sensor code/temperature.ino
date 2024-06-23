#include <OneWire.h>
#include <DallasTemperature.h>

#define temperatureSensorPin = 2;

OneWire oneWire(temperatureSensorPin);
DallasTemperature temperatureSensor(&oneWire);

float temperatureCelsius;
float temperatureFahrenheit;

void setup(){
  Serial.begin(9600);
  temperatureSensor.begin();
}

void loop(){
  temperatureSensor.requestTemperatures();
  temperatureCelsius = temperatureSensor.getTempCByIndex(0);
  temperatureFahrenheit = tempCelsius * 9 / 5 + 32;
  
  Serial.print("Temperature: ");
  Serial.print(tempCelsius);
  Serial.print("°C");
  Serial.print("  ~  ");
  Serial.print(tempFahrenheit);
  Serial.println("°F");

  delay(500);
}
