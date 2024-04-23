#include <SPI.h>
#include <TFT_eSPI.h>
#include "gothicFont.h"
#include "gothicFont24.h"
#include "i04n2x.h"
#include <stdlib.h>
#include <string.h>

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite sprite = TFT_eSprite(&tft);
TFT_eSprite sprite2 = TFT_eSprite(&tft);
TFT_eSprite sprite3 = TFT_eSprite(&tft);

void bigTextTest24(String s, int x, int y);
void bigTextTest48(String s, int x, int y);
void bigTextTest72(String s, int x, int y);


#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <Arduino_JSON.h>
const char* ssid = "id"; 
const char* password = "password"; 
String openWeatherMapApiKey = "key";
String city = "city";
String countryCode = "country";

unsigned long lastTime = 0;
unsigned long timerDelay = 10000;
String jsonBuffer;
//String jsonBuffer2;

void setup() {
  Serial.begin(115200);

  tft.begin();
  tft.setRotation(1);
  tft.setSwapBytes(true);
  tft.fillScreen(TFT_BLACK);



  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.println("Timer set to 10 seconds (timerDelay variable), it will take 10 seconds before publishing the first reading.");

  //for debug 
  Serial.println(ESP.getFreeHeap());
  

  bigTextTest48("Istanbul",20,10);
  trInfo(250,3);
  weatherImage(350, 50);
  bigTextTest72("01:26",10,70);
  bigTextTest24("23/04/2024",10,180);
  bigTextTest24C("Tue",180,180, TFT_GREEN);
  bigTextTest24("Temp: 15°C",20,240);
  bigTextTest24("Humid: 77%",20,280);

  Serial.println(ESP.getFreeHeap());
}
String serverPath = "http://api.openweathermap.org/data/2.5/weather?q=" + city + "," + countryCode + "&APPID=" + openWeatherMapApiKey + "&units=metric&lang=tr";
String tempT="";
String tempH="";
String tmpp;
String tmpp2;
void loop() {
  
    delay(3000);
    // Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED){

      
      
      
      jsonBuffer = httpGETRequest(serverPath.c_str());
      Serial.println(ESP.getFreeHeap());
      delay(1000);
      
      Serial.println(jsonBuffer);
      
/*
      jsonBuffer = "";
      Serial.println(ESP.getFreeHeap());
      delay(1000);
*/
      JSONVar myObject = JSON.parse(jsonBuffer);
      Serial.println(ESP.getFreeHeap());
      jsonBuffer = "";
      Serial.println(ESP.getFreeHeap());
      delay(1000);
      // JSON.typeof(jsonVar) can be used to get the type of the var
      if (JSON.typeof(myObject) == "undefined") {
        Serial.println("Parsing input failed!");
        return;
      }
    
      Serial.print("JSON object = ");
      Serial.println(myObject);
      Serial.print("Temperature: ");
      tempT=JSON.stringify(myObject["main"]["temp"]);
      Serial.println(tempT);
      
      //bigTextTest24(tempTH,20,240);

      Serial.print("Pressure: ");
      Serial.println(myObject["main"]["pressure"]);
      Serial.print("Humidity: ");
      tempH=JSON.stringify(myObject["main"]["humidity"]);
      Serial.println(tempH);
      
      Serial.print("Wind Speed: ");
      Serial.println(myObject["wind"]["speed"]);

      Serial.println(ESP.getFreeHeap());

      Serial.print("weather: ");
      Serial.println(myObject["weather"]);
      JSONVar myObjectweather =myObject["weather"][0];
      Serial.println(myObjectweather);
      Serial.print("icon: ");
      Serial.println(myObjectweather["icon"]);

      String serverPath2 = "http://worldtimeapi.org/api/timezone/Europe/Istanbul";
      jsonBuffer = httpGETRequest(serverPath2.c_str());  //jsonBuffer2
      Serial.println(jsonBuffer); //jsonBuffer2
      myObject = JSON.parse(jsonBuffer); //jsonBuffer2
      if (JSON.typeof(myObject) == "undefined") {
        Serial.println("Parsing input failed!");
        return;
      }
      Serial.print("JSON object = ");
      Serial.println(myObject);
      Serial.print("datetime: ");
      Serial.println(myObject["datetime"]);
      Serial.println(ESP.getFreeHeap());
      String timee = JSON.stringify(myObject["datetime"]);
      Serial.println(myObject["datetime"]);
      Serial.println(timee);
      Serial.println(ESP.getFreeHeap());
      Serial.println("test1");
      //Serial.println(timee.substring(0,10));
      //Serial.println(timee.substring(11,16));
      Serial.println("test2");
      tmpp=timee.substring(12,17);
      Serial.println(tmpp);
      Serial.println("test2.5");
      tmpp2=timee.substring(1,11);
      Serial.println(tmpp2);
      Serial.println("test2.75");
      //bigTextTest72(tmpp,10,70); //time 
      Serial.println("test3");
      
      //bigTextTest24(tmpp2,10,180);
      Serial.println("test4");

      Serial.println(ESP.getFreeHeap());
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    
    delay(10000);
    Serial.println("After Delay");
    Serial.println(tempT);
    Serial.println(tempH);
    Serial.println(tmpp);
    bigTextTest24(tempT,20,240);
    bigTextTest24(tempH,20,280);
    bigTextTest72(tmpp,10,70); //time 
    bigTextTest24(tmpp2,10,180);
}



