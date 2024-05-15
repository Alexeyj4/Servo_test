#include <ESP32Servo.h>
#include <GyverStepper.h>
#include <Esp_now_j4.h>

#define STEPPER1_IN1 19 //ULN2003 Motor Driver Pins:
#define STEPPER1_IN2 18
#define STEPPER1_IN3 5
#define STEPPER1_IN4 17

#define STEPPER2_IN1 22 //ULN2003 Motor Driver Pins:
#define STEPPER2_IN2 1
#define STEPPER2_IN3 3
#define STEPPER2_IN4 21

#define SERVO_H_PIN 12
#define SERVO_V_PIN 13
#define BUILTIN_LED_PIN 2

const int stepsPerRevolution = 2048;  // change this to fit the number of steps per revolution
uint8_t MAC[] = {0x78, 0x21, 0x84, 0xE1, 0x7E, 0xA0}; //для проверки отправителя
 
Servo servo_h;
Servo servo_v;
GStepper<STEPPER4WIRE> stepper1(stepsPerRevolution, STEPPER1_IN4, STEPPER1_IN2, STEPPER1_IN3, STEPPER1_IN1);// мотор с драйвером ULN2003 подключается по порядку пинов, но крайние нужно поменять местами// то есть у меня подключено D2-IN1, D3-IN2, D4-IN3, D5-IN4, но в программе поменял 5 и 2
GStepper<STEPPER4WIRE> stepper2(stepsPerRevolution, STEPPER2_IN4, STEPPER2_IN2, STEPPER2_IN3, STEPPER2_IN1);// мотор с драйвером ULN2003 подключается по порядку пинов, но крайние нужно поменять местами// то есть у меня подключено D2-IN1, D3-IN2, D4-IN3, D5-IN4, но в программе поменял 5 и 2
Esp_now_j4 esp_now(MAC);
 
void setup()
{  
  pinMode(BUILTIN_LED_PIN,OUTPUT);
  esp_now.begin();
  servo_h.attach(SERVO_H_PIN);
  servo_v.attach(SERVO_V_PIN);  
  stepper1.setRunMode(FOLLOW_POS);// режим следования к целевй позиции  
  stepper1.setMaxSpeed(400);// установка макс. скорости в шагах/сек  
  stepper1.setAcceleration(5000); // установка ускорения в шагах/сек/сек
  stepper2.setRunMode(FOLLOW_POS);// режим следования к целевй позиции  
  stepper2.setMaxSpeed(400);// установка макс. скорости в шагах/сек  //предположительно 400-максимум
  stepper2.setAcceleration(5000); // установка ускорения в шагах/сек/сек
  //Serial.begin(115200);  //debug
}
 
void loop()
{ 
  stepper1.tick();
  stepper2.tick();
 
  String s=esp_now.recvd();
  if(s!=""){
    digitalWrite(BUILTIN_LED_PIN,1);
    //Serial.println(s.substring(2).toInt()*coef+start_angle);//debug
    if(s.startsWith("h=")){
      servo_h.write(s.substring(2).toInt());
    }
    if(s.startsWith("v=")){
      servo_v.write(s.substring(2).toInt());
    }
    
    if(s.startsWith("l=")){
      int l_speed=s.substring(2).toInt();
      if(l_speed==0){
        stepper2.brake();
        stepper2.reset();
      }
      if(l_speed>0){
        stepper2.setMaxSpeed(abs(l_speed));
        stepper2.setTarget(-300000);      
      }
      if(l_speed<0){
        stepper2.setMaxSpeed(abs(l_speed));
        stepper2.setTarget(300000);      
      }
    }


    if(s.startsWith("r=")){
      int r_speed=s.substring(2).toInt();
      if(r_speed==0){
        stepper1.brake();
        stepper1.reset();
      }
      if(r_speed>0){
        stepper1.setMaxSpeed(abs(r_speed));
        stepper1.setTarget(300000);      
      }
      if(r_speed<0){
        stepper1.setMaxSpeed(abs(r_speed));
        stepper1.setTarget(-300000);      
      }
    }

    
    esp_now.clr();    
    digitalWrite(BUILTIN_LED_PIN,0);
  }
}
 
