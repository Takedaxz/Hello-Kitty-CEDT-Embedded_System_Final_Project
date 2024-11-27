#define BLYNK_TEMPLATE_ID "TMPL6F4TaHZbl"
#define BLYNK_TEMPLATE_NAME "Motion Sensor Template"
#define BLYNK_AUTH_TOKEN "KMk_GtDvqrO_M_qudoniDIzazP8f6xYW"

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial



#include <Arduino.h>
#include <ESP32Servo.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

#define LED_PIN 23
#define LDR_PIN 34
#define SERVO_PIN 14 // Pin connected to the servo signal

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "J";
char pass[] = "123456789";

const int motionsensor=26;   // the pin that OUTPUT pin of sensor is connected to
int pinStateCurrent   = LOW; // current state of pin
int pinStatePrevious  = LOW; // previous state of pin
int status1 = LOW;

Servo myServo; // Create Servo object
BlynkTimer timer;

// This function is called every time the device is connected to the Blynk.Cloud
BLYNK_CONNECTED()
{

Blynk.syncVirtual(V0);  // will cause BLYNK_WRITE(V0) to be executed
Blynk.syncVirtual(V1);  // will cause BLYNK_WRITE(V1) to be executed

}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(115200); // UART communication with ESP32-CAM
  myServo.attach(SERVO_PIN); // Attach the servo to the specified pin
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  pinMode(motionsensor, INPUT);
}

void loop() {
  int ldrValue = analogRead(LDR_PIN); // Read the LDR value
  // Serial.println(ldrValue);
  // Handle LED control based on LDR value
  Blynk.run();
  timer.run();

  pinStatePrevious = pinStateCurrent; // store old state
  pinStateCurrent = digitalRead(motionsensor);   // read new state

  if (pinStatePrevious == LOW && pinStateCurrent == HIGH) {   // pin state change: LOW -> HIGH
    Serial.println("Motion detected!");
    myServo.write(360); // Move the servo to 90 degrees (adjust if needed)
    Blynk.virtualWrite(V0, 1);
    Blynk.virtualWrite(V1, "Motion Detected");
    Blynk.logEvent("detected");
    // TODO: turn on alarm, light or activate a device ... here
    digitalWrite(LED_PIN, HIGH);
    delay(5000);

  }
  
  else
  if (pinStatePrevious == HIGH && pinStateCurrent == LOW) {   // pin state change: HIGH -> LOW
    Serial.println("Motion stopped!");
    
    // TODO: turn off alarm, light or deactivate a device ... here
    Blynk.virtualWrite(V0, 0);
    Blynk.virtualWrite(V1, "Motion Stopped");
    digitalWrite(LED_PIN, LOW);
    myServo.write(0); // Move the servo back to 0 degrees
  }

  if (ldrValue < 800) {
    digitalWrite(LED_PIN, HIGH); // Turn on LED
  } else {
    digitalWrite(LED_PIN, LOW); // Turn off LED
  }
}