#define BLYNK_TEMPLATE_ID "XXX"
#define BLYNK_TEMPLATE_NAME "XXX"
#define BLYNK_AUTH_TOKEN "XXX"

#include <DHT.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

#define BLYNK_PRINT Serial
#define DHTPIN 18
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

// WiFi credentials
char ssid[] = "J";
char pass[] = "123456789";

BlynkTimer timer;

BLYNK_CONNECTED() {
  Blynk.syncVirtual(V0); // Sync state if needed
  Blynk.syncVirtual(V1);
}
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
  Blynk.virtualWrite(V0, temp);      // Send temperature to Virtual Pin V0
  Blynk.virtualWrite(V1, humidity); // Send humidity to Virtual Pin V1
}

void setup() {
  // Initialize serial communication
  Serial.begin(115200);

  // Initialize DHT sensor
  dht.begin();

  // Connect to WiFi and Blynk
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  
  timer.setInterval(3000L, sendSensorData);

}

void loop() {
  Blynk.run();
  timer.run();
}
