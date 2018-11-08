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