
#include <SCServo.h>
SCSCL sc;
const byte PAIR_INDEX = 0;
const byte SERVO_COUNT = 8;
const byte POSE_COUNT = 4;
const byte ID[8] = {1,2,3,4,5,6,7,8};// 8个舵机
const int MIDDLE[SERVO_COUNT] = {451,571,451,571,451,571,451,571};// 中位值
const float DEG_PER_UNIT = 300.0F / 1024.0F;
const int SAEE_MIN = 200;//最小安全位置值
const int SAEE_MAX = 824;//最大安全位置值

const float ACTION_DEG[POSE_COUNT][SERVO_COUNT] = {
  {212,232,324,432,133,123,431,543},
  {149,915,620,148,165,954,213,423},
  {123,423,346,132,634,632,567,634},
  {123,195,149,325,248,421,243,542}
  };

int angleToUnits(float degrees){
  const float raw = degrees / DEG_PER_UNIT;
  return (int)(raw + (raw >= 0.0F ? 0.5 : -0.5));
}


bool tagetIsSafe(byte servoIndex,float degrees) {
  const int target = MIDDLE[servoIndex] + angleToUnits(degrees);
  return target >= SAEE_MIN && target <= SAEE_MAX;
}

bool tableIsSafe(){
  for (byte pose = 0;pose <POSE_COUNT; ++pose) {
    for (byte servo = 0; servo <SERVO_COUNT; ++servo){
        if (!tagetIsSafe(servo,ACTION_DEG[pose][servo])) return false;

    }
  }
  return true;
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