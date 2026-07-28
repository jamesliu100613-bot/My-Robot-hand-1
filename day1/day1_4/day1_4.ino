// 使食指中指归位
#include <SCServo.h>
SCSCL sc;
const byte SERVO_ID_1 = 1;
const byte SERVO_ID_2 = 2;
const int MIDDLE_1 = 451;
const int MIDDLE_2 = 571;
const int SPEED = 1000;
const int SAEE_MIN = 200;
const int SAEE_MAX = 824;
int safePosition(int position) {
  return constrain(position,SAEE_MIN,SAEE_MAX);
}

void movePair(int position1, int position2, int speedValue) {
  sc.RegWritePos(SERVO_ID_1,safePosition(position1),0,speedValue);//保存
  sc.RegWritePos(SERVO_ID_2,safePosition(position2),0,speedValue);
  sc.RegWriteAction();//一起释放
}





void setup() {
  Serial.begin(1000000);
  sc.pSerial = &Serial;
  delay(500);
  movePair(MIDDLE_1,MIDDLE_2,SPEED);
}

void loop() {
  // put your main code here, to run repeatedly:
}
