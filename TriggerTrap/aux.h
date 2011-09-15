/************************************************************************************
 * 	
 * 	Name    : Trigger Trap aux.h                         
 * 	Author  : Noah Shibley, NoMi Design                         
 * 	Date    : July 10th 2011                                    
 * 	Version : 0.1                                              
 * 	Notes   : The aux input port. Sub class of Sensor, inherits all Sensor functions 
 *         	  by default Aux should trigger the camera from any analog input sensor, 
 *			  based on the threshold set. 
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

#ifndef AUX_H
#define AUX_H

#include "globals.h"
#include "sensor.h"
#include <WProgram.h>


class Aux : 
public Sensor{

public:


  Aux()
  {
    sensorState_ = 0; //off
    threshold_ = 0; //the pivot point to measure start or stop
    sensorPin_ = AUX; //the AUX pin on the arduino

  }

  int auxLevel()
  {
    return sensorLevel_;
  }

  boolean trigger() //if true, trigger the camera. 
  {
    boolean auxStatus = false;

    switch (mode_)
    {
    case SOUND_START_MODE:

      auxStatus = start();
      break;
    case SOUND_STOP_MODE:

      auxStatus = stop();
      break;
    case SOUND_CHANGE_MODE:

      auxStatus = change();
      break;
    default: //no default option
      break;
    }

    return auxStatus;
  }

  //to change the behavior of the following functions for the aux, edit here. 
  //the current functional behavior is in the parent sensor class.
  //Or add new functions here, to customize the aux sensor
  /*
	boolean start()
   	{
   	
   	}
   	*/

  /*
	boolean stop()
   	{
   	
   	}
   	*/
  /*
	boolean change()
   	{
   	
   	}
   	*/





private:


  int sensorState_; //On or OFF, based on above or below the threshold
  int mode_; //trigger on START,STOP or CHANGE
  int threshold_; //value is the change threshold, the pivot between On and OFF
  int sensorLevel_; //incoming aux analog value
  int sensorPin_;




};

#endif

