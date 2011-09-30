/************************************************************************************
 * 	
 * 	Name    : Trigger Trap sleep.h                         
 * 	Author  : Noah Shibley, NoMi Design                         
 * 	Date    : July 10th 2011                                    
 * 	Version : 0.1                                              
 * 	Notes   : 
 * 
 * 	 Copyright (c) 2011 NoMi Design All right reserved.
 * 
 *     
 *
 * 
 * 	This file is part of Trigger Trap.
 * 
 * 		    Trigger Trap is free software: you can redistribute it and/or modify
 * 		    it under the terms of the GNU General Public License as published by
 * 		    the Free Software Foundation, either version 3 of the License, or
 * 		    (at your option) any later version.
 * 
 * 		    Trigger Trap is distributed in the hope that it will be useful,
 * 		    but WITHOUT ANY WARRANTY; without even the implied warranty of
 * 		    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * 		    GNU General Public License for more details.
 * 
 * 		    You should have received a copy of the GNU General Public License
 * 		    along with Trigger Trap.  If not, see <http://www.gnu.org/licenses/>.
 * 
 ***********************************************************************************/

#ifndef SLEEP_H
#define SLEEP_H


#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/wdt.h>
#include <avr/power.h>
#include <WProgram.h>

const int EXT_INT_0 = 0; 
const int EXT_INT_1 = 1; 
const int WATCH_DOG_TIMER = 2;

extern "C" void sleepHandler(void) __attribute__ ((signal)); 

class Sleep {

public:

  friend void sleepHandler(void);

Sleep();
  
  /* modes of sleep
   SLEEP_MODE_IDLE 
   SLEEP_MODE_ADC
   SLEEP_MODE_PWR_SAVE
   SLEEP_MODE_EXT_STANDBY
   SLEEP_MODE_STANDBY
   SLEEP_MODE_PWR_DOWN
   */

	void idleMode() { setSleepMode(SLEEP_MODE_IDLE);}
	void adcMode() {setSleepMode(SLEEP_MODE_ADC);}
	void pwrSaveMode() {setSleepMode(SLEEP_MODE_PWR_SAVE);}
	void extStandbyMode(){setSleepMode(SLEEP_MODE_EXT_STANDBY);}
	void standbyMode(){setSleepMode(SLEEP_MODE_STANDBY);}
	void pwrDownMode(){setSleepMode(SLEEP_MODE_PWR_DOWN);}
	
	void addInterrupt0(int mode){addWakeEvent(0,mode);}
	void addInterrupt1(int mode){addWakeEvent(1,mode);}
	void addWatchDog(int interval){addWakeEvent(WATCH_DOG_TIMER,interval);}

  
  void setSleepMode(int mode);
  void addWakeEvent(int interrupt,int intervalOrMode=0);
  void setWakeInterval(int interval);
  void sleepNow(int mode, int wakeEvent, int interval=0);
  void sleepNow();


private:

  int sleepMode_;              
  boolean extInt0_;
  boolean extInt1_;
  boolean watchDogTimeOut_;
  int interruptMode_; //Rising,Falling,LOW, HIGH

  void sleep_watchDog();

  void sleep_external();


};

#endif




