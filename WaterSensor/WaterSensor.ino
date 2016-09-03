int	led = 13;

void	setup()
{
  pinMode(led, OUTPUT);
  Serial.begin(9600);
}

void	loop()
{
  int	analogPin = 2;
  int	val = 0;

  val = analogRead(analogPin);
  (val > 500) ? digitalWrite(led, HIGH) : digitalWrite(led, LOW);
  Serial.println(val);
  delay(100);
}
