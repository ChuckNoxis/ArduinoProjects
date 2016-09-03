#include <SD.h>
#include <dht.h>
#include <Wire.h>
#include <SFE_BMP180.h>
#include "RTClib.h"
#include "state.h"

#define dht_in A0
#define dht_out A1
#define waterPin A2

dht		DHT_in;
dht		DHT_out;
File		myFile;
SFE_BMP180	pressure;
RTC_DS1307	RTC;

void	setup()
{
  int	i;

  Serial.begin(9600);
  while (!Serial) ; // wait for serial port to connect. Needed for Leonardo only
  /*
  **
  ** Initialisation of the BMP180
  **
  */
  delay(1000);//Wait recommended delay before accessing BMP180
  (pressure.begin()) ? succes(1) : error(1);
  /*
  **
  ** Initialisation of the SD Card
  **  
  */
  pinMode(10, OUTPUT);
  if (!SD.begin(4))
    error(2);
  else
    succes(2);
  /*
  **
  ** Initialisation of the RTC
  **
  */
  Wire.begin();
  RTC.begin();
  if (! RTC.isrunning())
    error(3);
  else
    succes(3);
  // This section grabs the current datetime and compares it to
  // the compilation time.  If necessary, the RTC is updated.
  DateTime now = RTC.now();
  DateTime compiled = DateTime(__DATE__, __TIME__);
  if (now.unixtime() < compiled.unixtime())
    {
      RTC.adjust(DateTime(__DATE__, __TIME__));
      clignote(0, 255, 63, 1, 1000);
    }
  Serial.println("Setup complete.");
  for (i = 0; i < 255; i++)
    {
      setColor(0, i, 255 - i);
      delay(15);
    }
}

static int     	givemethetime()
{
  DateTime	now = RTC.now();
  int		minute;
  int		second;

  minute = now.minute();
  second = now.second();
  /* if ((minute == 30 || minute == 0) && second == 0) */
  /*   return (1); */
  if ((minute % 2) == 0 && second == 0)
    return (1);
  else
    return (0);
}

void		loop()
{
  char		status;
  double	T_Bmp;
  double	Press;
  int		water;

  water = analogRead(waterPin);
  if (givemethetime() || water > 500)
    {
      DHT_in.read11(dht_in);
      DHT_out.read11(dht_out);
      status = pressure.startTemperature();
      if (status != 0)
	{
	  // Wait for the measurement to complete:
	  delay(status);
	  status = pressure.getTemperature(T_Bmp);
	  if (status != 0)
	    {
	      // The parameter is the oversampling setting, from 0 to 3 (highest res, longest wait).
	      // If request is successful, the number of ms to wait is returned.
	      status = pressure.startPressure(3);
	      if (status != 0)
		{
		  delay(status);
		  status = pressure.getPressure(Press, T_Bmp);
		  if (status != 0)
		    {
		      Serial.print("All OK\n");
		    }
		  else
		    error(5);
		}
	      else
		error(5);
	    }
	  else
	    error(5);
	}
      else
	error(5);
      // open the file. note that only one file can be open at a time,
      // so you have to close this one before opening another.
      myFile = SD.open("log.txt", FILE_WRITE);
      // if the file opened okay, write to it:
      if (myFile)
	{
	  // Get the current time
	  DateTime	now = RTC.now();
	  // Display the current time
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
	  myFile.print(water);
	  myFile.print("\",\""); 
	  myFile.print(DHT_in.humidity);
	  myFile.print("\",\"");
	  myFile.print(DHT_in.temperature);
	  myFile.print("\",\"");
	  myFile.print(T_Bmp, 2);
	  myFile.print("\",\"");
	  myFile.print((T_Bmp + DHT_in.temperature) / 2);
	  myFile.print("\",\"");
	  myFile.print(Press, 2);
	  myFile.print("\",\"");
	  myFile.print(DHT_out.humidity);
	  myFile.print("\",\"");
	  myFile.print(DHT_out.temperature);
	  myFile.println("\"");
	  Serial.print("Writing to log.txt...");
	  // close the file
	  myFile.close();
	  Serial.println("done.");
	}
      else
	error(6);
    }
  clignote(0, 255, 63, 2, 300);
  delay(90);
}
