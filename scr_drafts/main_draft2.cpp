
#include <Servo.h>
Servo myservo;

#include <SPI.h>
#include <MFRC522.h>

#define RFID_SS  53
#define RFID_RST 49

MFRC522 rfid( RFID_SS, RFID_RST );

byte data[MAX_LEN];
byte uid[5];
byte last_uid[5];
byte prev_uid[5];
byte clear_uid[] = {0,0,0,0,0};
char Number[40];
int r = 7;
int g = 6;
int b = 5;
int UV = 4;
int led = 13;
byte mstr0_uid[] = {25,19,93,162,245};
byte mstr1_uid[] = {20,105,18,43,68};
bool nempty = false;
bool empty = true;
bool stst = empty;
bool dopen = !true;
String Ustat = "";
int sheetC=2;

unsigned long currentTime;
unsigned long cloopTime;
int ledPins[] = { 4, 5 ,6, 7};
///////////////////////////////////////////
void setup() {
  SPI.begin();
  Serial.begin(9600);
  rfid.begin();
  currentTime = millis();
  cloopTime = currentTime;
  pinMode(led, OUTPUT);
  //////////////
    for (int thisLed = 0; thisLed < 4; thisLed++) {
      pinMode(ledPins[thisLed], OUTPUT);
      digitalWrite(ledPins[thisLed], HIGH);
    }
    myservo.attach(21);  // attaches the servo on pin 9 to the servo object
    myservo.write(180); //  10
      //Serial.println("CLEARSHEET");                 // clears starting at row 1
      //Serial.println("LABEL,Date,Time,Status,Number");

    delay(2000);
    digitalWrite(b, LOW);
    //*/
}

void loop() {

  if ( rfid.requestTag( MF1_REQIDL, data ) == MI_OK ) {
          if ( rfid.antiCollision( data ) == MI_OK ) {
                 memcpy( uid, data, 5 );
                 /*
                  for (int u = 0; u < 5; u++) {
                          Serial.print(uid[u]);
                          Serial.print(", ");
                        }
                       Serial.println("");
                       Serial.println("---scaning---0--");
                        /////////////////////////////////// */
                        sprintf(Number, "%02X%02X%02X%02X%02X" , uid[0], uid[1], uid[2], uid[3], uid[4]);
                   if (stst == empty ){
                                         if (!dopen){
                                                dopen = true;
                                                Lopen() ;  //myservo.write(0);  //170
                                                  Ustat = "Open";
                                                  sendinfo();
                                                 //Serial.println("--just open--");
                                                 digitalWrite(g, LOW);
                                                 digitalWrite(r, HIGH);
                                                 digitalWrite(b, HIGH);
                                                delay(3000);
                                         }
                                           else{

                                    if((memcmp(uid, mstr0_uid,5)==0)||(memcmp(uid, mstr1_uid,5)==0)||(memcmp(uid, prev_uid,5)==0)){
                                       //Serial.println("---mstr-or prev--");
                                       memcpy( prev_uid, clear_uid, 5 );
                                       dopen = false;
                                       digitalWrite(b, LOW);
                                       digitalWrite(r, HIGH);
                                       digitalWrite(g, HIGH);
                                       Lclose();  //myservo.write(180);   //10
                                        Ustat = "Close";
                                        sendinfo();
                                       delay(3000);
                                    }
                                    else {
                                          if (uid[0] != 0){
                                            memcpy( last_uid, uid, 5 );
                                            memcpy( prev_uid, uid, 5 );
                                             stst = nempty;
                                            Lclose();  //myservo.write(180);   //10;  //10
                                           //Serial.println("---save it---on UV");
                                           Ustat = "Use";
                                           sendinfo();
                                           delay(3000);
                                           digitalWrite(r, LOW);
                                           digitalWrite(g, HIGH);
                                           digitalWrite(b, LOW);
                                           digitalWrite(UV, LOW);
                                           for (int t = 0; t < 60; t++) {
                                             delay(500);
                                             digitalWrite(r, !LOW);
                                             digitalWrite(g, HIGH);
                                             digitalWrite(b, !LOW);
                                             
                                             delay(500);
                                             digitalWrite(r, LOW);
                                             digitalWrite(g, HIGH);
                                             digitalWrite(b, LOW);
                                            }
                                           delay(5000);
                                           digitalWrite(UV, HIGH);
                                           delay(2000);
                                           //digitalWrite(r, LOW);
                                           digitalWrite(b, HIGH);
                                           digitalWrite(g, LOW);
                                          }
                                    }
                             }
                   }
                   else {

                         if(memcmp(uid, last_uid,5)==0){
                                //Serial.println("---same---");
                                 memcpy( last_uid, clear_uid, 5 );
                                 // Serial.println("---clear mem---");
                                 Ustat = "Retrieved";
                                 sendinfo();
                                 stst = empty;
                                 digitalWrite(g, LOW);
                                 digitalWrite(r, HIGH);
                                 digitalWrite(b, HIGH);
                                  Lopen() ;  //myservo.write(0);  //170;   //170
                                  //Serial.println("---open--");
                                 delay(3000);
                           }
                         else if((memcmp(uid, mstr0_uid,5)==0)||(memcmp(uid, mstr1_uid,5)==0)){
                             //Serial.println("---master key---");
                                 memcpy( last_uid, clear_uid, 5 );
                                 // Serial.println("---clear mem---");
                                 Ustat = "Mstr";
                                 sendinfo();
                                 stst = empty;
                                 digitalWrite(g, LOW);
                                 digitalWrite(r, HIGH);
                                 digitalWrite(b, HIGH);
                                  Lopen() ;  //myservo.write(0);  //170;   //170
                                  //Serial.println("---open--");
                                delay(3000);
                         }
                         else{
                                //Serial.println("---not same---");
                                Ustat = "Invalid";
                                sendinfo();
                                for (int t = 0; t < 5; t++) {
                                 digitalWrite(r, LOW);
                                 digitalWrite(g, HIGH);
                                 digitalWrite(b, HIGH);
                                 delay(100);
                                 digitalWrite(r, HIGH);
                                 delay(100);
                                }
                                digitalWrite(r, LOW);
                                digitalWrite(b, HIGH);
                                digitalWrite(g, LOW);
                            }
                   }

                          // sendinfo();
          }
          delay(70);
  }


}
void sendinfo(){
         Serial.print("DATA,DATE,TIME," + Ustat);//send the Name to excel
         Serial.print(",");
         Serial.println(Number);

}
void Lopen (){
//open
  for (int pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(10);                       // waits 15ms for the servo to reach the position
  }
}

void Lclose (){
 //close
for (int pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(10);                       // waits 15ms for the servo to reach the position
  }
}
