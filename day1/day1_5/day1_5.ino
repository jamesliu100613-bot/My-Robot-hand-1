// 移动一根手指
#include <SCServo.h>
SCSCL sc;
const byte SERVO_ID_1 = 1;
const byte SERVO_ID_2 = 2;
const int MIDDLE_1 = 451;//舵机1中位
const int MIDDLE_2 = 571;//舵机2中位
const int SPEED = 1000;//速度
const int SAEE_MIN = 200;//最小安全位置值
const int SAEE_MAX = 824;//最大安全位置值
const int  offset = 80;

int safePosition(int position) {
  return constrain(position,SAEE_MIN,SAEE_MAX);
}
/**
安全位置限制
将输入的位置值限制在安全范围内（SAEE_MIN<position<SAEE_MAX）
防止舵机超出安全范围的位置
position目标位置值
return 限制后的安全位置值
 */

void movePair(int position1, int position2, int speedValue) {
  sc.RegWritePos(SERVO_ID_1,safePosition(position1),0,speedValue);//保存
  sc.RegWritePos(SERVO_ID_2,safePosition(position2),0,speedValue);
  sc.RegWriteAction();//一起释放
}

void setup() {
  Serial.begin(1000000);// 初始化串口，波特率1000000
  sc.pSerial = &Serial;// 将串口绑定到舵机控制器
  delay(500);// 延迟500ms等待舵机控制器初始化完成
}

void loop() {
  movePair(MIDDLE_1,MIDDLE_2,SPEED);// 移动到中位
  delay(1500);// 延迟1500ms等待舵机移动到中位
  movePair(MIDDLE_1-offset,MIDDLE_2+offset,SPEED);// 移动到偏移位
  delay(1500);// 延迟1500ms等待舵机移动到偏移位
  movePair(MIDDLE_1,MIDDLE_2,SPEED);
  delay(1500);// 延迟1500ms等待舵机移动到中位
  movePair(MIDDLE_1 + offset ,MIDDLE_2 - offset ,SPEED);// 移动到偏移位
  delay(1500);// 延迟1500ms等待舵机移动到偏移位

}
