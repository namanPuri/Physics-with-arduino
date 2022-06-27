#define thermistor A2
float R2 = 10000;
float logR1, R1;
float temp, tempF = 0;
float c1 = 0.001125308852122;
float c2 = 0.000234711863267;
float c3 = 0.000000085663516;

int to_temperature(){
    temp = 0;
    for(int i=0; i<256;i++){
      temp = temp + analogRead(thermistor);
    }
    temp = temp/256;
    R1 = R2 / ((512 / (float)temp) - 1);
    logR1 = log(R1);
    tempF = (1.0 / (c1 + c2*logR1 + c3*logR1*logR1*logR1));
    tempF = tempF - 273.15;
    return (int)tempF;
}

void setup() {
  // put your setup code here, to run once:
pinMode(thermistor, INPUT);
Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
Serial.println(to_temperature(),1);
delay(5000);
}
