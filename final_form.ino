#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <MeMCore.h>

//colour config
#define COLOUR_SAMPLE_SIZE 10 //cannot exceed 128
#define LDR_WAIT
#define RGBWait 150 //in milliseconds 
#define LDRWait 10 //in milliseconds
#define ORANGE_RED 140
//was 658

//movement config
#define ADJUST 0.2
#define ULTRASONIC_STOP 580
#define LEFT 1
#define RIGHT 2
#define LEFTLEFT 3
#define RIGHTRIGHT 4
#define U_TURN 5
#define LEFT_DELAY 225 //WAS 235
#define RIGHT_DELAY 250 //WAS 250
#define ULTRASONIC 12

//sound config
#define sound_300 A2
#define sound_3000 A3

//MUSIC NOTES
#define NOTE_D5  587
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_G5  784
#define NOTE_A5  880
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_D6  1175
#define NOTE_E6  1319
#define NOTE_F6  1397

//global variable
MeBuzzer buzzer;
MeDCMotor motor_left(9); //left
MeDCMotor motor_right(10);    //right'
MeRGBLed rgbled_7(7, 7==7?2:4);
MeUltrasonicSensor ultrasonic_3(3);
MeLineFollower linefollower_2(2);
MeLightSensor lightsensor_6(6);
int colourArray[] = {0,0,0};
int blackArray[] = {430,330,374};
int ambient = 0;
int colours[3][3] = {
  {255,0,0},
  {0,255,0},
  {0,0,255}
};
double a,b,ratio; //a is 300, b is 3000

//melody
int melody[] = {
NOTE_E5, NOTE_F5, NOTE_G5, NOTE_E6, NOTE_C6,
NOTE_D6, NOTE_C6, NOTE_C6, NOTE_B5, NOTE_B5,
NOTE_D5, NOTE_E5, NOTE_F5, NOTE_D6, NOTE_B5,
NOTE_C6, NOTE_B5, NOTE_A5, NOTE_G5, NOTE_G5,
NOTE_E5, NOTE_F5, NOTE_G5, NOTE_C6, NOTE_D6, NOTE_E6,
NOTE_D6, NOTE_C6, NOTE_A5, NOTE_D6, NOTE_E6, NOTE_F6,
NOTE_E6, NOTE_D6, NOTE_G5, NOTE_F6, NOTE_E6, NOTE_D6, NOTE_C6
};

int noteDurations[] = {
8, 8, 4, 4, 4,
8, 8, 4, 4, 4,
8, 8, 4, 4, 4,
8, 8, 4, 4, 4,
8, 8, 4, 8, 8, 4,
8, 8, 4, 8, 8, 4,
8, 8, 4, 4, 4, 4, 2
};

int cheatswitch = 0;

void colour();
void sound();
void cheat();
int getAvgReading(int);
void accelerate();
void turn(int);
void detectline();
void move_straight();
void check_challenge();
int sound_avg(int);

void setup() {
  accelerate();
  Serial.begin(9600);
  pinMode(A7, INPUT); //TO BE REMOVED ON SUBMISSION
  pinMode(ULTRASONIC, OUTPUT);  
}

void accelerate() {
  for (int i = 100; i < 256; i += 10) {
    rgbled_7.setColor(0,i,i,i);
    rgbled_7.show();
    motor_left.run(-i);
    motor_right.run(i);
    delay(10); 
  }
  rgbled_7.setColor(0,0,0,0);
  rgbled_7.show();
}

void end(){
  while(1){
    for (int thisNote = 0; thisNote < 39; thisNote++) {
      int noteDuration;
      noteDuration = 1000/noteDurations[thisNote];
      buzzer.tone(melody[thisNote],noteDuration);
      delay(noteDuration);
      buzzer.noTone(); 
    }
  }
}

