// -------- Que fait ce programme ? ---------
/*  Ecrit la valeur courante de milliS dans un fichier
  sur la carte SD et affiche le fichier dans le Terminal Série
*/
// --- Fonctionnalités utilisées ---
// Utilise la connexion série vers le PC
// Utilise une carte mémoire micro-SD avec le module Ethernet Arduino

// -------- Circuit à réaliser ---------
// La connexion série vers le PC utilise les broches 0 et 1 (via le câble USB)
// Connecter broche SCLK du module SD sur la broche 13
// Connecter broche MISO du module SD sur la broche 12
// Connecter broche MOSI du module SD sur la broche 11
// Connecter broche CS du module SD sur la broche 10

#include <SdFat.h> // inclusion de la librairie pour carte mémoire SD en connexion SPI

const int brocheSDCardSelect=8; // broche utilisée pour sélectionner la SD card

int testSD; // Variable utilisée pour tester valeur renvoyée par fonctions SD Card

Sd2Card card; // création d'un objet Sd2Card
SdVolume volume; // création d'un objet SdVolume
SdFile root; // création d'un objet SdFile pour le répertoire racine
SdFile file; // création d'un objet SdFile pour le fichier utilisé

void setup()
{
  Serial.begin(115200); // initialise connexion série à 115200 bauds
  //---- initialise l'utilisation de la carte mémoire SD en mode SPI
  pinMode(brocheSDCardSelect, OUTPUT); // met la broche 10 (SS) en sortie
  testSD = card.init(SPI_HALF_SPEED,brocheSDCardSelect); // initialise la carte SD à la vitesse et avec la broche CS voulues
  (testSD==1) ? (Serial.println("Initialisation SD card OK")) : (Serial.println("Erreur initialisation SD Card"));
  testSD=volume.init(&card); // initialise le volume FAT de la carte SD
  (testSD==1) ? (Serial.println("Initialisation Volume FAT OK")) : (Serial.println("Erreur initialisation Volume FAT"));
  testSD=root.openRoot(&volume); // ouverture du répertoire root
  (testSD==1) ? (Serial.println("Ouverture repertoire root OK")) : (Serial.println("Echec ouverture repertoire root"));
}


void loop()
{
  //---- ouverture du répertoire root
  if (!root.isOpen())
    { // si root pas ouvert
      testSD=root.openRoot(&volume); // si root pas ouvert, on l'ouvre
      (testSD==1) ? (Serial.println("Ouverture repertoire root OK")) : (Serial.println("Echec ouverture repertoire root"));  
    }
  else
    Serial.println("Root deja ouvert");
  // nom du fichier court (8 caractères maxi . 3 caractères maxi) (dit format DOS 8.3)
  char nomFichier[] = "Alllogs.log";
  file.writeError = false; // réinitialise la variable writeError
  // ouverture / création fichier en écriture + position fin du fichier
  testSD=file.open(&root, nomFichier, O_CREAT | O_APPEND | O_WRITE);
  // O_CREAT - crée le fichier si il n'existe pas
  // O_APPEND - se postionne à la fin du fichier avant chaque écriture
  // O_WRITE - ouverture en mode écriture de donnée

  if (testSD==1) Serial.print ("Ouverture fichier "), Serial.print(nomFichier),Serial.println(" OK");
  else Serial.println ("Echec ouverture fichier");

  /* // Réduit la longueur du fichier à la valeur indiquée. */
  /* testSD=file.truncate(0); // Longueur = 0 pour effacement complet du fichier */
  /* if (testSD==1) Serial.print ("Effacement fichier"), Serial.print(nomFichier),Serial.println(" OK"); */
  /* else Serial.println ("Echec effacement fichier"); */

  //----- Ecriture dans le fichier -----
  file.print("Ecriture de la valeur millis() dans le fichier = ");
  delay(10);
  file.println(millis());

  //----- vérification erreur ecriture
  // lit la variable writeError - attention writeError n'est pas une fonction
  if (file.writeError==1) Serial.println ("Une erreur est survenue lors de l'ecriture"); else Serial.println("Ecriture reussie");

  //----- fermeture de fichier -----
  testSD=file.close(); // fermeture fichier
  if (testSD==1) Serial.println("Fermeture fichier reussie !");

  //*************** lecture fichier sur SD Card ***************

  //---- ouverture du fichier en lecture ---
  //char nomFichier[] = "FICHIER.TXT"; // nom du fichier court (8 caractères maxi . 3 caractères maxi) (dit format DOS 8.3)
  testSD=file.open(&root, nomFichier, O_READ); // ouverture du fichier en lecture
  if (testSD==1) Serial.print ("Ouverture fichier "), Serial.print(nomFichier),Serial.println(" en lecture OK");
  else Serial.println ("Echec ouverture fichier");

  //------ lecture des données du fichier -----

  Serial.println();
  Serial.println("****** Debut du fichier ******");

  int c; // variable de reception données lues

  // lit tous les octets disponibles et les affiche sur le port série
  while ((c = file.read()) > 0) Serial.print(char(c));

  Serial.println("****** fin du fichier ******\n");

  //----- fermeture de fichier -----
  testSD=file.close(); // fermeture fichier
  if (testSD==1) Serial.println("Fermeture fichier reussie !");
  delay(10000);
}
