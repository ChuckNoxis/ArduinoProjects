/*
 Not all pins on the Mega and Mega 2560 support change interrupts, 
 so only the following can be used for RX: 
 10, 11, 12, 13, 50, 51, 52, 53, 62, 63, 64, 65, 66, 67, 68, 69
 */

#include <SoftwareSerial.h>

SoftwareSerial myBluetooth(10, 11); // RX (TxD), TX (Rx)

void setup()  
{
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  // set the data rate for the SoftwareSerial port
  myBluetooth.begin(9600);
}

void loop() // run over and over
{
  if (myBluetooth.available())
    Serial.write(myBluetooth.read());
  if (Serial.available())
    myBluetooth.write(Serial.read());
}

