#include <avr/power.h>
#include <avr/sleep.h>

void setup() {
  power_timer1_disable();    // Disable unused peripherals
  power_adc_disable();       // to save power
  
  for(uint8_t i=0; i<3; i++) {
    pinMode(i, OUTPUT);
    digitalWrite(i, LOW);
  }
}

void loop() {
  for(uint8_t i=0; i<3; i++) {
    fadeUp(i, 400);
    fadeDown(i, 400);
  }
}

void fadeUp(uint8_t fadePin, uint8_t time) {
  for(int bright = 1; bright < 100; bright++) {
    //set PWM lengths
    int on = bright;
    int off = 100 - bright;
 
    //software PWM for 'time' ms
    for(int run = 0; run < time; run++) {
      digitalWrite(fadePin, HIGH);
      delayMicroseconds(on);
      digitalWrite(fadePin, LOW);
      delayMicroseconds(off);
    }
  }
}

void fadeDown(uint8_t fadePin, uint8_t time) {
  for(int bright = 1; bright < 100; bright++) {
    //set inverse PWM lengths
    int on = 100 - bright;
    int off = bright;

    //software PWM for 'time' ms
    for(int run = 0; run < time; run++) {
      digitalWrite(fadePin, HIGH);
      delayMicroseconds(on);
      digitalWrite(fadePin, LOW);
      delayMicroseconds(off);
    }
  }
}
