#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED display settings
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// LCD display settings
// Set the LCD I2C address
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  // OLED setup
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);

  // Draw a smiley face on OLED
  display.drawCircle(64, 32, 30, SSD1306_WHITE);
  display.fillCircle(54, 24, 5, SSD1306_WHITE);
  display.fillCircle(74, 24, 5, SSD1306_WHITE);
  display.drawArc(50, 20, 28, 20, 0, 180, SSD1306_WHITE);
  display.display();

  // LCD setup
  lcd.init();  // initialize the LCD
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Hello, World!");  // Print a message to the LCD
}

void loop() {
  // No need to repeat the drawing, it stays on the screen
}
