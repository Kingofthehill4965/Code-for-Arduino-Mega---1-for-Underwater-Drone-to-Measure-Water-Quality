const int lightpin = A4;

void setup(){
  Serial.begin(9600)
}

void loop(){
  float light = analogRead(lightpin);
  Serial.println("Light : );
  Serial.print(light);
}
