#include <SPI.h>
#include <MFRC522.h>


constexpr uint8_t RST_PIN = 5;     // Configurable, see typical pin layout above
constexpr uint8_t SS_PIN = 4;     // Configurable, see typical pin layout above
 
MFRC522 mfrc522(SS_PIN, RST_PIN); // Instance of the class
MFRC522::MIFARE_Key key; 

String UIDstring="";    //To get UID using .byte function
String content="";      //To store UID in string type
String car="";          //To compare uid
char id_send[100];
void setup() { 
  Serial.begin(115200);
  SPI.begin();          // Init SPI bus
  mfrc522.PCD_Init();   // Init MFRC522 

  for (byte i = 0; i < 6; i++) {    //To get the key for rfid scanner type 
    key.keyByte[i] = 0xFF;
  }
  Serial.println(F("This code scan the MIFARE Classsic NUID.")); //just to check working
  //Serial.print(F("Using the following key:"));
  //printHex(key.keyByte, MFRC522::MF_KEY_SIZE);
}

void loop() {

  if (mfrc522.PICC_IsNewCardPresent())   //------- Look for new cards---------//
    {
      

  if ( ! mfrc522.PICC_ReadCardSerial())   //------- Verify if the NUID has been read--------//
    return;

  for (byte i = 0; i < mfrc522.uid.size; i++) {     //------ Store UID into UIDstring and content------//
    UIDstring = UIDstring + String(mfrc522.uid.uidByte[i]);
    //Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "); //Conditional operator - If the byte is less than hexadecimal 16 then 
    //Serial.print(mfrc522.uid.uidByte[i], HEX);
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));              // These two lines used for storing UID
  } 
   
  content.toUpperCase();
 /* if(content != car){                     //compares old and new uid to make sure an ID is not repeated twice in one cycle
    Serial.println("A CAR BROKE SIGNAL!!!");
    Serial.print("CAR ID:");
    Serial.println(content);
    //uid=content.toInt();
    Serial.print("uid> ");
    Serial.println(uid); 
  }
  car=content;
  */content.toCharArray(id_send,12);
  Serial.println(id_send);
  delay(1000);
  content="";
}}

