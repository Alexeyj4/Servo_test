#include "Servo.h"
 
#define SERVO1_PIN 9
#define SERVO2_PIN 10

int angle=0;
 
Servo Servo1;
Servo Servo2;
 
void setup()
{  
  Servo1.attach(SERVO1_PIN);
  Servo2.attach(SERVO2_PIN);
  Serial.begin(115200);  
}
 
void loop()
{ 
 

  if(Serial.available()){
    String received_string=Serial.readString();  
    int angle1=received_string.toInt();
    int angle2=180-received_string.toInt();
    Servo1.write(angle1);
    Servo2.write(angle2);    
    Serial.println("ok");

  }
  
}
 
