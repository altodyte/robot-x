//******************RB2_Example_2**************

/*This program provides an example of four features of the Arbotix_m
  These are:
    1.  Digital Input using a momentary switch
    2.  Digital Output using a LED
    3.  Analog Input use a 10K potentiometer 
    4.  RC servo control
    
  It expands RB2_Example_1 to include communication with the PC using an XBEE radio link.
  In this example the LED and the Serve will be controlled from the PC using LabVIEW
  While the switch and the pot will change indicators on a LabVIEW front panel.
*/

//*************************************************

#include <Servo.h>  //for use with RC servos

//*********************Globals**********************
Servo servo_one;            //create an RC servo object

boolean ledON;              //use to toggle onboard led
unsigned long newtime;      //the time at the start of loop()
unsigned long timeperiod;   //time for loop() in milliseconds.
int servo_one_pos = 1500;   //inital value for the servo position in microseconds
float pot_volts;            //the latest value of the pot in volts
boolean switch_state;       //the latest state of the switch
boolean blinkON;            //use to toggle the external LED

const int buffer_size = 40;
char readbuffer[buffer_size];    //a buffer to receive a character string
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
  // start serial port at 38400 bps:
  Serial.begin(38400);
  
  // wait to connect
  delay(1000);
  
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
  timeperiod = 100;    //loop time in milliseconds
  
}//end setup
//***********************The Loop function*************************
void loop(void)
{

  //use the computer clock to create a timed loop, read the present time to start the timer
  newtime = millis();
  
  //*********************  
  //*****Sense***********
  //*********************
  
  //read the pot value
  pot_volts = Read_pot();
  
  //read the value of the switch
  switch_state = digitalRead(switch_pin);
  
  //Read the data bytes from the PC
  int numread = 0;
  numread = Read_Command_String();
  
  //send a response in bytes to the PC if one has been read
  if (numread > 0)
  {
    Send_Return_String(numread);
  }
  
  //Parse the Command_String
  Parse_command_string();


  //********************
  //*****Think**********
  //********************
  
  //nothing to do for this demo

  //********************  
  //*****Act************
  //********************
  
  //set the state of the external LED
  //if passed value is 0 turn off the LED
  if (blinkON == false)
  {
    digitalWrite(blink_pin, HIGH);
  }
  else
  {
    digitalWrite(blink_pin, LOW);
  }
  
  //move the servo by the value passed
  servo_one.writeMicroseconds(servo_one_pos);
    
  //*************************
  //use computer clock to time the loop
   while(millis() < (newtime + timeperiod) )
  {
    //wait until time elaspes
  }

  //toggle the on board led
  Toggle_led();
  
}//end loop
//*******************************************
//**************Subroutines*****************
//******************************************
int Read_Command_String(void)  //returns number of bytes found
{
  //read the string as an array of type char
  int num = 0;  //number of bytes read
  if ( Serial.available() > 0)
  {
    //read until the '\n' is found
    num = Serial.readBytesUntil('\n', readbuffer, buffer_size);
  } 
  
  return num;
}//end Read_Command_String
//****************************************
void Send_Return_String(int numread)
{
  //send the response as a byte array
  byte returnbuffer[3];
    
  //if something has been sent, send a return
  //The return has the form $RV-----S-'\n'  11 bytes
  if (numread > 0)
  {
    returnbuffer[0] = 0x24;  //$
    returnbuffer[1] = 0x52;  //R
    returnbuffer[2] = 0x56;  //V
    
    Serial.write(returnbuffer, 3);       //return the bytes read
    Serial.print(pot_volts,3);           //sent the Pot voltage value
    Serial.write(0x53);                  //S
    Serial.print(switch_state, DEC);     //a boolean for the switch state
    Serial.write(0x0A);                  //a newline, \n
  }
  
  return;
}// end Send_Return_String
//*****************************************
void Parse_command_string(void)
{
  //the following converts the char number values into integers
  //the readbuffer is assumed to be $CS----L-'\n' 10 bytes
  char dummy[4];    
  int dummy_int;
  
  //for servo
  dummy[0] = readbuffer[3];
  dummy[1] = readbuffer[4];
  dummy[2] = readbuffer[5];
  dummy[3] = readbuffer[6];
  
  servo_one_pos = atoi(dummy);    //convert char/byte array into interger
  
  //for LED 
  dummy_int = readbuffer[8];     
  //convert the ascii dec representation of 0 into boolean   
  if (dummy_int == 48)
  {
    blinkON = false;
  }
  else
  {
    blinkON = true;
  }
  
   
}//end Parse_commmand_string
//*********************************************************
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

