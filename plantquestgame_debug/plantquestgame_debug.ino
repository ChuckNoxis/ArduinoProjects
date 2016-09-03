// constants won't change. They're used here to
// set pin numbers:
#include <Servo.h> 


int buttonPin = 5;     // the number of the pushbutton pin
// variables will change:
int buttonState = 0;         // variable for reading the pushbutton status


 int sensorPin = A0; // Variable addressed to the analog pin A0 which Will receive the signal from the sensor 
 int sensorValue = 0; // variable that will store the value from  sensorPin
 int humidity = 0;
 
// int ledgreen1 = 10;
 int ledgreen2 = 11;
 int ledgreen3 = 12; // LED addressed to the pin 11
 int ledred = 13;

Servo myservo;  // create servo object to control a servo 
                // twelve servo objects can be created on most boards
int pos = 0;    // variable to store the servo position 


//sound
int speakerPin = 7;   
int length = 295; // the number of notes
char notes[] = "EE E CE G  g  C  g e  a b ia gEGA FG E CDb C  g e  a b ia gEGA FG E CDb  GNFR E uaC aCD GNFR E 1 11   GNFR E uaC aCD L  D C   CC C CD EC ag  CC C CDE  CC C CD EC ag  EE E CE G  g  C  g e  a b ia gEGA FG E CDb C  g e  a b ia gEGA FG E CDb EC g u aF Fa  bAAAGFEC ag  EC g u aF Fa  bF FFEDCe ec  "; // a space represents a rest
float beats[] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, //Page 1
                2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 2, 1, 1, 2, 4, //Page 2
                1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 4, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 2, //Page4
                1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, //Page 5
                1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1.3, 1.3, 1.3, 1.3, 1.3, 1.3, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1.3, 1.3, 1.3, 1, 1, 1, 1, 1, 1, 2 }; //Page 6
                
int tempo = 60;

void playTone(int ton1, int duration) {
  for (long i = 0; i < duration * 1000L; i += ton1) {
    tone(speakerPin, ton1);
    delayMicroseconds(ton1);
  }
  noTone(speakerPin);
}

void playNote(char note, int duration) {
//                        c    c#   d    d#   e    f    f#   g    g#   a    a#   b
  char names[] = { ' ',  '!', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', 'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C', 'D', 'E', 'F', 'G', 'A', 'B', 'i', 'N', 'R', 'u',  '1', 'L', 'k'}; // [i = b flat] [N = G flat] [R = D#] [u = g#] [1 = C oct. 5] [L = E flat]
  int tones[] =  {   0, 1046, 138, 146, 155, 164, 174, 184, 195, 207, 220, 233, 246, 261, 293, 329, 349, 391, 440, 493, 523, 587, 659, 698, 783, 880, 987, 466, 740, 622, 415, 1046, 622u, 227};
  
  // play the tone corresponding to the note name
  for (int i = 0; i < 34; i++) {
    if (names[i] == note) {
      playTone(tones[i], duration);
    }
  }
}
//end sound



void setup() {
  
  Serial.begin(9600); // set the baud rate serial with 9600
  
    pinMode(ledgreen2, OUTPUT);
    pinMode(ledgreen3, OUTPUT);
    pinMode(ledred, OUTPUT); 
    pinMode(speakerPin, OUTPUT);
  // initialize the LED pin as an output:
    pinMode(buttonPin, INPUT_PULLUP);    
    myservo.attach(4);  // attaches the servo on pin 9 to the servo object
    //int buttonState = 0;
}

void loop(){
  buttonState = 0;
  sensorValue = analogRead(sensorPin);
  myservo.write(50);
  // read the value from the sensor:
  if(sensorValue<300 ) {
    digitalWrite(ledred, HIGH); // Set high logic level to the LED green, turn it on
    delay(500);
    digitalWrite(ledred, LOW); 
    delay(300);
    digitalWrite(ledgreen2, LOW);
    digitalWrite(ledgreen3, LOW);
  }
  else if(sensorValue>300 )
    {
      // read the state of the pushbutton value:
      buttonState = digitalRead(buttonPin);
      digitalWrite(ledgreen2, HIGH);
      digitalWrite(ledgreen3, HIGH);
      Serial.println(buttonPin);
      if (buttonState == 1)
	{
	  myservo.write(0);
	  delay(250);
	  myservo.write(50);
	  for (int i = 0; i < length; i++) {
	    //Serial.print(i);
	    if (notes[i] == ' ')
	      delay(beats[i] * tempo); // rest
	    else
	      playNote(notes[i], beats[i] * tempo);
	    //Serial.print("END !!!!\n");
	    delay(tempo / 2);
	  }
	}
    }
}
