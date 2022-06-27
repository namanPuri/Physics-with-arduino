#define ain A5
void setup() {
  // put your setup code here, to run once:
pinMode(ain, INPUT);
Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
Serial.println(analogRead(ain));
}
