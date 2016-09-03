/*
Programme de reception Bluetooth via le logiciel APP Inventor

- connectez une led sur la sortie digitale 11
- connecter le module bluetooth sur les broches 0 et 1 , puis l'alim GND 5V (une fois le programme uploadé)

Attention, avant d'uploader ce programme, veillez à débrancher le module bluetooth
car celui-ci utilise les broches 0 et 1 utilisées aussi pour uploader
un programme dans arduino

Les Petits Débrouillards 2013 GNU-GPL
*/
char NOMBRE[10]  = "Elioduino";    // le nom du module
char BPS         = '4';           // 1=1200 , 2=2400, 3=4800, 4=9600, 5=19200, 6=38400, 7=57600, 8=115200
char PASS[10]    = "9270";   // le mot de passe        

const int ledPin = 13;      // la led sera fixée à la broche 13
byte serialA; // variable de reception de donnée via RX

void setup()
{
  Serial.println("setup");

     Serial.begin(9600);
   pinMode(13,OUTPUT);
   digitalWrite(13,HIGH);
   delay(10000);
   digitalWrite(13,LOW);
   
   Serial.print("AT");
   delay(1000);

   Serial.print("AT+NAME");
   Serial.print(NOMBRE);
   delay(1000);

   Serial.print("AT+BAUD");
   Serial.print(BPS);
   delay(1000);

   Serial.print("AT+PIN");
   Serial.print(PASS);
   delay(1000);  
  
  digitalWrite(ledPin, HIGH); // allume sur la broche "ledpin"
  pinMode(ledPin, OUTPUT); // fixe la pin "ledpin" en sortie
}

void loop() {
  switch (serialA) {
  case 1: // si arduino reçois le chiffre 1 alors
    digitalWrite(ledPin, HIGH); // allume sur la broche "ledpin"
    break;
  case 2: // si arduino reçois le chiffre 2 alors
    digitalWrite(ledPin, LOW); // eteins la led sur la broche "ledpin"
    break;
  case 3: // si arduino reçois le chiffre 3 alors clignote
    digitalWrite(ledPin, HIGH);
    delay(100);
    digitalWrite(ledPin, LOW);
    delay(100);
  }
}

void serialEvent(){ // si arduino reçois quelquechose en sur l'entrée RX
  serialA = Serial.read(); // stocker la valeur reçue dans la variable SerialA
}
