int pressurepin = A8;

void setup(){
  Serial.begin(9600);
}

void loop(){
  int pressuresensorValue = analogRead(pressurepin);
  float voltage = pressuresensorValue * (5.0 / 1023.0);
  float baselinevoltage = 0;
  float pressure = (voltage - baselinevoltage) * (100/ 4.5 - baselinevoltage);
  mySensorData.print("Pressure : ");
  mySensorData.print(pressure);
}
