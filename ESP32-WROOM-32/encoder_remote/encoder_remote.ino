#include <ESP32Encoder.h>
#include <Esp_now_j4.h>
 
#define ENC_H_A_PIN 27 //horizontal control encoder A pin
#define ENC_H_B_PIN 26 //horizontal control encoder B pin
#define ENC_V_A_PIN 25 //vertical control encoder A pin
#define ENC_V_B_PIN 33 //vertical control encoder B pin
//При использовании WiFi для аналоговых входов используйте пины 32, 33, 34, 35, 36, 39. Остальные могут вызвать проблемы:
#define POT_L_X_PIN 35 //left x pot pin //joystick
#define POT_R_X_PIN 32 //right x pot pin //joystick
#define BUILTIN_LED_PIN 2
#define POT_LEVEL_F_3 3440 //forward
#define POT_LEVEL_F_2 2824 //forward
#define POT_LEVEL_F_1 2208 //forward
#define POT_LEVEL_B_1 1629 //backward
#define POT_LEVEL_B_2 1086 //backward
#define POT_LEVEL_B_3 543 //backward
#define MAX_MOTOR_SPEED 400 //максимальная скорость шагового двигателя

uint8_t MAC[] = {0x78, 0x21, 0x84, 0xE1, 0x35, 0xD0};
int enc2servo_coef=2;
int prev_h_angle=90;//servo
int prev_v_angle=90;//servo
int prev_l_speed=0;//motor
int prev_r_speed=0;//motor

ESP32Encoder h_enc;
ESP32Encoder v_enc;
Esp_now_j4 esp_now(MAC);

void setup()
{  
  esp_now.begin();
  pinMode(ENC_H_A_PIN, INPUT_PULLUP);
  pinMode(ENC_H_B_PIN, INPUT_PULLUP);
  pinMode(ENC_V_A_PIN, INPUT_PULLUP);
  pinMode(ENC_V_B_PIN, INPUT_PULLUP);
  pinMode(POT_L_X_PIN, INPUT);
  pinMode(POT_R_X_PIN, INPUT);  
  pinMode(BUILTIN_LED_PIN,OUTPUT);
  h_enc.attachFullQuad(ENC_H_A_PIN, ENC_H_B_PIN);
  h_enc.setCount(90*4/enc2servo_coef);
  v_enc.attachFullQuad(ENC_V_A_PIN, ENC_V_B_PIN);
  v_enc.setCount(90*4/enc2servo_coef);
  Serial.begin(115200);    
}
 
