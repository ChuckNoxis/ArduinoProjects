#include <dht.h>
#include <SFE_BMP180.h>
#include <Wire.h>

#define dht_dpin A0 //no ; here. Set equal to channel sensor is on

/*
Hardware connections:
- (GND) to GND
+ (VDD) to 3.3V
(WARNING: do not connect + to 5V or the sensor will be damaged!)

Any Arduino pins labeled:  SDA  SCL
Uno:                       A4   A5
Leonardo:                   2    3
*/

dht DHT;
SFE_BMP180 pressure;

/**
**
**  Setup is Here
**
*/

void setup()
{
  Serial.begin(9600);
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
}

/**
**
**  Main is Here
**
*/

void loop()
{
  char status;
  double T;
  double P;
  double a;

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
  Serial.println();
  delay(5000);//Don't try to access too frequently... in theory
  //should be once per two seconds, fastest, but seems to work after 0.8 second.
}// end loop()
