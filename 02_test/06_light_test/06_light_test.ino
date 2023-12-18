const int redPin = 3;   // Red LED connected to digital pin 3
const int greenPin = 5; // Green LED connected to digital pin 5
const int bluePin = 6;  // Blue LED connected to digital pin 6
const int lightSensorPin = A0; // Light sensor connected to analog pin A0

void setup() {
  // Set LED pins as outputs
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  
  // Initialize serial communication at 9600 baud
  Serial.begin(9600);
}

void loop() {
  // Read the light sensor value
  int sensorValue = analogRead(lightSensorPin);
  // Map the sensor value to green LED brightness (dimmer as it gets darker)
  int greenValue = map(sensorValue, 0, 500, 255, 0);
  // Map the sensor value to red LED brightness (brighter as it gets darker)
  int redValue = map(sensorValue, 0, 500, 0, 255);

  // Set the brightness of green and red LEDs
  analogWrite(greenPin, greenValue);
  analogWrite(redPin, redValue);

  // Short delay
  delay(100);
}
