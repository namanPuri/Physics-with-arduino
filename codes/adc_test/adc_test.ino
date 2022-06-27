int adc_result;
void adc_init(){
  ADMUX |= (1 << REFS0);
  ADMUX &= ~((1 << REFS1)|(1 << ADLAR) | (1 << MUX3) | (1 << MUX2) | (1 << MUX1) | (1 << MUX0));
  ADCSRA |= (1 << ADEN)|(1 << ADPS1);
  ADCSRA &= ~((1 << ADIF) | (1 << ADIE)| (1 << ADPS2)| (1 << ADPS0));
  //ADCSRB &= ~((1 << ADTS2) | (1 << ADTS1) | (1 << ADTS0));
}
unsigned long starts = 0;
void setup() {
  // put your setup code here, to run once:
pinMode(A0,INPUT);
pinMode(10,OUTPUT);
Serial.begin(115200);
adc_init();
}

void loop() {
  // put your main code here, to run repeatedly:
//   starts = micros();
  digitalWrite(10,HIGH);
   ADCSRA |= (1 << ADSC);
   while(ADCSRA & (1 << ADSC));
   adc_result = ADCL;
   adc_result = (ADCH << 8) + adc_result;
//   Serial.println(micros() - starts);
  digitalWrite(10,LOW);
}
