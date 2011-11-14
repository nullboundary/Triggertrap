/************************************************************************************
 * 	
 * 	Name    : Trigger Trap light.h                         
 * 	Author  : Noah Shibley, NoMi Design                         
 * 	Date    : July 10th 2011                                    
 * 	Version : 0.1                                              
 * 	Notes   : Timelapse interval trigger          
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

#ifndef TIMELAPSE_H
#define TIMELAPSE_H

//#include "globals.h"
#include "trigger.h"
#include <WProgram.h>
#include <TTSleep.h>

class TimeLapse : public Trigger{

public:


	TimeLapse();

/***********************************************************
 * 
 * trigger
 *
 *  returns true or false based on whether its time to take a picture
 * 
 ***********************************************************/
boolean trigger();
	
boolean delayFirstShot();

boolean batteryPower();
	
/***********************************************************
 * 
 * countDown
 *
 *  returns remaining time till trigger
 * 
 ***********************************************************/
	unsigned long countDown(); 
	
	int countDownInt();

/***********************************************************
 * 
 * incSetting
 *
 *  increment the value of the selected option setting
 * 
 ***********************************************************/
	void incSetting(char buffer[],int inc);

/***********************************************************
 * 
 * decSetting
 *
 *  decrement the value of the selected option setting
 * 
 ***********************************************************/
	void decSetting(char buffer[],int dec);
	
/***********************************************************
 * 
 * getModeMenu
 *
 *  Gets the Mode menu title for this trigger 
 * 
 ***********************************************************/	
	void getModeMenu(char buffer[]);

 /***********************************************************
 * 
 * getSelectMenu
 *
 *  Overrides the getSelectMenu in trigger with new menu set, for different options
 * 
 ***********************************************************/
	void getSelectMenu(char buffer[]);
	
	void getActiveMessage(char buffer[]);

private:

/***********************************************************
 * 
 * startTimer
 *
 *  sets the start time when the start button is pressed
 * 
 ***********************************************************/
	void startTimer();

void sleepNow(unsigned long elapsedTime);

	//  Sleep *sleep;

   	Sleep sleep;
 


};

#endif

