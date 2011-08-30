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

const int SOUND_IN = A6;
int const SOUND_START_MODE = 0;
int const SOUND_STOP_MODE = 1;
int const SOUND_CHANGE_MODE = 2;

//Menu Sensor Strings

   //Mode Menu Listing
   const prog_char soundMenu[] PROGMEM= {"Sound"};

  Sound::Sound() 
  {
    triggerState_ = 0; //off
 //   threshold_ = 0;
	setOption(TRIG_TYPE,0);
	setOption(1,0);
	setOption(2,0);
//	type_ = 0;
//	delay_ = 0;
	select_ = 0; 
    sensorPin_ = SOUND_IN;

 }

  int Sound::soundLevel()
  {
    return sensorLevel_;
  }

  boolean Sound::trigger()
  {
    boolean soundStatus = false;

	resetShutter(); //10 millisec delay, between high and low
	

    switch (option(TRIG_TYPE))
    {
    case SOUND_START_MODE:

      soundStatus = high();
      break;
    case SOUND_STOP_MODE:

      soundStatus = low();
      break;
    case SOUND_CHANGE_MODE:

      soundStatus = change();
      break;
    default: //no default option
      break;
    }

    return soundStatus;
  }

  //to change the behavior of the following functions for the mic, edit here. 
  //the current functional behavior is in the base trigger class.
  //Or add a new function here, to customize mic sensor
  /*
	boolean Sound::high()
   	{
   	
   	}
   	*/

  /*
	boolean Sound::low()
   	{
   	
   	}
   	*/
  /*
	boolean Sound::change()
   	{
   	
   	}
   	*/
void Sound::getModeMenu(char buffer[])
{
	 strcpy_P(buffer, soundMenu); 
}



