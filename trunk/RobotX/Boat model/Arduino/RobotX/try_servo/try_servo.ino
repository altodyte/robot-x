// try_servo
// this program test the function of a RC servo
//***********************************************
#include <Servo.h> 
  
Servo servo_one;  // create servo object to control a servo 
                 // a maximum of eight servo objects can be created 
  
int pos = 0;    // variable to store the servo position 

int led = 0;  // Pin 0 maps to the USER LED on the ArbotiX Robocontroller.

 
//*********************************************
void setup() 
{ 
   //The pin number is actual pin output number D12 >>> pin 12
   servo_one.attach(12);  // attaches the servo on pin 12 to the servo object 
   servo_one.write(90);   // set servo to mid position
   pinMode(led, OUTPUT);  // initialize the digital pin as an output.
   delay(1000);  

} 
  
//***********************************************
void loop() 
{ 
  
  servo_one.write(20);   // set servo to 20 degree position
  delay(90);  
  digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level) 
  for(pos = 0; pos < 180; pos += 1)  // goes from 90 degrees to 160 degrees 
   {                                  // in steps of 1 degree 
     servo_one.write(pos);     // tell servo to go to position in variable 'pos' 
     delay(5);                       // waits 15ms for the servo to reach the position 
   } 
   
   digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
   for(pos = 180; pos>=0; pos-=1)     // goes from 160 degrees to 20 degrees 
   {                                
     servo_one.write(pos);              // tell servo to go to position in variable 'pos' 
     delay(5);                       // waits 15ms for the servo to reach the position 
   } 
}
