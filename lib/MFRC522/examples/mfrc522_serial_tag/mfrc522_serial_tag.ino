/*
  Mifare RC522 Tag Serial Output
  Prints out the tag id only once when in range. 
  Same tag can be swiped again after 2 seconds
  Different tag can be swiped immediately
  
  www.hobbytronics.co.uk
  
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

byte data[MAX_LEN];
byte uid[5];
byte last_uid[5]; 
byte clear_uid[] = {0,0,0,0,0};
char tmp[20];

int led = 3;

unsigned long currentTime;
unsigned long cloopTime;

void setup() {
  SPI.begin();
  Serial.begin(9600);
  rfid.begin();
  currentTime = millis();
  cloopTime = currentTime;  
  pinMode(led, OUTPUT);  
}

void loop() {

  if ( rfid.requestTag( MF1_REQIDL, data ) == MI_OK ) {
    if ( rfid.antiCollision( data ) == MI_OK ) {
      memcpy( uid, data, 5 );
      if(memcmp(uid, last_uid,5)!=0){
        memcpy( last_uid, uid, 5 );     
        // Format data for output - HEX
        sprintf( tmp, "%02X%02X%02X%02X%02X" , uid[0], uid[1], uid[2], uid[3], uid[4]);
        Serial.println(tmp);
        digitalWrite(led, HIGH);
        cloopTime = millis();
      }  
    }
  } 
  
  // Timeout functions
  currentTime = millis();
  if(currentTime >= (cloopTime + (500)))
  {
    // Turn off LED after 500ms
    digitalWrite(led, LOW);
  }   
  if(currentTime >= (cloopTime + (2000)))
  {
    // Clear last used tag id after 2s so it can be swiped again
     memcpy( last_uid, clear_uid, 5 );
  }   
  
}
 