// 串口通信测试
int num = 0;
void setup() {
  Serial.begin(115200);//波特率
  delay(500);
  Serial.println("okok");//串口监测
  // put your setup code here, to run once:

}

void loop() {
  Serial.println(num);
  delay(500);
  num +=1;
  // put your main code here, to run repeatedly:

}
