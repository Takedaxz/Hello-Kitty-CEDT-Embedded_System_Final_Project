#define BLYNK_TEMPLATE_ID "XXX"
#define BLYNK_TEMPLATE_NAME "XXX"
#define BLYNK_AUTH_TOKEN "XXX"

#include <DHT.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <Arduino.h>
#include <ESP32Servo.h>
#include <SoftwareSerial.h>
#include <HTTPClient.h>

const int rxPin = 19;
const int txPin = 21;

const String serverName = "XXX";

SoftwareSerial serial2(rxPin, txPin);

String lineToken = "XXX";


#define BLYNK_PRINT Serial
#define DHTPIN 4
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

#define LED_PIN 23
#define LDR_PIN 34
#define SERVO_PIN 14 // Pin connected to the servo signal
#define MOTION_SENSOR_PIN 26 // Motion sensor pin

// WiFi credentials
char ssid[] = "Pattaradanai_2.4G";
char pass[] = "39393939";

BlynkTimer timer;

BLYNK_CONNECTED() {
  Blynk.syncVirtual(V0); // Sync state if needed
  Blynk.syncVirtual(V1);
  Blynk.syncVirtual(V2); // Sync state if needed
  Blynk.syncVirtual(V3);
  Blynk.syncVirtual(V4);
  Blynk.syncVirtual(V5);
  Blynk.syncVirtual(V6);
}


Servo myServo; // Create Servo object

const unsigned long motionCheckInterval = 10000; // ตรวจจับการเคลื่อนไหวใน 10 วินาที
unsigned long lastMotionTime = 0;                // เวลาในการตรวจจับการเคลื่อนไหวล่าสุด
bool motionDetected = false;
int check = 0;  
unsigned long currentTime = millis();
float checkIDR = 0;
bool doorOpen = false;

