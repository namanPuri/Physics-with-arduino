/*
 *Date - 25-06-2022 
 *Experiment - 2-Terminal Device Characteristics
 *Written by - Naman Puri
 */
#define button 7
double current = 0;
double voltage = 0;
bool flag = true;
uint16_t adc_result = 0;
void pwm_init(){                                        //setting up timer for PWM - acts as input for DUT
  TCCR1A = (1 << COM1A1)|(1 << WGM11);
  TCCR1A &= ~( (1 << WGM10)|(1 << COM1A0) );
  TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS10);
  TCCR1B &= ~((1 << CS12) | (1 << CS11));
  ICR1 = 1024;
}
void adc_init(){
  ADMUX |= (1 << REFS0);
  ADMUX &= ~((1 << REFS1) | (1 << ADLAR));
  ADCSRA |= (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1);
  ADCSRA &= ~((1 << ADPS0) | (1 << ADIF) | (1 << ADIE));
}
uint16_t read_voltage(){
  ADMUX &= ~((1 << MUX3) | (1 << MUX2) | (1 << MUX1));
  ADMUX |= (1 << MUX0);  
  ADCSRA |= (1 << ADSC);
  while(ADCSRA & (1 << ADSC));
  adc_result = ADCL;
  adc_result = (ADCH << 8) + adc_result;
  return adc_result;
}
uint16_t read_current(){
  ADMUX &= ~((1 << MUX3) | (1 << MUX0));
  ADMUX |= (1 << MUX2) | (1 << MUX1);
  ADCSRA |= (1 << ADSC);
  while(ADCSRA & (1 << ADSC));
  adc_result = ADCL;
  adc_result = (ADCH << 8) + adc_result;
  return adc_result;
}
void setup() {
  // put your setup code here, to run once:
pinMode(button, INPUT);                                 //button for taking reading
pinMode(9,OUTPUT);
pinMode(A6, INPUT);   //current
pinMode(A1, INPUT);   //voltage
pwm_init();
adc_init();
Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
if(!digitalRead(button)){                               //reading and debouncing button
  delay(10);
  while(!digitalRead(button));
  delay(10);
  flag = true;
}
if(flag){
  current = 0;
  voltage = 0;
  for(uint32_t i = 0; i < 600; i=i+10){
    OCR1A = i;
    delay(50);
    for(int j = 0; j<256 ; j++){
      current += read_current();
      voltage += read_voltage();
    }
  current = current / 16;
  current /= 10;
  current = (current*5)/16368;
  voltage = voltage / 16;
  voltage = (voltage*5)/16368;
  Serial.print(voltage,3);
  Serial.print(",");
  Serial.println(current*1000,1);
  flag = false;
  }
}
OCR1A = 0;
}
