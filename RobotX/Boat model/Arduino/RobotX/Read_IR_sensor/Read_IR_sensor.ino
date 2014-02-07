//**********Read_IR_Sensor***************

/*This routine will read values from an Sharp IR sensor and
  send the values to the PC using the XBEE

*/

int IRinputpin = 2;
int  IRvalue = 0;
float IRvolts = 0.0;
String sHeader = "IR value = ";

// Pin 0 maps to the USER LED on the ArbotiX Robocontroller.
int led = 0;  

//*************************************************
void setup()
{
  // start serial port at 38400 bps:
  Serial.begin(38400);
  
  // wait to connect
  delay(1000);
  
  // initialize the digital pin as an output.
  pinMode(led, OUTPUT);  
}

//****************************************************
void loop()
{
  // turn the LED on (HIGH is the voltage level)  
  digitalWrite(led, HIGH);   
  
  IRvalue = analogRead(IRinputpin);    // read the input pin
  
  IRvolts = (float)IRvalue * 5.0 / 1024.0;  //convert to volts
  
  // Send header and the result
  Serial.print(sHeader);
  Serial.println(IRvolts);
   
  delay(100);
  
  // turn the LED off by making the voltage LOW
  digitalWrite(led, LOW);  

  delay(900);  
}
//***************** program end*********************
