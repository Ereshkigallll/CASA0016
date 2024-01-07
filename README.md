# CASA0016 Project: "Hello, Robot!"

**"Tired of using serial monitors or having to turn on your computer to view sensor readings? Come try this sensor system that allows voice interaction!"**

This repository documents the code and other details of the CASA0016 personal project "Hello Robot" interactive device, so that it can be easily reproduced by others or by myself in the future.

If you find any problems, feel free to ask. :smiley::smiley::smiley:

![Device Pic](https://github.com/Ereshkigallll/CASA0016/blob/master/04_pic/face.jpg)

## 1. Sensors And Libraries Requirements

### 1.1 Sensors
This section will list all the parts required for this project.

- $2 \times$ Arduino Uno R3
- $1 \times$ DHT11 temperature/humidity sensor ( For more information about this sensor please check out [this document](https://components101.com/sensors/dht11-temperature-sensor). )
- $1 \times$ photoresistor
- $1 \times$ colour-changing LED light
- $1 \times$ capacitive touch sensor
- $1 \times$ OLED screen with a resolution of $128\times 64$
- $1 \times$ 1602 LCD screen
- $1 \times$ offline language learning voice recognition sensor ( For more information about this sensor please check out [this document](https://wiki.dfrobot.com/SKU_SEN0539-EN_Gravity_Voice_Recognition_Module_I2C_UART). )
- $1 \times$ speech synthesis module ( For more information about this sensor please check out [this document](https://wiki.dfrobot.com/Gravity_Speech_Synthesis_Module_V2_SKU_DFR0760). )


### 1.2 Libraries
- SoftwareSerial
- DHT
- LiquidCrystal_I2C
- Adafruit_GFX
- Adafruit_SSD1306
- Wire
- DFRobot_SpeechSynthesis_V2
- DFRobot_DF2301Q

## 2. Overview

Two Arduino R3 development boards are needed for this project, one to handle speech recognition as well as speech synthesis ( **slave board** ) and the other to handle everything else ( **master board** ). The reason for this is that the speech synthesis and speech recognition modules require significant power to support, and one arduino board is not enough to handle all the components. So I split the functions and used serial communication to connect the two boards.

:warning: When using serial communication to connect two Arduino boards, be sure to connect their ground wires to ensure communication stability.

## 3. Files And Usage

### 3.1 `00_master`

This code will be used on the master board, which will be fitted with an LCD screen, an OLED screen, a photoresistor, colour-changing LEDs, a touch sensor and a DHT11 temperature and humidity sensor.

In addition, the master board will also pack the sensor data and send it to the slave board. The specific data structure is as follows:

```
// Define a struct to hold sensor data
struct SensorData {
  int temperature;
  int humidity;
  int lightLevel;
  unsigned long touchDuration;
};
```

- The LCD screen will be used to display temperature and humidity information.

- The OLED screen will change the expression on it depending on the environment temperature. At 20 to 30 degrees Celsius, it is a happy expression.

- The photoresistor will work with a colour-changing LED, the LED will turn red when the brightness is unsuitable for a person to work and study, otherwise it will be green.

- The touch sensor adds more interactivity to the device, and after a certain length of touch, the speech synthesis module will play a random speech.

### 3.2 `01_slave`

This part of the code will be used on the slave board and contains mainly the logic for voice command recognition as well as speech synthesis output. The slave board will receive sensor data from the master board.

- The voice recognition module is used to recognise voice commands. Firstly, the user needs to start by saying the wake up word, "Hello, robot". Then, the user can issue the commands "Read temperature" and "Read humidity" to get the current temperature and humidity data from the output of the speech synthesis module.

- The speech synthesis module will be responsible for the output of voice messages, such as the current temperature and humidity, as well as voice reminders when the brightness is too low.

- These two modules are connected to the Arduino development board via the I2C bus.

### 3.3 `02_test`
This folder contains the tests I did on each component when building this device, so if you find a problem during the reproduction process, you may want to use these files first to test your own sensors or output components to see if there are any problems.

One of the files is called i2c_scanner, and this code is mainly used to determine the addresses of different devices that are connected to the same I2C bus. When you are not sure of the I2C address of your device, in addition to using this code to print the corresponding address in the serial port monitor, you can also check the technical documentation of this device to identify its I2C address.

### 3.4 `03_case_design`
This folder contains an enclosure I designed for this device. Based on testing, this enclosure is too large, so consider reducing its size if you need to use it. This sketch can be used on a laser cutting machine.

### 3.5 `04_pic`
Contains some images used in this Read Me file and can be ignored.

## 4. Circuits

I'll describe the wiring in as much detail as possible since images of some of the components can't be found in Fritzing.

Firstly, in order for the two Arduino's to communicate, we need to connect the RX of one board to the TX of the other, and the TX to the RX of the other, as well as making sure that the ground of both boards are connected.

Specifically, since I'm using a software serial port, I only need pin 10 of both boards to be connected, and pin 11 of both boards to be connected.

### 4.1 Speech Recognition and Synthesis Part
This section contains only the speech recognition and the speech synthesis wired to the Arduino board.

Since the speech recognition and speech synthesis modules use the I2C communication protocol, we only need to connect the SDA pins of the two modules to the A4 pin of the Arduino, and SCL to A5. At the same time, VCC is connected to the 5V power supply pin of Arduino, and GND is connected to the GND of Arduino.

### 4.2 Another Board
This section will contain all the remaining components connected to another Arduino board.

Firstly, all modules except the OLED screen are connected to the Arduino's 5V power supply pins, and the OLED works using a 3.3V supply.

Similar to the voice modules, both the LCD and OLED screens use the I2C communication protocol, so the wiring is similar to that of the voice modules, so it will not be repeated.

For the DHT11 module, since I'm using a 3-pin module, I just need to connect that remaining unconnected pin to the Arduino pin 2 that I've defined as being used to receive DHT11 data.

For the colour changeable LED light module, I connected the R pin to pin 5 of the Arduino and the G pin to pin 6 of the Arduino. Since I am not using the blue channel, pin B does not need to be connected.

For the touch sensor, I connected the remaining unconnected pin to pin 7 on the Arduino.

And for the brightness sensor (photoresistor), since my module includes an extra analogue-ready pin, it's only necessary to connect this pin to the Arduino's A0 pin.


## 5. Further Improvement
In the future, consideration may be given to redesigning the enclosure to make it take up less space. In addition to this, more interactions will be added and the creation of a graphical interface using lvgl will also be considered.