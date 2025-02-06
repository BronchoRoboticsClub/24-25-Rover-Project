// The purpose of this code is to keep the servos center at ~90 degrees. This allows the rover the ability to tank drive.
#include <Servo.h>

/*
F: Front
M: Middle
B: Back
R: Right
L: Left
S: Servo
*/


//Pin Locations, change the number based on where the servo is connected to the arduino
#define FRS 3   //PCB 7
#define MRS 5   //PCB 6
#define BRS 6   //PCB 8
#define FLS 9   //PCB 5
#define MLS 10  //PCB 3
#define BLS 11  //PCB 4

//Servo definitions, decalres the names as servos in the servo library.
Servo FrontRightServo;
Servo MiddleRightServo;
Servo BackRightServo;
Servo FrontLeftServo;
Servo MiddleLeftServo;
Servo BackLeftServo;

//Runs when the arduino is powered on.
void setup() 
{
  //Sets the pre-defined servos location the pre-determined pins.
  FrontRightServo.attach(FRS);
  MiddleRightServo.attach(MRS);
  BackRightServo.attach(BRS);
  FrontLeftServo.attach(FLS);
  MiddleLeftServo.attach(MLS);
  BackLeftServo.attach(BLS);
}

void loop() 
{
  //Declares a variable that all servos set to.
  int center = 90;
  
  //The servos are customized based on the desired center to center themselves
  FrontRightServo.write(center - 4);
  MiddleRightServo.write(center + 5);
  BackRightServo.write(center + 3);
  FrontLeftServo.write(center + 3);
  MiddleLeftServo.write(center + 1);
  BackLeftServo.write(center + 6);
  //Waits for 100ms to not overload the servos with constant updates
  delay(100);
}
