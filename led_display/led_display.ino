#include <Arduino.h>

// 定义引脚
int pinOE = 3;
int pinR1 = 4; // 控制红色
int pinG1 = 5; // 控制绿色
int pinSCK = 6;
int pinLAT = 7;
int pinA = 8;
int pinB = 9;
int pinC = 10;
int pinD = 11;

// 定义笑脸图案
// 0 表示LED关闭，1 表示LED开启
// 这里以16x16点阵为例
const int smileFace[16][16] = {
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // 1
  {0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0}, // 2
  {0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0}, // 3
  {0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0}, // 4
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // 5
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // 6
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // 7
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // 8
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // 9
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // 10
  {0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0}, // 11
  {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0}, // 12
  {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0}, // 13
  {0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0}, // 14
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // 15
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // 16
};


void setup() {
  pinMode(pinOE, OUTPUT);
  pinMode(pinR1, OUTPUT);
  pinMode(pinG1, OUTPUT);
  pinMode(pinSCK, OUTPUT);
  pinMode(pinLAT, OUTPUT);
  pinMode(pinA, OUTPUT);
  pinMode(pinB, OUTPUT);
  pinMode(pinC, OUTPUT);
  pinMode(pinD, OUTPUT);

  // 初始化所有引脚状态
  digitalWrite(pinOE, HIGH);
  digitalWrite(pinR1, HIGH);
  digitalWrite(pinG1, HIGH);
  digitalWrite(pinSCK, LOW);
  digitalWrite(pinLAT, LOW);
}

void loop() {
  for (int row = 0; row < 16; row++) {
    // 关闭显示以防止虚影
    digitalWrite(pinOE, HIGH);

    // 设置行地址
    digitalWrite(pinA, bitRead(row, 0));
    digitalWrite(pinB, bitRead(row, 1));
    digitalWrite(pinC, bitRead(row, 2));
    digitalWrite(pinD, bitRead(row, 3));

    // 准备发送数据到列
    for (int col = 0; col < 16; col++) {
      // 根据图案数组决定是否点亮绿色LED
      digitalWrite(pinG1, smileFace[row][col] ? LOW : HIGH); // 如果数组中是1，点亮绿色
      digitalWrite(pinR1, HIGH); // 确保红色LED关闭

      // 发送时钟脉冲
      digitalWrite(pinSCK, HIGH);
      digitalWrite(pinSCK, LOW);
    }

    // 锁存当前行数据
    digitalWrite(pinLAT, HIGH);
    delayMicroseconds(10);
    digitalWrite(pinLAT, LOW);

    // 开启显示当前行
    digitalWrite(pinOE, LOW);
    // 每行显示短暂延迟
    delayMicroseconds(100);
  }

  // 控制整体刷新率
  delay(10);
}
