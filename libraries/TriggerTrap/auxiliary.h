/************************************************************************************
 * 	
 * 	Name    : Triggertrap auxiliary.h                         
 * 	Author  : Noah Shibley / NoMi Design                         
 * 	Date    : July 10th 2011                                    
 * 	Version : 0.1                                              
 * 	Notes   : The aux sensor port. Sub class of Trigger, inherits all Trigger functions           
 * 
 * 			  Code for Triggertrap device (TTv1) and Triggertrap Shield (TT-k)                      
 * 
 * 	Copyright (c) 2012 NoMi Design (http://n0m1.com) All right reserved.
 * 
 * 	This file is part of Triggertrap. See Triggertrap.com for more information.
 *
 *  For more information about the Auxiliary port on the TTv1 and TT-k devices,
 *   including information about the voltages and types of signal the Aux mode
 *   expects in order to be able to function correctly,  please refer to the 
 *   Aux mode in the Triggertrap user manual. 
 *   URL: http://triggertrap.com/user-manual/#AuxMode  
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

