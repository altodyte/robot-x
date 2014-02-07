//******************RB2_Example_1**************

/*This program provides an example of four features of the Arbotix_m
  These are:
    1.  Digital Input using a momentary switch
    2.  Digital Output using a LED
    3.  Analog Input use a 10K potentiometer 
    4.  RC servo control
*/

//*************************************************

#include <Servo.h>  //for use with RC servos

//*********************Globals**********************
Servo servo_one;        //create an RC servo object

boolean ledON;               //use to toggle onboard led
int servo_one_pos = 1500;   //inital value for the servo position in microseconds

//********************Mapping of Controller Board Pins***********
//
//**************Mapping of DIO pins on the Arbotix-M*************
int led_pin = 0;        //User LED on the Arbotix-M
int blink_pin = 1;      //remote LED
int switch_pin = 23;    //momentary switch is on pin D23

//*****************Mapping of Analog Input pins*******************
int pot_pin = 2;        //10K pot is at Analog input pin A2

//****************Mapping of RC Servo Devices********************
int servo_one_pin = 15;    //Servo One is on pin D15

//*****************************************************************

//***************************The Setup Loop************************
void setup(void)
{
  //setup the digital input pins
  pinMode(switch_pin, INPUT);      //set pin as input
  digitalWrite(switch_pin, HIGH);  //turn on pullup resistor for switch
  
  //setup the digital output pins
  pinMode(led_pin, OUTPUT);        //initalize the pin as an output
  pinMode(blink_pin, OUTPUT);      //initalize the pin as an output
  
  //setup the RC servo
  servo_one.attach(servo_one_pin);
  servo_one.writeMicroseconds(servo_one_pos);
  
  //setup up the on board led
  ledON = false;
  
}//end setup
//***********************The Loop function*************************
void loop(void)
{
  
  float pot_volts;          //the latest value of the pot in volts
  boolean switch_state;   //the latest state of the switch
  
  //toggle the on board led
  Toggle_led();
  
  //*********************  
  //*****Sense***********
  //*********************
  
  //read the pot value
  pot_volts = Read_pot();
  
  //read the value of the switch
  switch_state = digitalRead(switch_pin);

  //********************
  //*****Think**********
  //********************
  
  //nothing to do for this demo

  //********************  
  //*****Act************
  //********************
  
  //set the state of the external LED
  //if switch is LOW turn on the LED
  if (switch_state == false)
  {
    digitalWrite(blink_pin, HIGH);
  }
  else
  {
    digitalWrite(blink_pin, LOW);
  }
  
  //move the servo by the value of the pot
  //convert the volts into microseconds and move the servo
  servo_one_pos = pot_volts * 200.0 + 1000.0;
  servo_one.writeMicroseconds(servo_one_pos);
  
  
  //*************************
  //set the loop timing
  delay(100);
  
}//end loop
//*******************************************
//**************Subroutines*****************
//******************************************
void Toggle_led(void)
{
  //This loop changes the state of the on board led each time it is called
  //It uses the global variable ledON store the state between calls
  
  //toggle the led on the board
  if (ledON == false)
  {
    ledON = true;
    digitalWrite(led_pin, HIGH);   // turn the LED on (HIGH is the voltage level)
  }  
  else
  {
    ledON = false;
    digitalWrite(led_pin, LOW);    // turn the LED off by making the voltage LOW
  }
}//end Toggle_led
//***************************************************************
float Read_pot(void)  //returns the value as volts
{
  //This routine reads the potentiometer voltage and
  //This routine assumes the 5.0 VDC voltage is the reference
  //being used on the Arbotix-M board, the default
  
  int pot_value;
  float pot_volts;
  
  //read the value of the pot as an integer 0 to 1023
  pot_value = analogRead(pot_pin);
  
  //convert to volts
  pot_volts = (float)pot_value * 5.0 / 1024;
  
  return pot_volts;
  
}//end Read_pot
//the end ************************************** 

