#define BLYNK_TEMPLATE_ID "TMPL68GxZvqfm"
#define BLYNK_TEMPLATE_NAME "Temperature Sensor Template"
#define BLYNK_AUTH_TOKEN "dfZ5qkbzl68zmTsVVgIWxSJHlw8E74Jn"

#include <DHT.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <Arduino.h>
#include <ESP32Servo.h>
#include <SoftwareSerial.h>

const int rxPin = 19;
const int txPin = 21;

SoftwareSerial serial2(rxPin, txPin);

#define BLYNK_PRINT Serial
#define DHTPIN 18
#define DHTTYPE DHT11

#define LED_PIN 23
#define LDR_PIN 34
#define SERVO_PIN 14 // Pin connected to the servo signal
#define MOTION_SENSOR_PIN 26 // Motion sensor pin

Servo myServo; // Create Servo object

const unsigned long motionCheckInterval = 5000; // ตรวจจับการเคลื่อนไหวใน 5 วินาที
unsigned long lastMotionTime = 0;                // เวลาในการตรวจจับการเคลื่อนไหวล่าสุด
bool motionDetected = false;
int check = 0;  
unsigned long currentTime = millis();


DHT dht(DHTPIN, DHTTYPE);

// WiFi credentials
char ssid[] = "J";
char pass[] = "123456789";

BlynkTimer timer;

//BLYNK_CONNECTED() {
  //Blynk.syncVirtual(V0); // Sync state if needed
  //Blynk.syncVirtual(V1);
  //Blynk.syncVirtual(V2); // Sync state if needed
  //Blynk.syncVirtual(V3);
//}
// This function sends temperature and humidity data to Blynk
void sendSensorData() {
  float temp = dht.readTemperature();
  float humidity = dht.readHumidity();

  // Check if readings are valid
  if (isnan(temp) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.print("Temp: ");
  Serial.print(temp);
  Serial.print(" C, Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");

  // Send data to Blynk
  //Blynk.virtualWrite(V0, temp);      // Send temperature to Virtual Pin V0
  //Blynk.virtualWrite(V1, humidity); // Send humidity to Virtual Pin V1
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

  // Connect to WiFi and Blynk
  //Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  
  timer.setInterval(10000L, sendSensorData);



}

void loop() {
  //Blynk.run();
  timer.run();

  if (serial2.available()) {
    String receivedData = serial2.readStringUntil('\n');
    receivedData.trim(); // ลบช่องว่างและ newline ออก

    if (receivedData == "true") {
      Serial.println("Password Correct! Opening Door...");
      myServo.write(90); // เปิดประตู
      motionDetected = true;
      lastMotionTime = 0;
    }
  }

  if (check) {
    delay(2000);
    check = 0;
  }

  currentTime = millis();
  if (digitalRead(MOTION_SENSOR_PIN) == HIGH) {
    if (!motionDetected) {
      motionDetected = true;
      myServo.write(90);
      lastMotionTime = currentTime;
      Serial.println("Motion detected");
    } else if (currentTime - lastMotionTime <= motionCheckInterval) {
      lastMotionTime = currentTime;
    } 
  }

  if (motionDetected && currentTime - lastMotionTime > motionCheckInterval) {
    motionDetected = false;
    myServo.write(0);
    lastMotionTime = 0;
    Serial.println("No Motion detected!");
    Serial.println("Closing the Door...!");
    check = 1;
  }
  
  

  // อ่านค่าจาก LDR เพื่อควบคุม LED
  int ldrValue = analogRead(LDR_PIN);
  if (ldrValue < 800) {
    digitalWrite(LED_PIN, HIGH); // เปิด LED
    Blynk.virtualWrite(V3, 1); 
  } else {
    digitalWrite(LED_PIN, LOW);
    Blynk.virtualWrite(V3, 0);  // ปิด LED
  }
}
