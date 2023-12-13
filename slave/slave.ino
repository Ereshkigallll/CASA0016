#include <SoftwareSerial.h>
#include <DHT.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define RX_PIN 10  // 虚拟串口RX引脚
#define TX_PIN 11  // 虚拟串口TX引脚
#define DHT_PIN 2  // DHT11传感器连接的引脚
#define DHT_TYPE DHT11
#define LIGHT_SENSOR_PIN A0  // 亮度传感器连接的模拟引脚
#define RED_PIN 5   // 红色LED连接的PWM引脚
#define GREEN_PIN 6 // 绿色LED连接的PWM引脚
#define TOUCH_SENSOR_PIN 7 // 触摸传感器的数字引脚
#define OLED_RESET -1

SoftwareSerial mySerial(RX_PIN, TX_PIN);
DHT dht(DHT_PIN, DHT_TYPE);

// LCD屏幕设置
LiquidCrystal_I2C lcd(0x27, 16, 2);

Adafruit_SSD1306 display(128, 64, &Wire, OLED_RESET);

unsigned long touchStartTime = 0;
bool isTouching = false;

struct SensorData {
  int temperature;
  int humidity;
  int lightLevel;  // 添加亮度数据
  unsigned long touchDuration;
};

void setup() {
  mySerial.begin(9600);
  Serial.begin(9600); // 开启硬件串口用于调试输出

  Serial.println("主机Arduino已启动");
  Serial.println("正在通过虚拟串口发送数据...");
  dht.begin();
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(TOUCH_SENSOR_PIN, INPUT);

  // 初始化LCD屏幕
  lcd.init();
  lcd.backlight();

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // 初始化OLED
}

void loop() {
  SensorData data;
  data.temperature = dht.readTemperature();
  Serial.println(data.temperature);
  data.humidity = dht.readHumidity();
  Serial.println(data.humidity);
  data.lightLevel = analogRead(LIGHT_SENSOR_PIN);  // 读取亮度数据
  Serial.println(data.lightLevel);
  // 读取触摸传感器
  int touchState = digitalRead(TOUCH_SENSOR_PIN);
  if (touchState == HIGH) {
    if (!isTouching) {
      touchStartTime = millis(); // 开始触摸
      isTouching = true;
    }
  } else {
    if (isTouching) {
      data.touchDuration = millis() - touchStartTime; // 计算触摸时长
      Serial.println(data.touchDuration);
      touchStartTime = 0;
      isTouching = false;
    } else {
      data.touchDuration = 0; // 无触摸
      Serial.println(data.touchDuration);
    }
  }

  mySerial.write((const byte*)&data, sizeof(data));
  Serial.println("数据已发送");

  // 在LCD上显示温度和湿度
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(data.temperature);
  lcd.print(" C");

  lcd.setCursor(0, 1);
  lcd.print("Humid: ");
  lcd.print(data.humidity);
  lcd.print("%");

  // 更新OLED显示
  display.clearDisplay();
  display.setTextSize(5);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  if (data.temperature >= 20 && data.temperature <= 30) {
    // 开心的表情
    display.println(":)");
  } else if (data.temperature < 20) {
    // 发抖的表情
    display.println(":/");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Too Cold!");
    lcd.setCursor(0, 1);
    lcd.print("Clothes Please!");
    delay(2000);
  } else {
    // 出汗的表情
    display.println(":'(");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Too Hot!");
    lcd.setCursor(0, 1);
    lcd.print("Ice Please!");
    delay(2000);
  }
  display.display();

  int greenValue = map(data.lightLevel, 0, 700, 255, 0);
  int redValue = map(data.lightLevel, 0, 700, 0, 255);
  analogWrite(GREEN_PIN, greenValue);
  analogWrite(RED_PIN, redValue);

  delay(5000);
}
