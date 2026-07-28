#include <SCServo.h>
SCSCL sc;
const byte SERVO_ID_1 = 1;
const byte SERVO_ID_2 = 2;
const byte SERVO_ID_3 = 3;
const byte SERVO_ID_4 = 4;
const byte SERVO_ID_5 = 5;
const byte SERVO_ID_6 = 6;
const byte SERVO_ID_7 = 7;
const byte SERVO_ID_8 = 8;
const int MIDDLE_1 = 451;
const int MIDDLE_2 = 571;
const int MIDDLE_3 = 451;
const int MIDDLE_4 = 571;
const int MIDDLE_5 = 451;
const int MIDDLE_6 = 571;
const int MIDDLE_7 = 451;
const int MIDDLE_8 = 571;
const int SPEED = 500;
const int SAEE_MIN = 200;//最小安全位置值
const int SAEE_MAX = 824;//最大安全位置值
const float DEG_PER_UNIT = 300.0F / 1024.0F;



/**
 * 配置错误指示
 * 当配置不安全时，让板载LED持续闪烁
 */
void indicateConfigurtionError(){
  while (true) {
    digitalWrite(LED_BUILTIN,HIGH);
    delay(500);
    digitalWrite(LED_BUILTIN,LOW);
  }
}


/**
 * 安全位置限制（自动夹紧）
 * 防止舵机超出安全范围
 * position 目标位置值
 * return 限制后的安全位置值
 */
int safePosition(int position) {
  return constrain(position,SAEE_MIN,SAEE_MAX);
}

int angleToUnits(float degrees){
  const float raw = degrees / DEG_PER_UNIT;
  return (int)(raw + (raw >= 0.0F ? 0.5 : -0.5));
}



void moveIndex(int p1, int p2) {
  int po1 = MIDDLE_1 + angleToUnits(p1);
  int po2 = MIDDLE_2 + angleToUnits(p2);
  
  sc.RegWritePos(SERVO_ID_1, safePosition(po1), SPEED);
  sc.RegWritePos(SERVO_ID_2, safePosition(po2), SPEED);
  sc.RegWriteAction();
}


void moveMiddle(int p3, int p4) {
  int po3 = MIDDLE_3 + angleToUnits(p3);
  int po4 = MIDDLE_4 + angleToUnits(p4);
  
  sc.RegWritePos(SERVO_ID_3, safePosition(po3), SPEED);
  sc.RegWritePos(SERVO_ID_4, safePosition(po4), SPEED);
  sc.RegWriteAction();
}
void moveRing(int p5, int p6) {
  int po5 = MIDDLE_5 + angleToUnits(p5);
  int po6 = MIDDLE_6 + angleToUnits(p6);
  
  sc.RegWritePos(SERVO_ID_5, safePosition(po5), SPEED);
  sc.RegWritePos(SERVO_ID_6, safePosition(po6), SPEED);
  sc.RegWriteAction();
}
void moveThvmb(int p7, int p8) {
  int po7 = MIDDLE_7 + angleToUnits(p7);
  int po8 = MIDDLE_8 + angleToUnits(p8);
  
  sc.RegWritePos(SERVO_ID_7, safePosition(po7), SPEED);
  sc.RegWritePos(SERVO_ID_8, safePosition(po8), SPEED);
  sc.RegWriteAction();
}



void setup() {
  Serial.begin(1000000);// 初始化串口，波特率1000000
  sc.pSerial = &Serial;// 将串口绑定到舵机控制器
  moveThvmb(0,-0);  
  moveRing(0,0); 
  moveIndex(0,0); 
  moveMiddle(0,0);
  delay(500);
  moveThvmb(-20,20); 
  moveIndex(-60,60); 
  moveMiddle(-60,60);
  moveIndex(-60,70); 
  moveMiddle(-70,60);
  moveRing(-20,-20); 
  delay(500);
  moveThvmb(80.0,-80.0);  
  moveRing(90.0,-90.0); 


}

void loop() {
}
