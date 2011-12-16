/************************************************************************************
 * 	
 * 	Name    : Trigger Trap laser.h                         
 * 	Author  : Noah Shibley, NoMi Design                         
 * 	Date    : July 10th 2011                                    
 * 	Version : 0.1                                              
 * 	Notes   : The laser break beam sensor. Sub class of Sensor, inherits all Sensor functions           
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

#include "laser.h"


int const MAKE_LASER_MODE = 0;
int const BREAK_LASER_MODE = 1;
int const CHANGE_LASER_MODE = 2;

   //Menu Sensor Strings

   //Mode Menu Listing
   const prog_char laserMenu[] PROGMEM= {"Laser"};

	 //Option Menu default
	const prog_char laserType[] PROGMEM = "type";
	const prog_char laserDelay[] PROGMEM="delay";

	const prog_char * laserOptionMenu[] PROGMEM  = 	   //options menu
	{   
	laserType,	
	laserDelay,
	};


 const prog_char onMake[] PROGMEM="On Make";
 const prog_char onBreak[] PROGMEM="On Break";	
 const prog_char onChange[] PROGMEM="On Chnge";


const prog_char * laserSettingMenu[] PROGMEM  = 	   // setting menu options
{   
	onMake,onBreak,onChange
};

const int LASER_TYPE = 0;
const int LASER_DELAY = 1;
const int LASER_THRESHOLD = 2; 

  Laser::Laser() 
  {
	maxOptionMenu = 2; 
    triggerState_ = false; //off
	abortTrigger = false; 
	setOption(LASER_TYPE,0);
	setOption(LASER_DELAY,0);
	setOption(LASER_THRESHOLD,128);
	select_ = 0; 
    sensorPin_ = LASER_SENSOR;
	focusPulseTime_ = DEFAULT_FOCUS_TIME;
	shutterPulseTime_ = DEFAULT_SHUTTER_TIME;
	focusArmed = false; //focus on/off default
	shutterArmed = true; //shutter on/off default
	IRShutter_ = false;  //IR on/off default
	
  }

  int Laser::sensorLevel()
  {
	  sensorLevel_ = analogRead(sensorPin_) >> 2;
	  return sensorLevel_;
  }

  boolean Laser::trigger()
  {
	
	
	setOption(LASER_THRESHOLD,128); //just set laser to always be 128. Its really the safest value
   
 	boolean laserStatus = false;

	shutter(true); 


    switch (option(LASER_TYPE))
    {
    case MAKE_LASER_MODE:

      laserStatus = rise();
      break;
    case BREAK_LASER_MODE:

      laserStatus = fall();
      break;
    case CHANGE_LASER_MODE:

      laserStatus = change();
      break;
    default: //no default option
      break;
    }

    if(laserStatus == true)
    {
		delayCount = millis(); //start counting till delay is up
		focusReady = true; 
		shutterReady = true;
		IRReady = true; 
		return laserStatus;
	}	
	else
	{
    	return laserStatus;
	}

  }

  //to change the behavior of these functions for laser, edit here
  //Or add a new function here, to customize laser sensor
  /*
	boolean high()
   	{
   	
   	}
   	*/

  /*
	boolean low()
   	{
   	
   	}
   	*/
  /*
	boolean change()
   	{
   	
   	}
   	*/

/***********************************************************
 * 
 * decSetting
 *
 * 
 * 
 ***********************************************************/
void Laser::decSetting(char buffer[], int dec)
{

	
		switch (select_)
	    {
	     case LASER_TYPE:
	 	  decOption(LASER_TYPE, 2,dec);
		  getSettingMenu(buffer); 
	      break;
	    case LASER_DELAY:
	      decOption(LASER_DELAY, 9999,dec);
	      if(option(LASER_DELAY) == 0) //delay 0 is off
		  {
				buffer[0] = 0;
				strcat(buffer,"Off");
				strcat(buffer,"\0");
		  }
		  else
		  {
	      	formatMSString(option(LASER_DELAY),buffer);
 	  	  }
	      break;
	    default: 
	      break;
	    }

}

/***********************************************************
 * 
 * incSetting
 *
 * 
 * 
 ***********************************************************/
void Laser::incSetting(char buffer[], int inc)
{
	
	
		switch (select_)
	    {
	     case LASER_TYPE:
	 	  incOption(LASER_TYPE, 2,inc);
		  getSettingMenu(buffer); 
	      break;
	    case LASER_DELAY:
	      incOption(LASER_DELAY, 9999,inc);
		  if(option(LASER_DELAY) == 0) //delay 0 is off
		  {
				buffer[0] = 0;
				strcat(buffer,"Off");
				strcat(buffer,"\0");
		  }
		  else
		  {
		  	formatMSString(option(LASER_DELAY),buffer);
	 	  }
	      break;
	    default: 
	      break;
	    }
		
}


  

void Laser::getModeMenu(char buffer[])
{
	 strcpy_P(buffer, laserMenu); 
	
}

/***********************************************************
 * 
 * getOptionMenu
 *
 *  
 * 
 ***********************************************************/
void Laser::getOptionMenu(char buffer[])
{
	 //reads the timeSelectMenu options from flash memory
	 strcpy_P(buffer, (const prog_char *)pgm_read_word(&(laserOptionMenu[select_])));
}

void Laser::getSettingMenu(char buffer[])
{
	strcpy_P(buffer, (const prog_char *)pgm_read_word(&(laserSettingMenu[option(LASER_TYPE)]))); 
}

/***********************************************************
 * 
 * getActiveMessage
 *
 * get the current sensors LCD message to print during trap Active mode.
 * 
 ***********************************************************/
void Laser::getActiveMessage(char buffer[])
{
	buffer[0] = 0;
	
	itoa (sensorLevel(),buffer,10);
	
	
}


