#include <ESP32Servo.h>
#include <Esp_now_j4.h>
 
#define SERVO_H_PIN 12
#define SERVO_V_PIN 13
#define BUILTIN_LED_PIN 2

uint8_t MAC[] = {0x78, 0x21, 0x84, 0xE1, 0x7E, 0xA0}; //для проверки отправителя

int coef=1; //пересчёт значения энкодера в угол сервопривода
int start_angle=90; //добавление при нуле энкодера
 
Servo servo_h;
Servo servo_v;
Esp_now_j4 esp_now(MAC);
 
void setup()
{  
  pinMode(BUILTIN_LED_PIN,OUTPUT);
  esp_now.begin();
  servo_h.attach(SERVO_H_PIN);
  servo_v.attach(SERVO_V_PIN);  
  Serial.begin(9600);  
}
 
void loop()
{ 
  String s=esp_now.recvd();
  if(s!=""){
    digitalWrite(BUILTIN_LED_PIN,1);
    //Serial.println(s.substring(2).toInt()*coef+start_angle);//debug
    if(s.startsWith("h=")){
      servo_h.write(s.substring(2).toInt()*coef+start_angle);
    }
    if(s.startsWith("v=")){
      servo_v.write(s.substring(2).toInt()*coef+start_angle);
    }
    esp_now.clr();    
    digitalWrite(BUILTIN_LED_PIN,0);
  }
}
 
