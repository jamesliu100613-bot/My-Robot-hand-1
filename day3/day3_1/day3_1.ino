// 舵机限位保护与同步控制
#include <SCServo.h>
SCSCL sc;
const byte PAIR_INDEX = 0;
const byte SERVO_COUNT = 8;
const byte POSE_COUNT = 4;
const byte ID[8] = {1,2,3,4,5,6,7,8};// 8个舵机
const int MIDDLE[SERVO_COUNT] = {451,571,451,571,451,571,451,571};// 中位值
const float DEG_PER_UNIT = 300.0F / 1024.0F
const int TEST_OFFSET = 200;// 摆动幅度
const int MOVE_SPEED = 600;// 默认运动速度
const int SPEED_LIMIT = 600;// 速度上限
const int safeSpeed = 600;// 安全速度（用于限速）
const int SPEED =600;//速度
const int SAEE_MIN = 200;//最小安全位置值
const int SAEE_MAX = 824;//最大安全位置值
const byte EASE_STEPS = 5;// 缓动步数（把一段运动拆成5步，实现过渡）
const unsigned int STEP_WAIT_MS = 180;// 每个缓动步之间的间隔时间(ms)



/**
 * 安全位置限制（自动夹紧）
 * 防止舵机超出安全范围
 * position 目标位置值
 * return 限制后的安全位置值
 */
int safePosition(int position) {
  return constrain(position,SAEE_MIN,SAEE_MAX);
}

/**
 * 安全速度限制
 * 将速度限制在[1, SPEED_LIMIT]范围内，避免速度为0或过大
 * s 输入速度
 * return 限制后的安全速度
 */
int safespeed(int s){
  return constrain(s,1,SPEED_LIMIT);
}

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
 * 安全位置限制2
 * 将一对舵机的目标位置同时保存
 * 再统一触发动作，保证两个舵机同步运动。
 */
void safePositionTarget(byte firstIndex,int position1, int position2) {
  sc.RegWritePos(ID[firstIndex],safePosition(position1),0,safespeed(MOVE_SPEED));//保存
  sc.RegWritePos(ID[firstIndex+1],safePosition(position2),0,safespeed(MOVE_SPEED));
  sc.RegWriteAction();//一起释放
}


//*配置安全检查
//若有任一舵机的中位±测试幅度超出[SAEE_MIN, SAEE_MAX]，返回false
bool configurationIsSate(){
  for (byte i = 0; i < 8; ++i){
    if (MIDDLE[i]-TEST_OFFSET<SAEE_MIN) return false;
    if (MIDDLE[i]+TEST_OFFSET>SAEE_MAX) return false;
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



bool Xiabiao (int index){
  if (index <= 7) return true;
}


void printAt(int index){
  while (true) {
    Serial.println("ok");
    Serial.println(ID[index]);
    Serial.println(MIDDLE[index]);
  }
}






void setup() {
  //pinMode(LED_BUILTIN,HIGH);
  //if (!configurationIsSate())indicateConfigurtionError();
  Serial.begin(115200);// 初始化串口，波特率1000000
  sc.pSerial = &Serial;// 将串口绑定到舵机控制器
  printAt(7);
}

void loop() {
//先空着
}
