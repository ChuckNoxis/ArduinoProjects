//uncomment this line if using a Common Anode LED
//#define COMMON_ANODE
const int	ledRed = 3;
const int	ledGreen = 5;
const int	ledBlue = 6;

void	error(int error_n)
{
  int	wait;
  int	error;
  int	i;

  if (error_n <= 3)
    error = error_n * 250;
  else
    error = 900;
  wait = 1000 - error;
  /*
  ** Nous vous devons une pause eternelllle.
  */
  while (1)
    {
      i = error_n;
      while (i)
	{
	  setColor(255, 0, 0);
	  delay(wait);
	  setColor(0, 0, 0);
	  i--;
	  if (i)
	    delay(wait);
	}
      delay(5000);
    }
}

void	succes(int succes_n)
{
  int	wait;
  int	succes;

  if (succes_n <= 3)
    succes = succes_n * 250;
  else
    succes = 900;
  wait = 1000 - succes;
  while (succes_n)
    {
      setColor(255, 0, 0);
      delay(wait);
      setColor(0, 0, 0);
      succes_n--;
      if (succes_n)
	delay(wait);
    }
}

void	clignote(int red, int green, int blue, int nb, int wait)
{
  while (nb)
    {
      setColor(red, green, blue);
      delay(wait);
      setColor(0, 0, 0);
      nb--;
      if (nb)
	delay(wait);
    }
}

void	setColor(int red, int green, int blue)
{
#ifdef COMMON_ANODE
  red = 255 - red;
  green = 255 - green;
  blue = 255 - blue;
#endif
  analogWrite(ledRed, red);
  analogWrite(ledGreen, green);
  analogWrite(ledBlue, blue);
}