void turn(int direction) {
  if (direction == LEFT) {
    rgbled_7.setColor(0,255,0,0); //shine red on left
    rgbled_7.show();
    motor_left.run(255);
    motor_right.run(255);
    delay(LEFT_DELAY);
    rgbled_7.setColor(0,0,0,0);
    rgbled_7.show(); //off after move straight delay
  }
  if (direction == RIGHT) {
    motor_left.run(-255);
    motor_right.run(-255);
    delay(RIGHT_DELAY);
  }
  if (direction == LEFTLEFT) {
    rgbled_7.setColor(0,0,255,0); //shines green when orange
    rgbled_7.show();
    motor_left.run(255);
    motor_right.run(255);
    delay(LEFT_DELAY);
    motor_left.run(-200);
    motor_right.run(200);
    delay(150); //was 180
    bool hitWall = false;
    while(!hitWall) {
      rgbled_7.setColor(0,0,0,255);
      rgbled_7.show();
      digitalWrite(ULTRASONIC, LOW);
      delayMicroseconds(2);
      digitalWrite(ULTRASONIC, HIGH);
      delayMicroseconds(2);
      digitalWrite(ULTRASONIC, LOW);
      pinMode(ULTRASONIC, INPUT);
      if (pulseIn(ULTRASONIC, HIGH, 30000) < ULTRASONIC_STOP) {
        hitWall = true;
      }
      delay(20); //WAS 20
    }
    rgbled_7.setColor(0,0,0,0);
    rgbled_7.show();
    motor_left.run(255);
    motor_right.run(255);
    delay(LEFT_DELAY + 80); //WAS 350
    rgbled_7.setColor(0,0,0,0); //off light
    rgbled_7.show();
  }
  if (direction == RIGHTRIGHT) {
    motor_left.run(-255);
    motor_right.run(-255);
    delay(RIGHT_DELAY);
    motor_left.run(-255);
    motor_right.run(255);
    delay(150);
    bool hitWall = false;
    while(!hitWall) {
      rgbled_7.setColor(0,0,0,255);
      rgbled_7.show();
      digitalWrite(ULTRASONIC, LOW);
      delayMicroseconds(2);
      digitalWrite(ULTRASONIC, HIGH);
      delayMicroseconds(2);
      digitalWrite(ULTRASONIC, LOW);
      pinMode(ULTRASONIC, INPUT);
      if (pulseIn(ULTRASONIC, HIGH, 30000) < ULTRASONIC_STOP) {
        hitWall = true;
      }
      delay(15); //WAS 20
    }
    rgbled_7.setColor(0,0,0,0);
    rgbled_7.show();
    motor_left.run(-255);
    motor_right.run(-255);
    delay(RIGHT_DELAY + 90); //WAS 350
  }
  if (direction == U_TURN) {
    rgbled_7.setColor(0,0,0,0);
    rgbled_7.show();

    motor_left.run(-180);
    motor_right.run(-225);
    delay(580);
    motor_left.run(0);
    motor_right.run(0);
    delay(600);
    accelerate(); 

    
    //motor_left.run(0);
    //motor_right.run(0);
  }
  accelerate();
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

void check_challenge() {
  /*
  rgbled_7.setColor(0,0,0,0);
  rgbled_7.show();
  delay(RGBWait);
  int total = 0;
  for(int i = 0; i < 3; i++){
    rgbled_7.setColor(0,colours[i][0],colours[i][1],colours[i][2]);
    rgbled_7.show();
    delay(RGBWait);
    colourArray[i] = getAvgReading(5);
    colourArray[i] = colourArray[i] - blackArray[i];
    total += colourArray[i];
  }
  //Serial.println(total);
  delay(500);
  a = sound_avg(300);
  b = sound_avg(3000);
  if(total < 500){
    if(!(a < 190 && b < 20)){  
      sound();
    }
  } else {
      colour();
  }*/
  colour();
}

void sound(){

  ratio = b/a;
  Serial.print(" ");
  Serial.println(a);
  Serial.println(b);
  Serial.println(ratio);
  if (ratio < 0.5){
    turn(LEFT);
  } else {
    int right = 0;
    for(int j = 0; j < 5; ++j){
      a = sound_avg(300);
      b = sound_avg(3000);
      int new_ratio = (b-15)/abs(a-180);
      if(new_ratio >= 15){
        right = 1;
        break;
      }
    }
    int new_ratio = (b-15)/abs(a-180);
    if (right == 0){
      turn(U_TURN);
    } else {
      turn(RIGHT);
    }
  }
}


void colour(){
  rgbled_7.setColor(0,0,0,0);
  rgbled_7.show();
  delay(RGBWait);
  ambient = getAvgReading(5);
  int total = 0;
  for(int i = 0; i < 3; i++){
    rgbled_7.setColor(0,colours[i][0],colours[i][1],colours[i][2]);
    rgbled_7.show();
    delay(RGBWait);
    colourArray[i] = getAvgReading(5);
    colourArray[i] = colourArray[i] - blackArray[i];
    total += colourArray[i];
  }
  if(total > 1500){
    turn(U_TURN);
  } else if (total < 100){
    end();
  } else if (colourArray[2] > colourArray[0] && colourArray[2] > colourArray[1]){
    turn(RIGHTRIGHT);
  } else if (colourArray[1] > colourArray[0] && colourArray[1] > colourArray[2]){
    turn(RIGHT);
  } else if (colourArray[0] > colourArray[1] && colourArray[0] > colourArray[2]){
    total = colourArray[1] + colourArray[2] - ambient;
    Serial.println(total);
      if(total < ORANGE_RED){//was 280
        turn(LEFT);
      } else if (total >= ORANGE_RED){ //was 280
        turn(LEFTLEFT);
      }
    } else {
      turn(LEFT);
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

void detect_line() {
  if (linefollower_2.readSensors() != 3) {
    motor_left.run(0);
    motor_right.run(0);
    delay(200);
    check_challenge();
  }
}

void move_straight() {
  detect_line(); 
  rgbled_7.setColor(0,0,0,0);
  rgbled_7.show();
  float ir_left = (analogRead(A1) / 1023.000 * 5);
  float ir_right = (analogRead(A0) / 1023.000 * 5);
  if (ir_right >= 3.25 && ir_left >= 2.70 || ir_left >= 3.30 && ir_right >= 2.70){
    //rgbled_7.setColor(0,0,0,60); //no walls(blue)
    //rgbled_7.show();
    motor_left.run(-255);
    motor_right.run(245);
  } else if (ir_left - ir_right < ADJUST && ir_left - ir_right > -ADJUST - 0.05) {
    //rgbled_7.setColor(0,0,0,0);
    //rgbled_7.show();
    motor_left.run(-255);
    motor_right.run(245);
  } else if (ir_left - ir_right >= ADJUST) { //adjust left
    //rgbled_7.setColor(0,0,0,0);
    //rgbled_7.show();
    motor_left.run(-150);//was 125
    motor_right.run(255);
  } else if (ir_left - ir_right <= -ADJUST - 0.05) {//adjust right
    //rgbled_7.setColor(0,0,0,0);
    //rgbled_7.show();
    motor_left.run(-255);
    motor_right.run(150);//was 125
 }
 delay(20);
}

void loop() {
  cheat();
  move_straight();
}

void cheat() {
  if (analogRead(A7) <= 10) {
    if (cheatswitch == 0) {
      cheatswitch = 1;
      rgbled_7.setColor(0,255,165,0);
      rgbled_7.show();
    }
    else { 
      cheatswitch = 0;
      rgbled_7.setColor(0,255,20,147);
      rgbled_7.show();
    }
    delay(500);
    motor_left.run(0);
    motor_right.run(0);
    delay(3000);
    accelerate();
  }
}
