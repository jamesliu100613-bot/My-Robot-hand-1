/*
 * 机械手控制程序 - day4
 * 控制8个串行总线舵机实现多种手势动作
 * 使用SCServo库进行舵机控制
 * 每个手指由两个舵机控制，分别控制近端和远端关节
 */
#include <SCServo.h>
SCSCL sc;                           // 舵机控制器对象，用于发送指令到舵机

//每个手指由两个舵机控制关节
const byte SERVO_ID_1 = 1; 
const byte SERVO_ID_2 = 2; 
const byte SERVO_ID_3 = 3; 
const byte SERVO_ID_4 = 4; 
const byte SERVO_ID_5 = 5;  
const byte SERVO_ID_6 = 6;  
const byte SERVO_ID_7 = 7; 
const byte SERVO_ID_8 = 8; 

// 各舵机中位位置值（初始/中立位置），基于舵机实际校准结果
const int MIDDLE_1 = 451;
const int MIDDLE_2 = 571;
const int MIDDLE_3 = 451;
const int MIDDLE_4 = 571;
const int MIDDLE_5 = 451; 
const int MIDDLE_6 = 571; 
const int MIDDLE_7 = 451;
const int MIDDLE_8 = 571;

const int SPEED = 200; 
const int SAEE_MIN = 200;           // 最小安全位置值
const int SAEE_MAX = 824;           // 最大安全位置值
const float DEG_PER_UNIT = 300.0F / 1024.0F;  // 角度转换系数：1单位=300/1024度


 //危险时LED闪烁
void indicateConfigurtionError(){
  while (true) {
    digitalWrite(LED_BUILTIN,HIGH);  
    delay(500);                       
    digitalWrite(LED_BUILTIN,LOW);   
  }
}


/**
 * 安全位置限制函数
 * 将目标位置限制在安全范围内，防止舵机超出机械极限
 */
int safePosition(int position) {
  return constrain(position, SAEE_MIN, SAEE_MAX);  // 使用constrain函数限制范围
}


/**
 * 角度转舵机单位函数
 * 将角度值转换为舵机位置单位值（0-1023）
 */
int angleToUnits(float degrees){
  const float raw = degrees / DEG_PER_UNIT;        
  return (int)(raw + (raw >= 0.0F ? 0.5 : -0.5));
}


/**
 * 控制食指运
 */
void moveIndex(int p1, int p2) {
  int po1 = MIDDLE_1 + angleToUnits(p1);          
  int po2 = MIDDLE_2 + angleToUnits(p2);          
  
  sc.RegWritePos(SERVO_ID_1, safePosition(po1), SPEED); 
  sc.RegWritePos(SERVO_ID_2, safePosition(po2), SPEED); 
  sc.RegWriteAction();                             // 触发所有缓冲指令，使舵机同时运动
}


//中指
void moveMiddle(int p3, int p4) {
  int po3 = MIDDLE_3 + angleToUnits(p3);          
  int po4 = MIDDLE_4 + angleToUnits(p4);          
  
  sc.RegWritePos(SERVO_ID_3, safePosition(po3), SPEED); 
  sc.RegWritePos(SERVO_ID_4, safePosition(po4), SPEED); 
  sc.RegWriteAction();               
}


//控制无名指运动
void moveRing(int p5, int p6) {
  int po5 = MIDDLE_5 + angleToUnits(p5);          
  int po6 = MIDDLE_6 + angleToUnits(p6);          
  
  sc.RegWritePos(SERVO_ID_5, safePosition(po5), SPEED); 
  sc.RegWritePos(SERVO_ID_6, safePosition(po6), SPEED); 
  sc.RegWriteAction();               
}


/**
 * 控制拇指运动
 */
void moveThvmb(int p7, int p8) {
  int po7 = MIDDLE_7 + angleToUnits(p7);          
  int po8 = MIDDLE_8 + angleToUnits(p8);          
  sc.RegWritePos(SERVO_ID_7, safePosition(po7), SPEED); 
  sc.RegWritePos(SERVO_ID_8, safePosition(po8), SPEED); 
  sc.RegWriteAction();               
}


/**
 * 归零/复位函数
 * 将所有手指恢复到中位（初始）位置
 */
