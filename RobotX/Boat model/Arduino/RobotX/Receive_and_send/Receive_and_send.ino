//*****Receive_and_send************

//This routine tests the serial communications

int led = 0; // Pin 0 maps to the USER LED on the ArbotiX Robocontroller.
boolean ledON;    //used to toggle the led
unsigned long newtime;
unsigned long timeperiod;
//*******************************************************************************
void setup()
{
  // start serial port at 38400 bps:
  Serial.begin(38400);
  
  // wait to connect
  delay(1000);
  
  pinMode(led, OUTPUT);  // initialize the digital pin as an output.
  ledON = false;        //used to toggle the board led with loop timer
  timeperiod = 100;    //loop time in milliseconds
}// end setup
//******************************************************************************
void loop()
{
  //use the computer clock to create a timed loop, start the loop timer
  newtime = millis();
  
  //Read the data bytes from the PC
  int numread;
  numread = Read_Command_String();
  
  delay(1);

  //send a response in bytes to the PC
  Send_Return_String(numread);
  
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

//**************subroutines******************************
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
    digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level) 
  }  
  else
  {
    ledON = false;
    digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
  }
}//end Toggle_led
//************* end of program****************************

