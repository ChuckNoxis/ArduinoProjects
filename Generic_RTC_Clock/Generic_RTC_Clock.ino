#include <Wire.h> //I2C communication library
#include "ds3231.h" //Real Time Clock library

uint8_t secset = 0; //Index for second RTC setting
uint8_t minset = 1; //Index for minute RTC setting
uint8_t hourset = 2; //Index for hour RTC setting
uint8_t wdayset = 3; //Index for weekday RTC setting
uint8_t mdayset = 4; //Index for date RTC setting
uint8_t monset = 5; //Index for month RTC setting
uint8_t yearset = 6; //Index for year RTC setting

//Alarm time variables
uint8_t wake_HOUR = 0;
uint8_t wake_MINUTE = 0;
uint8_t wake_SECOND = 0;
uint8_t wake_SET = 1; //Default alarm to ON in case of power failure or reset

void setup()
{
Serial.begin(9600); //Initialize serial port, if needed (not used)
Wire.begin(); //Initialize I2C communication library
DS3231_init(0x00); //Initialize Real Time Clock for 1Hz square wave output (no RTC alarms on output pin)

pinMode(8, INPUT); //Set pin for time/date mode button to input
digitalWrite(8, HIGH); //Turn on pullup resistors

pinMode(9, INPUT); //Set pin for time/date set button to input
digitalWrite(9, HIGH); //Turn on pullup resistors

pinMode(10, OUTPUT); //Set pin for external alarm indicator output
digitalWrite(10, LOW); //Initialize external alarm to off state
// init done
}

void loop()
{
  char tempF[6]; //Local variable to store converted temperature reading from Real Time Clock module
  float temperature; //Intermediate temperature variable to convert Celsius to Farenheit
  unsigned long now = millis(); //Local variable set to current value of Arduino internal millisecond run-time timer
  struct ts t; //Structure for retrieving and storing time and date data from real time clock
  
  //Force a temperature conversion if one is not in progress for rapid update and better clock accuracy
  //Maintain 1Hz square wave output
  if((DS3231_get_addr(0x0E) & 0x20) == 0){DS3231_init(0x20);} //Check for CONV flag to see if conversion is in progress first, else start conversion
  temperature = DS3231_get_treg(); //Get temperature from real time clock
  dtostrf(temperature, 5, 1, tempF); //Convert temperature to string for display
  
  printDay(t.wday); //Lookup day of week string from retrieved RTC data and write to display buffer
  
  printMonth(t.mon); //Lookup month string from retrieved RTC data and write to display buffer
  
  if(t.mday<10){Serial.print("0");} //Add leading zero to date display if date is single-digit
  Serial.print(t.mday); //Write date to display buffer
  Serial.print(", "); //Write spaces and comma between date and year
  
  Serial.print(t.year); //Write year to display buffer
  
  //RTC is operated in 24-hour mode and conversion to 12-hour mode done here, in software
  Serial.print(t.hour);   
  Serial.print(":"); //Serial hour-minute separator
  if(t.min<10){Serial.print("0");} //Add leading zero if single-digit minute
  Serial.print(t.min); //Serial retrieved minutes
  
  Serial.print(":"); //Display minute-seconds separator
  if(t.sec<10){Serial.print("0");} //Add leading zero for single-digit seconds
  Serial.print(t.sec); //Display retrieved seconds
}

 

//Function to display month string from numerical month argument
void printMonth(int month)
{
  switch(month)
    {
    case 1: Serial.print("Jan ");break;
    case 2: Serial.print("Feb ");break;
    case 3: Serial.print("Mar ");break;
    case 4: Serial.print("Apr ");break;
    case 5: Serial.print("May ");break;
    case 6: Serial.print("Jun ");break;
    case 7: Serial.print("Jul ");break;
    case 8: Serial.print("Aug ");break;
    case 9: Serial.print("Sep ");break;
    case 10: Serial.print("Oct ");break;
    case 11: Serial.print("Nov ");break;
    case 12: Serial.print("Dec ");break;
    default: Serial.print("--- ");break; //Serial dashes if error - avoids scrambling Serial
    } 
}


//Function to Serial day-of-week string from numerical day-of-week argument
void printDay(int day)
{
  switch(day)
    {
    case 1: Serial.print("Mon ");break;
    case 2: Serial.print("Tue ");break;
    case 3: Serial.print("Wed ");break;
    case 4: Serial.print("Thu ");break;
    case 5: Serial.print("Fri ");break;
    case 6: Serial.print("Sat ");break;
    case 7: Serial.print("Sun ");break;
    default: Serial.print("--- ");break; //Serial dashes if error - avoids scrambling Serial
    } 
}

//Subroutine to adjust a single date/time field in the RTC
void set_rtc_field(struct ts t,  uint8_t index)
{
  uint8_t century;
  
  if (t.year > 2000) {
    century = 0x80;
    t.year_s = t.year - 2000;
  } else {
    century = 0;
    t.year_s = t.year - 1900;
  }
  
  uint8_t TimeDate[7] = { t.sec, t.min, t.hour, t.wday, t.mday, t.mon, t.year_s };
  
  Wire.beginTransmission(DS3231_I2C_ADDR);
  Wire.write(index);
  TimeDate[index] = dectobcd(TimeDate[index]);
  if (index == 5){TimeDate[5] += century;}
  Wire.write(TimeDate[index]);
  Wire.endTransmission();
  
   //Adjust the month setting, per data sheet, if the year is changed
  if (index == 6){
    Wire.beginTransmission(DS3231_I2C_ADDR);
    Wire.write(5);
    TimeDate[5] = dectobcd(TimeDate[5]);
    TimeDate[5] += century;
    Wire.write(TimeDate[5]);
    Wire.endTransmission();
  } 
}
