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

#ifndef LASER_H
#define LASER_H

//#include "globals.h"
#include "trigger.h"
#include <WProgram.h>

class Laser : public Trigger{

public:


	Laser();
	//virtual ~Laser();
	

	int sensorLevel();
	

	boolean trigger();
 
 //to change the behavior of these functions for laser, edit here
  //Or add a new function here, to customize laser sensor
  /*
	boolean start()
  
   */

  /*
	boolean getStop()
  
   	*/
  /*
	boolean change()
   	
  */

	void incSetting(char buffer[], int inc);
	
	void decSetting(char buffer[], int inc);

	void getModeMenu(char buffer[]);

	void getActiveMessage(char buffer[]);

	void getSelectMenu(char buffer[]);

	void getSettingMenu(char buffer[]);

private:
	

  
/*
  int sensorState_; //On or OFF, based on above or below the threshold
  int type_; //trigger on START,STOP or CHANGE
  int select_; //select type delay threshold
  int threshold_; //value is the change threshold, the pivot between On and OFF
  int sensorLevel_; //incoming sensor value
  int sensorPin_; //the sensor pin
  int delay_; //time delay after sensor triggered
  int max_delay_;
*/
};

#endif

