/*****************************
This program is run on an ESP32 and accepts API requests to change an LED Ring's colors and send button presses back to a main console. 

*****************************/

//Include LIbraries
#include <WiFi.h>
#include <AsyncTCP.h>
#include <AsyncEventSource.h>
#include <AsyncJson.h>
#include <AsyncWebSocket.h>
#include <AsyncWebSynchronization.h>
#include <ESPAsyncWebServer.h>
#include <FastLED.h>
#include <Preferences.h>

#include "configuration.h"

//Define Pins
const int ledPin = 3;
const int buttonPin = 4;
const int buzzerPin = 2;
int buttonState = 0;

//Create LED ring object
#define NUM_LEDS 12
CRGB leds[NUM_LEDS];

//Set web server port
AsyncWebServer server(80);

//Fuction used to update the dome and send a buzzer sound.
bool sendDomeCommand(String color, int buzzerLength){
  //Strip the # character from the color code and convert to rgb values
  String colorRecieved = color.substring(1);
  char totalColorValues[8];
  colorRecieved.toCharArray(totalColorValues, 8);
  unsigned long colorValuesTotal = strtol(totalColorValues, NULL, 16);
  byte r = colorValuesTotal >> 16;
  byte g = colorValuesTotal >> 8;
  byte b = colorValuesTotal;

  //Set the LEDs to the requested color
  for (int i = 0; i < NUM_LEDS; i++){
      leds[i].setRGB( r, g, b);
    }
    FastLED.show();

    //Play buzzer for requested length
    digitalWrite(buzzerPin, HIGH);
    delay(buzzerLength);
    digitalWrite(buzzerPin, LOW);

    return true;
}

void setup() {
  Serial.begin(115200);  //Turn on serial connection
  delay(1000);

  //Connect to WiFi network defined in configuration.h
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  //Print connection status and ip address once connected. 
  Serial.println("\nConnecting");
  while(WiFi.status() != WL_CONNECTED){
        Serial.print(".");
        delay(100);
    }

  IPAddress IP = WiFi.localIP();
  Serial.println("\nDevice IP address: ");
  Serial.println(IP);

  //Processor for api requests
  server.on("/api", HTTP_GET, [](AsyncWebServerRequest *request){
    if(!request->authenticate(apiuser, apikey))
     return request->requestAuthentication();
    if ((request->hasParam("color")) && (request->hasParam("buzzer"))){
      sendDomeCommand(request->getParam("color")->value(), request->getParam("buzzer")->value().toInt());
      request->send(200, "text/html", "Success");
    }
    else{
      request->send(400, "text/html", "Please submit a color and buzzer variables");
    }
  });

  //Creates FastLED Object
  FastLED.addLeds<NEOPIXEL, ledPin>(leds, NUM_LEDS);

  //Setup button and buzzer pins
  pinMode(buttonPin, INPUT);
  pinMode(buzzerPin, OUTPUT);

  //Start web server
  server.begin();
  
  //Startup sequence
  digitalWrite(buzzerPin, HIGH);
  
  for(int j = 0; j < 30; j++){
    static uint8_t hue = 0;
    for(int i = 0; i < NUM_LEDS; i++) {
      // Set the i'th led to red 
      leds[i] = CHSV(hue++, 255, 255);
      // Show the leds
      FastLED.show(); 
      for(int i = 0; i < NUM_LEDS; i++) { leds[i].nscale8(250); }
      // Wait a little bit before we loop around and do it again
      delay(10);
    }
  }

  digitalWrite(buzzerPin, LOW);

  //Turn off dome lights
  sendDomeCommand("#000000", 0);
}


void loop() {
  //Trigger Profile 01 on button press. 
  buttonState = digitalRead(buttonPin);
  if (buttonState == HIGH){
    sendDomeCommand("#FFFFFF", 0);
  }
}
