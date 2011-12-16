/************************************************************************************
 * 	
 * 	Name    : Trigger Trap auxiliary.h                         
 * 	Author  : Noah Shibley, NoMi Design                         
 * 	Date    : July 10th 2011                                    
 * 	Version : 0.1                                              
 * 	Notes   : The aux sensor port. Sub class of Trigger, inherits all Trigger functions           
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

#ifndef AUXILIARY_H
#define AUXILIARY_H


#include "utility/trigger.h"
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"    
#else
#include "WProgram.h"
#endif

class Aux : public Trigger{

public:


	Aux();

	int sensorLevel();

	boolean trigger();
	
	void setThreshold(int threshold){ setOption(2,threshold); } //TODO change 2 to something more clear, define
		
  //To change the behavior of these functions for the light sensor, edit here
  //Or add a new function here, to customize light sensor
  /*
	boolean start();
   	*/

  /*
	boolean getStop();
   	*/
  /*
	boolean change();
   	*/


	void getModeMenu(char buffer[]);
	
	void getActiveMessage(char buffer[]);

//	void getOptionMenu(char buffer[]);

//	void getSettingMenu(char buffer[]);


private:



};

#endif

