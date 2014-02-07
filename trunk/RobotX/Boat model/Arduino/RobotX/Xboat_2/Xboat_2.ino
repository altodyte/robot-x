//*****Xboat_2************

/*This is the operating program for the model of the RobotX boat.
  It is based upon the Arbotix-M controller PCD and Arduion Software.
  It assumes that theRE is a program running on a PC, likely written in LabVIEW.
  
  The communication between the controller and the PC is done via XBEE devices.
  They are set to 38400 baud, 8, N, 1, N and have a preset protocol.
  
  The PC initiates all communication and expects a return confirming operation.
*/  
//***************************************************************************

#include <Servo.h>  //for RC servos
#include <ax12.h>  //for Dynamixel servos

//****************************GLOBALS*****************************************
boolean ledON;               //used to toggle the led
unsigned long newtime;       //the time at the start of loop()
unsigned long timeperiod;    //time for loop() in milliseconds.
int estop_val;               //etop_val == LOW is stopped
int loop_count = 0;          //number of loop() run

//create RC servo objects
Servo starboard_motor;
Servo port_motor;
Servo right_rudder;
Servo left_rudder;

int sm_pos = 1500;    //starboard motor position 0 to 2500
int pm_pos = 1500;    //port motor position
int rr_pos = 1500;    //right rudder position
int lr_pos = 1500;    //left rudder position
int cp_pos = 1500;    //camera pan position
int h_code = 1;      //Status code from PC 1 == OK, -100 == ESTOP
float bat_volt = 8.00;  //Lipo battery voltage
float ir_volt = 1.00;  //IR distance voltage

char readbuffer[40];
//******************MAPPING OF CONTROLLER BOARD PINS***************************
//
//*******************MAPPING OF DIO PINS *************************************
int led_pin = 0;   //USER LED on the ArbotiX Robocontroller, the blinking light
int ESTOP_pin = 23;     //manual switch on the model...stops all servos
//******************MAPPING OF ANALOG INPUT PINS******************************
int bat_pin = 1;   //Lipo battery voltage
int ir_pin = 2;    //Sharp IR distance device
//******************MAPPING OF RC SERVO DEVICES*****************************
int sm_pin = 12;  //starboard motor speed
int pm_pin = 13;  //port motor speed
int rr_pin = 14;  //right rudder position
int lf_pin = 15;  //left rudder position
//********************MAPPING OF DYNAMIXEL SERVO DEVICES**********************
int cp_num = 1;  //the camera panning servo number
//****************************************************************************

//***********************THE SETUP LOOP**************************************
void setup()
{
  // start serial port at 38400 bps:
  Serial.begin(38400);
  
  // wait to connect
  delay(1000);

  //for the Dynamixel serovs
  SetPosition(1,0); //set the position of servo # 1 to '0'

  //setup up the RC servos 
  starboard_motor.attach(sm_pin);
  starboard_motor.writeMicroseconds(sm_pos);
  port_motor.attach(pm_pin);
  port_motor.writeMicroseconds(pm_pos);
  
  //setup input pins
  pinMode(ESTOP_pin, INPUT);           // set pin to input
  digitalWrite(ESTOP_pin, HIGH);       // turn on pullup resistors, 20k resitor to 5VDC on input pin
  
  //setup output pins
  pinMode(led_pin, OUTPUT);  // initialize the digital pin as an output for onboard LED
  
  //setup the timing and blinky led
  ledON = false;        //used to toggle the board led with loop timer
  timeperiod = 100;    //loop time in milliseconds
  
}// end setup
//*************************THE MAIN PROGRAM LOOP******************************
void loop()
{
  
  loop_count = loop_count + 1;
  
  //use the computer clock to create a timed loop, read the present time to start the timer
  newtime = millis();
  
  //read the value of the ESTOP switch, value is LOW when button is pressed, otherwise HIGH
  estop_val = digitalRead(ESTOP_pin);

  //Read the sensor values
  Read_sensors();
  
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
  
  //Run the arbiter
  
  //Move the motors
  Move_the_motors();
  
  //Move the rudders

  
  //Move the camera
  Move_the_camera();  
  
  //*********************************
  //use computer clock to time the loop
 
  while(millis() < (newtime + timeperiod) )
  {
    //wait until time elaspes
  }
  
  //change the state of the on board LED
  Toggle_led();
  
}// end void
//****************************************************

