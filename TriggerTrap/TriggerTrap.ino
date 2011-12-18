
/************************************************************************************
 * 	
 * 	Name    : Triggertrap                          
 * 	Author  : Noah Shibley, NoMi Design                         
 * 	Date    : December 15th 2011                                    
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

#include <LiquidCrystalFast.h>
#include <I2C.h>
#include <TTSleep.h>
#include <TTIRremote.h>
#include <TriggerTrap.h>

const int LASER_MODE = 0;
const int SOUND_MODE = 1;
const int LIGHT_MODE = 2;
const int TIMELAPSE_MODE = 3; 
const int AUX_MODE = 4;
const int FLIPFLOP_MODE = 5;


TriggerTrap tt;


/***********************************************************
 * 
 * Setup
 * 
 ***********************************************************/
void setup() {   

  tt.setup();

}

/***********************************************************
 * 
 * loop
 * 
 ***********************************************************/
void loop() {

  tt.update();
  
  if(tt.trigger() == true)
  {
     if(tt.triggerMode() == SOUND_MODE)
     {
       //Serial.println(sound trigger);
     }  
    
  } // 

}










