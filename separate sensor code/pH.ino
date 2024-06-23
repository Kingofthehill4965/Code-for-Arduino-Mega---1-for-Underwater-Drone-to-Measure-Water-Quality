#define pHSensorPin A0;

float ph_calibration_value = 21.34;

unsigned long int avgval;

int buffer_arr[10],temp;

void setup(){
 Serial.begin(9600);
}

void loop(){
  for(int b=0;b<10;b++){
    buffer_arr[b]=analogRead(pHSensorPin);
    delay(30);
  }
 
  for(int c=0;c<9;c++){
    for(int d=c+1;d<10;d++){
      if(buffer_arr[c]>buffer_arr[d]){
        temp=buffer_arr[c];
        buffer_arr[c]=buffer_arr[d];
        buffer_arr[d]=temp;
      }
    }
  }
  
  avgval=0;
 
  for(int e=2;e<8;e++){
    avgval+=buffer_arr[e];
  }
  
  float volt=(float)avgval * 5.0/1024/6;
  float ph = -5.70 * volt + ph_calibration_value;
  
  Serial.print("pH: ");
  Serial.println(ph);
  
  delay(1000);
}