//*************SUBROUTINES******************************
int Read_Command_String(void)  //returns number of bytes found
{
  //As LabVIEW has no type CHAR, and C has no type STRING use bytes!!!!
  //read the string as an array of type byte
  
  int num = 0;  //number of bytes read
  
  if ( Serial.available() > 0)
  {
    num = Serial.readBytesUntil('\n', readbuffer, 40);
  } 
  
  return num;
}//end Read_Command_String
//****************************************
void Send_Return_String(int numread)
{
  //send the response as a byte array
  byte returnbuffer[3];
  
  
  //if something has been sent, send a return
  if (numread > 0)
  {
    returnbuffer[0] = 0x24;  //$
    returnbuffer[1] = 0x52;  //R
    returnbuffer[2] = 0x0A;  // \n
    
    Serial.write(returnbuffer, 3);  //return the bytes read
  }
  
  return;
}// end Send_Return_String
//******************************************
void Toggle_led(void)
{
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
//*******************************************************
void Move_the_motors(void)
{
  //move the motors
  
  //check ESTOP, value is low when button is pressed
  //and the passed stop value h_code
  if (estop_val == LOW || h_code == -100)
  {
    //stop the motors
    starboard_motor.writeMicroseconds(1500);
    port_motor.writeMicroseconds(1500);
  }
  else
  {
    //move the motors to the passed positions
    starboard_motor.writeMicroseconds(sm_pos);
    port_motor.writeMicroseconds(pm_pos);
  }
}//end Move_the_motors
//******************************************************
void Read_sensors(void)
{
  //read the Sharp IR distance sensor
  int ir_value;
  ir_value = analogRead(ir_pin);
  
  //convert to volts
  ir_volt = (float)ir_value * 5.0 / 1024.0;

}//end Read_sensors
//********************************************************
void Parse_command_string(void)
{
  char dummy[4];    
  dummy[0] = readbuffer[3];
  dummy[1] = readbuffer[4];
  dummy[2] = readbuffer[5];
  dummy[3] = readbuffer[6];
  
  sm_pos = atoi(dummy);
      
  dummy[0] = readbuffer[8];
  dummy[1] = readbuffer[9];
  dummy[2] = readbuffer[10];
  dummy[3] = readbuffer[11];
  
  pm_pos = atoi(dummy);

  dummy[0] = readbuffer[13];
  dummy[1] = readbuffer[14];
  dummy[2] = readbuffer[15];
  dummy[3] = readbuffer[16];
  
  rr_pos = atoi(dummy);
  
  dummy[0] = readbuffer[18];
  dummy[1] = readbuffer[19];
  dummy[2] = readbuffer[20];
  dummy[3] = readbuffer[21];
  
  lr_pos = atoi(dummy);

  dummy[0] = readbuffer[23];
  dummy[1] = readbuffer[24];
  dummy[2] = readbuffer[25];
  dummy[3] = readbuffer[26];
  
  cp_pos = atoi(dummy);
/*  
  dummy[0] = readbuffer[28];
  dummy[1] = readbuffer[29];
  dummy[2] = readbuffer[30];
  dummy[3] = readbuffer[31];
  
  h_code = atoi(dummy);
*/
  
}//end Parse_commmand_string

//*********************************************************
void Move_the_camera(void)
{
  float m = 0.51;
  float b = -255.0;
  float cp_value;
  
  //calculate the camera position of 0 to 1023;
  cp_value = m * (float)cp_pos + b;
  
  //check ESTOP, value is low when button is pressed
  //and the passed stop value h_code
  if (estop_val == LOW || h_code == -100)
  {
    //leave the camera alone
    
  }
  else
  {
    //move the camera to the passed positions
    SetPosition(1, (int)cp_value);
  }
}//end Move_rudders
//************* end of program****************************

