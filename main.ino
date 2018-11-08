#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <MeMCore.h>
#include "config.c"
#include "turn.c"
#include "colour.c"
#include "sound.c"

void setup() {
  pinMode(ULTRASONIC, OUTPUT);
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
    colour();
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
