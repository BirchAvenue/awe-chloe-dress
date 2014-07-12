#define GEMMA

#ifdef GEMMA
  #include <avr/power.h>
  #include <avr/sleep.h>
  #define PIN_BTN_UP 2
  #define PIN_BTN_DN 1
  #define PIN_BTN_PW 0

  #define PIN_FIRST 0
  #define PIN_LAST 2
#else
  #define PIN_BTN_UP 12
  #define PIN_BTN_DN 11
  #define PIN_BTN_PW 10

  #define PIN_FIRST 10
  #define PIN_LAST 12
#endif

bool breakloop = false;
bool powerdown = false;

#define MAX_MODE 5
uint8_t LED_MODE = 5;
uint8_t pinStateUp = 0;
uint8_t pinStateDn = 0;
uint8_t pinStatePw = 0;
uint8_t last_random = PIN_FIRST;


void setup() {
#ifdef GEMMA
  power_timer1_disable();    // Disable unused peripherals
  power_adc_disable();       // to save power
#endif
  
  for(uint8_t i=PIN_FIRST; i<=PIN_LAST; i++) {
    pinMode(i, OUTPUT);
    digitalWrite(i, LOW);
  }
  
  for(uint8_t i=PIN_FIRST; i<=PIN_LAST; i++) {
    digitalWrite(i, HIGH);
    delay(5000);
    digitalWrite(i, LOW);
  }
} // setup()


void loop() {
  for(uint8_t i=PIN_FIRST; i<=PIN_LAST; i++) {
    fadeUp(i, 100);
    if(breakloop==true) break;
    
    fadeDown(i, 100);
    if(breakloop==true) break;
  }
  /* 
  uint8_t random_pin;
  uint8_t next_pin;
  uint8_t last_pin;
  
  if(LED_MODE>MAX_MODE+1)
    LED_MODE = 1;
  
  switch(LED_MODE) {
    case 0:  // Off
      for(uint8_t i=PIN_FIRST; i<=PIN_LAST; i++) {
        digitalWrite(i, LOW);
        checkInput();
        if(breakloop==true) break;
      }
      break;
    
    case 1:  // Crossfade
      for(uint8_t i=PIN_FIRST; i<=PIN_LAST; i++) {
        next_pin = i+1;
        if(next_pin>PIN_LAST)
          next_pin = PIN_FIRST;
        
        last_pin = i-1;
        if(last_pin<PIN_FIRST)
          last_pin = PIN_LAST;
        
        crossFadeUp(i, 400, last_pin);
        if(breakloop==true) break;
        
        crossFadeDown(i, 400, next_pin);
        if(breakloop==true) break;
      }
      break;

    case 2: // Cycle fade
      for(uint8_t i=PIN_FIRST; i<=PIN_LAST; i++) {
        fadeUp(i, 100);
        if(breakloop==true) break;
        
        fadeDown(i, 100);
        if(breakloop==true) break;
      }
      break;
    
    case 3:  // Random fade
      random_pin = random(PIN_FIRST, PIN_LAST+1);
      
      fadeUp(random_pin, 100);
      if(breakloop==true) break;
      
      fadeDown(random_pin, 100);
      if(breakloop==true) break;
      
      break;
    
    case 4:  // Failfade
      for(uint8_t i=PIN_FIRST; i<=PIN_LAST; i++) {
        next_pin = i+1;
        if(next_pin>PIN_LAST)
          next_pin = PIN_FIRST;
        
        last_pin = i-1;
        if(last_pin<PIN_FIRST)
          last_pin = PIN_LAST;
        
        failFadeUp(i, 400, last_pin);
        if(breakloop==true) break;
        
        failFadeDown(i, 400, next_pin);
        if(breakloop==true) break;
      }
      break;
      
    case 5:  // Fast spazz
      for(uint8_t i=PIN_FIRST; i<=PIN_LAST; i++) {
        digitalWrite(i, HIGH);
        delay(150);
        digitalWrite(i, LOW);
        delay(150);
        
        checkInput();
        
        if(breakloop==true) break;
      }
      break;
    
    default: // Fade cycle
      for(uint8_t i=PIN_FIRST; i<=PIN_LAST; i++) {
        fadeUp(i, 400);
        if(breakloop==true) break;
        
        fadeDown(i, 400);
        if(breakloop==true) break;
      }
      break;
  }
  
  
  if(breakloop==true)
    breakloop = false; */
} // loop()


void checkInput() {
  pinMode(PIN_BTN_UP, INPUT);
  pinMode(PIN_BTN_DN, INPUT);
  pinMode(PIN_BTN_PW, INPUT);
  
  uint8_t pinStateNew;
 
  
  // Check if Up was pushed
  pinStateNew = digitalRead(PIN_BTN_UP);
  if(pinStateNew!=pinStateUp) {
    if(pinStateNew==1)
      LED_MODE++;
    
    breakloop = true;
    pinStateUp = pinStateNew;
  }
  
  
  // Check if Down was pushed
  pinStateNew = digitalRead(PIN_BTN_DN);
  if(pinStateNew!=pinStateDn) {
    if(pinStateNew==1)
      LED_MODE--;
    
    breakloop = true;
    pinStateDn = pinStateNew;
  }
  
  
  // Check if Power was pushed
  pinStateNew = digitalRead(PIN_BTN_PW);
  if(pinStateNew!=pinStatePw) {
    if(pinStateNew==1)
      if(LED_MODE==0)
        LED_MODE = 1;
      else
        LED_MODE = 0;
    
    breakloop = true;
    pinStatePw = pinStateNew;
  }
  
  if(breakloop==true)
    modeChange();
  
  pinMode(PIN_BTN_UP, OUTPUT);
  pinMode(PIN_BTN_DN, OUTPUT);
  pinMode(PIN_BTN_PW, OUTPUT);
} // checkInput()


