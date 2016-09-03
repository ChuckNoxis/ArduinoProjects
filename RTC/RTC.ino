#include <Wire.h>
#include "RTClib.h"  // Credit: Adafruit

RTC_DS1307 RTC;

void setup() {
 
  // Begin the Serial connection 
  Serial.begin(9600);
 
  // Instantiate the RTC
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

void		givemethetime()
{
  DateTime	now = RTC.now();
  int		minute;
  int		second;
  int		timetowait;
  
  minute = now.minute();
  second = now.second();
  if ((minute == 30 || minute == 0) && second == 0)
    Serial.print("c bon");
  else
    {
      if (minute < 30)
	{
	  minute = 29 - minute;
	  second = 59 - second;
	  timetowait = minute * 60 + second;
	}
      else
	{
	  minute = 59 - minute;
	  second = 59 - second;
	  timetowait = minute * 60 + second;
	}
      Serial.println(timetowait);
      delay(1000);
    }
}

void loop() {
  givemethetime();
  // Get the current time
  DateTime now = RTC.now();   
 
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
}
