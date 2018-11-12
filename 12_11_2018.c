#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <MeMCore.h>

//colour config
#define COLOUR_SAMPLE_SIZE 10 //cannot exceed 128
#define LDR_WAIT
#define RGBWait 150 //in milliseconds 
#define LDRWait 10 //in milliseconds
#define ORANGE_RED 740 //was 658

//movement config
#define ADJUST 0.2
#define ULTRASONIC_STOP 580
#define LEFT 1
#define RIGHT 2
#define LEFTLEFT 3
#define RIGHTRIGHT 4
#define U_TURN 5
#define LEFT_DELAY 240 //WAS 235
#define RIGHT_DELAY 250 //WAS 270
#define ULTRASONIC 12

//sound config
#define nosound 200
#define diff 100
#define input_300 A2
#define input_3000 A3

//global variable
MeDCMotor motor_left(9); //left
MeDCMotor motor_right(10);    //right'
MeRGBLed rgbled_7(7, 7==7?2:4);
MeUltrasonicSensor ultrasonic_3(3);
MeLineFollower linefollower_2(2);
MeLightSensor lightsensor_6(6);
int colourArray[] = {0,0,0};
int blackArray[] = {392,306,330};
int colours[3][3] = {
  {255,0,0},
  {0,255,0},
  {0,0,255}
};
int sound_300 = 0, sound_3000 = 0;

void setup() {
  Serial.begin(9600);
  pinMode(ULTRASONIC, OUTPUT);  
}

void turn(int direction) {
  if (direction == LEFT) {
   rgbled_7.setColor(0,255,0,0); //shine red on left
    rgbled_7.show();
    motor_left.run(255);
    motor_right.run(255);
    delay(LEFT_DELAY);
    motor_left.run(-255);
    motor_right.run(255);
    delay(600);
    rgbled_7.setColor(0,0,0,0);
    rgbled_7.show(); //off after move straight delay
  }
  if (direction == RIGHT) {
    motor_left.run(-255);
    motor_right.run(-255);
    delay(RIGHT_DELAY);
    motor_left.run(-255);
    motor_right.run(255);
    delay(600);
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
    delay(LEFT_DELAY + 90); //WAS 350
    motor_left.run(-255);
    motor_right.run(255);
    delay(600);
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
    delay(RIGHT_DELAY + 120); //WAS 350
    motor_left.run(-255);
    motor_right.run(255);
    delay(600);
  }
  if (direction == U_TURN) {
    rgbled_7.setColor(0,0,0,0);
    rgbled_7.show();

    motor_left.run(-180);
    motor_right.run(-225);
    delay(560);
    
    //motor_left.run(0);
    //motor_right.run(0);
  }
}

void check_challenge() {
  sound_300 = analogRead(input_300);
  sound_3000 = analogRead(input_3000);
  if ((sound_300 + sound_3000) > nosound) {
    sound();
  } else {
    colour();
  }
}

void sound() {
    if (abs(sound_300 - sound_3000) < nosound){
      turn(U_TURN);
    } else if (sound_300 - sound_3000 > diff && sound_300 > 300){
      turn(LEFT);
    } else if (sound_3000 - sound_300 > diff && sound_3000 > 200){
      turn(RIGHT);
    }
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
  /*
  Serial.print("Red: ");
  Serial.print(colourArray[0]);
  Serial.print("Green: ");
  Serial.print(colourArray[1]);
  Serial.print("Blue: ");
  Serial.println(colourArray[2]);
  */
  if(total > 1500){
    Serial.println("WHITE");
    turn(U_TURN);
  } else if (total < 100){
    Serial.println("BLACK");
    end();
  } else if (colourArray[2] > colourArray[0] && colourArray[2] > colourArray[1]){
    Serial.println("BLUE");
    turn(RIGHTRIGHT);
  } else if (colourArray[1] > colourArray[0] && colourArray[1] > colourArray[2]){
    Serial.println("GREEN");
    turn(RIGHT);
  } else if (colourArray[0] > colourArray[1] && colourArray[0] > colourArray[2]){
    total = colourArray[1] + colourArray[2];
    Serial.print("TOTAL :");
    Serial.print(total );
    Serial.print(" ");
    Serial.print(colourArray[0]);
    Serial.print(" ");
    Serial.print(colourArray[1]);
    Serial.print(" ");
    Serial.println(colourArray[2]);
    if(total < ORANGE_RED){//was 280
      Serial.println("RED");
      turn(LEFT);
    } else if (total >= ORANGE_RED){ //was 280
      Serial.println("ORANGE");
      turn(LEFTLEFT);
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

void detect_line() {
  if (linefollower_2.readSensors() != 3) {
    motor_left.run(0);
    motor_right.run(0);
    delay(200);
    motor_left.run(150);
    motor_right.run(-150);
    delay(175);
    motor_left.run(0);
    motor_right.run(0);
    delay(150);
    check_challenge();
    }
}

void move_straight() {
  detect_line(); 
  rgbled_7.setColor(0,0,0,0);
  rgbled_7.show();
  float ir_left = (analogRead(A1) / 1023.000 * 5);
  float ir_right = (analogRead(A0) / 1023.000 * 5);
  //Serial.print(ir_left);
  //Serial.print(" ");
  //Serial.print(ir_right);
  //Serial.print(" ");
  //Serial.println( ir_left - ir_right);
  delay(20);
  if (ir_left - ir_right < ADJUST && ir_left - ir_right > -ADJUST - 0.05) {
  //  rgbled_7.setColor(0,0,0,0);
    rgbled_7.show();
    motor_left.run(-255);
    motor_right.run(255);
  } else if (ir_left - ir_right >= ADJUST) { //adjust left
  //  rgbled_7.setColor(0,0,255,0);
    rgbled_7.show();
    motor_left.run(-125);
    motor_right.run(255);
  } else if (ir_left - ir_right <= -ADJUST - 0.05) {//adjust left
 //   rgbled_7.setColor(0,255,0,0);
    rgbled_7.show();
    motor_left.run(-255);
    motor_right.run(125);
  }
}

void loop() {
  move_straight();
}

void end(){
  while(1);
}