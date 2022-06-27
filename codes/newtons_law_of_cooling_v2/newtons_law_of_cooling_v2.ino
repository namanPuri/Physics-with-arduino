/*
 *Date - 25-06-2022 
 *Experiment - Newtons law of cooling
 *Written by - Naman Puri
 *Module used - MAX31855
 */
#include <SPI.h>
#include "Adafruit_MAX31855.h"
#define MAXDO   11
#define MAXCS   2
#define MAXCLK  6

// initialize the Thermocouple
Adafruit_MAX31855 thermocouple(MAXCLK, MAXCS, MAXDO);

void setup() {
  Serial.begin(115200);           //initializing Serial at 115200 baud rate
  delay(500);
}

void loop() {
   double c = thermocouple.readCelsius(); //library function to read temperature from the module
   if (isnan(c)) {
   } else {
     Serial.println(c);
   }
   delay(5000);
}
