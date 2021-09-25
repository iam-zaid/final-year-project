/*
  WriteSingleField
  
  Description: Writes a value to a channel on ThingSpeak every 20 seconds.
  
  Hardware: ESP8266 based boards
  
  !!! IMPORTANT - Modify the secrets.h file for this project with your network connection and ThingSpeak channel details. !!!
  
  Note:
  - Requires ESP8266WiFi library and ESP8622 board add-on. See https://github.com/esp8266/Arduino for details.
  - Select the target hardware from the Tools->Board menu
  - This example is written for a network using WPA encryption. For WEP or WPA, change the WiFi.begin() call accordingly.
  
  ThingSpeak ( https://www.thingspeak.com ) is an analytic IoT platform service that allows you to aggregate, visualize, and 
  analyze live data streams in the cloud. Visit https://www.thingspeak.com to sign up for a free account and create a channel.  
  
  Documentation for the ThingSpeak Communication Library for Arduino is in the README.md folder where the library was installed.
  See https://www.mathworks.com/help/thingspeak/index.html for the full ThingSpeak documentation.
  
  For licensing information, see the accompanying license file.
  
  Copyright 2018, The MathWorks, Inc.
*/

#include "ThingSpeak.h"
#include "secrets.h"
#include <ESP8266WiFi.h>

char ssid[] = "realme X";   // your network SSID (name) 
char pass[] = "whateverittakes";   // your network password
int keyIndex = 0;            // your network key Index number (needed only for WEP)
WiFiClient  client;

unsigned long myChannelNumber = 1015405;
const char * myWriteAPIKey = "MCYI7KW77RG0PLE5";

int number = 0;
int sensor = 14;
unsigned long start_time = 0;
unsigned long end_time = 0;
int steps=0;
float steps_old=0;
float temp=0;
float rps=0;

void setup() {
  Serial.begin(115200);  // Initialize serial
  pinMode(sensor,INPUT_PULLUP);
  WiFi.begin(ssid,pass); 
  ThingSpeak.begin(client);  // Initialize ThingSpeak
    // Connect or reconnect to WiFi
/*  if(WiFi.status() != WL_CONNECTED){
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(SECRET_SSID);
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(ssid, pass);  // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      delay(5000);     
  */  
    Serial.println("\nConnected.");
}


void loop() {


  
  // Write to ThingSpeak. There are up to 8 fields in a channel, allowing you to store up to 8 different
  // pieces of information in a channel.  Here, we write to field 1.

   start_time=millis();
 end_time=start_time+1000;
 while(millis()<end_time)
 {
   if(digitalRead(sensor))
   {
    steps=steps+1; 
    while(digitalRead(sensor));
   }
  /* Serial.println("STEPS: ");
   Serial.print(steps);
  */
 }
    temp=steps-steps_old;
    steps_old=steps;
    rps=(temp/20);
    Serial.print("SPEED: ");
    Serial.println(rps);
    if(rps>35){
        Serial.println("OVERSPEEDING OVERSPEEDING");
      }
  ThingSpeak.writeField(myChannelNumber, 1,rps, myWriteAPIKey);
  delay(5000); // Wait 20 seconds to update the channel again
}
