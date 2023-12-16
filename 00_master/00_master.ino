#include <SoftwareSerial.h>  // Include SoftwareSerial library for virtual serial communication
#include <DHT.h>  // Include DHT library for temperature and humidity sensor
#include <LiquidCrystal_I2C.h>  // Include LiquidCrystal_I2C library for LCD display via I2C
#include <Adafruit_GFX.h>  // Include Adafruit GFX library for graphics functions
#include <Adafruit_SSD1306.h>  // Include Adafruit SSD1306 library for OLED display

#define RX_PIN 10  // Virtual serial RX pin
#define TX_PIN 11  // Virtual serial TX pin
#define DHT_PIN 2  // Pin connected to DHT11 sensor
#define DHT_TYPE DHT11
#define LIGHT_SENSOR_PIN A0  // Analog pin connected to the light sensor
#define RED_PIN 5   // PWM pin connected to the red LED
#define GREEN_PIN 6 // PWM pin connected to the green LED
#define TOUCH_SENSOR_PIN 7 // Digital pin for the touch sensor
#define OLED_RESET -1 // Define the reset pin for the OLED display (set to -1 if not used)

// Initialize SoftwareSerial
SoftwareSerial mySerial(RX_PIN, TX_PIN);
DHT dht(DHT_PIN, DHT_TYPE);

// LCD screen setting
LiquidCrystal_I2C lcd(0x27, 16, 2);

// OLED screen setting
Adafruit_SSD1306 display(128, 64, &Wire, OLED_RESET);

//Touching logic parameters setting
unsigned long touchStartTime = 0;
bool isTouching = false;

// Define a struct to hold sensor data
struct SensorData {
  int temperature;
  int humidity;
  int lightLevel;
  unsigned long touchDuration;
};

void setup() {
  mySerial.begin(9600);  // Initialize virtual serial communication at 9600 baud
  Serial.begin(9600); // Initialize hardware serial port for debugging output

  Serial.println("Master Arduino started");
  Serial.println("Sending message via virtual serial port...");

  dht.begin();

  pinMode(RED_PIN, OUTPUT);  // Set the red LED pin as an output
  pinMode(GREEN_PIN, OUTPUT);  // Set the green LED pin as an output
  pinMode(TOUCH_SENSOR_PIN, INPUT);  // Set the touch sensor pin as an input

  // Initialize LCD screen
  lcd.init();
  lcd.backlight();

  // Initialize OLED screen
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
}

void loop() {
  SensorData data;
  data.temperature = dht.readTemperature(); // Read temperature from DHT sensor
  Serial.println(data.temperature);         // Print temperature to serial monitor
  data.humidity = dht.readHumidity();       // Read humidity from DHT sensor
  Serial.println(data.humidity);            // Print humidity to serial monitor
  data.lightLevel = analogRead(LIGHT_SENSOR_PIN);  // Read light level from light sensor
  Serial.println(data.lightLevel);                  // Print light level to serial monitor
  
  // Read touch sensor state
  int touchState = digitalRead(TOUCH_SENSOR_PIN);
  if (touchState == HIGH) {
    if (!isTouching) {
      touchStartTime = millis(); // Record the start time of touch
      isTouching = true;
    }
  } else {
    if (isTouching) {
      data.touchDuration = millis() - touchStartTime; // Calculate touch duration
      Serial.println(data.touchDuration);             // Print touch duration to serial monitor
      touchStartTime = 0;
      isTouching = false;
    } else {
      data.touchDuration = 0; // No touch detected
      Serial.println(data.touchDuration);
    }
  }

  mySerial.write((const byte*)&data, sizeof(data)); // Send sensor data over virtual serial
  Serial.println("Data sent.");                        // Notify that data has been sent

  // Display temperature and humidity on LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(data.temperature);
  lcd.print(" C");

  lcd.setCursor(0, 1);
  lcd.print("Humid: ");
  lcd.print(data.humidity);
  lcd.print("%");

  // Update OLED display based on temperature
  display.clearDisplay();
  display.setTextSize(5);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  if (data.temperature >= 20 && data.temperature <= 30) {
    // Display happy face for comfortable temperature
    display.println(":)");
  } else if (data.temperature < 20) {
    // Display shivering face for cold temperature
    display.println(":/");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Too Cold!");
    lcd.setCursor(0, 1);
    lcd.print("Clothes Please!");
    delay(2000);
  } else {
    // Display sweating face for hot temperature
    display.println(":'(");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Too Hot!");
    lcd.setCursor(0, 1);
    lcd.print("Ice Please!");
    delay(2000);
  }
  display.display();

  // Adjust LED brightness based on light level
  int greenValue = map(data.lightLevel, 0, 700, 255, 0);
  int redValue = map(data.lightLevel, 0, 700, 0, 255);
  analogWrite(GREEN_PIN, greenValue); // Set green LED brightness
  analogWrite(RED_PIN, redValue);     // Set red LED brightness

  delay(5000); // Wait for 5 seconds before next loop iteration
}

