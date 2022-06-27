uint16_t my_array[50] = {0};

volatile int i = 0;
void uart_init(){
  UCSR0A |= (1 << U2X0);
  UCSR0B |= (1 << UDRIE0)| (1 << TXEN0);
  UCSR0B &= ~(1 << UCSZ02);
  UCSR0C |= (1 << USBS0) | (1 << UCSZ01) | (1 << UCSZ00);
  UCSR0C &= ~((1 << UMSEL01) | (1 << UMSEL00) | (1 << UPM01) | (1 << UPM00) | (1 << UCPOL0));
  UBRR0 = 8;
}
ISR(USART_UDRE_vect){
  UDR0 = my_array[i];
  i++;
  if(i==2){UCSR0B &= ~(1 << TXEN0);}
}

void setup() {
  // put your setup code here, to run once:
  sei();
  uart_init();
  for(int i = 0; i< 2; i++){
    my_array[i] = ;
  }
  i = 0;
  UDR0 = my_array[i];
}

void loop() {
  // put your main code here, to run repeatedly:

}
