#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <LiquidCrystal_I2C.h>
#include <DHT11.h>

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

unsigned long touchStartTime = 0;  // 触摸开始的时间
unsigned long lastSwitchTime = 0;
const unsigned long switchInterval = 2000;  // 2秒切换一次
bool showTemperature = true;
bool isTouching = false;
const int CMD_ID_TEMPERATURE = 69;
const int CMD_ID_HUMIDITY = 5;

void setup() {
  // 初始化串口
  Serial.begin(9600);
  Wire.begin(8);  // 作为从设备加入I2C总线，地址为8
  Wire.onReceive(receiveEvent); // 注册接收事件处理函数
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
}

void loop() {
  // 读取亮度值
  int lightValue = analogRead(LIGHT_SENSOR_PIN);
  int greenValue = map(lightValue, 0, 500, 255, 0);  // 亮度低时绿色亮度减少
  int redValue = map(lightValue, 0, 500, 0, 255);    // 亮度低时红色亮度增加

  analogWrite(GREEN_PIN, greenValue);  // 设置绿色通道亮度
  analogWrite(RED_PIN, redValue);      // 设置红色通道亮度
  if (lightValue < 150) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Too dark!");
    lcd.setCursor(0, 1);
    lcd.print("Open Lights!!");
    delay(3000);
  }


  // 读取触摸传感器状态
  int touchState = digitalRead(TOUCH_SENSOR_PIN);
  // 如果检测到触摸
  if (touchState == HIGH) {
    // 如果触摸刚开始
    if (!isTouching) {
      touchStartTime = millis();  // 记录开始触摸的时间
      isTouching = true;          // 标记触摸正在进行
    }

    // 计算触摸持续的时间
    unsigned long touchDuration = millis() - touchStartTime;

    // 根据触摸持续的时间显示不同的表情
    if (touchDuration > 10000) {  // 触摸超过10秒
                                  // 显示哭泣的表情
      display.clearDisplay();
      display.setCursor(0, 0);
      display.print(":'(");  // 哭泣的表情
      display.display();
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("WWWWW Please");
      lcd.setCursor(0, 1);
      lcd.print("Leave me Alone");
      delay(3000);
    } else if (touchDuration > 5000) {  // 触摸超过5秒
                                        // 显示愤怒的表情
      display.clearDisplay();
      display.setCursor(0, 0);
      display.print("~_~");  // 愤怒的表情
      display.display();
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("This Feeling...");
      lcd.setCursor(0, 1);
      lcd.print("Not Bad!");
      delay(3000);
    }
    // 这里不需要处理触摸小于5秒的情况，因为触摸还在进行中
  } else {
    if (isTouching) {
      // 触摸结束，处理触摸时间小于5秒的情况
      unsigned long touchDuration = millis() - touchStartTime;
      if (touchDuration < 5000) {
        display.clearDisplay();
        display.setCursor(0, 0);
        display.print(":D");  // 特别开心的表情
        display.display();
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Please");
        lcd.setCursor(0, 1);
        lcd.print("Don't Touch Me");
        delay(3000);  // 显示五秒
      }
      isTouching = false;  // 重置触摸状态
      touchStartTime = 0;  // 重置触摸时间
    }
  }


  lcd.clear();
  lcd.setCursor(0, 0);
  // 读取温度和湿度
  int humidity = dht11.readHumidity();
  int temperature = dht11.readTemperature();

  // 检查数据是否有效
  if (humidity != DHT11::ERROR_CHECKSUM && humidity != DHT11::ERROR_TIMEOUT && temperature != DHT11::ERROR_CHECKSUM && temperature != DHT11::ERROR_TIMEOUT) {
    // 在LCD屏幕上显示温度和湿度
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Temp: ");
    lcd.print(temperature);
    lcd.print(" C");
    lcd.setCursor(0, 1);
    lcd.print("Humidity: ");
    lcd.print(humidity);
    lcd.print("%");

    // 检查是否该切换LCD显示的内容
    if (millis() - lastSwitchTime > switchInterval) {
      showTemperature = !showTemperature;  // 切换显示状态
      lastSwitchTime = millis();           // 更新最后切换时间
    }

    // 在OLED屏幕上显示表情
    display.clearDisplay();
    display.setCursor(0, 0);
    if (temperature < 20) {
      display.print(":(");  // 温度低于20°C时显示悲伤的表情
      if (showTemperature || temperature >= 20) {
        // 显示温度和湿度
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Temp: ");
        lcd.print(temperature);
        lcd.print(" C");
        lcd.setCursor(0, 1);
        lcd.print("Humidity: ");
        lcd.print(humidity);
        lcd.print("%");
      } else {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("I'm Cold!");
        lcd.setCursor(0, 1);
        lcd.print("Clothes Please!");
      }
    } else if (temperature > 30) {
      display.print(":@");  // 温度高于30°C时显示愤怒的表情
      // 在LCD屏幕上显示温度和湿度或"Too hot!"
      if (showTemperature || temperature <= 30) {
        // 显示温度和湿度
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Temp: ");
        lcd.print(temperature);
        lcd.print(" C");
        lcd.setCursor(0, 1);
        lcd.print("Humidity: ");
        lcd.print(humidity);
        lcd.print("%");
      } else {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Too Hot!");
        lcd.setCursor(0, 1);
        lcd.print("Buy Me Ice!!");
      }
    } else {
      display.print(":)");  // 温度适宜时显示快乐的表情
    }
    display.display();
  } else {
    // 错误处理
    Serial.println("Error reading DHT11 sensor");
    if (humidity == DHT11::ERROR_CHECKSUM || temperature == DHT11::ERROR_CHECKSUM) {
      Serial.println("Checksum error");
    } else if (humidity == DHT11::ERROR_TIMEOUT || temperature == DHT11::ERROR_TIMEOUT) {
      Serial.println("Timeout error");
    }
  }

  delay(10000);  // 原始延时
}

void receiveEvent(int howMany) {
  while (Wire.available()) {
    int commandId = Wire.read(); // 读取命令ID

    switch (commandId) {
      case 69:  // 假设1是读取温度的命令ID
        Serial.print(commandId);
        Serial.println("Calling sendTemperature");  // 调试信息
        sendTemperature();
        break;
      case 5:  // 假设2是读取湿度的命令ID
        sendHumidity();
        break;
      // 可以增加更多case来处理其他命令
    }
  }
}

void sendTemperature() {
  Serial.println("In sendTemperature");  // 调试信息
  int temperature = (int)dht11.readTemperature();
  Serial.print(temperature);
  Wire.write('T');
  Wire.write((byte*)&temperature, sizeof(temperature));
  Serial.println("Data sent");
}

void sendHumidity() {
  int humidity = (int)dht11.readHumidity();
  Wire.write('H');
  Wire.write((byte*)&humidity, sizeof(humidity));
}