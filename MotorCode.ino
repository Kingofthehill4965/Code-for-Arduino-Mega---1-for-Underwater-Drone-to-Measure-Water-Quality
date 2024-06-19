const int motorPin1 = 1;
const int motorpin2 = 2;
const int motorpin3 = 3;
const int motorpin4 = 4;
const int motorpin5 = 5;
const int motorpin6 = 6;
const int motorpin7 = 7;
const int motorpin8 = 8;
const int motorpin9 = 9;
const int motorpin10 = 10;

const int totalCycles = 15;

void setup() {
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
}

void loop() {
  digitalWrite(motorPin1, HIGH);
  delay(15000);
  digitalWrite(motorPin1, LOW);
  delay(300000);
  digitalWrite(motorPin2, HIGH);
  delay(15000);
  digitalWrite(motorPin2, LOW);
  delay(300000);
  digitalWrite(motorPin3, HIGH);
  delay(15000);
  digitalWrite(motorPin3, LOW);
  delay(300000);
  digitalWrite(motorPin4, HIGH);
  delay(15000);
  digitalWrite(motorPin4, LOW);
  delay(300000);
  digitalWrite(motorPin5, HIGH);
  delay(15000);
  digitalWrite(motorPin5, LOW);
  delay(300000);
  digitalWrite(motorPin6, HIGH);
  delay(15000);
  digitalWrite(motorPin6, LOW);
  delay(300000);
  digitalWrite(motorPin7, HIGH);
  delay(15000);
  digitalWrite(motorPin7, LOW);
  delay(300000);
  digitalWrite(motorPin8, HIGH);
  delay(15000);
  digitalWrite(motorPin8, LOW);
  delay(300000);
  digitalWrite(motorPin9, HIGH);
  delay(15000);
  digitalWrite(motorPin9, LOW);
  delay(300000);
  digitalWrite(motorPin10, HIGH);
  delay(15000);
  digitalWrite(motorPin10, LOW);
  while (true) {
    // Infinite loop to keep the motor off after the cycles are completed
  }
}
