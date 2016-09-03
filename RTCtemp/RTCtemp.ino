
void	loop()
{
  int tempC = 0;

  tempC = RTC.getTemperature();
  Serial.print(tempC);
  Serial.println("C");
}


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

int	tmp()
{
  byte tMSB, tLSB;
  float temp3231;

  //temp registers (11h-12h) get updated automatically every 64s
  Wire.beginTransmission(DS3231_CTRL_ID);
  Wire.write(0x11);
  Wire.endTransmission();
  Wire.requestFrom(DS3231_CTRL_ID, 2);

  if(Wire.available()) {
    tMSB = Wire.read(); //2's complement int portion
    tLSB = Wire.read(); //fraction portion

    temp3231 = (tMSB & B01111111); //do 2's math on Tmsb
    temp3231 += ( (tLSB >> 6) * 0.25 ); //only care about bits 7 & 8
    //temp3231 = ((((short)tMSB << 8) | (short)tLSB) >> 6) / 4.0;
    //temp3231 = (temp3231 * 1.8) + 32.0;
    return temp3231;
  }
  else {
    //oh noes, no data!
  }

  return 0;
}

void loop() {
  delay(5000);
}
