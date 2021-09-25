#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN         9          // Configurable, see typical pin layout above
#define SS_PIN          10         // Configurable, see typical pin layout above
int red=7;
int yellow=6;
int green=5;
int i=0;

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance[ 0
String UIDstring="";                 // USed to detect UID
String content="";  //Used to store
String car="";       //Used to compare 

void setup() {
   Serial.begin(9600);   // Initialize serial communications with the PC
  while (!Serial);    // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
  SPI.begin();      // Init SPI bus
  mfrc522.PCD_Init();   // Initialise MFRC522
  delay(4);       // Optional delay. Some board do need more time after init to be ready, see Readme
  mfrc522.PCD_DumpVersionToSerial();  // Show details of PCD - MFRC522 Card Reader details
  Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));
pinMode(red,OUTPUT);
  digitalWrite(red,HIGH);
pinMode(yellow,OUTPUT);
  digitalWrite(yellow,HIGH);
pinMode(green,OUTPUT);
  digitalWrite(green,HIGH);
  delay(500);
  digitalWrite(green,LOW);
  digitalWrite(yellow,LOW);
  digitalWrite(red,LOW);

}

void greensig(){
  digitalWrite(green,HIGH);
  delay(5000);
  digitalWrite(green,LOW);
}

void yellowsig(){
  digitalWrite(yellow,HIGH);
  delay(2000);
  digitalWrite(yellow,LOW);
}


void loop() {

  i=300;
  greensig();
  delay(50);
  yellowsig();
  delay(50);
  while(i>1){
    digitalWrite(red,HIGH);
    IDdetect();
    i--;
  }
  digitalWrite(red,LOW);
  delay(50);
}

void IDdetect(){
    
  if ( ! mfrc522.PICC_IsNewCardPresent()) {                  // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
    return 0;
  } 
  if ( ! mfrc522.PICC_ReadCardSerial()) {                         // Select one of the cards
    return 0;
  }

  
  //Serial.print("CAR UID:");

  for (byte i = 0; i < mfrc522.uid.size; i++) {                     //This is just for read UID!
    UIDstring = UIDstring + String(mfrc522.uid.uidByte[i]);
    //Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    //Serial.print(mfrc522.uid.uidByte[i], HEX);                        // The above two lines used to extract UID
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));              // These two lines used for storing UID
  } 
  content.toUpperCase();
  if(content != car){
      Serial.println("!!A CAR BROKE SIGNAL!!!!");
      Serial.print("CAR ID:");
      Serial.println(content);
  }
  car=content;
  content=""; 
}

