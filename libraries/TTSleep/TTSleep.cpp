#include "TTSleep.h"

Sleep* Sleep::pSleep = 0; 

Sleep::Sleep()
{
	pSleep = this;	//the ptr points to this object
	timeSleep = 0;  // total time due to sleep
	calibv = 0.93; // ratio of real clock with WDT clock
	byte isrcalled = 0;  // WDT vector flag

}

void Sleep::setSleepMode(int mode)
{
  sleepMode_ = mode;
}

// Calibrate watchdog timer with millis() timer(timer0)
void Sleep::calibrateTime() {
  // timer0 continues to run in idle sleep mode
  set_sleep_mode(SLEEP_MODE_IDLE);
  long tt1=millis();
  sleepNow(256);
  long tt2=millis();
  calibv = 256.0/(tt2-tt1);
}

// Estimated millis is real clock + calibrated sleep time
unsigned long Sleep::WDTMillis() {
  return millis()+timeSleep;
}

// Delay function
void Sleep::sleepDelay(unsigned long sleepTime) {
 // ADCSRA &= ~(1<<ADEN);  // adc off
 // PRR = 0xEF; // modules off

  set_sleep_mode(sleepMode_);
  //int trem = sleepNow(sleepTime*calibv);  //there is something wrong here, calv is subtracting too much time, off by a factor of 10?
   int trem = sleepNow(sleepTime); 
  timeSleep += (sleepTime-trem);

 // PRR = 0x00; //modules on
 // ADCSRA |= (1<<ADEN);  // adc on
}

// internal function.  
int Sleep::sleepNow(unsigned long remainTime) {
  byte WDTps = 9;  // WDT Prescaler value, 9 = 8192ms
  
  isrcalled = 0;
  sleep_enable();
  while(remainTime > 0) {
	Serial.println(remainTime);
	delay(100);
    //work out next prescale unit to use
    while ((0x10<<WDTps) > remainTime && WDTps > 0) {
      WDTps--;
    }
    // send prescaler mask to WDT_On
    WDT_On((WDTps & 0x08 ? (1<<WDP3) : 0x00) | (WDTps & 0x07));
    isrcalled=0;
    while (isrcalled==0) {
      // turn bod off
      MCUCR |= (1<<BODS) | (1<<BODSE);
      MCUCR &= ~(1<<BODSE);  // must be done right before sleep
      sleep_cpu();  // sleep here
    }
    // calculate remaining time
    remainTime -= (0x10<<WDTps);
	if ((long) remainTime < 0 ) {remainTime = 0;} //check for unsigned underflow, but converting to signed
	
  }
  sleep_disable();
  return remainTime;
}

// Internal function: Start watchdog timer
// byte psVal - Prescale mask
void Sleep::WDT_On(byte psMask)
{
  // prepare timed sequence first
  byte ps = (psMask | (1<<WDIE)) & ~(1<<WDE);
  cli();
  wdt_reset();
  /* Clear WDRF in MCUSR */
  MCUSR &= ~(1<<WDRF);
  // start timed sequence
  WDTCSR |= (1<<WDCE) | (1<<WDE);
  // set new watchdog timeout value
  WDTCSR = ps;
  sei();
}

// Internal function.  Stop watchdog timer
void Sleep::WDT_Off() {
  cli();
  wdt_reset();
  /* Clear WDRF in MCUSR */
  MCUSR &= ~(1<<WDRF);
  /* Write logical one to WDCE and WDE */
  /* Keep old prescaler setting to prevent unintentional time-out */
  WDTCSR |= (1<<WDCE) | (1<<WDE);
  /* Turn off WDT */
  WDTCSR = 0x00;
  sei();
}


// wdt int service routine
ISR(WDT_vect) {
  Sleep::pSleep->WDT_Off();
  Sleep::pSleep->isrcalled=1;
}
