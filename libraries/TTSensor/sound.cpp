/************************************************************************************
 * 	
 * 	Name    : Trigger Trap Sound.h                         
 * 	Author  : Noah Shibley, NoMi Design                         
 * 	Date    : July 10th 2011                                    
 * 	Version : 0.1                                              
 * 	Notes   : The sound input mic. Sub class of Sensor, inherits all Sensor functions           
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

#include "sound.h"

const int SOUND_IN = A7;

//Menu Sensor Strings

   //Mode Menu Listing
   const prog_char soundMenu[] PROGMEM= {"Sound"};
   
	 //Option Menu default
	const prog_char soundThreshold[] PROGMEM = "threshld";
	const prog_char soundDelay[] PROGMEM="delay";

	const prog_char * soundOptionMenu[] PROGMEM  = 	   //options menu
	{   
	soundThreshold,	
	soundDelay,
	};

  Sound::Sound() 
  {
	maxOptionMenu = 2;
    triggerState_ = false; //off
    abortTrigger = false; 
	setOption(TRIG_TYPE,2);
	setOption(TRIG_THRESHOLD,0);
	setOption(TRIG_DELAY,0);
	select_ = 1; 
    sensorPin_ = SOUND_IN;
	

 }

  int Sound::sensorLevel()
  {
   	sensorLevel_ = analogRead(sensorPin_) >> 2;
	return sensorLevel_;
  }

  boolean Sound::trigger()
  {
    boolean soundStatus = false;

	shutter(true);
 
    soundStatus = change();
    
    if(soundStatus == true)
    {
		delayCount = millis(); //start counting till delay is up
		startBttnTime = delayCount; //don't call millis twice, just use delayCount, same value.
		shutterReady = true;
		return soundStatus;
	}	
	else
	{
    	return soundStatus;
	}
		
  }

  //to change the behavior of the following functions for the mic, edit here. 
  //the current functional behavior is in the base trigger class.
  //Or add a new function here, to customize mic sensor
  /*
	boolean Sound::change()
   	{
   	
   	}
   	*/

/***********************************************************
 * 
 * getSelectMenu
 *
 *  
 * 
 ***********************************************************/
void Sound::getSelectMenu(char buffer[])
{
	 //reads the timeSelectMenu options from flash memory
	 strcpy_P(buffer, (const prog_char *)pgm_read_word(&(soundOptionMenu[select_])));
}

void Sound::getModeMenu(char buffer[])
{
	 strcpy_P(buffer, soundMenu); 
}

/***********************************************************
 * 
 * getActiveMessage
 *
 * get the current sensors LCD message to print during trap Active mode.
 * 
 ***********************************************************/
void Sound::getActiveMessage(char buffer[])
{
	buffer[0] = 0;
	
	itoa (sensorLevel(),buffer,10);
	
	
}



