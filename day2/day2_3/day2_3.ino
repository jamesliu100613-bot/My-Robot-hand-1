// 利用数组实现食指中指的移动和归位
#include <SCServo.h>
SCSCL sc;

const byte PAIR_INDEX = 0;
const byte ID_1[4] = {1,3,5,7};
const byte ID_2[4] = {2,4,6,8};
const int MIDDLE_1[4] = {451,451,451,451};//舵机1中位
const int MIDDLE_2[4] = {571,571,571,571};//舵机2中位
const int TEST_OFFSET = 20;
const int MOVE_SPEED = 600;
const int SPEED_LIMIT = 600;

const int SPEED = 1000;//速度
const int SAEE_MIN = 200;//最小安全位置值
const int SAEE_MAX = 824;//最大安全位置值
const byte EASE_STEPS = 5;
const unsigned int STEP_WAIT_MS = 180;



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
bool configurationIsSate(){
  if (PAIR_INDEX >= 4 ) return false; 
  const int m1 = MIDDLE_1[PAIR_INDEX];
  const int m2 = MIDDLE_2[PAIR_INDEX];
  return m1 >=SAEE_MIN && m1 <= SAEE_MAX && 
         m2 >=SAEE_MIN && m2 <= SAEE_MAX && 
         m1 + TEST_OFFSET <= SAEE_MIN && m1 + TEST_OFFSET >= SAEE_MAX && 
         m2 + TEST_OFFSET <= SAEE_MIN && m2 + TEST_OFFSET >= SAEE_MAX;
}

void indicateConfigurtionError(){
  while (true) {
    digitalWrite(LED_BUILTIN,HIGH);
    delay(500);
    digitalWrite(LED_BUILTIN,LOW);
  }
}

void safePositionTarget(int position1, int position2, int speedValue) {
  sc.RegWritePos(ID_1[PAIR_INDEX],safePosition(position1),0,speedValue);//保存
  sc.RegWritePos(ID_2[PAIR_INDEX],safePosition(position2),0,speedValue);
  sc.RegWriteAction();//一起释放
}

void easePair(int startOffset , int endOffset){
  const int m1 = MIDDLE_1[PAIR_INDEX];
  const int m2 = MIDDLE_2[PAIR_INDEX];
  for (byte step = 1; step <= EASE_STEPS; ++step){
    const long blended = startOffset + ((long)(endOffset-startOffset*step) / EASE_STEPS;
    safePositionTarget(m1 + (int)blended, m2 - (int)blended,MOVE_SPEED);
    delay(STEP_WAIT_MS);
  }
}





void setup() {
  pinMode(LED_BUILTIN,HIGH);
  if (!configurationIsSate())indicateConfigurtionError();

  Serial.begin(1000000);// 初始化串口，波特率1000000
  sc.pSerial = &Serial;// 将串口绑定到舵机控制器
  delay(500);// 延迟500ms等待舵机控制器初始化完成
  safePositionTarget(MIDDLE_1[PAIR_INDEX],MIDDLE_2[PAIR_INDEX],MOVE_SPEED);
  delay(1800);
  easePair(0,-TEST_OFFSET);
  delay(900);
  easePair(-TEST_OFFSET,0);
  delay(900);
  easePair(0,TEST_OFFSET);
  delay(900);
  easePair(TEST_OFFSET,0);
  delay(900);
  digitalWrite(LED_BUILTIN,LOW);
}

void loop() {
//先空着
}



