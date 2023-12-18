#include <Wire.h> // Include Wire library for I2C communication

void setup() {
  Wire.begin(); // Initialize I2C communication
  Serial.begin(9600); // Initialize serial communication at 9600 baud
  while (!Serial);  // Wait for the serial console to connect

  Serial.println("\nI2C Scanner"); // Print the title of the program
}

void loop() {
  byte error, address; // Variables for error and I2C address
  int nDevices; // Variable to store the number of devices found

  Serial.println("Scanning..."); // Inform the user that scanning has started

  nDevices = 0; // Initialize the device count
  for (address = 1; address < 127; address++) { // Loop through all possible I2C addresses
    Wire.beginTransmission(address); // Start I2C transmission
    error = Wire.endTransmission(); // End I2C transmission and get the status

    if (error == 0) {
      // If no error, I2C device is found
      Serial.print("I2C device found at address 0x");
      if (address < 16) {
        Serial.print("0"); // Print leading zero for addresses less than 0x10
      }
      Serial.println(address, HEX); // Print the address in hexadecimal
      nDevices++; // Increment the device count
    } else if (error == 4) {
      // If an unknown error occurred
      Serial.print("Unknown error at address 0x");
      if (address < 16) {
        Serial.print("0"); // Print leading zero for addresses less than 0x10
      }
      Serial.println(address, HEX); // Print the address in hexadecimal
    }
  }
  if (nDevices == 0) {
    Serial.println("No I2C devices found\n"); // Inform the user if no devices were found
  } else {
    Serial.println("done\n"); // Indicate that scanning is complete
  }

  delay(5000);  // Wait 5 seconds before the next scan
}

