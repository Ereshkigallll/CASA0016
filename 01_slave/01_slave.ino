#include <Wire.h>  // Include Wire library for I2C communication
#include <SoftwareSerial.h>  // Include SoftwareSerial library for virtual serial communication
#include "DFRobot_SpeechSynthesis_V2.h" // Include library for DFRobot Speech Synthesis
#include "DFRobot_DF2301Q.h" // Include library for DFRobot DF2301Q module

#define RX_PIN 11  // Define the RX pin for virtual serial communication
#define TX_PIN 10  // Define the TX pin for virtual serial communication

// Initialize speech synthesis and DF2301Q modules with I2C addresses
DFRobot_SpeechSynthesis_I2C ss(&Wire, 0x40);
DFRobot_DF2301Q_I2C sc(&Wire, 0x64);

SoftwareSerial mySerial(RX_PIN, TX_PIN); // Initialize virtual serial port

// Define a struct to hold sensor data
struct SensorData {
  int temperature;       // Variable to store temperature data
  int humidity;          // Variable to store humidity data
  int lightLevel;        // Variable to store light level data
  unsigned long touchDuration; // Variable to store touch duration
};

void setup() {
  Wire.begin();  // Initialize I2C bus
  mySerial.begin(9600); // Initialize virtual serial communication at 9600 baud
  Serial.begin(9600);   // Initialize hardware serial port for debugging output

  Serial.println("Slave Arduino started");  // Print a startup message to the serial monitor
  Serial.println("Waiting for data..."); // Notify about waiting to receive data

  // Initialize speech synthesis module
  ss.begin();
  ss.setVolume(2);       // Set the volume for speech synthesis
  ss.setLanguage(ss.eEnglishl); // Set the language for speech synthesis

  // Initialize DF2301Q module
  sc.setVolume(4);       // Set the volume for DF2301Q
  sc.setWakeTime(10);    // Set the wake time for DF2301Q
}

void loop() {
  // Check if there is enough data available on the serial port
  if (mySerial.available() >= sizeof(SensorData)) {
    SensorData data;
    mySerial.readBytes((char*)&data, sizeof(data)); // Read sensor data from serial port

    // Convert sensor readings to String for speech synthesis
    String tempStr = String(data.temperature);
    String humidStr = String(data.humidity);
    String lightStr = String(data.lightLevel);  // Convert light level to String

    // Print and announce temperature
    Serial.print("Temperature: ");
    Serial.println(data.temperature);
    int CMDID = 0;
    CMDID = sc.getCMDID();
    if (CMDID == 69) {
      Serial.print(CMDID);
      ss.speak(F("The real time temperature is"));
      ss.speak(tempStr);
      ss.speak(F("Degrees centigrade"));
    }

    // Print and announce humidity
    Serial.print("Humidity: ");
    Serial.println(data.humidity);
    if (CMDID == 5) {
      Serial.print(CMDID);
      ss.speak(F("The real time humidity is"));
      ss.speak(humidStr);
      ss.speak(F("percent"));
    }

    // Print and announce brightness
    Serial.print("Brightness: ");
    Serial.println(data.lightLevel);
    if (data.lightLevel <= 200){
      ss.speak(F("It is too dark!"));
      ss.speak(F("Please turn on the light!"));
    }

    // Announce touch duration and respond randomly
    Serial.print("Touch duration: ");
    Serial.println(data.touchDuration);
    if (data.touchDuration > 0) {
      int randomNumber = random(1, 4);  // Generate a random number between 1 and 3
      if (randomNumber == 1) {
        ss.speak(F("Please do not touch me"));
      } else if (randomNumber == 2) {
        ss.speak(F("I like this feeling"));
      } else if (randomNumber == 3) {
        ss.speak(F("Thank you"));
      }
    }
  }
  
}
