#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>

#include <MeMCore.h>

#define COLOUR_SAMPLE_SIZE 10 //cannot exceed 128
#define ADJUST 0.2

MeDCMotor motor_9(9); //left 
MeDCMotor motor_10(10);    //right

double angle_rad = PI/180.0;
double angle_deg = 180.0/PI;
MeRGBLed rgbled_7(7, 7==7?2:4);
MeUltrasonicSensor ultrasonic_3(3);
MeLineFollower linefollower_2(2);
MeLightSensor lightsensor_6(6);
MeLimitSwitch sw_1_2(1,2);


MeBuzzer buzzer;

void setup(){
    pinMode(9,INPUT);
    pinMode(9,OUTPUT);
    Serial.begin(9600);
    pinMode(A7,INPUT); //for on board button  (analogRead(A7) > 10 ? 0 : 1)
}

//rgb of black and white
float black[] = {0, 0, 0};
float white[] = {0, 0, 0};
float greyDiff[] = {0, 0, 0};
float mystery_colour[] = {0, 0, 0};
int RGB[3][3] = { {255,0,0},{0,255,0},{0,0,255} };

void loop(){/*
  int a, b, c;
  Serial.print("A0 ");
  Serial.print(analogRead(A0) / 1023.000 * 5);
  Serial.print("  A1 ");
  Serial.print(analogRead(A1) / 1023.000 * 5);
  Serial.print("  DIFF ");
  Serial.println(analogRead(A1) / 1023.000 * 5);
  delay(100);
  
  //  colour_calibration();
  //  move_straight();
 //   stop_black_or_wall(); */
 move_straight();
}




