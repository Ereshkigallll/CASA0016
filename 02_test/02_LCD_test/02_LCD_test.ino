#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// 初始化LiquidCrystal_I2C对象
// 参数为LCD的I2C地址和列数、行数
LiquidCrystal_I2C lcd(0x27, 16, 2); // 通常I2C地址为0x27或0x3F

void setup() {
  // 初始化LCD
  lcd.init();
  // 打开LCD背光
  lcd.backlight();

  // 在LCD上显示文本
  lcd.setCursor(0, 0); // 将光标设置到第一行的开始位置
  lcd.print("Hello, World!");

  lcd.setCursor(0, 1); // 将光标设置到第二行的开始位置
  lcd.print("LCD Test");
}

void loop() {
  // 这里不需要任何代码
}
