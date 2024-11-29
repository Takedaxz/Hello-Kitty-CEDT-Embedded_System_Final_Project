#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>

#define LED_PIN 23
#define LDR_PIN 34

// Replace with your WiFi credentials
const char* ssid = "testWifi";
const char* password = "1234567890";

// LINE Notify token
String lineToken = "9szxfrDJKWMtGkpO0txNctfrHGmsnKha8rYu6xfRKXj";

void setup() {
  Serial.begin(115200);
  
  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi!");
}

void sendLineNotification(String message) {
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(115200);
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin("https://notify-api.line.me/api/notify");
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    http.addHeader("Authorization", "Bearer " + lineToken);

    String payload = "message=" + message;
    int httpResponseCode = http.POST(payload);
    
    if (httpResponseCode > 0) {
      Serial.println("Notification sent: " + String(httpResponseCode));
    } else {
      Serial.println("Error sending notification: " + String(httpResponseCode));
    }
    http.end();
  } else {
    Serial.println("WiFi not connected");
  }
}

void loop() {
  bool text = true; 
  Serial.println("Waiting for message...");
  String check=Serial.readString();
  if (check=="false") {
    
  } else {
    text=false;
  }
  
  if (text) {
    sendLineNotification("🚨 Motion detected at your garage!");
    delay(3000);
  }
}