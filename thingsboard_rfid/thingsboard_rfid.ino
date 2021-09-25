#include <ArduinoJson.h>
#include "ThingsBoard.h"
#include <ESP8266WiFi.h>
#include <SPI.h>
#include <MFRC522.h>

//---------- WIFI and Device Configuration----------//
#define WIFI_AP             "realme X"
#define WIFI_PASSWORD       "whateverittakes"
#define TOKEN               "KDhtyDyIDFQ4t3iDZXtU"
#define THINGSBOARD_SERVER  "demo.thingsboard.io"

//***************************************************
WiFiClient espClient;     //Initialize ThingsBoard client
ThingsBoard tb(espClient);      // Initialize ThingsBoard instance
int status = WL_IDLE_STATUS;    // the Wifi radio's status
#define SERIAL_DEBUG_BAUD   115200   //--- Baud rate for debug serial

//------------RFID pins and class configuration---------/
constexpr uint8_t RST_PIN = 5;     // Configurable, see typical pin layout above
constexpr uint8_t SS_PIN = 4;     // Configurable, see typical pin layout above

MFRC522 mfrc522(SS_PIN, RST_PIN); // Instance of the class
MFRC522::MIFARE_Key key; 

String UIDstring="";    //To get UID using .byte function
String content="";      //To store UID in string type
String car="";          //To compare uid
char akash[100];

void setup() {
  
  Serial.begin(SERIAL_DEBUG_BAUD);    // initialize serial for debugging
  SPI.begin();          // Init SPI bus
  mfrc522.PCD_Init();   // Init MFRC522 
  for (byte i = 0; i < 6; i++) 
  {    
    key.keyByte[i] = 0xFF;          //To get the key for rfid scanner type 
  }
  WiFi.begin(WIFI_AP, WIFI_PASSWORD);   // Initialising WiFi 
  InitWiFi();
}

void loop() {
  delay(1000);

  if (WiFi.status() != WL_CONNECTED) {
    reconnect();
  }

  if (!tb.connected()) {                  //TO connect to thingsboard server 
    // Connect to the ThingsBoard
    Serial.print("Connecting to: ");
    Serial.print(THINGSBOARD_SERVER);
    Serial.print(" with token ");
    Serial.println(TOKEN);
    if (!tb.connect(THINGSBOARD_SERVER, TOKEN)) {
      Serial.println("Failed to connect");
      return;
    }
  }
  Serial.println("Sending data...");

  if ( ! mfrc522.PICC_IsNewCardPresent())   //------- Look for new cards---------//
    return;

  if ( ! mfrc522.PICC_ReadCardSerial())   //------- Verify if the NUID has been readed--------//
    return;

  for (byte i = 0; i < mfrc522.uid.size; i++) {     //------ Store UID into UIDstring and content------//
    UIDstring = UIDstring + String(mfrc522.uid.uidByte[i]);
    //Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "); //Conditional operator - If the byte is less than hexadecimal 16 then 
    //Serial.print(mfrc522.uid.uidByte[i], HEX);
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));              // These two lines used for storing UID
  } 
   
  content.toUpperCase();
  /*if(content != car){                     //compares old and new uid to make sure an ID is not repeated twice in one cycle
    Serial.println("A CAR BROKE SIGNAL!!!");
    Serial.print("CAR ID:");
    Serial.println(content);
    //uid=content.toInt();
    //Serial.print("uid> ");
    //Serial.println(uid); 
  }
  car=content;
  delay(1000);
  content="";
*/
  content.toCharArray(akash,12);
  Serial.println(akash);
  tb.sendTelemetryString("UID", akash);    //Send value to thingsboard server

  //tb.loop();
}

void InitWiFi()
{
  Serial.println("Connecting to AP ...");
  // attempt to connect to WiFi network

  WiFi.begin(WIFI_AP, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to AP");
}

void reconnect() {
  // Loop until we're reconnected
  status = WiFi.status();
  if ( status != WL_CONNECTED) {
    WiFi.begin(WIFI_AP, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("Connected to AP");
  }
}
