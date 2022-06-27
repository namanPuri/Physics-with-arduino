double current = 0;
double voltage = 0;
bool flag = true;
void TIM16_WriteOCR1A(unsigned int i)
{
  unsigned char sreg;
  sreg = SREG;
  /* Disable interrupts */
  cli();
  /* Set TCNT1 to i */
  OCR1A = i;
  /* Restore global interrupt flag */
  SREG = sreg;
}
void pwm_init(){
  TCCR1A = (1 << COM1A1)|(1 << WGM11);
  TCCR1A &= ~( (1 << WGM10)|(1 << COM1A0) );
  TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS10);
  TCCR1B &= ~((1 << CS12) | (1 << CS11));
  ICR1 = 0xffff;
}
void setup() {
  // put your setup code here, to run once:
pinMode(9,OUTPUT);
pinMode(A0, INPUT);   //current
pinMode(A1, INPUT);   //voltage
pwm_init();
Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
//OCR1A = 65535;
if(flag){
current = 0;
voltage = 0;
for(uint32_t i = 0; i < 55000; i=i+250){
  OCR1A = i;
  delay(50);
  for(int j = 0; j<16 ; j++){
    current += analogRead(A0);
    voltage += analogRead(A1);
  }
  current /= 16;
  current /= 10;
  current = (current*5)/1024;
  voltage /= 16;
  voltage = (voltage*5)/1024;
  Serial.print(voltage,3);
  Serial.print(",");
  Serial.println(current*1000,1);
  flag = false;
}
}
OCR1A = 0;
//Serial.println(analogRead(A1));
}
