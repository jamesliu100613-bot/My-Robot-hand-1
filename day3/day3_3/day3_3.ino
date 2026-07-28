#include <SCServo.h>
SCSCL sc;
const byte SERVO_ID_1 = 1;
const byte SERVO_ID_2 = 2;
const int MIDDLE_1 = 451;
const int MIDDLE_2 = 571;
const int SPEED = 1000;
const int SAEE_MIN = 200;//最小安全位置值
const int SAEE_MAX = 824;//最大安全位置值
const float DEG_PER_UNIT = 300.0F / 1024.0F;

/**
 * 安全位置限制1
 *  - 小于SAEE_MIN：归零（返回SAEE_MIN）
 *  - 大于SAEE_MAX：归零（返回SAEE_MAX）
 *  - 在范围内：直接返回原值
 */
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
  
  sc.RegWritePos(SERVO_ID_1, safePosition(po1), 500);
  sc.RegWritePos(SERVO_ID_2, safePosition(po2), 500);
  sc.RegWriteAction();
}








// bool tagetIsSafe(byte servoIndex,float degrees) {
//   const int target = MIDDLE[servoIndex] + angleToUnits(degrees);
//   return target >= SAEE_MIN && target <= SAEE_MAX;
// }

// bool tableIsSafe(){
//   for (byte pose = 0;pose <POSE_COUNT; ++pose) {
//     for (byte servo = 0; servo <SERVO_COUNT; ++servo){
//         if (!tagetIsSafe(servo,ACTION_DEG[pose][servo])) return false;

//     }
//   }
//   return true;
// }


void setup() {
  //pinMode(LED_BUILTIN,HIGH);
  //if (!configurationIsSate())indicateConfigurtionError();
  Serial.begin(1000000);// 初始化串口，波特率1000000
  sc.pSerial = &Serial;// 将串口绑定到舵机控制器
  delay(500);
  moveIndex(20.0,-20.0); 
  delay(500);
  moveIndex(-40,40); 
}

void loop() {
//先空着
}