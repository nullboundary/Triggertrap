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

#include "auxiliary.h"

const int AUX = A2;  //Aux Pin
int const AUX_START_MODE = 0;
int const AUX_STOP_MODE = 1;
int const AUX_CHANGE_MODE = 2;

//Mode Menu Listing
const prog_char auxMenu[] PROGMEM= {"Aux"};

 //Option Menu titles
const prog_char trigType[] PROGMEM="type";
const prog_char trigDelay[] PROGMEM="delay";
const prog_char trigThreshold[] PROGMEM = "threshold";


  Aux::Aux() 
  {
    triggerState_ = 0; //off
	setOption(TRIG_TYPE,0);
	setOption(1,0);
	setOption(2,0);

	select_ = 0; 
    sensorPin_ = AUX;

};
 
  

  int Aux::auxLevel()
  {
    return sensorLevel_;
  }


  boolean Aux::trigger()
  {
    boolean auxStatus = false;

	shutter(); 

    switch (option(TRIG_TYPE))
    {
    case AUX_START_MODE:

      auxStatus = high();
      break;
    case AUX_STOP_MODE:

      auxStatus = low();
      break;
    case AUX_CHANGE_MODE:

      auxStatus = change();
      break;
    default: //no default option
      break;
    }

     if(auxStatus == true)
    {
		delayCount = millis(); //start counting till delay is up
		shutterReady = true;
		return auxStatus;
	}	
	else
	{
    	return auxStatus;
	}
  }

  //To change the behavior of these functions for the aux sensor, edit here
  //Or add a new function here, to customize aux sensor
  /*
	boolean Aux::high()
   	{
   	
   	}
   	*/

  /*
	boolean Aux::low()
   	{
   	
   	}
   	*/
  /*
	boolean Aux::change()
   	{
   	
   	}
   	*/
	void Aux::getModeMenu(char buffer[])
  {
	 strcpy_P(buffer, auxMenu); 

  }
