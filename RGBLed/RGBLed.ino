/*
Adafruit Arduino - Lesson 3. RGB LED
*/
int redPin = 11;
int greenPin = 10;
int bluePin = 9;
//uncomment this line if using a Common Anode LED
//#define COMMON_ANODE
void setup()
{
  Serial.begin(9600);
  while (!Serial) ; 
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
}

void loop()
{
  int a = 0;
  int b = 0;
  int c = 0;
  
  while (a <= 255, b <= 255, c <= 255)
  {
    if (a < 255)
      a++;
    if (a == 255 && b < 255)
      b++;
    if (b == 255 && c < 255)
      c++;
    setColor(a,b,c);
    Serial.print(a);
    Serial.print(" ");
    Serial.print(b);
    Serial.print(" ");
    Serial.println(c);
    delay(25);
  }
/*  setColor(255, 0, 0); // red
  delay(1000);
  setColor(0, 255, 0); // green
  delay(1000);
  setColor(0, 0, 255); // blue
  delay(1000);
  setColor(255, 255, 0); // yellow
  delay(1000);
  setColor(80, 0, 80); // purple
  delay(1000);
  setColor(0, 255, 255); // aqua
  delay(1000);*/
  delay(10000);
}

void setColor(int red, int green, int blue)
{
  #ifdef COMMON_ANODE
  red = 255 - red;
  green = 255 - green;
  blue = 255 - blue;
  #endif
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);
}
