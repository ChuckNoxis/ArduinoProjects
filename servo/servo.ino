// constants won't change. They're used here to
// set pin numbers:
#include <Servo.h> 

Servo myservo;  // create servo object to control a servo 

void setup() {
  
  Serial.begin(9600); // set the baud rate serial with 9600
  myservo.attach(4);  // attaches the servo on pin 9 to the servo object
}

void loop()
{
  myservo.write(0);
  delay(250);
  myservo.write(50);
  delay(1000);  
}
