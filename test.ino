#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <MeMCore.h>

//colour config
#define COLOUR_SAMPLE_SIZE 10 //cannot exceed 128
#define LDR_WAIT
#define RGBWait 150 //in milliseconds 
#define LDRWait 10 //in milliseconds
#define ORANGE_RED 230 //was 658

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
#define nosound 200
#define diff 100
#define sound_300 A2
#define sound_3000 A3

//global variable
MeDCMotor motor_left(9); //left
MeDCMotor motor_right(10);    //right'
MeRGBLed rgbled_7(7, 7==7?2:4);
MeUltrasonicSensor ultrasonic_3(3);
MeLineFollower linefollower_2(2);
MeLightSensor lightsensor_6(6);
int colourArray[] = {0,0,0};
int blackArray[] = {392,306,330};
int ambient = 0;
int colours[3][3] = {
  {255,0,0},
  {0,255,0},
  {0,0,255}
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
  while(1);
}


void turn(int direction) {
  if (direction == LEFT) {
    rgbled_7.setColor(0,255,0,0); //shine red on left
    rgbled_7.show();
    motor_left.run(255);
    motor_right.run(255);
    delay(LEFT_DELAY);
    motor_left.run(0);
    motor_right.run(0);
    delay(600);
    rgbled_7.setColor(0,0,0,0);
    rgbled_7.show(); //off after move straight delay
  }
  if (direction == RIGHT) {
    motor_left.run(-255);
    motor_right.run(-255);
    delay(RIGHT_DELAY);
    motor_left.run(0);
    motor_right.run(0);
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
    delay(LEFT_DELAY + 80); //WAS 350
    motor_left.run(0);
    motor_right.run(0);
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
    delay(RIGHT_DELAY + 90); //WAS 350
    motor_left.run(0);
    motor_right.run(0);
    delay(600);
  }
  if (direction == U_TURN) {
    rgbled_7.setColor(0,0,0,0);
    rgbled_7.show();

    motor_left.run(-180);
    motor_right.run(-225);
    delay(560);
    motor_left.run(0);
    motor_right.run(0);
    delay(600);
    accelerate();   //added 
    
    //motor_left.run(0);
    //motor_right.run(0);
  }
  accelerate();
}

void check_challenge() {
  if(analogRead(sound_3000) > 100){  
    sound();
  } else {
    colour();
  }
}

void sound() {
  if (analogRead(sound_300) > 240 && analogRead(sound_3000) < 220) {
    turn(LEFT);
    return;
  }
  if (analogRead(sound_300) < 250 && analogRead(sound_3000) > 250) {
    turn(RIGHT);
    return;
  }
  if (analogRead(sound_300) > 240 && analogRead(sound_3000) > 200) {
    turn(U_TURN);
  }
}


void colour(){
  //ambient_light
  rgbled_7.setColor(0,0,0,0);
  rgbled_7.show();
  delay(RGBWait);
  ambient = getAvgReading(5);
  Serial.print("AMBIENT :");
  Serial.println(ambient);
  
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
    total = colourArray[1] + colourArray[2] - ambient;
    Serial.print("TOTAL :");
    Serial.print(total );
    Serial.print(" ");
    Serial.print(colourArray[0]);
    Serial.print(" ");
    Serial.print(colourArray[1]);
    Serial.print(" ");
    Serial.println(colourArray[2]);
    if (cheatswitch == 1) {
      if(total < ORANGE_RED){//was 280
        Serial.println("RED");
        turn(LEFT);
      } else if (total >= ORANGE_RED){ //was 280
        Serial.println("ORANGE");
        turn(LEFTLEFT);
      }
    } else {
      turn(LEFT);
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
    /*motor_left.run(150);
    motor_right.run(-150);
    delay(175);
    motor_left.run(0);
    motor_right.run(0);
    delay(150);
   
    */check_challenge();
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
  if (ir_right >= 3.25 && ir_left >= 2.70 || ir_left >= 3.30 && ir_right >= 2.70){
    rgbled_7.setColor(0,0,0,60); //no walls(blue)
    rgbled_7.show();
    motor_left.run(-255);
    motor_right.run(245);
  } else if (ir_left - ir_right < ADJUST && ir_left - ir_right > -ADJUST - 0.05) {
  //  rgbled_7.setColor(0,0,0,0);
    rgbled_7.show();
    motor_left.run(-255);
    motor_right.run(245);
  } else if (ir_left - ir_right >= ADJUST) { //adjust left
    rgbled_7.setColor(0,0,0,0);
    rgbled_7.show();
    motor_left.run(-150);//was 125
    motor_right.run(255);
  } else if (ir_left - ir_right <= -ADJUST - 0.05) {//adjust right
    rgbled_7.setColor(0,0,0,0);
    rgbled_7.show();
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
      Serial.println("cheat switch is now on");
      cheatswitch = 1;
      rgbled_7.setColor(0,255,165,0);
      rgbled_7.show();
    }
    else { 
      Serial.println("cheat switch is now off");
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
