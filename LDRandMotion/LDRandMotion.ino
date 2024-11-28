#include <Arduino.h>
#include <ESP32Servo.h>

#define LED_PIN 23
#define LDR_PIN 34
#define SERVO_PIN 14 // Pin connected to the servo signal
#define MOTION_SENSOR_PIN 26 // Motion sensor pin

Servo myServo; // Create Servo object

const int bufferSize = 8; // จำนวนข้อมูลใน buffer (5 วินาที x 200ms ต่อค่า)
int motionBuffer[bufferSize] = {0}; // Buffer สำหรับเก็บค่าจาก motion sensor
unsigned long lastSampleTime = 0; // เวลาในการอ่านค่า motion sensor ล่าสุด
const unsigned long sampleInterval = 500; // อ่านค่าทุก 200ms
int nomotioncount = 0;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(MOTION_SENSOR_PIN, INPUT);
  Serial.begin(115200);
  myServo.attach(SERVO_PIN);
}

void loop() {
  unsigned long currentTime = millis();

  // อ่านค่า motion sensor ทุก 200ms
  if (currentTime - lastSampleTime >= sampleInterval) {
    lastSampleTime = currentTime;

    // เลื่อนค่าใน buffer
    for (int i = 0; i < bufferSize - 1; i++) {
      motionBuffer[i] = motionBuffer[i + 1];
    }
    // เพิ่มค่าใหม่ลงใน buffer
    motionBuffer[bufferSize - 1] = digitalRead(MOTION_SENSOR_PIN);

    // ตรวจสอบว่ามี motion ใน buffer หรือไม่
    bool motionDetected = false;
    int motioncount = 0;
    int threadhold = 1;
    for (int i = 0; i < bufferSize; i++) {
      if (motionBuffer[i] == HIGH) {
        motioncount++;
        if(motioncount == threadhold){
          motionDetected = true;
          break;
        }
        
      }
    }

    // ถ้ามี motion
    if (motionDetected) {
      Serial.println("Motion detected!");
      nomotioncount=0;
      digitalWrite(LED_PIN, HIGH); // เปิด LED
      myServo.write(90);          // หมุน servo ไปที่ 90 องศา
    } else {
      nomotioncount++;
      Serial.println("No motion detected!");
      if(nomotioncount == 4){
        digitalWrite(LED_PIN, LOW); // ปิด LED
        myServo.write(0);  
        nomotioncount = 0;
        delay(2000);
      }
    }
  }

  // อ่านค่าจาก LDR เพื่อควบคุม LED
  int ldrValue = analogRead(LDR_PIN);
  if (ldrValue < 800) {
    digitalWrite(LED_PIN, HIGH); // เปิด LED
  } else {
    digitalWrite(LED_PIN, LOW); // ปิด LED
  }
}
