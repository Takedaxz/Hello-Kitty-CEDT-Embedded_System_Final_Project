#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <Wire.h>
#include <SoftwareSerial.h>

// Define buzzer pin
#define BUZZER_SENSOR_PIN 5

// Define RX and TX pins for SoftwareSerial
const int rxPin = 21;
const int txPin = 19;

// Initialize SoftwareSerial
SoftwareSerial serial2(rxPin, txPin);

// Keypad configuration
const byte ROWS = 4; // Four rows
const byte COLS = 4; // Four columns
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'U'}, // Example custom key
  {'4', '5', '6', 'D'}, // Example custom key
  {'7', '8', '9', '*'}, // '*' to clear
  {'L', '0', 'R', '#'}  // '#' to submit
};

byte rowPins[ROWS] = {32, 33, 25, 26};
byte colPins[COLS] = {13, 12, 14, 27};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// LCD configuration
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Password settings
String password = "1234"; // Correct password
String inputPassword = "";
int count = 0;

void setup() {
  // Serial communication for debugging
  Serial.begin(74880);
  serial2.begin(9600);

  // Initialize I2C communication for LCD
  Wire.begin(22, 23);
  
  // Configure the buzzer pin as output
  pinMode(BUZZER_SENSOR_PIN, OUTPUT);

  // Initialize the LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Enter Password:");
}

void loop() {
  // Read key press from the keypad
  char key = keypad.getKey();

  if (key) {
    if (key == '#') { // Submit password
      if (inputPassword == password) {
        // Correct password: display success message
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Welcome Home!");
        Serial.println("Correct Password!");
        serial2.println("true");
        delay(3000);
      } else {
        // Incorrect password: display error message and activate buzzer
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Error! Try Again");
        count++;
        if(count >= 3) {
          serial2.println("false");
          count = 0;
        }
        Serial.println("Wrong Password!");

        // Activate buzzer with pip-pip-pip pattern
        Serial.println("Activating Buzzer...");
        for (int i = 0; i < 5; i++) { // Beep 3 times
          digitalWrite(BUZZER_SENSOR_PIN, HIGH);
          delay(200); // Beep duration (200ms)
          digitalWrite(BUZZER_SENSOR_PIN, LOW);
          delay(200); // Pause between beeps (200ms)
        }
        Serial.println("Buzzer Deactivated.");
      }

      // Reset input password and LCD for next attempt
      inputPassword = "";
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Enter Password:");
    } else if (key == '*') { // Clear input
      inputPassword = "";
      lcd.setCursor(0, 1);
      lcd.print("                "); // Clear second row
      Serial.println("Input Cleared");
    } else {
      // Append the pressed key to the input password
      inputPassword += key;
      lcd.setCursor(0, 1);
      lcd.print(inputPassword);
      Serial.print("Key Pressed: ");
      Serial.println(key);
    }
  }
}