void move_straight() {
  float ir_left = (analogRead(A1) / 1023.000 * 5);
  float ir_right = (analogRead(A0) / 1023.000 * 5);
  Serial.print(ir_left);
  Serial.print(" ");
  Serial.print(ir_right);
  Serial.print(" ");
  Serial.println( ir_left - ir_right);
  delay(20);
  if (ir_left - ir_right < ADJUST && ir_left - ir_right > -ADJUST - 0.05) {
    rgbled_7.setColor(0,0,0,0);
  rgbled_7.show();
    motor_9.run(-255);
    motor_10.run(255);
  }
  else if(ir_left - ir_right >= ADJUST) { //adjust left
    rgbled_7.setColor(0,0,255,0);
  rgbled_7.show();
    motor_9.run(-100);
    motor_10.run(255);
  }
  else if(ir_left - ir_right <= -ADJUST - 0.05) {//adjust left
    rgbled_7.setColor(0,255,0,0);
  rgbled_7.show();
    motor_9.run(-255);
    motor_10.run(100);
  //code involving IR sensors to move straight
}
}
/*
int average_colour(){ //returns average colour reading: /255
  float sum = 0; //sum limit is 32,767*
  for (int i = 0; i < COLOUR_SAMPLE_SIZE; i += 1) {
    //Serial.println(lightsensor_6.read());
    sum += lightsensor_6.read();
    delay(20);    
  }
//  Serial.print("average_colour returns ");
//  Serial.println(sum / COLOUR_SAMPLE_SIZE);
  return sum / COLOUR_SAMPLE_SIZE;
}

void stop_black_or_wall() {
  
  while (linefollower_2.readSensors() != 3 || ultrasonic_3.distanceCm() <= 5) {//if black line is detected
    motor_9.run(0);
    motor_10.run(0);
    colour_calibration();
    turn_based_on_colour(); 
    }//where sound and colour sensor turns on //where sound and colour sensor turns on
}

  if (white[0] == 0 && black[0] == 0) {
    return;
  if (analogRead(A7) <= 10) { //if onboard button is not pressed, the function will end, continues otherwise
    return;
  }
  Serial.print("Colour ");
  }
  for (int i = 0; i < 3; i += 1) {
    rgbled_7.setColor(0,RGB[i][0],RGB[i][1],RGB[i][2]); //shines r g b light
    rgbled_7.show();
    mystery_colour[i] = ((average_colour() - black[i]) / (greyDiff[i]) * 255);
    Serial.print(" ");
    Serial.print(mystery_colour[i]); //stores and prints white RGB
  }
  Serial.println(" ");
  rgbled_7.setColor(0,0,0,0);
  rgbled_7.show();
}

void turn_based_on_colour(){
  if (white[0] == 0 && black[0] == 0) {
    return;
  } 
  colour_detector();
  rgbled_7.setColor(0,0,0,0);
  rgbled_7.show();
  delay(1000);
  rgbled_7.setColor(0,mystery_colour[0],mystery_colour[1],mystery_colour[2]);
  rgbled_7.show();
  delay(5000);
   rgbled_7.setColor(0,0,0,0);
  rgbled_7.show();
  return;
  /*if (mystery_colour[0] > 220 && mystery_colour[1] > 220 && mystery_colour[2] > 220) { //white
    // u-turn
    Serial.println("WHITE");
  }
  else if (mystery_colour[0] > 200 && mystery_colour[1] < 100 && mystery_colour[2] < 100) { //red
    //turn left
    Serial.println("RED");
  }
  else if (mystery_colour[0] < 100 && mystery_colour[1] > 200 && mystery_colour[2] < 100) { //green
     // turn right
     Serial.println("GREEN");
}
  else if (mystery_colour[0] < 100 && mystery_colour[1] < 100 && mystery_colour[2] > 200) { //blue/cyan
    //2 successive right-turns in two grids
    Serial.println("BLUE");
  }
  else if (mystery_colour[0] > 180 && mystery_colour[1] > 100 && mystery_colour[2] < 100) { //orange
    //2 successive left-turns in two grids
    Serial.println("ORANGE/YELLOW"); 
  }
  else if (mystery_colour[0] < 100 && mystery_colour[1] < 100 && mystery_colour[2] < 100) { //orange
    //2 successive left-turns in two grids
    Serial.println("BLACK"); 
  }
  colour_calibration();
}

void colour_calibration() { //calibrates RGB of black and white
  if (analogRead(A7) > 10) { //if onboard button is not pressed, the function will end, continues otherwise
    return;
  }
  //stops motor during calibration
  motor_9.run(0);
  motor_10.run(0);
  //colour_calibration begins here
  for (int i = 0; i < 5; i += 1) { //5 sec to put white followed by black sample
    rgbled_7.setColor(0,255,255,255);
    rgbled_7.show();
    delay(50);
    rgbled_7.setColor(0,0,0,0);
    rgbled_7.show();
    delay(950);
  }
  Serial.print("white ");
  for (int i = 0; i < 3; i += 1) {
    rgbled_7.setColor(0,RGB[i][0],RGB[i][1],RGB[i][2]); //shines red light
    rgbled_7.show();
    white[i] = average_colour();
    Serial.print(" ");
    Serial.print(white[i]); //stores and prints white RGB
  }
  for (int i = 0; i < 5; i += 1) { //5 sec to put white followed by black sample
    rgbled_7.setColor(0,255,255,255);
    rgbled_7.show();
    delay(50);
    rgbled_7.setColor(0,0,0,0);
    rgbled_7.show();
    delay(950);
  }
  Serial.println(" ");
  Serial.print("black ");
  for (int i = 0; i < 3; i += 1) {
    rgbled_7.setColor(0,RGB[i][0],RGB[i][1],RGB[i][2]); //shines red light
    rgbled_7.show();
    black[i] = average_colour();
    Serial.print(" ");
    Serial.print(black[i]); //stores and prints black RGB
    greyDiff[i] = white[i] - black[i];
  }
  Serial.println(" ");
  rgbled_7.setColor(0,0,0,0);
  rgbled_7.show();
}
*/



/*  USEFUL TRASH  
    Serial.println(ultrasonic_3.distanceCm());
    Serial.println(linefollower_2.readSensors());
    Serial.println(lightsensor_6.read());
    Serial.println(digitalRead(9));
    digitalWrite(9,1);
    rgbled_7.setColor(0,0,0,255);
        rgbled_7.show();
    digitalWrite(9,0);
    buzzer.tone(330, 250);
    delay(20);
    buzzer.tone(523, 250);
    delay(20);
    buzzer.tone(494, 250);
    delay(20);
    buzzer.tone(440, 250);
    delay(100);
    while(1);*/
