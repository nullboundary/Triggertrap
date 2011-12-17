/************************************************************************************
 * 	
 * 	Name    : Triggertrap TriggerTrap.cpp                        
 * 	Author  : Noah Shibley, NoMi Design                         
 * 	Date    : July 10th 2011                                    
 * 	Version : 0.1                                              
 * 	Notes   : Code for Triggertrap device (TTv1) and Triggertrap Shield (TT-K)                      
 * 
 * 	Copyright (c) 2011 NoMi Design All right reserved.
 * 
 * 	This file is part of Triggertrap. See Triggertrap.com for more information.
 * 
 * 		    Triggertrap is free software: you can redistribute it and/or modify
 * 		    it under the terms of the GNU General Public License as published by
 * 		    the Free Software Foundation, either version 3 of the License, or
 * 		    (at your option) any later version.
 * 
 * 		    Triggertrap is distributed in the hope that it will be useful,
 * 		    but WITHOUT ANY WARRANTY; without even the implied warranty of
 * 		    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * 		    GNU General Public License for more details.
 * 
 * 		    You should have received a copy of the GNU General Public License
 * 		    along with Triggertrap.  If not, see <http://www.gnu.org/licenses/>.
 * 
 ***********************************************************************************/

#include "triggerTrap.h"


TriggerTrap::TriggerTrap()
{
	
}

void TriggerTrap::setup()
{
  tui.setup(laser,mic,light,timeLapse,aux); //pass the sensors to the UI
}

void TriggerTrap::update()
{
	 tui.update(); //update the UI
}

boolean TriggerTrap::trigger()
{	
	boolean triggerStatus = false;
	 
  	if(tui.trapActive() == true) //if start button pressed
    {
	   switch (tui.trigger()) //which trigger are we using.
	   {
	   case TIMELAPSE_TRIG:
		triggerStatus = timeLapse.trigger();
	   break;

	   case LASER_TRIG:
	   triggerStatus = laser.trigger();
	   break;

	   case SOUND_TRIG:
	   triggerStatus = mic.trigger();
	   break;

	   case LIGHT_TRIG:
		triggerStatus = light.trigger();
	   break;  

	   case AUX_TRIG:
	   triggerStatus = aux.trigger();
	   break;

	   default: //no default option.
	   break;
	   }
	}
	
	return triggerStatus;
	
}

int TriggerTrap::triggerMode()
{
	return tui.trigger();	
}



