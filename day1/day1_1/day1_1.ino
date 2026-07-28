// 激活LED
const int LED_PIN = 13;     //默认LED引脚13
void setup() {
  pinMode(LED_PIN,OUTPUT);
  //激活LED

}

void loop() {
  digitalWrite(LED_PIN,HIGH);//高电平
  delay(500);
  digitalWrite(LED_PIN,LOW);//低电平
  delay(500);//时间=500毫秒
  //闪烁

}
