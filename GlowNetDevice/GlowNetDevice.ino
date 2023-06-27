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
//#include <SPIFFSEditor.h>
//#include <StringArray.h>
//#include <WebAuthentication.h>
//#include <WebHandlerImpl.h>
//#include <WebResponseImpl.h>
#include <FastLED.h>
#include <Preferences.h>

#include "configuration.h"

//Setup preferences
Preferences profiles;


const int ledPin = 3;
const int buttonPin = 4;
const int buzzerPin = 2;
int buttonState = 0;
#define NUM_LEDS 12
CRGB leds[NUM_LEDS];

String header;


//Set web server port
AsyncWebServer server(80);

//Create index file
const char index_html[] PROGMEM = R"rawliteral(
  <!DOCTYPE HTML>
  <html>
    <head>
      <title>Safety LED Configuratiom</title>
    </head>
    <body>
      <h1>Profiles</h1>
      <label for='r'>Red:</label><input type=number id='r' name='r'><br>
      <label for='b'>Blue:</label><input type=number id='b' name='b'><br>
      <label for='g'>Green:</label><input type=number id='g' name='g'><br>
    </body>
  </html>
)rawliteral";

const char profileUpdate_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>Update Profiles</title>
</head>
  <body>
    <h1>Edit Profiles</h1>
    <form action='' method='get'>
    <label for='profileNumber'>Profile Number:</label>
    <input required type='number' size='2' max='99' min='0' id='profileNumber' name='profileNumber' value='%PROFILEPLACEHOLDER%'><br>
    <label for='color'>LED Color:</label>
    <input required type='color' name='color' id='color' value='#FFFFFF'><br>
    <label for='hertz'>Buzzer Hertz:</label>
    <input required type='number' size='4' max='6000' min='1500' id='hertz' name='hertz' value='5000'><br>
    <label for='timeLength'>Length of Buzeer(ms):
    <input id='timeLength' name='timeLength' type='number' size='4' max='9999' min='0' required><br>
    <br><input id='submit' name='submit' type='submit' value='Save'>
    </form>
  </body>
</html>
)rawliteral";

String profileProcessor(const String& var){
  if(var == "PROFILEPLACEHOLDER"){
    return "01";
  }
}

bool updateDome(String profileID){
  //Start preferences
  profiles.begin("profiles", false);

  //Set color variable
  String colorProfile = profileID + "-color";
  char colorProfileChar[9];
  colorProfile.toCharArray(colorProfileChar, 9);
  String color = profiles.getString(colorProfileChar).substring(1);
  char totalColorValues[8];
  color.toCharArray(totalColorValues, 8);

  unsigned long colorValuesTotal = strtol(totalColorValues, NULL, 16);
  byte r = colorValuesTotal >> 16;
  byte g = colorValuesTotal >> 8;
  byte b = colorValuesTotal;

  //Set timeLength variable
  String timeLengthProfile = profileID + "-timeLength";
  char timeLengthProfileChar[14];
  timeLengthProfile.toCharArray(timeLengthProfileChar, 14);
  int timeLength = profiles.getInt(timeLengthProfileChar);

  for (int i = 0; i < NUM_LEDS; i++){
      leds[i].setRGB( r, g, b);
    }
    FastLED.show();
    Serial.println(timeLength);
    digitalWrite(buzzerPin, HIGH);
    delay(timeLength);
    digitalWrite(buzzerPin, LOW);

  return profiles.isKey(colorProfileChar);

  profiles.end();

  

}

void setup() {
  Serial.begin(115200);  //Turn on serial connection
  delay(1000);
  //WiFi.softAP(ssid, password);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  //IPAddress IP = WiFi.softAPIP();

  Serial.println("\nConnecting");
  while(WiFi.status() != WL_CONNECTED){
        Serial.print(".");
        delay(100);
    }

  IPAddress IP = WiFi.localIP();
  Serial.println("AP IP address: ");
  Serial.println(IP);

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });

  server.on("/api", HTTP_GET, [](AsyncWebServerRequest *request){
    if(!request->authenticate(apiuser, apikey))
     return request->requestAuthentication();
    String profileNumber;
    if (request->hasParam("profile")){
      profileNumber = request->getParam("profile")->value();
      if(updateDome(profileNumber)){
        request->send(200, "text/html", "Success");
      }
      else if(!updateDome(profileNumber)){
        request->send(406, "text/html", "Profile does not exist");
      }
    }
  });

  server.on("/profile", HTTP_GET, [](AsyncWebServerRequest *request){
    if(!request->authenticate(apiuser, apikey))
     return request->requestAuthentication();
     if (request->hasParam("submit")){
       //Start preferences
      profiles.begin("profiles", false);
      String profileID = request->getParam("profileNumber")->value();

      //Set color variable
      String colorProfile = profileID + "-color";
      char colorProfileChar[9];
      colorProfile.toCharArray(colorProfileChar, 9);
      String color = request->getParam("color")->value();
      profiles.putString(colorProfileChar, color);

      //Set timeLength variable
      String timeLengthProfile = profileID + "-timeLength";
      char timeLengthProfileChar[14];
      timeLengthProfile.toCharArray(timeLengthProfileChar, 14);
      profiles.putInt(timeLengthProfileChar, request->getParam("timeLength")->value().toInt());
     }
    request->send_P(200, "text/html", profileUpdate_html, profileProcessor);
  });

  FastLED.addLeds<NEOPIXEL, ledPin>(leds, NUM_LEDS);
  pinMode(buttonPin, INPUT);
  pinMode(buzzerPin, OUTPUT);

  //Start server
  server.begin();

  profiles.begin("profiles", false);

  profiles.putString("00-color", "#000000");
  profiles.putInt("00-timeLength", 0);

  profiles.end();

  updateDome("00");

  //Startup sequence
  for(int j = 0; j < 50; j++){
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
  updateDome("00");
}


void loop() {
  
}
