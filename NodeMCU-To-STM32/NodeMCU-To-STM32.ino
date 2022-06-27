#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include "credentials_config.h"
// Setting credentials
const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWORD;
char inChar;
String payload = "";
// Setting server name
String serverName = "http://iotgemons.freeoda.com/status.txt";

// Get Request every timerDelay variable value in milliseconds
unsigned long lastTime = 0;
unsigned long timerDelay = 200;
int recordIndex;
int tempIndex = 0;
String recordFrame = "";
String response = "";
void setup() {
  // Setting baudrate to 115200
  Serial.begin(115200);
  // connect to wifi
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  // waiting for wifi to be connected
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.println("Timer set to 5 seconds (timerDelay variable), it will take 5 seconds before publishing the first reading.");
}

void loop() {
  tempIndex = 0;
  recordFrame = "";
  // HTTP request every timerDelay value
  if ((millis() - lastTime) > timerDelay) {
    //Check if wifi is still connected
    if (WiFi.status() == WL_CONNECTED) {
      WiFiClient client;
      HTTPClient http;
      // setting URL for HTTP request
      http.begin(client, serverName.c_str());
      // Send HTTP GET request
      int httpResponseCode = http.GET();
      // Check if server responded
      if (httpResponseCode > 0) {
        // We add # to indicate end of response data
        payload = http.getString();
        Serial.println(payload+'#'); 
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
      // Free resources
      http.end();
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    // Resetting time
    lastTime = millis();
  }

}
