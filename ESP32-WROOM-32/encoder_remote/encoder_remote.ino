#include <ESP32Encoder.h>
#include <Esp_now_j4.h>
 
#define ENC_H_A_PIN 27 //horizontal control encoder A pin
#define ENC_H_B_PIN 26 //horizontal control encoder B pin
#define ENC_V_A_PIN 25 //vertical control encoder A pin
#define ENC_V_B_PIN 33 //vertical control encoder B pin
#define BUILTIN_LED_PIN 2

uint8_t MAC[] = {0x78, 0x21, 0x84, 0xE1, 0x35, 0xD0};

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
  h_enc.clearCount();
  v_enc.attachFullQuad(ENC_V_A_PIN, ENC_V_B_PIN);
  v_enc.clearCount();
  Serial.begin(9600);    
}
 
void loop()
{  
  digitalWrite(BUILTIN_LED_PIN,1);
  int h_angle=(int32_t)(floor(h_enc.getCount()/4));
  Serial.print("h=");  
  Serial.println(h_angle);
  esp_now.send("h="+String(h_angle));    

  int v_angle=(int32_t)(floor(v_enc.getCount()/4));
  Serial.print("v=");  
  Serial.println(v_angle); 
  esp_now.send("v="+String(v_angle));    
  digitalWrite(BUILTIN_LED_PIN,0);
  delay(100);   
}
 
