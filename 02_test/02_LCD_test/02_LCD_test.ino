#include <Wire.h>  // Include Wire library for I2C communication
#include <LiquidCrystal_I2C.h> // Include LiquidCrystal_I2C library for LCD control

// Initialize a LiquidCrystal_I2C object
// Parameters: I2C address of the LCD, number of columns and rows
LiquidCrystal_I2C lcd(0x27, 16, 2); // Common I2C addresses are 0x27 or 0x3F

void setup() {
  // Initialize the LCD
  lcd.init();
  // Turn on the LCD backlight
  lcd.backlight();

  // Display text on the LCD
  lcd.setCursor(0, 0); // Set the cursor to the beginning of the first line
  lcd.print("Hello, World!");

  lcd.setCursor(0, 1); // Set the cursor to the beginning of the second line
  lcd.print("LCD Test");
}

void loop() {
  // No code needed here
}

