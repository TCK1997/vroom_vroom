#define COLOUR_SAMPLE_SIZE 10 //cannot exceed 128
#define LDR_WAIT
#define ADJUST 0.2
#define ULTRASONIC_STOP 580
#define LEFT 1
#define RIGHT 2
#define LEFTLEFT 3
#define RIGHTRIGHT 4
#define U_TURN 5
#define ULTRASONIC 12
#define RGBWait 150 //in milliseconds 
#define LDRWait 10 //in milliseconds

MeDCMotor motor_left(9); //left
MeDCMotor motor_right(10);    //right'
MeRGBLed rgbled_7(7, 7==7?2:4);
MeUltrasonicSensor ultrasonic_3(3);
MeLineFollower linefollower_2(2);
MeLightSensor lightsensor_6(6);

int colourArray[] = {0,0,0};
int blackArray[] = {392,306,330};
MeRGBLed rgbled_7(7, 7==7?2:4);
MeLightSensor lightsensor_6(6);
int colours[3][3] = {
  {255,0,0},
  {0,255,0},
  {0,0,255}
};