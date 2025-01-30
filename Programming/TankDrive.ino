//Define the motors to their respective pins
#include <math.h>
#define FrontRightMotorDirection 2
#define FrontRightMotorPower 3

#define FrontLeftMotorDirection 4
#define FrontLeftMotorPower 5

#define BackRightMotorPower 6
#define BackRightMotorDirection 7

#define BackLeftMotorDirection 8
#define BackLeftMotorPower 9

#define MiddleRightMotorPower 11
#define MiddleRightMotorDirection 13

#define MiddleLeftMotorPower 10
#define MiddleLeftMotorDirection 12

#define num 64    //Sets the varible num to a maximum number
int deadZone = 20, endZone = 200, center = 90;    //Creates varibles for various code uses
byte inputs[num] = {}, header[4] = {255, 254, 253, 252};    //Creates an array for the input variables coming from the controller

//creates a structure that stores all the following controller information under the name PS4
struct Controller
{
  double LStickX;
  double LStickY;
  double RStickX;
  double RStickY;
  int8_t Square;
  int8_t Cross;
  int8_t Circle;
  int8_t Triangle;
  int8_t RTrigger;
} PS4;

void TankDrive()
{
  //If the left joystick is either positive or negative set the direction of the left motors to forwards or backwards respectively 
  if (PS4.LStickY >= 0)
  {
    digitalWrite(FrontLeftMotorDirection, HIGH);
    digitalWrite(MiddleLeftMotorDirection, HIGH);
    digitalWrite(BackLeftMotorDirection, HIGH);
  }
  else
  {
    digitalWrite(FrontLeftMotorDirection, LOW);
    digitalWrite(MiddleLeftMotorDirection, LOW);
    digitalWrite(BackLeftMotorDirection, LOW);
  }

  //If the right joystick is either positive or negative set the direction of the right motors to forwards or backwards respectively 
  if (PS4.RStickY >= 0)
  {
    digitalWrite(FrontRightMotorDirection, HIGH);
    digitalWrite(MiddleRightMotorDirection, HIGH);
    digitalWrite(BackRightMotorDirection, HIGH);
  }
  else
  {
    digitalWrite(FrontRightMotorDirection, LOW);
    digitalWrite(MiddleRightMotorDirection, LOW);
    digitalWrite(BackRightMotorDirection, LOW);
  }

  //If the trigger is held then the rover goes half speed
  if (PS4.RTrigger != 0)
  {
    PS4.LStickY /= 2;
    PS4.RStickY /= 2;
  }

  //Sets the motots speed equal to the displacement of the joysticks
  analogWrite(FrontLeftMotorPower, abs(PS4.LStickY));
  analogWrite(MiddleLeftMotorPower, abs(PS4.LStickY));
  analogWrite(BackLeftMotorPower, abs(PS4.LStickY));
  analogWrite(FrontRightMotorPower, abs(PS4.RStickY));
  analogWrite(MiddleRightMotorPower, abs(PS4.RStickY));
  analogWrite(BackRightMotorPower, abs(PS4.RStickY));
}

void controllerData()    //The function definition for correcting the joysticks
{
  //The purpose 
  if (Serial.available() > 8)     //Checks to see if the Esp32 is sending more than 8 pieces of information this limits overflow
  {
    int numBytes = Serial.readBytes(inputs, Serial.available()), h = 0, i;      //creates needed variables and reads the serial from the Esp32
    for (i = 0; i < numBytes; i++)      //Loops through the entire information sent from the Esp32
    {
      if (inputs[i] == header[h])     //If the current input is equal to the current header
      {
        if (h == 3)   //If the full header has been found
        {
          i++;      //goes to the next piece of information
          break;      //Leaves the loop
        }
        h++;      //If header matches and still in the loop then increment header
      }
      else      //If the current information is not the same as the header 
        h = 0;      //reset the header count
    }
    if (i < numBytes - 3)     //Checks after clearing the header to see if there is enough information from the controller
    {
      //Stores the controller information while mapping the joysticks to the motor levels
      Controller PS4 = {map(inputs[i], -128, 128, -255, 255), 
      map(inputs[i + 1], -128, 128, -255, 255), 
      map(inputs[i + 2], -128, 128, -255, 255), 
      map(inputs[i + 3], -128, 128, -255, 255), 
      inputs[i + 4], inputs[i + 5], inputs[i + 6], inputs[i + 7], inputs[i + 8]};
      Serial.flush();
    }

    //Sets the values of the joysticks to 0 if they fall in the pre-defined deadzone
    if (PS4.LStickX <= deadZone && PS4.LStickX >= -deadZone)
      PS4.LStickX = 0;
    if (PS4.LStickY <= deadZone && PS4.LStickY >= -deadZone)
      PS4.LStickY = 0;
    if (PS4.RStickX <= deadZone && PS4.RStickX >= -deadZone)
      PS4.RStickX = 0;
    if (PS4.RStickY <= deadZone && PS4.RStickY >= -deadZone)
      PS4.RStickY = 0;
    /*      Might not be necessary since the variable are mapped with min and maxes
    if (PS4.LStickX > endZone)
      PS4.LStickX = 255;
    if (PS4.LStickY > endZone)
      PS4.LStickY = 255;
    if (PS4.RStickX > endZone)
      PS4.RStickX = 255;
    if (PS4.RStickY > endZone)
      PS4.RStickY = 255;
    if (PS4.LStickX < -endZone)
      PS4.LStickX = -255;
    if (PS4.LStickY < -endZone)
      PS4.LStickY = -255;
    if (PS4.RStickX < -endZone)
      PS4.RStickX = -255;
    if (PS4.RStickY < -endZone)
      PS4.RStickY = -255;
    */
  }
}

//Runs when the arduino is powered on
void setup()
{
  Serial.begin(9600);   //Sets the baud rate to 9600 which is typical for arduino
  //declares the pinMode for the motors
  pinMode(FrontRightMotorPower, OUTPUT);
  pinMode(MiddleRightMotorPower, OUTPUT);
  pinMode(BackRightMotorPower, OUTPUT);
  pinMode(FrontLeftMotorPower, OUTPUT);
  pinMode(MiddleLeftMotorPower, OUTPUT);
  pinMode(BackLeftMotorPower, OUTPUT);
  pinMode(FrontRightMotorDirection, OUTPUT);
  pinMode(MiddleRightMotorDirection, OUTPUT);
  pinMode(BackRightMotorDirection, OUTPUT);
  pinMode(FrontLeftMotorDirection, OUTPUT);
  pinMode(MiddleLeftMotorDirection, OUTPUT);
  pinMode(BackLeftMotorDirection, OUTPUT);
}

void loop()
{
  controllerData();     //Runs the function to gather information from the controller
  TankDrive();      //Runs the function to drive the rover with the given controller information
}
