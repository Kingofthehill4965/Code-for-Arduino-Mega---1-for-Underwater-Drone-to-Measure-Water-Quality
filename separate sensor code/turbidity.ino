#define turbiditySensorPin A2;

void setup(){
  Serial.begin(9600);
}

void loop(){
  int turbiditySensorValue = analogRead(turbiditySensorPin);
  float turbidity = sensorValue * (5.0 / 1024);
  Serial.println(turbidity);
  delay(1000);
}
