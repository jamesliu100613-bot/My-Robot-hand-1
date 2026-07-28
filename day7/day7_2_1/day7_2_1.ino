// 手势控制LED程序 - 接收串口指令控制LED亮灭
// 配合Python手势识别程序使用

String gesture = "";    // 存储手势数据
const int LED_PIN = 13; // LED引脚（板载LED）
String data = "";       // 临时存储串口数据

void setup() {
  Serial.begin(115200);       // 初始化串口（波特率需与Python端一致）
  pinMode(LED_PIN, OUTPUT);   // 设置LED引脚为输出模式
}

void loop() {
  // 检查是否有串口数据
  if (Serial.available()) {
    data = Serial.readStringUntil('\n');  // 读取一行数据
    
    // 根据手势指令控制LED
    if (data == "GESTURE:3") {
      digitalWrite(LED_PIN, HIGH);  // 手势"3"：LED亮
    }
    if (data == "GESTURE:4") {
      digitalWrite(LED_PIN, LOW);   // 手势"4"：LED灭
    }
  }
}