String httpGETRequest(const char* serverName) {
  WiFiClient client;
  HTTPClient http;
    
  // Your IP address with path or Domain name with URL path 
  http.begin(client, serverName);
  
  // Send HTTP POST request
  int httpResponseCode = http.GET();
  
  String payload = "{}"; 
  
  if (httpResponseCode>0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();

  return payload;
}



void bigTextTest48(String s, int x, int y){
  
  sprite.createSprite(200,100);
  sprite.fillSprite(TFT_BLACK);
  sprite.loadFont(gothicFont);
  sprite.setTextColor(TFT_WHITE, TFT_BLACK);
  sprite.drawString(s,0,0);
  sprite.unloadFont();
  sprite.pushSprite(x,y);
  sprite.deleteSprite();
}

void bigTextTest24(String s, int x, int y){
  
  sprite3.createSprite(200,50);
  sprite3.fillSprite(TFT_RED);
  sprite3.loadFont(gothicFont24);
  sprite3.setTextColor(TFT_WHITE, TFT_BLACK);
  sprite3.drawString(s,0,0);
  sprite3.unloadFont();
  sprite3.pushSprite(x,y);
  sprite3.deleteSprite();
}

void bigTextTest24C(String s, int x, int y, uint32_t colour){
  
  sprite.createSprite(200,100);
  sprite.fillSprite(TFT_BLACK);
  sprite.loadFont(gothicFont24);
  sprite.setTextColor(colour, TFT_BLACK);
  sprite.drawString(s,0,0);
  sprite.unloadFont();
  sprite.pushSprite(x,y);
  sprite.deleteSprite();
}

void bigTextTest72(String s, int x, int y){
  
  
  sprite.deleteSprite();
  sprite2.createSprite(64,100);
  sprite2.fillSprite(TFT_BLACK);
  sprite2.setCursor(0,0,7);
  sprite2.setTextColor(TFT_YELLOW);
  sprite2.setTextSize(2);
  sprite2.print(s.substring(0,1));

  sprite2.pushSprite(14,70); //0

  sprite2.fillSprite(TFT_BLACK);
  sprite2.setCursor(0,0,7);
  sprite2.setTextColor(TFT_YELLOW);
  sprite2.setTextSize(2);
  sprite2.print(s.substring(1,2));
  sprite2.pushSprite(78,70); //0

  sprite2.deleteSprite();
  sprite2.createSprite(24,100); //14
  sprite2.fillSprite(TFT_BLACK);
  
  sprite2.setCursor(0,0,7);
  sprite2.setTextColor(TFT_YELLOW);
  sprite2.setTextSize(2);
  sprite2.print(":");

  sprite2.pushSprite(142,70); //:


  sprite2.deleteSprite();
  sprite2.createSprite(64,100);
  sprite2.fillSprite(TFT_BLACK);

  sprite2.setCursor(0,0,7);
  sprite2.setTextColor(TFT_YELLOW);
  sprite2.setTextSize(2);
  sprite2.print(s.substring(3,4));

  sprite2.pushSprite(166,70); //0

  sprite2.fillSprite(TFT_BLACK);
  sprite2.setCursor(0,0,7);
  sprite2.setTextColor(TFT_YELLOW);
  sprite2.setTextSize(2);
  sprite2.print(s.substring(4,5));
  sprite2.pushSprite(230,70); //0

  sprite.deleteSprite();
  sprite2.deleteSprite();
  sprite.createSprite(200,100);
}



void trInfo(int x, int y){
  
  sprite.createSprite(100,100);
  sprite.fillSprite(TFT_BLACK);
  sprite.loadFont(gothicFont);
  sprite.setTextColor(TFT_WHITE, TFT_RED);
  sprite.fillRoundRect(  0, 0,  80, 50, 15, TFT_RED);
  sprite.drawString("TR",15,7);
  sprite.unloadFont();
  sprite.pushSprite(x,y);
  sprite.deleteSprite();
}

void weatherImage(int x, int y){
  sprite.createSprite(100,100);
  sprite.fillSprite(TFT_BLACK);
  sprite.setSwapBytes(true);
  sprite.pushImage(0,0,100,100,i04n2x);
  sprite.pushSprite(x,y,TFT_BLACK);

  sprite.createSprite(100,100);
  sprite.fillSprite(TFT_BLACK);
  sprite.loadFont(gothicFont24);
  sprite.fillRoundRect(  0, 0,  100, 30, 10, TFT_WHITE);
  sprite.setTextColor(TFT_BLACK, TFT_WHITE);
  sprite.drawString("Clouds",10,5);
  sprite.pushSprite(x,y+80);
  sprite.deleteSprite();
}