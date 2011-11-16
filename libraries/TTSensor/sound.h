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

#ifndef SOUND_H
#define SOUND_H

//#include "globals.h"
#include "trigger.h"
#include <WProgram.h>

class Sound : public Trigger{

public:


	Sound();

	int sensorLevel();
	
	boolean trigger();
	
  //to change the behavior of the following functions for the mic, edit here. 
  //the current functional behavior is in the parent sensor class.
  //Or add a new function here, to customize mic sensor

  /*
	boolean change();
   	*/

	void getSelectMenu(char buffer[]);
	
	void getModeMenu(char buffer[]);
	
	void getActiveMessage(char buffer[]);

private:



};

#endif

