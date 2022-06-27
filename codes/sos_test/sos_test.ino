const int trigger_1 = A2;
const int trigger_2 = A3;
const int coil = A4;

bool triggered = false;

void setup() {
  Serial.begin(115200);
  
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);
}

void loop() {
  if(digitalRead(trigger_1) == LOW)
    triggered = true;

  if(triggered)
    Serial.println(analogRead(coil));

  if(digitalRead(trigger_2) == LOW)
    triggered = false;

}