void modeChange() {
  for(uint8_t i=0; i<=LED_MODE; i++)
    for(uint8_t j=PIN_FIRST; j<=PIN_LAST; j++) {
      digitalWrite(i, HIGH);
      delay(250);
      digitalWrite(i, LOW);
      delay(250);
    }
}


void fadeUp(uint8_t fadePin, uint8_t time) {
  for(uint8_t bright = 1; bright < 100; bright++) {
    //set PWM lengths
    uint8_t on = bright;
    uint8_t off = 100 - bright;
 
    //software PWM for 'time' ms
    for(uint8_t run = 0; run < time; run++) {
      digitalWrite(fadePin, HIGH);
      delayMicroseconds(on);
      digitalWrite(fadePin, LOW);
      delayMicroseconds(off);
      
      checkInput();
      
      if(breakloop==true) break;
    }
    
    if(breakloop==true) break;
  }
} // fadeUp()


void fadeDown(uint8_t fadePin, uint8_t time) {
  for(uint8_t bright = 1; bright < 100; bright++) {
    //set inverse PWM lengths
    uint8_t on = 100 - bright;
    uint8_t off = bright;

    //software PWM for 'time' ms
    for(uint8_t run = 0; run < time; run++) {
      digitalWrite(fadePin, HIGH);
      delayMicroseconds(on);
      digitalWrite(fadePin, LOW);
      delayMicroseconds(off);
      
      checkInput();
      
      if(breakloop==true) break;
    }
    
    if(breakloop==true) break;
  }
} // fadeDown()


void crossFadeUp(uint8_t fadePin, uint8_t time, uint8_t lastPin) {
  for(uint8_t bright = 1; bright < 100; bright++) {
    //set PWM lengths
    uint8_t on = bright;
    uint8_t off = 100 - bright;
 
    //software PWM for 'time' ms
    for(uint8_t run = 0; run < time; run++) {
      digitalWrite(fadePin, HIGH);
      //if(bright>=75) digitalWrite(lastPin, HIGH);
      delayMicroseconds(on);
      
      digitalWrite(fadePin, LOW);
      //if(bright>=75) digitalWrite(lastPin, LOW);
      delayMicroseconds(off);
      
      checkInput();
      
      if(breakloop==true) break;
    }
    
    if(breakloop==true) break;
  }
} // crossFadeUp()


void crossFadeDown(uint8_t fadePin, uint8_t time, uint8_t nextPin) {
  for(uint8_t bright = 1; bright < 100; bright++) {
    //set inverse PWM lengths
    uint8_t on = 100 - bright;
    uint8_t off = bright;

    //software PWM for 'time' ms
    for(uint8_t run = 0; run < time; run++) {
      digitalWrite(fadePin, HIGH);
      //if(bright<=25) digitalWrite(nextPin, HIGH);
      delayMicroseconds(on);
      
      digitalWrite(fadePin, LOW);
      //if(bright<=25) digitalWrite(nextPin, LOW);
      delayMicroseconds(off);
      
      checkInput();
      
      if(breakloop==true) break;
    }
    
    if(breakloop==true) break;
  }
} // crossFadeDown()


void failFadeUp(uint8_t fadePin, uint8_t time, uint8_t lastPin) {
  for(uint8_t bright = 1; bright < 100; bright++) {
    //set PWM lengths
    uint8_t on = bright;
    uint8_t off = 100 - bright;
 
    //software PWM for 'time' ms
    for(uint8_t run = 0; run < time; run++) {
      digitalWrite(fadePin, HIGH);
      if(bright>=75) digitalWrite(lastPin, HIGH);
      delayMicroseconds(on);
      
      digitalWrite(fadePin, LOW);
      if(bright>=75) digitalWrite(lastPin, LOW);
      delayMicroseconds(off);
      
      checkInput();
      
      if(breakloop==true) break;
    }
    
    if(breakloop==true) break;
  }
} // failFadeUp()


void failFadeDown(uint8_t fadePin, uint8_t time, uint8_t nextPin) {
  for(uint8_t bright = 1; bright < 100; bright++) {
    //set inverse PWM lengths
    uint8_t on = 100 - bright;
    uint8_t off = bright;

    //software PWM for 'time' ms
    for(uint8_t run = 0; run < time; run++) {
      digitalWrite(fadePin, HIGH);
      if(bright<=25) digitalWrite(nextPin, HIGH);
      delayMicroseconds(on);
      
      digitalWrite(fadePin, LOW);
      if(bright<=25) digitalWrite(nextPin, LOW);
      delayMicroseconds(off);
      
      checkInput();
      
      if(breakloop==true) break;
    }
    
    if(breakloop==true) break;
  }
} // failFadeDown()
