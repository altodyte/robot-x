//*****Receive_and_send_1************

//This routine tests the serial communications

int led = 0; // Pin 0 maps to the USER LED on the ArbotiX Robocontroller.
boolean ledON;    //used to toggle the led
unsigned long newtime;
unsigned long timeperiod;

char readbuffer[40];
//*******************************************************************************
void setup()
{
  // start serial port at 38400 bps:
  Serial.begin(38400);
  
  // wait to connect
  delay(1000);
  
  pinMode(led, OUTPUT);  // initialize the digital pin as an output.
  ledON = false;        //used to toggle the board led with loop timer
}// end setup
//******************************************************************************
void loop()
{

  //Read the data bytes from the PC
  int numread = 0;
  numread = Read_Command_String();
  
  //send a response in bytes to the PC
  if (numread > 0)
  {
    Send_Return_String(numread);
  }
  
  delay(100);
  
  //change the state of the on board LED
  Toggle_led();
  
}// end void
//****************************************************

//**************subroutines******************************
int Read_Command_String(void)
{
  //read the string as a array of type char
  
  int num = 0;  //number of chars read

  //read the input char by char until '\n' is found
  if ( Serial.available() > 0) 
  {
    num =  Serial.readBytesUntil('\n', readbuffer, 40);
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
    Serial.write(readbuffer);  //return the bytes read
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
    digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level) 
  }  
  else
  {
    ledON = false;
    digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
  }
}//end Toggle_led
//************* end of program****************************

