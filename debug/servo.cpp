//Library for the Servo Motor
#include <Servo.h>
//Import as myservo
Servo myservo;

//Ports of RFID
#define RFID_SS  53
#define RFID_RST 49

// Library for RFID
#include <SPI.h>
#include <MFRC522.h>

//Import MFRC522 function and use it as rfid
MFRC522 rfid(RFID_SS, RFID_RST);

byte data[MAX_LEN];

void close_lid();
void open_lid();

int status = 0;
int trial = 0;

void setup() {
  SPI.begin();
  Serial.begin(9600);
  rfid.begin();
  Serial.print("\n");
  //Connect the servo to pin 21
  myservo.attach(21);

  //Initial Degree is 180
  myservo.write(180);
}

void loop() {

      if (status==0){
      trial += 1;
      Serial.print("Trial ");
      Serial.print(trial);
      Serial.println(": Opening the lid");
      open_lid();
      status = 1;
      Serial.println("---The lid has successfully opened---");
      Serial.print("\n");
      delay(500);

      } else {
      Serial.print("Trial ");
      Serial.print(trial);
      Serial.println(": Closing the lid");
      close_lid();
      status = 0;
      Serial.println("---The lid has successfully closed---");
      Serial.print("\n");
      delay(500);
    }

    }

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
