//SmartRoom Capacity Monitor:
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// Initialize the LCD (Address 0x27 is common, some use 0x3F)
LiquidCrystal_I2C lcd(0x27, 16, 2);

const int sensor1 = 2; // Entry Sensor
const int sensor2 = 3; // Exit Sensor

int count = 0;

void setup() {
  pinMode(sensor1, INPUT);
  pinMode(sensor2, INPUT);
  
  lcd.init();
  lcd.backlight();
  
  // Initial Display
  lcd.setCursor(0, 0);
  lcd.print("Visitor Counter");
  lcd.setCursor(0, 1);
  lcd.print("Total: 0");
}

void loop() {
  // Logic for Entering
  if (digitalRead(sensor1) == LOW) {
    // Wait for person to trigger the second sensor
    unsigned long startTime = millis();
    while (digitalRead(sensor2) == HIGH) {
      if (millis() - startTime > 2000) return; // Timeout after 2s if they don't pass
    }
    count++;
    updateLCD();
    delay(500); // Prevent double counting
  }

  // Logic for Exiting
  if (digitalRead(sensor2) == LOW) {
    // Wait for person to trigger the first sensor
    unsigned long startTime = millis();
    while (digitalRead(sensor1) == HIGH) {
      if (millis() - startTime > 2000) return; // Timeout after 2s if they don't pass
    }
    if (count > 0) count--;
    updateLCD();
    delay(500); // Prevent double counting
  }
}

void updateLCD() {
  lcd.setCursor(0, 1);
  lcd.print("Total:          "); // Clear the line
  lcd.setCursor(7, 1);
  lcd.print(count);
}