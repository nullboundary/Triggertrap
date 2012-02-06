/************************************************************************************
 * 	
 * 	Name    : Trigger Trap                          
 * 	Author  : Noah Shibley, NoMi Design                         
 * 	Date    : July 10th 2011                                    
 * 	Version : 0.1                                              
 * 	Notes   : Boxed Vanilla Version                      
 * 
 * 	Copyright (c) 2011 NoMi Design.
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
#include <I2C.h>
#include <PinChangeInt.h>
#include <PinChangeIntConfig.h>
#include <LCD.h>
#include <LiquidCrystal.h>
#include <LiquidCrystal_SR_LCD3.h>
#include <AtTouch.h>
#include <TTSleep.h>
#include <TTIRremote.h>
#include <TriggerTrap.h>




const int LASER_MODE = 0;
const int SOUND_MODE = 1;
const int LIGHT_MODE = 2;
const int TIMELAPSE_MODE = 3; 
const int AUX_MODE = 4;


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
  
  if(tt.trigger())
  {
    
     if(tt.triggerMode() == SOUND_MODE)
     {
       //Serial.println("sound trigger");
     }
     else if(tt.triggerMode() == LASER_MODE)
     {
       //Serial.println("laser trigger");
     }  
     else if(tt.triggerMode() == LIGHT_MODE)
     {
       //Serial.println("light trigger");
     }  
     else if(tt.triggerMode() == TIMELAPSE_MODE)
     {
       //Serial.println("timelapse trigger");
     }   
     else if(tt.triggerMode() == AUX_MODE)
     {
       //Serial.println("aux trigger");
     }    
    
    
    
  }

}