void sendDataToGoogleSheets(float sensorValue, String sensorName) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String serverPath = serverName + "?sensor=" + sensorName + "&value=" + String(sensorValue);
    http.begin(serverPath.c_str());
    int httpResponseCode = http.GET();
    
    if (httpResponseCode > 0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
    } else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    http.end();
  } else {
    Serial.println("WiFi Disconnected");
  }
}
void sendSensorData() {
  float temp = dht.readTemperature();
  float humidity = dht.readHumidity();

  // Check if readings are valid
  if (isnan(temp) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }


  sendDataToGoogleSheets(temp, "Temperature");
  sendDataToGoogleSheets(humidity, "Humidity");
  sendDataToGoogleSheets(checkIDR, "LED");

  // Send data to Blynk
  Blynk.virtualWrite(V0, temp);      // Send temperature to Virtual Pin V0
  Blynk.virtualWrite(V1, humidity); // Send humidity to Virtual Pin V1
  Blynk.virtualWrite(V2, checkIDR); // Send checkIDR to Virtual Pin V2
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


void setup() {
    // Initialize serial communication
  Serial.begin(115200);
  serial2.begin(9600);

  pinMode(LED_PIN, OUTPUT);
  pinMode(MOTION_SENSOR_PIN, INPUT);
  myServo.attach(SERVO_PIN);

  // Initialize DHT sensor
  dht.begin();

  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi!");

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  
  timer.setInterval(300000L, sendSensorData);

}

void loop() {
  Blynk.run();
  timer.run();


  int ldrValue = analogRead(LDR_PIN);
  if (ldrValue < 800) {
    digitalWrite(LED_PIN, HIGH); // เปิด LED
    checkIDR = 1;
  } else {
    digitalWrite(LED_PIN, LOW);
    checkIDR = 0;
  }

  if (serial2.available()) {
    String receivedData = serial2.readStringUntil('\n');
    receivedData.trim(); // ลบช่องว่างและ newline ออก


    if (receivedData == "true" && !doorOpen) {
      Serial.println("Password Correct! Opening Door...");
      myServo.write(90); // เปิดประตู
      motionDetected = true;
      Blynk.virtualWrite(V4, "Door is opening"); // Send door to Virtual Pin V4
      Blynk.virtualWrite(V3, "Motion Detected!!"); // Send motionDetected to Virtual Pin V3
      sendDataToGoogleSheets(1, "Motion");
      sendDataToGoogleSheets(1, "Servo");
      lastMotionTime = millis();
      doorOpen = true;

    } else if (receivedData == "false" && !doorOpen) {
      sendLineNotification("🚨 Password Wrong 3 times check your gargage!!!!!!!!");
    }

  }


  if(digitalRead(MOTION_SENSOR_PIN) == HIGH && !doorOpen && !motionDetected) {
    motionDetected = true;
    Serial.println("Motion detected");
    Blynk.virtualWrite(V3, "Motion Detected!!"); // Send motionDetected to Virtual Pin V3
    sendDataToGoogleSheets(1, "Motion");
    sendLineNotification("🚨 Motion detected at your garage! IP: http://127.0.0.1:5500/Hello-Kitty-CEDT-Embedded_System_Final_Project/Website/index.html"); //Send notifications
    currentTime = millis();
    lastMotionTime = currentTime;
    
  }

  if(currentTime - lastMotionTime > 120000 && !doorOpen) {
    motionDetected = false;
    Serial.println("No Motion detected");
    Blynk.virtualWrite(V3, "No Motion Detected!!"); // Send motionDetected to Virtual Pin V3
    sendDataToGoogleSheets(0, "Motion");
    currentTime = millis();
    lastMotionTime = currentTime;
    delay(1000);
  }



  if(doorOpen) {
    currentTime = millis();
    if (digitalRead(MOTION_SENSOR_PIN) == HIGH) {
      if(currentTime - lastMotionTime <= motionCheckInterval) {
        lastMotionTime = currentTime;
      }
    }
    
    if (currentTime - lastMotionTime > motionCheckInterval) {
      doorOpen = false;
      motionDetected = false;
      myServo.write(0);
      Serial.println("No Motion detected!");
      Blynk.virtualWrite(V3, "No Motion Detected!!"); // Send motionDetected to Virtual Pin V3
      Serial.println("Closing the Door...!");
      Blynk.virtualWrite(V4, "Door is Closing"); // Send door to Virtual Pin V4
      Blynk.virtualWrite(V5, 0);
      sendDataToGoogleSheets(0, "Motion");
      sendDataToGoogleSheets(0, "Servo");
      currentTime = millis();
      lastMotionTime = currentTime;
      delay(2000);
    }

  }
}

BLYNK_WRITE(V5) {
  int buttonState = param.asInt();  // รับค่าปุ่มเป็น 0 หรือ 1
  if (buttonState == 1) {
    if(!doorOpen){
      motionDetected = true;
      doorOpen = true;
      myServo.write(90);
      Blynk.virtualWrite(V4, "Door is opening"); // Send door to Virtual Pin V4
      sendDataToGoogleSheets(1, "Motion");
      sendDataToGoogleSheets(1, "Servo");
      lastMotionTime = millis();
    }
    
  } else {
    if(doorOpen) {
      doorOpen = false;
      motionDetected = false;
      Serial.println("Closing the Door...!");
      myServo.write(0);
      Blynk.virtualWrite(V4, "Door is Closing"); // Send door to Virtual Pin V4
      sendDataToGoogleSheets(0, "Motion");
      sendDataToGoogleSheets(0, "Servo");
      currentTime = millis();
      lastMotionTime = currentTime;
      delay(2000);  
    }
    
  }
}

BLYNK_WRITE(V6) {
  int buttonState2 = param.asInt();  // รับค่าปุ่มเป็น 0 หรือ 1
  if (buttonState2 == 1) {
    if(!doorOpen){
      motionDetected = true;
      doorOpen = true;
      myServo.write(90);
      Blynk.virtualWrite(V4, "Door is opening"); // Send door to Virtual Pin V4
      sendDataToGoogleSheets(1, "Motion");
      sendDataToGoogleSheets(1, "Servo");
      lastMotionTime = millis();
    }
    
  } else {
    if(doorOpen && !motionDetected) {
      doorOpen = false;
      motionDetected = false;
      Serial.println("Closing the Door...!");
      myServo.write(0);
      Blynk.virtualWrite(V4, "Door is Closing"); // Send door to Virtual Pin V4
      sendDataToGoogleSheets(0, "Motion");
      sendDataToGoogleSheets(0, "Servo");
      currentTime = millis();
      lastMotionTime = currentTime;
      delay(2000);  
    }
    
  }
}
