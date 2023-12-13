const int redPin = 3;   // R引脚连接到数字3号引脚
const int greenPin = 5; // G引脚连接到数字5号引脚
const int bluePin = 6;  // B引脚连接到数字6号引脚
const int lightSensorPin = A0; // 亮度传感器连接到A0引脚

void setup() {
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // 读取亮度值
  int sensorValue = analogRead(lightSensorPin);
  int greenValue = map(sensorValue, 0, 500, 255, 0); // 亮度低时绿色亮度减少
  int redValue = map(sensorValue, 0, 500, 0, 255); // 亮度低时红色亮度增加

  analogWrite(greenPin, greenValue); // 设置绿色通道亮度
  analogWrite(redPin, redValue); // 设置红色通道亮度

  delay(100); // 短暂延时
}
