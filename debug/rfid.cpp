#include <Arduino.h>

//Ports of RFID
#define RFID_SS  53
#define RFID_RST 49

// Library for RFID
#include <SPI.h>
#include <MFRC522.h>

//Import MFRC522 function and use it as rfid
MFRC522 rfid( RFID_SS, RFID_RST );

byte data[MAX_LEN];
byte uid[5];

int trial = 0;

void setup() {
  SPI.begin();
  Serial.begin(9600);
  rfid.begin();
  Serial.println("\n");
}

void loop() {
  //If there is an RFID tag is detected
  if (rfid.requestTag(MF1_REQIDL, data ) == MI_OK) {

    //Ensuring there's only one RFID tag and not 2
    if (rfid.antiCollision(data) == MI_OK ) {
        trial += 1;
        memcpy(uid, data, 5);
        Serial.print("Trial ");
        Serial.println(trial);
        if (uid[0] != 0){
        Serial.println("-----RFID tag detected-----");
        Serial.print("Card UID: ");
        for(int t = 0; t < 5; t++){
        Serial.print(uid[t]);
        Serial.print(" ");
        }

      } else {
        Serial.println("-----RFID tag not detected-----");
      }

      }
        Serial.println("\n");
    }
    delay(1000);
  }
