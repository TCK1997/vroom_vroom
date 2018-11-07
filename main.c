#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <MeMCore.h>

#define COLOUR_SAMPLE_SIZE 10 //cannot exceed 128
#define LDR_WAIT
#define ADJUST 0.2
#define ULTRASONIC_STOP 580
#define LEFT 1
#define RIGHT 2
#define LEFTLEFT 3
#define RIGHTRIGHT 4
#define ULTRASONIC 12

MeDCMotor motor_left(9); //left
MeDCMotor motor_right(10);    //right'
double angle_rad = PI/180.0;
double angle_deg = 180.0/PI;
MeRGBLed rgbled_7(7, 7==7?2:4);
MeUltrasonicSensor ultrasonic_3(3);
MeLineFollower linefollower_2(2);
MeLightSensor lightsensor_6(6);

void setup() {
  pinMode(ULTRASONIC, OUTPUT);
}

void turn(int direction) {
  if (direction == LEFT) {
    motor_left.run(255);
    motor_right.run(255);
    delay(350);
    motor_left.run(-255);
    motor_right.run(255);
    delay(600);
  }
  if (direction == RIGHT) {
    motor_left.run(-255);
    motor_right.run(-255);
    delay(350);
    motor_left.run(-255);
    motor_right.run(255);
    delay(600);
  }
  if (direction == LEFTLEFT) {
    motor_left.run(255);
    motor_right.run(255);
    delay(270);
    motor_left.run(-255);
    motor_right.run(255);
    delay(200);
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
      delay(20);
    }
    rgbled_7.setColor(0,0,0,0);
    rgbled_7.show();
    motor_left.run(255);
    motor_right.run(255);
    delay(350);
    motor_left.run(-255);
    motor_right.run(255);
    delay(600);
  }
  if (direction == RIGHTRIGHT) {
    motor_left.run(-255);
    motor_right.run(-255);
    delay(270);
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
      delay(20);
    }
    rgbled_7.setColor(0,0,0,0);
    rgbled_7.show();
    motor_left.run(-255);
    motor_right.run(-255);
    delay(350);
    motor_left.run(-255);
    motor_right.run(255);
    delay(600);
  }
}

void colour() {

}

void sound() {

}

void check_challenge() {
  if (true) {
    sound();
  } else {
    colour();
  }
}

void detect_line() {
  if (linefollower_2.readSensors() == 0) {
    motor_left.run(0);
    motor_right.run(0);
    delay(300);
    turn(LEFTLEFT);
  }
  //check_challenge();
}


void move_straight() {
  detect_line();
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
    motor_left.run(-255);
    motor_right.run(255);
  } else if (ir_left - ir_right >= ADJUST) { //adjust left
    rgbled_7.setColor(0,0,255,0);
    rgbled_7.show();
    motor_left.run(-125);
    motor_right.run(255);
  } else if (ir_left - ir_right <= -ADJUST - 0.05) {//adjust left
    rgbled_7.setColor(0,255,0,0);
    rgbled_7.show();
    motor_left.run(-255);
    motor_right.run(125);
  }
}

void loop() {
  move_straight();
}
