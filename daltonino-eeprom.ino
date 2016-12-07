// charger eeprom avc données alpha

#include <SD.h>
#include <SPI.h>
#include <EEPROM.h>
File myFile;
const int pinSD = 53; // Mega
char filename[] = "RALFR.CSV";
char c;
String ligne = "";




void setup()
{
  int eeAddress = 0;   //Location we want the data to be put.
  unsigned int  lgch;
  int cle;
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  Serial.print("Initializing SD card...");
  pinMode(pinSD, OUTPUT);
  if (!SD.begin(pinSD)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
  if (SD.exists(filename)) {
    Serial.println("fic exists.");
    myFile = SD.open(filename, FILE_READ);
    while (myFile.available()) {
      c = myFile.read();
      if (c == '\n') {
        //Serial.println(ligne);
        // traitement
         cle = ligne.substring(0, 4).toInt();
        String tmp = ligne.substring(5, ligne.indexOf(",", 6));
        Serial.print(cle);
        Serial.print(" - ");
        Serial.println(tmp);
        ligne = "";
        lgch = tmp.length();
        Serial.print(lgch);
        Serial.print(" - ");
        Serial.println(tmp);
        // on va écrire <freq(int)><lgchaine(int)><chaine>
        // fin de chaine sur freq=0

        EEPROM.put(eeAddress, cle);
        eeAddress += sizeof(cle);

        EEPROM.put(eeAddress, lgch);
        eeAddress += sizeof(lgch);

        for (int j = 0; j < lgch; j++) {
          EEPROM.put(eeAddress++, tmp.charAt(j));
        }

      } else if (c == '\r') {
        Serial.println("=====");
      } else {
        ligne += (c);
        //Serial.print(c);
      }
    }
    myFile.close();
    cle = 0;
    EEPROM.put(eeAddress, cle);
    readData();
  }
  else {
    Serial.println("fic doesn't exist.");
  }
}

void loop()
{
  // nothing happens after setup finishes.
}


void readData() {
  int  eeAddress = 0;
  unsigned  int frl , lg2;
  char  c;
  String nom;
  EEPROM.get(eeAddress, frl);
  eeAddress += sizeof(frl);
  while (frl != 0) {
    nom = "";
    Serial.print(frl);
    EEPROM.get(eeAddress, lg2);
    eeAddress += sizeof(lg2);
    //Serial.println(lg2);
    Serial.print(" - ");
    for (int k = 0; k < lg2; k++) {
      EEPROM.get(eeAddress++, c);
      nom += c;
      //Serial.print(c);
    }
    Serial.println(nom);
    //Serial.println("");
    EEPROM.get(eeAddress, frl);
    eeAddress += sizeof(frl);

  }
}

