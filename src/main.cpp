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
digitalWrite(b, LOW);
Serial.println("Displaying Blue Light");
Serial.print("\n");
}
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
void loop() {
  //If there is an RFID tag detected
  if (rfid.requestTag(MF1_REQIDL, data) == MI_OK ) {

    //Ensuring there is only one RFID tag
    if (rfid.antiCollision(data) == MI_OK ) {

        Serial.println("RFID tag detected");
        //Copy data to UID
        memcpy(uid, data, 5 );

        sprintf(Number, "%02X%02X%02X%02X%02X" , uid[0], uid[1], uid[2], uid[3], uid[4]);

        //If its empty
        if (status == empty ){

            //1st Scan
            if (lid_status == false){
                open_lid() ;
                lid_status = true;
                Serial.println("The lid has opened");

                //Data transfer
                Serial.println("Transferring Data");
                data_status = "Open";
                send_info();

                //Displaying green light
                digitalWrite(g, LOW);
                digitalWrite(r, HIGH);
                digitalWrite(b, HIGH);
                Serial.println("Displaying Green Light");

                //Delay 3 seconds
                delay(3000);

            //4th Scan
            } else{

                    //If its the same
                    if((memcmp(uid, masterkey_1,5)==0)||(memcmp(uid, masterkey_2,5)==0)||(memcmp(uid, prev_uid,5)==0)){

                        memcpy(prev_uid, clear_uid, 5);
                        lid_status = false;

                        //Displaying blue light
                        digitalWrite(b, LOW);
                        digitalWrite(r, HIGH);
                        digitalWrite(g, HIGH);
                        Serial.println("Displaying Blue Light");

                        //Closing the lid
                        close_lid();
                        Serial.println("=The lid has closed");

                        //Data transfer
                        Serial.println("Transferring Data");
                        data_status = "Close";
                        send_info();
                        delay(3000);

                    } else {

                        //2nd Scan
                        if (uid[0] != 0){
                        memcpy( last_uid, uid, 5 );
                        memcpy( prev_uid, uid, 5 );

                        status = not_empty;

                        //Closing the lid
                        close_lid();
                        Serial.println("The lid has closed");

                        //Data transfer
                        Serial.println("Transferring Data");
                        data_status = "Use";
                        send_info();
                        delay(3000);

                        //Displaying violet light
                        digitalWrite(r, LOW);
                        digitalWrite(g, HIGH);
                        digitalWrite(b, LOW);
                        Serial.println("Displaying Violet Light");

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

                        //Delay for Yellow light
                        delay(2000);
                        digitalWrite(UV, HIGH);
                        delay(1000);

                        Serial.println("Displaying Yellow Light");
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
                  Serial.println("It is the same RFID tag");
                  memcpy(last_uid, clear_uid, 5);

                  //Opening th lid
                  Serial.println("Transferring Data");
                  data_status = "Retrieved";
                  send_info();
                  status = empty;

                  //Displaying Green Light
                  Serial.println("Displaying Green Light");
                  digitalWrite(g, LOW);
                  digitalWrite(r, HIGH);
                  digitalWrite(b, HIGH);
                  open_lid();

                  delay(3000);

              } else if ((memcmp(uid, masterkey_1,5)==0)||(memcmp(uid, masterkey_2,5)==0)){
                  Serial.println("It is the master key");
                  memcpy(last_uid, clear_uid, 5);
                  open_lid();

                  //Data transfer
                  Serial.println("Transferring Data");
                  data_status = "Mstr";
                  send_info();
                  status = empty;

                  //Displaying green light
                  digitalWrite(g, LOW);
                  digitalWrite(r, HIGH);
                  digitalWrite(b, HIGH);
                  Serial.println("Displaying Green Light");

                  //Delay for 3 seconds
                  delay(3000);

              } else{
                  Serial.println("It is not the same RFID tag. Please scan the same one");

                  //Data transfer
                  Serial.println("Transferring Data");
                  data_status = "Invalid";
                  send_info();

                  //Blinking red light
                  Serial.println("Displaying red light");
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
