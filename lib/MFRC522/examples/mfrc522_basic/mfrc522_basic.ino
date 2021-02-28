/*
  Mifare RC522 Basic test program
  Prints out the tag id when in range
  
  PINOUT:

   RC522 MODULE    UNO     MEGA
   SDA(SS)         D10     D53
   SCK             D13     D52
   MOSI            D11     D51
   MISO            D12     D50
   PQ              Not Connected
   GND             GND     GND
   RST             D9      D9
   3.3V            3.3V    3.3V

*/

#include <SPI.h>
#include <MFRC522.h>

#define RFID_SS  10
#define RFID_RST 9

MFRC522 rfid( RFID_SS, RFID_RST );

void setup() {
  SPI.begin();
  Serial.begin(115200);
  rfid.begin();
}

void loop() {
  byte data[MAX_LEN];
  byte uid[5];

  if ( rfid.requestTag( MF1_REQIDL, data ) == MI_OK ) {
    if ( rfid.antiCollision( data ) == MI_OK ) {
      memcpy( uid, data, 5 );
      for ( int i = 0; i < 5; i++ ) {
        Serial.print( uid[i], HEX );
      }
      Serial.println();
    }
  }
}