// 利用数组实现多个手指的移动和归位
#include <SCServo.h>
SCSCL sc;
const byte PAIR_INDEX = 0;
const byte ID[8] = {1,2,3,4,5,6,7,8};// 8个舵机
const int MIDDLE[8] = {451,571,451,571,451,571,451,571};// 中位值
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



void easePair(byte firstIndex,int startOffset , int endOffset){
  const int m1 = MIDDLE[firstIndex];// 第一个舵机的中位
  const int m2 = MIDDLE[firstIndex+1];// 第二个舵机的中位
  for (byte step = 1; step <= EASE_STEPS; ++step){
    const long offset = startOffset + ((long)(endOffset-startOffset*step) / EASE_STEPS);
    safePositionTarget(firstIndex,m1 + (int)offset, m2 - (int)offset);
    delay(STEP_WAIT_MS);
  }
}

/**
 * 测试舵机
 * 用于验证该对舵机能否正常摆动
 */
void testPair(byte firstIndex){
  safePositionTarget(firstIndex,MIDDLE[firstIndex],MIDDLE[firstIndex + 1]);//先回到中位
  delay(100);
  easePair(firstIndex,0,-TEST_OFFSET);
  delay(100);
  easePair(firstIndex,-TEST_OFFSET,0);
  delay(100);
  easePair(firstIndex,0,TEST_OFFSET);
  delay(100);
  easePair(firstIndex,TEST_OFFSET,0);
  delay(100);
  digitalWrite(LED_BUILTIN,LOW);
}


void setup() {
  //pinMode(LED_BUILTIN,HIGH);
  //if (!configurationIsSate())indicateConfigurtionError();
  Serial.begin(1000000);// 初始化串口，波特率1000000
  sc.pSerial = &Serial;// 将串口绑定到舵机控制器
  delay(500);// 延迟500ms等待舵机控制器初始化完成
  testPair(0);// 测试第1对舵机（ID 1,2）
  testPair(2);// 测试第2对舵机（ID 3,4）
  testPair(4);// 测试第3对舵机（ID 5,6）
  testPair(6);// 测试第4对舵机（ID 7,8）
}

void loop() {
//先空着
}