void GuiLin(){
  moveRing(0, 0);    
  moveIndex(0, 0);  
  moveThvmb(0, 0);  
  moveMiddle(0, 0); 
  delay(500);       
}


/**
 * 手势：伸出食指（数字1）
 * 食指伸直，其余手指弯曲
 */
void onlymoveIndex() {
  moveThvmb(40, -40); 
  moveIndex(-90, 90); 
  moveMiddle(80, -80);
  moveRing(80, -80);   
  delay(1000);          
  GuiLin();           
}


/**
 * 手势：伸出中指
 * 中指伸直，其余手指弯曲
 */
void ZhongZhi(){
  moveThvmb(40, -40);  
  moveIndex(90, -90);  
  moveMiddle(-60, 60); 
  moveRing(60, -60);    
  delay(1000); 
  GuiLin();            
}


/**
 * 手势：胜利 - "耶"
 * 食指和中指伸直，拇指和无名指弯曲
 */
void yeah(){
  moveMiddle(-60, 60);
  moveIndex(-60, 70); 
  moveMiddle(-70, 60);   
  moveThvmb(80.0, -80.0);
  moveRing(90.0, -90.0); 
  delay(1000);          
  GuiLin();           
}


/**
 * 手势：张开手掌
 * 所有手指伸直展开
 */
void Zhangkai(){
  moveThvmb(-40, 40); 
  moveRing(-60, 60);   
  moveIndex(-60, 60); 
  moveMiddle(-60, 60);
  delay(1000);          
  GuiLin();           
}


/**
 * 手势：张开-合（握拳前的准备动作）
 * 手指呈半张开状态
 */
void Zhangkai_he(){
  moveThvmb(-40, 40); 
  moveIndex(-80, 20);  
  moveMiddle(-40, 80); 
  moveRing(0, 60);      
  delay(1000);          
  GuiLin();           
}


/**
 * 手势：张开-分
 * 手指呈另一种半张开状态
 */
void Zhangkai_fen(){
  moveThvmb(-40, 40); 
  moveIndex(0, 60);    
  moveMiddle(-60, 60);
  moveRing(-60, 0);     
  delay(1000);          
  GuiLin();           
}


/**
 * 手势：666
 * 拇指和小指弯曲，食指和中指伸直，无名指半弯曲
 */
void LiuLiuLiu(){
  moveThvmb(-40, 40);
  moveIndex(90, -90);   
  moveMiddle(30, -30);
  moveRing(-60, 0);    
  delay(1000);         
  GuiLin();          
}


/**
 * 手势：OK
 * 拇指和食指指尖接触形成圆圈，中指和无名指伸直
 */
void ok(){
  moveThvmb(40, -40);   
  moveIndex(90, -90);  
  moveMiddle(-60, 60);
  moveRing(-60, 60);   
  delay(1000);          
  GuiLin();           
}


/**
 * 手势：拳头
 * 所有手指弯曲握紧
 */
void quantou(){
  moveThvmb(40, -40);
  moveIndex(90, -90);
  moveMiddle(60, -60);
  moveRing(60, -60);  
  delay(1000);         
  GuiLin();          
}


/**
 * 组合动作：握拳-展开-再握拳
 * 先张开手掌，再握紧拳头，最后归零
 */
void wouzhe(){
  Zhangkai();       
  quantou();        
  GuiLin();         
}


/**
 * Arduino初始化函数
 * 仅在开机时执行一次
 */
void setup() {
  Serial.begin(1000000);      // 初始化串口通信，波特率1000000（1Mbps）
  sc.pSerial = &Serial;       // 将串口对象绑定到舵机控制器，用于发送指令
}

/**
 * 无限循环执行，依次展示各种手势
 */
void loop() {
  onlymoveIndex();      // 手势：数字1（伸出食指）
  ZhongZhi();           // 手势：伸出中指
  yeah();               // 手势：胜利（耶）
  Zhangkai();           // 手势：张开手掌
  Zhangkai_he();        // 手势：张开-合
  Zhangkai_fen();       // 手势：张开-分
  LiuLiuLiu();          // 手势：666
  ok();                 // 手势：OK
  quantou();            // 手势：拳头
  wouzhe();             // 组合动作：握拳-展开-再握拳
}