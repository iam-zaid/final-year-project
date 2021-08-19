#include "UbidotsESPMQTT.h"

 
/****************************************
   Define Constants
 ****************************************/
#define TOKEN "BBFF-GN5wtfd7JoSpyIxHP4nGl0vJAvTOUU" // Your Ubidots TOKEN
#define WIFINAME "realme X" //Your SSID
#define WIFIPASS "whateverittakes" // Your Wifi Pass
#define DEVICE_LABEL  "Nodemcu"  // Put here your Ubidots device label
#define VARIABLE1  "servo"  // Ubidots variable name for servo

 
int led = 14;//led pin connection to nodemcu
float value=0; // To store incoming value.
int angle;
 
Ubidots client(TOKEN);
 
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i=0;i<length;i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  if((char)payload[0] == '1'){
    digitalWrite(led,HIGH);
  }
  else{
    digitalWrite(led,LOW);
  }
}

void setup() {
 
  client.ubidotsSetBroker("industrial.api.ubidots.com"); // Sets the broker properly for the business account
  client.setDebug(true); // Pass a true or false bool value to activate debug messages
  Serial.begin(115200);
  pinMode(led,OUTPUT);
  client.wifiConnection(WIFINAME, WIFIPASS);
  client.begin(callback);
  client.ubidotsSubscribe(DEVICE_LABEL, VARIABLE1); //Insert the dataSource and Variable's Labels
  

}

void loop() {

  if (!client.connected()) {
    client.reconnect();
  client.ubidotsSubscribe(DEVICE_LABEL, VARIABLE1); //Insert the dataSource and Variable's Labels:
 // angle=client.getValue(VARIABLE);
  //Serial.println(value);
  }
  client.loop();
}

