#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <LiquidCrystal_I2C.h>
#include <DHT11.h>
#include "DFRobot_SpeechSynthesis_V2.h"
#include "DFRobot_DF2301Q.h"
#include <DFRobot_SGP40.h>

#define DHT11_PIN 2          // DHT11数据引脚连接到Arduino的数字2号引脚
#define TOUCH_SENSOR_PIN 3   // 触摸传感器连接到数字3号引脚
#define LIGHT_SENSOR_PIN A0  // 亮度传感器引脚
#define RED_PIN 9            // 红色LED引脚
#define GREEN_PIN 10         // 绿色LED引脚

DHT11 dht11(DHT11_PIN);

// OLED屏幕设置
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// LCD屏幕设置
LiquidCrystal_I2C lcd(0x27, 16, 2);

// 语音识别模块设置
DFRobot_DF2301Q_I2C DF2301Q(&Wire, 0x64);

//语音合成模块设置
DFRobot_SpeechSynthesis_I2C ss(&Wire, 0x40);

//空气质量模块设置
DFRobot_SGP40 sgp(&Wire);

unsigned long touchStartTime = 0;  // 触摸开始的时间
unsigned long lastSwitchTime = 0;
const unsigned long switchInterval = 2000;  // 2秒切换一次
bool showTemperature = true;

void setup() {
  // 启动I2C总线
  Wire.begin();
  // 初始化串口
  Serial.begin(9600);

  // 初始化OLED显示屏
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    for (;;)
      ;  // 无限循环等待
  }
  display.clearDisplay();
  display.setTextSize(5);
  display.setTextColor(SSD1306_WHITE);

  // 初始化LCD屏幕
  lcd.init();
  lcd.backlight();

  // 初始化触摸传感器引脚为输入模式
  pinMode(TOUCH_SENSOR_PIN, INPUT);

  // 初始化LED引脚
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);

  //初始化语音合成模块
  ss.begin();
  ss.setVolume(2);
  ss.setLanguage(ss.eEnglishl);

  //初始化语音识别模块
  DF2301Q.setWakeTime(4);
  DF2301Q.setVolume(4);

  //初始化空气质量传感器
  while (sgp.begin(/*duration = */ 10000) != true) {
    Serial.println("Failed to initialize SGP40, please check wiring and cable connections");  //You need to press the Gravity cable all the way in.
    delay(1000);
  }
}

void loop() {
  lcd.clear();
  // 在LCD上显示文本
  lcd.setCursor(0, 0); // 将光标设置到第一行的开始位置
  lcd.print("Hello, World!");

  lcd.setCursor(0, 1); // 将光标设置到第二行的开始位置
  lcd.print("LCD Test");

  delay(2000);
}
