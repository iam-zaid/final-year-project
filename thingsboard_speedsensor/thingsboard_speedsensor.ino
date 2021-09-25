#include <ArduinoJson.h>
#include "ThingsBoard.h"
#include <ESP8266WiFi.h>

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

//-----*** SPEED SENSOR pin and required variabless--------//

int number = 0;
int sensor = 15;    //Sensor's ouput pin attached here
unsigned long start_time = 0;   //start and end time for calculating rps using steps
unsigned long end_time = 0;
int steps=0;
float steps_old=0;          //For counting steps using old and new steps
float temp=0;             //To store rps temporarily    
float rps=0;              //To store revolutions per minute
float speed_veh;          //For displaying speed


void setup() {
  
  Serial.begin(SERIAL_DEBUG_BAUD);    // initialize serial for debugging
  pinMode(sensor,INPUT_PULLUP);       // Defining pin config
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

 start_time=millis();       //***The logic to calculating speed using sensor
 end_time=start_time+1000;
 while(millis()<end_time)
 {
   if(digitalRead(sensor))
   {
    steps=steps+1;  
    while(digitalRead(sensor));
   }

 }
   //Serial.println("STEPS: ");
   //Serial.print(steps);
    temp=steps-steps_old;
    steps_old=steps;
    rps=(temp/20);
    speed_veh=rps*100;
  
  //tb.sendTelemetryString("words", "trial");
  //delay(1000);
  tb.sendTelemetryFloat("integer", speed_veh);

  tb.loop();
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
