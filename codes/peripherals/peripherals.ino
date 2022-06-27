#define button1 4
#define button2 7
#define r 11
#define g 10
#define b 6
#define buzzer 3
void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);
pinMode(r, OUTPUT);
pinMode(g, OUTPUT);
pinMode(b, OUTPUT);
pinMode(buzzer, OUTPUT);
pinMode(button1, INPUT);
pinMode(button2, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
Serial.println(digitalRead(button1));
Serial.println(digitalRead(button2));
for(int i = 255; i > 0; i--){
  analogWrite(r, i);
  delay(10);
}
analogWrite(r,255);
delay(100);
for(int i = 255; i > 0; i--){
  analogWrite(g, i);
  delay(10);
}
analogWrite(g,255);
delay(100);
for(int i = 255; i > 0; i--){
  analogWrite(b, i);
  delay(10);
}
analogWrite(b,255);
delay(100);
for(int i = 0; i < 255; i++){
  analogWrite(buzzer, i);
  delay(10);
}
analogWrite(buzzer,0);
}
