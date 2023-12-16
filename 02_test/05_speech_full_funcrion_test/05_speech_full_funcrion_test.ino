#include <Wire.h>
#include "DFRobot_SpeechSynthesis_V2.h"
#include "DFRobot_DF2301Q.h"

DFRobot_DF2301Q_I2C DF2301Q(&Wire, 0x64);
DFRobot_SpeechSynthesis_I2C ss(&Wire, 0x40);

void setup() {
  // put your setup code here, to run once:
  Wire.begin();  // 启动I2C总线
  Serial.begin(9600);
  ss.begin();
  ss.setVolume(2);
  ss.setLanguage(ss.eEnglishl);
  DF2301Q.setVolume(1);
  DF2301Q.setWakeTime(4);
  //uint8_t wakeTime = 0;
  //wakeTime = DF2301Q.getWakeTime();
  //Serial.print("wakeTime = ");
  //Serial.println(wakeTime);
  //DF2301Q.playByCMDID(23);  // Common word ID
}

void loop() {
  // put your main code here, to run repeatedly:
  int temp = 24;
  char strTemp[10];
  sprintf(strTemp, "%d", temp);
  uint8_t CMDID = 0;
  CMDID = DF2301Q.getCMDID();
  Serial.println(CMDID);
  switch (CMDID) {
    case 69:
      DF2301Q.setMuteMode(1);
      delay(1000);
      ss.speak(F("The real time temperature is"));
      ss.speak(strTemp);
      ss.speak(F("Degrees centigrade"));
      Serial.println(CMDID);
      DF2301Q.setMuteMode(0);
      CMDID=0;
      break;
    default:
      Serial.print("wrong command");
      break;
  }
  delay(1000);
}
