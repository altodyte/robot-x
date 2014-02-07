/***************************
 * First_program
 * This sketch sends positional commands to the AX servo 
 * attached to it - the servo must set to ID # 1
 * The sketch will send a value, i, to the servo.
 * The LED is lit when the servo increases position and off for decreasing position
 * 'For' loops are used to increment and decrement the value of 'i'
 ***************************/

//************************************************************
//import ax12 library to send DYNAMIXEL commands
//is found in the library Bioloid
#include <ax12.h>

// Pin 0 maps to the USER LED on the ArbotiX Robocontroller.
int led = 0;

//****************************************************
// the setup routine runs once when you press reset:
void setup()
{
    // initialize the digital pin as an output.
    pinMode(led, OUTPUT);
    
    SetPosition(1,0); //set the position of servo # 1 to '0'
    delay(100);//wait for servo to move
}

//********************************************************
void loop()
{
  digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
  //increment from 0 to 1023
  for(int i=0;i<1024;i++)
  {
    SetPosition(1,i); //set the position of servo #1 to the current value of 'i'
    delay(10);//wait for servo to move
  }
  digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
  //decrement from 1024 to 0
  for(int i=1024;i>0;i--)
  {
    SetPosition(1,i);//set the position of servo #1 to the current value of 'i'
    delay(10);//wait for servo to move
  }
}



