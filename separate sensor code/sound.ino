#define soundSensorPin = 6;

void setup(){
  pinMode(soundSensorPin, INPUT);
  Serial.begin(9600);
}

void loop(){
  int sound = digitalRead(soundSensorPin);
  mySensorData.print("Sound (decibels)");
  mySensorData.print(sound);
}
