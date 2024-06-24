int soundsensor = 6;

void setup(){
  pinMode(soundsensor, INPUT);
  Serial.begin(9600);
}

void loop(){
  int data = digitalRead(sensor);
  mySensorData.print("Sound (decibels)");
  mySensorData.print(data);
}
