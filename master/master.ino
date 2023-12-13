#include <Wire.h>
#include <SoftwareSerial.h>
#include "DFRobot_SpeechSynthesis_V2.h"
#include "DFRobot_DF2301Q.h"

#define RX_PIN 11  // 虚拟串口RX引脚
#define TX_PIN 10  // 虚拟串口TX引脚

DFRobot_SpeechSynthesis_I2C ss(&Wire, 0x40);
DFRobot_DF2301Q_I2C sc(&Wire, 0x64);

SoftwareSerial mySerial(RX_PIN, TX_PIN);

struct SensorData {
  int temperature;
  int humidity;
  int lightLevel;  // 添加亮度数据
  unsigned long touchDuration;
};

void setup() {
  Wire.begin();  // 启动I2C总线
  mySerial.begin(9600);
  Serial.begin(9600); // 开启硬件串口用于调试输出

  Serial.println("从机Arduino已启动");
  Serial.println("等待接收数据...");

  ss.begin();
  ss.setVolume(2);
  ss.setLanguage(ss.eEnglishl);

  sc.setVolume(4);
  sc.setWakeTime(10);
}

void loop() {
  if (mySerial.available() >= sizeof(SensorData)) {
    SensorData data;
    mySerial.readBytes((char*)&data, sizeof(data));

    String tempStr = String(data.temperature);
    String humidStr = String(data.humidity);
    String lightStr = String(data.lightLevel);  // 转换亮度为String

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
    Serial.print("Humidity: ");
    Serial.println(data.humidity);
    if (CMDID == 5) {
      Serial.print(CMDID);
      ss.speak(F("The real time humidity is"));
      ss.speak(humidStr);
      ss.speak(F("percent"));
    }
    Serial.print("Brightness: ");
    Serial.println(data.lightLevel);
    if (data.lightLevel <= 200){
      ss.speak(F("It is too dark!"));
      ss.speak(F("Please turn on the light!"));
    }
    Serial.print("Touch duration: ");
    Serial.println(data.touchDuration);
    if (data.touchDuration > 0) {
      int randomNumber = random(1, 4);  // 生成一个1到3的随机数
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
