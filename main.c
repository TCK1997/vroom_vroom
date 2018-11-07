#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <MeMCore.h>

#define COLOUR_SAMPLE_SIZE 10 //cannot exceed 128
#define LDR_WAIT
#define ADJUST 0.2

MeDCMotor motor_left(9); //left
MeDCMotor motor_right(10);    //right

void setup() {

}

void turn(int direction) {

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
  check_challenge();
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
    motor_left.run(-100);
    motor_right.run(255);
  } else if (ir_left - ir_right <= -ADJUST - 0.05) {//adjust left
    rgbled_7.setColor(0,255,0,0);
    rgbled_7.show();
    motor_left.run(-255);
    motor_right.run(100);
  }
}

void loop() {
  move_straight();
}
