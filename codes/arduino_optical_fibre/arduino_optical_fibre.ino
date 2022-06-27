/*
 *Date - 25-06-2022 
 *Experiment - Demonstration of communication through optical fibre
 *Written by - Naman Puri
 */
#define PWM 5
#define AIN A2
void timerInit(){                                           //setting up PWM at 62.5khz
  TCCR0A|=(1<<COM0B1) | (1<<WGM01) | (1<<WGM00);
  TCCR0A&=~(1<<COM0B0);
  TCCR0B|= (1<<CS00);
  TCCR0B&=~((1<<CS02) | (1<<CS01) | (1<<WGM02));
}

void analogRInit(){                                         //setting up ADC   
  ADMUX |= (1<<MUX1);
  ADMUX&= ~((1<<MUX0) | (1<<MUX2) | (1<<MUX3));
  ADMUX|=(1<<REFS0) | (1<<ADLAR) | (1<<REFS1);
  ADCSRA|=(1<<ADEN) | (1<<ADPS1);   
  ADCSRA&=~((1<<ADPS0)|(1<<ADPS2)); 
}

void setup() {
  // put your setup code here, to run once:
  pinMode(PWM,OUTPUT);                                        //setting PWM pin as output
  pinMode(AIN,INPUT);                                         //setting analog pin as input
  timerInit();
  analogRInit();
  OCR0B=128;
}

void loop() {
  // put your main code here, to run repeatedly:
  ADCSRA|=(1<<ADSC);                                         //starting conversion
  while(((ADCSRA & (1<<ADSC))>>6)==1);                       //waiting for conversion to complete
  OCR0B=ADCH;                                                //making significant 8 bits of output as PWM duty cycle
}
