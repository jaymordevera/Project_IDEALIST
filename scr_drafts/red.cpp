//Library for the Servo Motor
#include <Servo.h>
//Import as myservo
Servo myservo;

//Libraries for RFID
#include <SPI.h>
#include <MFRC522.h>

//Declare ports for RFID
#define RFID_SS  53
#define RFID_RST 49

//Import MFRC522 function and use it as rfid
MFRC522 rfid( RFID_SS, RFID_RST );

//Functions
void send_info();
void close_lid();
void open_lid();

//Arrays
byte data[MAX_LEN];
byte uid[5];
byte last_uid[5];
byte prev_uid[5];
byte clear_uid[] = {0,0,0,0,0};
char Number[40];

//Color for LED Lights
int r = 7;
int g = 6;
int b = 5;
int UV = 4;
int led = 13;
int trial = 0;
int green_trial = 0;
int red_trial = 0;

//UID of master keys
byte masterkey_1[] = {25,19,93,162,245};
byte masterkey_2[] = {20,105,18,43,68};

//Initial Conditions
bool not_empty = false;
bool empty = true;
bool status = empty;
bool lid_status = false;

//Database
String data_status = "";
int sheetC=2;

//Pins of LEDs
int ledPins[] = { 4, 5 ,6, 7};

/*----------------------------------------------------------------------------*/
void setup() {
  SPI.begin();
  Serial.begin(9600);
  rfid.begin();;
  pinMode(led, OUTPUT);

  //Turning off the LEDs
  for (int thisLed = 0; thisLed < 4; thisLed++) {
      pinMode(ledPins[thisLed], OUTPUT);
      digitalWrite(ledPins[thisLed], HIGH);
    }

}
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
void loop() {
  digitalWrite(g, HIGH);
  digitalWrite(r, LOW);
  digitalWrite(b, HIGH);
}
