/*
 *Date - 25-06-2022 
 *Experiment - To demonstrate RC charging and discharging
 *Written by - Naman Puri
 */
uint16_t adc_result = 0;
uint16_t buf[256] = {0};
bool flag= true;
void adc_init(){
  ADMUX |= (1 << REFS0);
  ADMUX &= ~((1 << REFS1)|(1 << ADLAR) | (1 << MUX3) | (1 << MUX2) | (1 << MUX1) | (1 << MUX0));
  ADCSRA |= (1 << ADEN)| (1 << ADPS1);
  ADCSRA &= ~((1 << ADIF) | (1 << ADIE)| (1 << ADPS2) |(1 << ADPS0));
}
void setup() {
  // put your setup code here, to run once:
pinMode(8, OUTPUT);
pinMode(A0,INPUT);
pinMode(7,INPUT);
Serial.begin(115200);
adc_init();
}

void loop() {
  // put your main code here, to run repeatedly:
  if(!digitalRead(7)){
    flag = !flag;
    delay(10);
    while(!digitalRead(7));
    delay(10);
    if(flag){
      digitalWrite(8,HIGH);
      for(int i = 0; i < 256; i++){
        ADCSRA |= (1 << ADSC);
        while(ADCSRA & (1 << ADSC));
        adc_result = ADCL;
        adc_result = (ADCH << 8) + adc_result;
        buf[i] = adc_result;
      }
     for(int i = 0; i< 256; i++){
      Serial.println(buf[i]);
     }
    }
    else if(!flag){
      digitalWrite(8,LOW);
      for(int i = 0; i < 256; i++){
        ADCSRA |= (1 << ADSC);
        while(ADCSRA & (1 << ADSC));
        adc_result = ADCL;
        adc_result = (ADCH << 8) + adc_result;
        buf[i] = adc_result;
      }
      for(int i = 0; i< 256; i++){
        Serial.println(buf[i]);
     }
  }
 }
}
