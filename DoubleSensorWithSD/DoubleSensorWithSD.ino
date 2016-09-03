#include <SD.h>
#include <dht.h>
#include <SFE_BMP180.h>
#include <Wire.h>
#include "RTClib.h"

#define dht_dpin A0 //no ; here. Set equal to channel sensor is on

/*
Hardware connections:
- (GND) to GND
+ (VDD) to 3.3V
(WARNING: do not connect + to 5V or the sensor will be damaged!)

Any Arduino pins labeled:  SDA  SCL
Uno:                       A4   A5
Leonardo:                   2    3
 * SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 4
*/

dht DHT;
SFE_BMP180 pressure;
File myFile;
RTC_DS1307 RTC;

/*
**
**  Setup is Here
**
*/

void setup()
{
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  delay(300);//Let system settle
  Serial.println("Humidity, Temperature & Pressure By Elio Desnoulez\n");
  delay(700);//Wait rest of 1000ms recommended delay before accessing sensor
  // Initialize the sensor (it is important to get calibration values stored on the device).
  if (pressure.begin())
    Serial.println("Pressure sensor's init succeed\n");
  else
    {
      Serial.println("Pressure sensor's init failed\n\n");
      while(1); // Pause forever.
    }
  Serial.print("Initializing SD card...");
  // On the Ethernet Shield, CS is pin 4. It's set as an output by default.
  // Note that even if it's not used as the CS pin, the hardware SS pin 
  // (10 on most Arduino boards, 53 on the Mega) must be left as an output 
  // or the SD library functions will not work. 
  pinMode(10, OUTPUT);
  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
  Wire.begin();
  RTC.begin();
  // Check if the RTC is running.
  if (! RTC.isrunning()) {
    Serial.println("RTC is NOT running");
  }

  // This section grabs the current datetime and compares it to
  // the compilation time.  If necessary, the RTC is updated.
  DateTime now = RTC.now();
  DateTime compiled = DateTime(__DATE__, __TIME__);
  if (now.unixtime() < compiled.unixtime()) {
    Serial.println("RTC is older than compile time! Updating");
    RTC.adjust(DateTime(__DATE__, __TIME__));
  }
  Serial.println("Setup complete.");
}

/*
**
**  Main is Here
**
*/

void		loop()
{
  char		status;
  double	T;
  double	P;
  double	a;

  DHT.read11(dht_dpin);
  Serial.print("Humidity = ");
  Serial.print(DHT.humidity);
  Serial.println("%");
  Serial.print("Temperature DHT = ");
  Serial.print(DHT.temperature);
  Serial.println(" C");
  status = pressure.startTemperature();
  if (status != 0)
    {
      // Wait for the measurement to complete:
      delay(status);
      status = pressure.getTemperature(T);
      if (status != 0)
        {
          Serial.print("Temperature BMP180 = ");
          Serial.print(T,2);
          Serial.println(" C");
          Serial.print("Average Temperature = ");
          Serial.print((T + DHT.temperature) / 2);
          Serial.println(" C");
          // The parameter is the oversampling setting, from 0 to 3 (highest res, longest wait).
          // If request is successful, the number of ms to wait is returned.
          status = pressure.startPressure(3);
	  if (status != 0)
	    {
	      delay(status);
	      // Note also that the function requires the previous temperature measurement (T).
	      // (If temperature is stable, you can do one temperature measurement for a number of pressure measurements.)
	      status = pressure.getPressure(P,T);
	      if (status != 0)
		{
		  Serial.print("Pressure = ");
		  Serial.print(P,2);
		  Serial.println(" hPa");
		}
	      else Serial.println("error retrieving pressure measurement\n");
	    }
          else Serial.println("error starting pressure measurement\n");
	}
      else Serial.println("error retrieving temperature measurement\n");
    }
  else Serial.println("error starting temperature measurement\n");
    /**if (SD.exists("ninja.log"))
	{
	  myFile.println("Date,Heure,TempDHT,TempBMP180,Average Temp,Humidity,Pressure");
	  Serial.println("Setup of ninja.log File");
	}**/
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  myFile = SD.open("lololo.txt", FILE_WRITE);
  // Get the current time
  DateTime now = RTC.now();
  // if the file opened okay, write to it:
  if (myFile) {
      Serial.print("Writing to ninja.log...");
        // Display the current time
      Serial.print("Current time: ");
      Serial.print(now.day(), DEC);
      Serial.print('/');
      Serial.print(now.month(), DEC);
      Serial.print('/');
      Serial.print(now.year(), DEC);
      Serial.print(' ');
      Serial.print(now.hour(), DEC);
      Serial.print(':');
      Serial.print(now.minute(), DEC);
      Serial.print(':');
      Serial.print(now.second(), DEC);
      Serial.println();
      myFile.print(now.day(), DEC);
      myFile.print('/');
      myFile.print(now.month(), DEC);
      myFile.print('/');
      myFile.print(now.year(), DEC);
      myFile.print(',');
      myFile.print(now.hour(), DEC);
      myFile.print(':');
      myFile.print(now.minute(), DEC);
      myFile.print(':');
      myFile.print(now.second(), DEC);
      myFile.print(",\"");
      myFile.print(DHT.temperature);
      myFile.print("\",\"");
      myFile.print(T,2);
      myFile.print("\",\"");
      myFile.print((T + DHT.temperature) / 2);
      myFile.print("\",\"");
      myFile.print(DHT.humidity);
      myFile.print("\",\"");
      myFile.print(P,2);
      myFile.println("\"");
      myFile.close();
      Serial.println("done.\n");
    }
  else
    Serial.println("error opening ninja.log");
  delay(10000);//Don't try to access too frequently... in theory
  //should be once per two seconds, fastest, but seems to work after 0.8 second.
}// end loop()
