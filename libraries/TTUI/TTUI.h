/************************************************************************************
 * 	
 * 	Name    : Trigger Trap touchUI.h                        
 * 	Author  : Noah Shibley, NoMi Design                         
 * 	Date    : July 10th 2011                                    
 * 	Version : 0.1                                              
 * 	Notes   :  All the interface stuff for trigger trap, buttons and screen menus                 
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

#ifndef TTTUI_H
#define TTUI_H

#include <LiquidCrystalFast.h>
#include <AtTouch.h>
#include <TTSensor.h>
#include <WProgram.h>

//#define SERIAL_DEBUG  //do you want status messages printed to serial? 

byte const MODE_BTTN = 2; //mode bttn address
byte const SELECT_BTTN = 3;//select bttn address
byte const UP_BTTN = 6; //up bttn address
byte const DOWN_BTTN = 5;  //down bttn address

const byte POWER_UI = PORTB6;				// A3 = Digital out - UI power mosfet switch
const byte KEY_PAD_LEDS = PORTB7;				// D4 = Digital out - LED on keypad
const byte START_BUTTON = 2;				// D2 = Digital in - Start button 
const byte KEY_CHANGE = 3;				// D3 = Digital in - key change interrupt for touch ic

const int LCD_CONTRAST = 60; 
const unsigned int UI_SLEEP_MS = 30000; //time before leds and screen turn off, with no activity

extern "C" void startHandler(void) __attribute__ ((signal));  //ISR function for interrupt 

class TTUI: public LiquidCrystalFast {


public:


	TTUI();

friend void startHandler(void); //make the ISR routine a friend of this class

/***********************************************************
 * 
 * begin
 *
 * pass every Trigger object to the UI
 * 
 ***********************************************************/
void setup(Trigger& laser, Trigger& sound, Trigger& light, Trigger& timeLapse, Trigger& aux );

/***********************************************************
 * 
 * update
 *
 * does all UI stuff necessary each loop
 * 
 ***********************************************************/
void update();

/***********************************************************
 * 
 * updateActive
 *
 * handles update when the trap is active
 * 
 ***********************************************************/
void updateActive();

/***********************************************************
 * 
 * trigger
 *
 * returns which trigger object has been selected by the UI (mode button)
 * 
 ***********************************************************/
int trigger(){ return (int) currentTrigger; }

/***********************************************************
 * 
 * trapActive
 *
 * returns the status of whether or not the start button has been pressed
 * 
 ***********************************************************/
boolean trapActive() { return trapActive_; }



private:
	
	private: 
		
	  volatile boolean trapActive_;	//start button pressed
	  byte currentTrigger;   //which trigger (mode) is active
	  static TTUI* pTTUI; //ptr to TTUI class for the ISR

	  AtTouch touch;  //touch sensor library object
	
	  Trigger *triggers[5]; //array of trigger object pointers
	
	  //char lcdBufferLine1[9];
	  //char lcdBufferLine2[9];

	  int activeRefreshTime;
	  unsigned long 	previousMillis_UIPower; //time counter for powering down the leds and screen
	  
	  boolean state_UIPower;	//UI power on, or off flag
	  volatile unsigned long prevIntTime;  //debounce
 
/***********************************************************
 * 
 * bttnMode
 *
 * increments the Mode to the next trigger object
 * 
 ***********************************************************/
	void bttnMode();  
	
/***********************************************************
 * 
 * bttnSelect
 *
 * increments the Select menu to the next select option
 * 
 ***********************************************************/
	void bttnSelect();

/***********************************************************
 * 
 * bttnUp
 *
 * increments the selected option value
 * 
 ***********************************************************/
	void bttnUp(boolean hold);

/***********************************************************
 * 
 * bttnDown
 *
 * decrements the selected option value
 * 
 ***********************************************************/
	void bttnDown(boolean hold);

/***********************************************************
 * 
 * uiPowerOn
 *
 * Turn on power to the keys and the LCD screen
 * 
 ***********************************************************/	
	void uiPowerOn();
	
/***********************************************************
 * 
 * uiPowerOff
 *
 * Turn off power to the keys and the LCD screen
 * 
 ***********************************************************/	
	void uiPowerOff();	
 
/***********************************************************
 * 
 * uiPowerTimeOut
 *
 * Turn off power to the keys and the LCD screen, after a certain time
 * 
 ***********************************************************/
	void uiPowerTimeOut();

/***********************************************************
 * 
 * saveSettings
 *
 * Save current menu setting to Eeprom 
 * 
 ***********************************************************/
	void saveSettings();

/***********************************************************
 * 
 * restoreSetting
 *
 * load current settings from Eeprom
 * 
 ***********************************************************/
	void restoreSettings();
	
void printCmp(char newBuffer[], char oldBuffer[],int line);
	/***********************************************************
	 * 
	 * initStart
	 *
	 * This is called by the start button ISR. 
	 * 
	 ***********************************************************/
	void initStart(unsigned long startTime);


};

#endif 


