#include <ESP32Encoder.h>
#include <Esp_now_j4.h>
 
#define ENC_H_A_PIN 27 //horizontal control encoder A pin
#define ENC_H_B_PIN 26 //horizontal control encoder B pin
#define ENC_V_A_PIN 25 //vertical control encoder A pin
#define ENC_V_B_PIN 33 //vertical control encoder B pin
#define BUILTIN_LED_PIN 2

uint8_t MAC[] = {0x78, 0x21, 0x84, 0xE1, 0x35, 0xD0};
int prev_h_angle=90;
int prev_v_angle=90;

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
  pinMode(BUILTIN_LED_PIN,OUTPUT);
  h_enc.attachFullQuad(ENC_H_A_PIN, ENC_H_B_PIN);
  h_enc.setCount(90*4);
  v_enc.attachFullQuad(ENC_V_A_PIN, ENC_V_B_PIN);
  v_enc.setCount(90*4);
  Serial.begin(9600);    
}
 
void loop()
{   
 
  int h_enc_angle=(int32_t)(floor(h_enc.getCount()/4)); //0-180 
  int v_enc_angle=(int32_t)(floor(v_enc.getCount()/4)); //0-180
  
  //выравнивание угла до диапазона 0-180:
  if(h_enc_angle>(180)){
    h_enc_angle=180;
    h_enc.setCount(180*4);
  }
  if(h_enc_angle<0){
    h_enc_angle=0;
    h_enc.setCount(0);
  }
  if(v_enc_angle>(180)){
    v_enc_angle=180;
    v_enc.setCount(180*4);
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
    Serial.print("h=");  
    Serial.println(h_angle);
    esp_now.send("h="+String(h_angle));    
    prev_h_angle=h_angle;    
    delay(50); 
    digitalWrite(BUILTIN_LED_PIN,0);  
  }
  
  //проверка, был ли изменён v-угол, и установка нового угла, если да:
  if(v_angle!=prev_v_angle){
    digitalWrite(BUILTIN_LED_PIN,1);  
    Serial.print("v=");  
    Serial.println(v_angle);
    esp_now.send("v="+String(v_angle));    
    prev_v_angle=v_angle;
    delay(50);    
    digitalWrite(BUILTIN_LED_PIN,0); 
  }
  delay(200);    

}
 
