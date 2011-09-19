/************************************************************************************
 * 	
 * 	Name    : Trigger Trap light.h                         
 * 	Author  : Noah Shibley, NoMi Design                         
 * 	Date    : July 10th 2011                                    
 * 	Version : 0.1                                              
 * 	Notes   : The ambient light sensor. Sub class of Sensor, inherits all Sensor functions           
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

#include "light.h"

const int AMBIENT_LIGHT_SENSOR = A0;
int const LIGHT_START_MODE = 0;
int const LIGHT_STOP_MODE = 1;
int const LIGHT_CHANGE_MODE = 2;

//Menu Sensor Strings

   //Mode Menu Listing
   const prog_char lightMenu[] PROGMEM= {"Light"};

  Light::Light() 
  {
    triggerState_ = 0; //off
	setOption(TRIG_TYPE,0);
	setOption(1,0);
	setOption(2,0);
 //   threshold_ = 0;
//	delay_ = 0;
//	type_ = 0;
	select_ = 0; 
    sensorPin_ = AMBIENT_LIGHT_SENSOR;

};
 
  

  int Light::sensorLevel()
  {
    sensorLevel_ = analogRead(sensorPin_) >> 2;
	return sensorLevel_;
  }

  boolean Light::trigger()
  {
    boolean lightStatus = false;

	resetShutter(); //10 millisec delay, between high and low
	

    switch (option(TRIG_TYPE))
    {
    case LIGHT_START_MODE:

      lightStatus = high();
      break;
    case LIGHT_STOP_MODE:

      lightStatus = low();
      break;
    case LIGHT_CHANGE_MODE:

      lightStatus = change();
      break;
    default: //no default option
      break;
    }

	if(lightStatus == true)
    {
		delayCount = millis(); //start counting till delay is up
		shutterReady = true;
		return lightStatus;
	}	
	else
	{
    	 return lightStatus;
	}
   
  }

  //To change the behavior of these functions for the light sensor, edit here
  //Or add a new function here, to customize light sensor
  /*
	boolean Light::high()
   	{
   	
   	}
   	*/

  /*
	boolean Light::low()
   	{
   	
   	}
   	*/
  /*
	boolean Light::change()
   	{
   	
   	}
   	*/
	void Light::getModeMenu(char buffer[])
  {
	 strcpy_P(buffer, lightMenu); 

  }
