/************************************************************************************
 * 	
 * 	Name    : Trigger Trap trigger.h                        
 * 	Author  : Noah Shibley, NoMi Design nomidesign.net                       
 * 	Date    : July 10th 2011                                    
 * 	Version : 0.1                                              
 * 	Notes   :  The base class for all triggers. 
 *			   DO NOT EDIT - EDITING WILL CHANGE ALL SENSORS BEHAVIOR
 *			   ,inherit instead to a sub class and change there.            
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

#ifndef TRIGGER_H
#define TRIGGER_H

//#include "globals.h"
#include <WProgram.h>
#include <avr/pgmspace.h>

//most triggers use these menu options. 
const int TRIG_TYPE = 0; //Select menu option 0. 
const int TRIG_DELAY = 1;  //Select menu option 1. 
const int TRIG_THRESHOLD = 2;	 //Select menu option 2. 		

const int CAMERA_TRIGGER_A = 12;		// D12 = Digital out - Camera Trigger A
const int CAMERA_TRIGGER_B = 13;		// D13 = Digital out - Camera Trigger B

#define SERIAL_DEBUG //remove if you don't want serial statements

class Trigger {

public:


/***********************************************************
 * 
 * setSelect
 *
 * Set the select option that is active, this will override the UI menu setting. 
 *   
 ***********************************************************/
void setSelect(int select);

/***********************************************************
 * 
 * select
 *
 * return which select option is active. 
 *   
 ***********************************************************/
int select();

/***********************************************************
 * 
 * incSelect
 *
 * increment the select option that is active 
 *   
 ***********************************************************/
void incSelect();

/***********************************************************
 * 
 * option
 *
 * Return the value of a menu setting for this object. Ex, timelapse delta value
 * 
 ***********************************************************/
const unsigned int option(int menuOption);

/***********************************************************
 * 
 * setOption
 *
 * set the value of a menu option for this object. Ex. light threshold value.
 * 
 ***********************************************************/
void setOption(int menuOption,unsigned int value);

/***********************************************************
 * 
 * incOption
 *
 * increment the value of a menu option for this object +1. 
 * 
 ***********************************************************/
void incOption(int menuOption, unsigned int maxValue);

/***********************************************************
 * 
 * incOption
 *
 * decrement the value of a menu option for this object -1. 
 * 
 ***********************************************************/
void decOption(int menuOption, unsigned int maxValue);

/***********************************************************
 * 
 * shotCount
 *
 * returns how many shots where taken since the start button was activated. 
 * 
 ***********************************************************/
int shotCount(){ return shotCounter_; }


/***********************************************************
 * 
 * sensorLevel
 *
 * returns the analog level of the sensor being used, this is virtual 
 * 
 ***********************************************************/
virtual int sensorLevel() { return 0; }

/***********************************************************
 * 
 * trigger
 *
 * if returns true, camera ready to trigger, this is virtual, override in sub class. 
 * 
 ***********************************************************/
virtual boolean trigger() = 0;

/***********************************************************
 * 
 * shutter
 *
 *  Take the picture, bring shutterA and B low
 * 
 ***********************************************************/
void shutter(boolean noDelay=false);

/***********************************************************
 * 
 * high
 *
 * if trigger just went high, trigger the camera. (return true)
 * 
 ***********************************************************/
virtual boolean high();
/***********************************************************
 * 
 * low
 *
 * if trigger just went low, trigger the camera. (return true)
 * 
 ***********************************************************/
virtual boolean low();
/***********************************************************
 * 
 * change
 *
 * if trigger changed state (high/low), trigger the camera. (return true)
 * 
 ***********************************************************/
virtual boolean change();

/***********************************************************
 * 
 * incSetting
 *
 * go to the next Select menu string, pass in a char array for printing
 * 
 ***********************************************************/
virtual void incSetting(char buffer[]);

/***********************************************************
 * 
 * decSetting
 *
 * go to the previous Select menu string, pass in a char array for printing
 * 
 ***********************************************************/
virtual void decSetting(char buffer[]);

/***********************************************************
 * 
 * formatTimeString
 *
 * format seconds into minutes and seconds place in char buffer. 
 * 
 ***********************************************************/
void formatTimeString(unsigned int data, char buffer[]);

/***********************************************************
 * 
 * formatThresholdString
 *
 * format into current sensor level : setting level. 
 * 
 ***********************************************************/
void formatThresholdString(unsigned int data, char buffer[]);

/***********************************************************
 * 
 * getModeMenu
 *
 * get the current Mode menu string and place it in the char buffer for LCD or serial
 * 
 ***********************************************************/
virtual	void getModeMenu(char buffer[]);

/***********************************************************
 * 
 * getSelectMenu
 *
 * get the current select menu string and place it in the char buffer for LCD or serial
 * 
 ***********************************************************/
virtual void getSelectMenu(char buffer[]);

/***********************************************************
 * 
 * getModeMenu
 *
 * get the current Mode menu string and place it in the char buffer for LCD or serial
 * 
 ***********************************************************/
virtual void getSettingMenu(char buffer[]);

virtual void start(unsigned long startTime);

/***********************************************************
 * 
 * resetShutter
 *
 * this resets the shutter HIGH after being LOW for 10 ms. 
 * 
 ***********************************************************/
void resetShutter();
void setShutters(boolean cameraA, boolean cameraB) { cameraA_ = cameraA; cameraB_ = cameraB; }

protected: 
	
  boolean triggerState_; //On or OFF, based on above or below the threshold
  byte select_; //trigger on START,STOP or CHANGE
  byte sensorLevel_; //incoming sensor value
  unsigned int shotCounter_; // current shot since pressing the start button
  byte sensorPin_; //the sensor pin
 
  boolean shutterStateA_; //status of shutter A, high or low. Goes low for small amount of time at shot
  boolean shutterStateB_; //status of shutter B, high or low. Goes low for small amount of time at shot
  boolean cameraA_;  //camera A on
  boolean cameraB_; //camera B on
  boolean shutterReady; //trigger is ready, take a picture

  unsigned long shutterDelay; //keep track of the time the shutter has been low
  unsigned long delayCount; //when trigger is ready, start counting, till delay time is up
  unsigned long startBttnTime; //the time when the start button is pressed. 

private:
	
  unsigned int optionValues[5];
	
 

};

#endif

