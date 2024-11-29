#include <Arduino.h>

void setup() {
    Serial.begin(115200);
    Serial.println("ESP32-CAM test");
}

void loop() {
    delay(1000);
    Serial.println("Running...");
}