void loop()
{   
 
  int h_enc_angle=(int32_t)(floor(h_enc.getCount()/4)*enc2servo_coef); //(0-180)/enc2servo_coef
  int v_enc_angle=(int32_t)(floor(v_enc.getCount()/4)*enc2servo_coef); //(0-180)/enc2servo_coef
  
  //выравнивание угла до диапазона 0-180:
  if(h_enc_angle>(180)){
    h_enc_angle=180;
    h_enc.setCount(180*4/enc2servo_coef);
  }
  if(h_enc_angle<0){
    h_enc_angle=0;
    h_enc.setCount(0);
  }
  if(v_enc_angle>(180)){
    v_enc_angle=180;
    v_enc.setCount(180*4/enc2servo_coef);
  }
  if(v_enc_angle<0){
    v_enc_angle=0;
    v_enc.setCount(0);
  }

  int h_angle=180-h_enc_angle;//инверсия угла энкодера (180-0)
  int v_angle=180-v_enc_angle;//инверсия угла энкодера (180-0)

  //проверка, был ли изменён h-угол, и установка нового угла, если да:
  if(h_angle!=prev_h_angle){
    digitalWrite(BUILTIN_LED_PIN,1);  
    Serial.print("h=");Serial.println(h_angle);
    esp_now.send("h="+String(h_angle));    
    prev_h_angle=h_angle;            
    delay(50);  
    digitalWrite(BUILTIN_LED_PIN,0); 
  }
  
  //проверка, был ли изменён v-угол, и установка нового угла, если да:
  if(v_angle!=prev_v_angle){
    digitalWrite(BUILTIN_LED_PIN,1);  
    Serial.print("v=");Serial.println(v_angle);
    esp_now.send("v="+String(v_angle));    
    prev_v_angle=v_angle;
    delay(50); 
    digitalWrite(BUILTIN_LED_PIN,0);
  }

  //считывание левого с джойстика  
  int adc_left_x_joystick=analogRead(POT_L_X_PIN);
  int l_speed;  

  if(POT_LEVEL_F_3<adc_left_x_joystick){ l_speed=MAX_MOTOR_SPEED; }    
  if((POT_LEVEL_F_2<adc_left_x_joystick)and(adc_left_x_joystick<POT_LEVEL_F_3)){ l_speed=int(MAX_MOTOR_SPEED/3*2); }
  if((POT_LEVEL_F_1<adc_left_x_joystick)and(adc_left_x_joystick<POT_LEVEL_F_2)){ l_speed=int(MAX_MOTOR_SPEED/3); }
  if((POT_LEVEL_B_1<adc_left_x_joystick)and(adc_left_x_joystick<POT_LEVEL_F_1)){ l_speed=0; }
  if((POT_LEVEL_B_2<adc_left_x_joystick)and(adc_left_x_joystick<POT_LEVEL_B_1)){ l_speed=int(-1*MAX_MOTOR_SPEED/3); }
  if((POT_LEVEL_B_3<adc_left_x_joystick)and(adc_left_x_joystick<POT_LEVEL_B_2)){ l_speed=int(-1*MAX_MOTOR_SPEED/3*2); }
  if(adc_left_x_joystick<POT_LEVEL_B_3){ l_speed=int(-1*MAX_MOTOR_SPEED); }  
  
  if(prev_l_speed!=l_speed){//проверка, было ли изменение
    digitalWrite(BUILTIN_LED_PIN,1);
    Serial.print("l=");Serial.println(l_speed);
    esp_now.send("l="+String(l_speed)); 
    prev_l_speed=l_speed; 
    delay(50);
    digitalWrite(BUILTIN_LED_PIN,0); 
  }

  //считывание правого с джойстика
  int adc_right_x_joystick=analogRead(POT_R_X_PIN);
  int r_speed;
  
  if(POT_LEVEL_F_3<adc_right_x_joystick){ r_speed=MAX_MOTOR_SPEED; }    
  if((POT_LEVEL_F_2<adc_right_x_joystick)and(adc_right_x_joystick<POT_LEVEL_F_3)){ r_speed=int(MAX_MOTOR_SPEED/3*2); }
  if((POT_LEVEL_F_1<adc_right_x_joystick)and(adc_right_x_joystick<POT_LEVEL_F_2)){ r_speed=int(MAX_MOTOR_SPEED/3); }
  if((POT_LEVEL_B_1<adc_right_x_joystick)and(adc_right_x_joystick<POT_LEVEL_F_1)){ r_speed=0; }
  if((POT_LEVEL_B_2<adc_right_x_joystick)and(adc_right_x_joystick<POT_LEVEL_B_1)){ r_speed=int(-1*MAX_MOTOR_SPEED/3); }
  if((POT_LEVEL_B_3<adc_right_x_joystick)and(adc_right_x_joystick<POT_LEVEL_B_2)){ r_speed=int(-1*MAX_MOTOR_SPEED/3*2); }
  if(adc_right_x_joystick<POT_LEVEL_B_3){ r_speed=int(-1*MAX_MOTOR_SPEED); }  
  
  if(prev_r_speed!=r_speed){//проверка, было ли изменение
    digitalWrite(BUILTIN_LED_PIN,1);
    Serial.print("r=");Serial.println(r_speed);
    esp_now.send("r="+String(r_speed)); 
    prev_r_speed=r_speed; 
    delay(50);
    digitalWrite(BUILTIN_LED_PIN,0); 
  }
  
       

}
 
