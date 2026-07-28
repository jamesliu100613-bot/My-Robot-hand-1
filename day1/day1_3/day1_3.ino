// 角度转换为偏移量
const float suanfa = 300.0/1024.0;
float x ;
float angleToOffset(float x) {
   x = x/suanfa;
  return (int)x ;
}

void setup() {
  Serial.begin(115200);
  delay(500);
  float angles[] = {-35.0,0.0,35.0,90.0};
  const int  ANGLE_COUNT = sizeof(angles) / sizeof(angles[0]);
  for (int i = 0; i<ANGLE_COUNT;i++){
    Serial.print(angles[i]);
    Serial.print("->");
    Serial.println(angleToOffset(angles[i]));
  }


}

void loop() {
  // put your main code here, to run repeatedly:

}