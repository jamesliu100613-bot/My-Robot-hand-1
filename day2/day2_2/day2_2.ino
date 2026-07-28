//食指中指的移动和归位
#include <SCServo.h>
SCSCL sc;
const byte SERVO_ID_1 = 1;
const byte SERVO_ID_2 = 2;
const int MIDDLE_1 = 451;//舵机1中位
const int MIDDLE_2 = 571;//舵机2中位
const byte SERVO_ID_7 = 7;//舵机7
const byte SERVO_ID_8 = 8;//舵机8
const int MIDDLE_7 = 451;//舵机7中位
const int MIDDLE_8 = 571;//舵机8中位
const int SPEED = 1000;//速度
const int SAEE_MIN = 200;//最小安全位置值
const int SAEE_MAX = 824;//最大安全位置值
const int  offset = 80;

int safePosition(int position) {
  return constrain(position,SAEE_MIN,SAEE_MAX);
}
int safePositionManual(int position) {
  if (position<SAEE_MIN){
    return constrain(position - position,SAEE_MIN,SAEE_MAX);
  }
  else if (position>SAEE_MAX) {
    return constrain(position - position,SAEE_MIN,SAEE_MAX);
  }
  else if (SAEE_MAX>position>SAEE_MIN){
    return constrain(position,SAEE_MIN,SAEE_MAX);
  }
}
void explainPosition(int position) {
  const int constrained = safePosition(position);
  const int Manual = safePositionManual(position);
  
  Serial.print(F("Inout")); Serial.print(position);
  if (position<SAEE_MIN){
    Serial.print(F("lowwer ->"));
  }
  else if (position>SAEE_MAX){
    Serial.print(F("upper ->"));
  }
  else {
    Serial.print(F("range ->"));
  }
  Serial.print(constrained);
  Serial.print(F("manal="));
  Serial.println(Manual);
}

/**
安全位置限制
将输入的位置值限制在安全范围内（SAEE_MIN<position<SAEE_MAX）
防止舵机超出安全范围的位置
position目标位置值
return 限制后的安全位置值
 */




void movePair12(int position1, int position2, int speedValue) {
  sc.RegWritePos(SERVO_ID_1,safePosition(position1),0,speedValue);//保存
  sc.RegWritePos(SERVO_ID_2,safePosition(position2),0,speedValue);
  sc.RegWriteAction();//一起释放
}

void movePair78(int position1, int position2, int speedValue) {
  sc.RegWritePos(SERVO_ID_7, safePosition(position1), 0, speedValue);
  sc.RegWritePos(SERVO_ID_8, safePosition(position2), 0, speedValue);
  sc.RegWriteAction();
}










void setup() {
  Serial.begin(1000000);// 初始化串口，波特率1000000
  sc.pSerial = &Serial;// 将串口绑定到舵机控制器
  delay(500);// 延迟500ms等待舵机控制器初始化完成
  explainPosition(1000);
}

void loop() {
  movePair12(MIDDLE_1,MIDDLE_2,SPEED);// 移动到中位
  delay(1500);// 延迟1500ms等待舵机移动到中位
  movePair12(MIDDLE_1-offset,MIDDLE_2+offset,SPEED);// 移动到偏移位
  delay(1500);// 延迟1500ms等待舵机移动到偏移位
  movePair12(MIDDLE_1,MIDDLE_2,SPEED);
  delay(1500);// 延迟1500ms等待舵机移动到中位
  movePair12(MIDDLE_1 + offset ,MIDDLE_2 - offset ,SPEED);// 移动到偏移位
  delay(1500);// 延迟1500ms等待舵机移动到偏移位
  //舵机1，2小幅度移动


  movePair78(MIDDLE_7, MIDDLE_8, SPEED);// 移动到中位
  delay(1500);// 延迟1500ms等待舵机移动到中位
  movePair78(MIDDLE_7-offset,MIDDLE_8+offset,SPEED);// 移动到偏移位
  delay(1500);// 延迟1500ms等待舵机移动到偏移位
  movePair78(MIDDLE_7,MIDDLE_8,SPEED);
  delay(1500);// 延迟1500ms等待舵机移动到中位
  movePair78(MIDDLE_7 + offset ,MIDDLE_8 - offset ,SPEED);// 移动到偏移位
  delay(1500);// 延迟1500ms等待舵机移动到偏移位
  //舵机7，8

}


