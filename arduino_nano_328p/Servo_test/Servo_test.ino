#include "Servo.h"
 
#define SERVO1_PIN 9

int angle=0;
 
Servo Servo1;
 
void setup()
{  
  Servo1.attach(SERVO1_PIN);
  Serial.begin(9600);  
}
 
void loop()
{ 
  Servo1.write(angle); 
  delay(10);
  angle++;
  if(angle>180){
    angle=0;
    Servo1.write(angle);
    delay(1000);
  }
  
  

  if(Serial.available()){
    String received_string=Serial.readString();  
    int angle=received_string.toInt();
    Servo1.write(angle);
    Serial.print(angle);Serial.print(" - ");
    Serial.println("ok");

  }
  
}
 
