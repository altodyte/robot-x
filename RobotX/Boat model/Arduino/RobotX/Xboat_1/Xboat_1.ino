//*****Xboat_1************

/*This is the operating program for the model of the RobotX boat.
  It is based upon the Arbotix-M controller PCD and Arduion Software.
  It assumes that theRE is a program running on a PC, likely written in LabVIEW.
  
  The communication between the controller and the PC is done via XBEE devices.
  They are set to 38400 baud, 8, N, 1, N and have a preset protocol.
  
  The PC initiates all communication and expects a return confirming operation.
*/  
//*********************************************************************************e <Servo


/****************************GLOBALS*****************************************
boolean ledON;               //used to toggle the led
unsigned long newtime;       //the time at the start of loop()
unsigned long timeperiod;    //time for loop() in milliseconds.
int estop_val;               //ESTOP value

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
float bat_volt = 8.00;  //Lipo battery voltage
float ir_volt = 1.00;  //IR distance voltage

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
  
  //use the computer clock to create a timed loop, read the present time to start the timer
  newtime = millis();
  
  //read the value of the ESTOP switch, value is LOW when button is pressed, otherwise HIGH
  estop_val = digitalRead(ESTOP_pin);

  //Read the sensor values
  Read_sensors();
  
  //Read the data bytes from the PC
  int numread;
  numread = Read_Command_String();
  
  delay(1);

  //send a response in bytes to the PC
  Send_Return_String(numread);
  
  //Run the arbiter
  
  //Move the motors
  Move_the_motors();
  
  //Move the rudders
  
  //Move the camera
  
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
int Read_Command_String(void)
{
  //As LabVIEW has no type CHAR, and C has no type STRING use bytes!!!!
  //if there are bytes, read them all
  int i;
  byte readbuffer[10];
  int num;  //number of bytes read

  num = Serial.available();
  if (num > 0) 
  {
    //read each byte
    for ( i =0; i < num; i++)
    {
      readbuffer[i] = Serial.read();
    }
  }  
  return num;
}//end Read_Command_String
//****************************************
void Send_Return_String(int numread)
{
  //if something has been sent, send a return
  byte newline = 0x0A;   //newline char in hex
  if (numread > 0)
  {
    Serial.print(numread);  //return the bytes read
    Serial.write(newline);    //end with a newline character
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
  if (estop_val == LOW)
  {
    //stop the motors
    starboard_motor.writeMicroseconds(1500);
    port_motor.writeMicroseconds(1500);
  }
  else
  {
    //move the motors to the passed positions
    starboard_motor.writeMicroseconds(sm_pos);
    port_motor.writeMicroseconds(sm_pos);
  }
}//end Move_the_motors
//******************************************************
void Read_sensors(void)
{
  //read the Sharp IR distance sensor
  int ir_value;
  ir_value = analogRead(ir_pin);
  
  //convert to volts
  ir_volt = (float)IRvalue * 5.0 / 1024.0;

}//end Read_sensors
//************* end of program****************************

