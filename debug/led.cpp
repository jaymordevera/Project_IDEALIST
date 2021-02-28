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

//Connect the servo to pin 21
myservo.attach(21);

//Initial Degree is 180
myservo.write(180);

//Serial.println("CLEARSHEET");
//Serial.println("LABEL,Date,Time,Status,Number");

delay(2000);

trial = 1;
Serial.println("");
Serial.print("Solid Blue Trial ");
Serial.print(trial);
Serial.print(": ");
Serial.println("The lid is currently close");
Serial.print("-------Displaying Solid Blue Light-------");
digitalWrite(b, LOW);
}
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
void loop() {
  //If there is an RFID tag detected
  if (rfid.requestTag(MF1_REQIDL, data) == MI_OK ) {

    //Ensuring there is only one RFID tag
    if (rfid.antiCollision(data) == MI_OK ) {
        //Copy data to UID
        memcpy(uid, data, 5 );

        /*
        for (int u = 0; u < 5; u++) {
        Serial.print(uid[u]);
        Serial.print(", ");
        }
        Serial.println("");
        Serial.println("---scaning---0--");
        */

        //If its empty
        if (status == empty ){

            //1st Scan
            if (lid_status == false){
                open_lid() ;
                lid_status = true;

                //Displaying green light
                green_trial += 1;
                Serial.println("\n");
                Serial.print("Solid Green Trial ");
                Serial.print(green_trial);
                Serial.print(": ");
                Serial.println("The lid is currently open");
                Serial.print("------Displaying Solid Green Light------");
                digitalWrite(g, LOW);
                digitalWrite(r, HIGH);
                digitalWrite(b, HIGH);

                //Delay 3 seconds
                delay(3000);

            //4th Scan
            } else{

                    //If its the same
                    if((memcmp(uid, masterkey_1,5)==0)||(memcmp(uid, masterkey_2,5)==0)||(memcmp(uid, prev_uid,5)==0)){

                        memcpy(prev_uid, clear_uid, 5);
                        lid_status = false;

                        close_lid();

                        //Displaying blue light
                        trial += 1;
                        Serial.println("\n");
                        Serial.print("Solid Blue Trial ");
                        Serial.print(trial);
                        Serial.print(": ");
                        Serial.println("The lid is currently close");
                        Serial.print("------Displaying Solid Blue Light------");
                        digitalWrite(b, LOW);
                        digitalWrite(r, HIGH);
                        digitalWrite(g, HIGH);

                        delay(3000);

                    } else {

                        //2nd Scan
                        if (uid[0] != 0){
                        memcpy( last_uid, uid, 5 );
                        memcpy( prev_uid, uid, 5 );

                        status = not_empty;

                        //Closing the lid
                        close_lid();

                        //Displaying violet light
                        Serial.println("\n");
                        Serial.print("Blinking Violet Trial ");
                        Serial.print(trial);
                        Serial.print(": ");
                        Serial.println("The N95/Face shields were placed inside");
                        Serial.print("-------Displaying Blinking Violet Light-------");
                        digitalWrite(r, LOW);
                        digitalWrite(g, HIGH);
                        digitalWrite(b, LOW);

                        delay(3000);

                        Serial.println("\n");
                        Serial.print("Simulated UV-C Light Trial ");
                        Serial.print(trial);
                        Serial.print(": ");
                        Serial.println("The N95/Face shields were placed inside");
                        Serial.print("-------Turning on Simulated UV-C Light-------");
                        //Turning on UV light
                        digitalWrite(UV, LOW);

                        //Blinking function
                        for (int t = 0; t < 10; t++) {
                            //Turn off
                            delay(500);
                            digitalWrite(r, !LOW);
                            digitalWrite(g, HIGH);
                            digitalWrite(b, !LOW);
                            //Turn on
                            delay(500);
                            digitalWrite(r, LOW);
                            digitalWrite(g, HIGH);
                            digitalWrite(b, LOW);
                        }

                        Serial.println("\n");
                        Serial.print("Simulated UV-C Light Trial ");
                        Serial.print(trial);
                        Serial.print(": ");
                        Serial.println("The 60s was done");
                        Serial.print("-----Turning off Simulated UV-C Light-----");
                        //Delay for Yellow light
                        delay(2000);
                        digitalWrite(UV, HIGH);
                        delay(1000);

                        Serial.println("\n");
                        Serial.print("Solid Yellow Trial ");
                        Serial.print(trial);
                        Serial.print(": ");
                        Serial.println("The N95/Face shields are ready to be retrieved");
                        Serial.print("-------Displaying Solid Yellow Light-------");
                        digitalWrite(b, HIGH);
                        digitalWrite(g, LOW);
                        }
                        }
            }
        // If not not_empty
        // 3rd Scan
        } else{
              // Is it the same card?
              if(memcmp(uid, last_uid,5)==0){
                  memcpy(last_uid, clear_uid, 5);

                  //Displaying Green Light
                  green_trial += 1;
                  Serial.println("\n");
                  Serial.print("Solid Green Trial ");
                  Serial.print(green_trial);
                  Serial.print(": ");
                  Serial.println("The lid is currently open");
                  Serial.print("------Displaying Green Light------");
                  digitalWrite(g, LOW);
                  digitalWrite(r, HIGH);
                  digitalWrite(b, HIGH);
                  open_lid();
                  status = empty;

                  delay(3000);

              } else if ((memcmp(uid, masterkey_1,5)==0)||(memcmp(uid, masterkey_2,5)==0)){
                  memcpy(last_uid, clear_uid, 5);
                  open_lid();

                  status = empty;

                  //Displaying green light
                  green_trial += 1;
                  Serial.println("\n");
                  Serial.print("Solid Green Trial ");
                  Serial.print(green_trial);
                  Serial.print(": ");
                  Serial.println("The lid is currently open");
                  Serial.print("------Displaying Green Light------");
                  digitalWrite(g, LOW);
                  digitalWrite(r, HIGH);
                  digitalWrite(b, HIGH);

                  //Delay for 3 seconds
                  delay(3000);

              } else{

                  //Blinking red light
                  red_trial += 1;
                  Serial.println("\n");
                  Serial.print("Blinking Red Trial ");
                  Serial.print(red_trial);
                  Serial.print(": ");
                  Serial.println("Wrong RFID tag was scanned");
                  Serial.print("------Displaying Blinking Red Light------");
                  for (int t = 0; t < 5; t++) {
                  digitalWrite(r, LOW);
                  digitalWrite(g, HIGH);
                  digitalWrite(b, HIGH);
                  delay(100);
                  digitalWrite(r, HIGH);
                  delay(100);
                  }

                  //Return to previous color
                  digitalWrite(r, LOW);
                  digitalWrite(b, HIGH);
                  digitalWrite(g, LOW);
                  }
                  }
          }
          delay(70);
  }
}
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
void send_info(){
         Serial.print("DATA,DATE,TIME," + data_status);//send the Name to excel
         Serial.print(",");
         Serial.println(Number);
}
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
void open_lid (){
//open
  for (int pos = 180; pos >= 0; pos -= 1) {
    myservo.write(pos);
    delay(10);
  }
}
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
void close_lid (){
//close
for (int pos = 0; pos <= 180; pos += 1) {
    myservo.write(pos);
    delay(10);
  }
}
/*----------------------------------------------------------------------------*/
