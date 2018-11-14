#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <MeMCore.h>

#define sound_300 A2
#define sound_3000 A3
#define bound 20

double a,b,diff,ratio; 
MeRGBLed rgbled_7(7, 7==7?2:4);

void setup() {
  Serial.begin(9600);
}


void loop() {
  rgbled_7.setColor(0,0,0,0);
  rgbled_7.show();
  delay(1000);
  a = sound_avg(300);
  b = sound_avg(3000);
  Serial.print("300: ");
  Serial.print(a);
  Serial.print(" 3000: ");
  Serial.print(b);
  Serial.print(" ratio: ");
  Serial.print(ratio);
  Serial.print(" diff: ");
  Serial.println(diff);
  ratio = b/a;
  diff = a - b;
  if (a < 190 && b < 20){
    Serial.println("NOSOUND");
  }
  else if (ratio < 0.5){
    //rgbled_7.setColor(0,0,255,0); //u-turn green
    rgbled_7.show();
    Serial.println("LEFT");
  } else{
    a -= 180; b -= 15;
    ratio = b/a;
    if (ratio < 30){
    //rgbled_7.setColor(0,255,0,0); //right-turn red
    rgbled_7.show();
    Serial.println("U_TURN");
    Serial.println(ratio);
    } else if (ratio >= 30){
    //rgbled_7.setColor(0,0,0,255); // left-turn blue
    rgbled_7.show();
    Serial.println("RIGHT");
    Serial.println(ratio);
  } 
  }
  
  delay(500);
}

int sound_avg(int sound){
  int total = 0;
  for(int i = 0; i < 50; i++){
    if(sound == 300){
      total += analogRead(sound_300);
    } else{
      total += analogRead(sound_3000);
    }
  }
  return total /50;
}
