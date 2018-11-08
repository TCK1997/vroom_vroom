#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <MeMCore.h>

#define RGBWait 150 //in milliseconds 
#define LDRWait 10 //in milliseconds
int colourArray[] = {0,0,0};
int blackArray[] = {392,306,330};
MeRGBLed rgbled_7(7, 7==7?2:4);
MeLightSensor lightsensor_6(6);
int colours[3][3] = {
  {255,0,0},
  {0,255,0},
  {0,0,255}
};

void setup(){
  Serial.begin(9600);
}

void colour(){
  int total = 0;
  for(int i = 0; i < 3; i++){
    rgbled_7.setColor(0,colours[i][0],colours[i][1],colours[i][2]);
    rgbled_7.show();
    delay(RGBWait);
    colourArray[i] = getAvgReading(5);
    colourArray[i] = colourArray[i] - blackArray[i];
    total += colourArray[i];
  }
  Serial.print("Red: ");
  Serial.print(colourArray[0]);
  Serial.print("Green: ");
  Serial.print(colourArray[1]);
  Serial.print("Blue: ");
  Serial.println(colourArray[2]);
  if(total > 1500){
    Serial.println("WHITE");
  } else if (total < 100){
    Serial.println("BLACK");
  } else if (colourArray[2] > colourArray[0] && colourArray[2] > colourArray[1]){
    Serial.println("BLUE");
  } else if (colourArray[1] > colourArray[0] && colourArray[1] > colourArray[2]){
    Serial.println("GREEN");
  } else if (colourArray[0] > colourArray[1] && colourArray[0] > colourArray[2]){
    total = colourArray[1] + colourArray[2];
    Serial.print("TOTAL :");
    Serial.print(total);
    if(total < 280){
      Serial.println("RED");
    } else if (total >= 280){
      Serial.println("ORANGE");
    }
  }
}

int getAvgReading(int times){      
  int reading;
  int total = 0;
  for(int i = 0;i < times;i++){
     reading = lightsensor_6.read();
     total = reading + total;
     delay(LDRWait);
  }
  return total/times;
}

void loop(){
  colour();
}
