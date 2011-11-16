/************************************************************************************
 * 	
 * 	Name    : Trigger Trap                          
 * 	Author  : Noah Shibley, NoMi Design                         
 * 	Date    : July 10th 2011                                    
 * 	Version : 0.1                                              
 * 	Notes   : Boxed Vanilla Version                      
 * 
 * 	Copyright (c) 2011 NoMi Design All right reserved.
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

#include <LiquidCrystalFast.h>
#include <I2C.h>
#include <AtTouch.h>
#include <TTSleep.h>
#include <IRremote.h>
//#include <TTSensor.h>
#include <TTUI.h>
#include <laser.h>
#include <light.h>
#include <trigger.h>
#include <sound.h>
#include <timeLapse.h>
#include <auxiliary.h>

//#define SERIAL_DEBUG


const int NUM_MODES = 5; //number of trigger modes
const int TIMELAPSE_MODE = 3; 
const int LASER_MODE = 0;
const int SOUND_MODE = 1;
const int LIGHT_MODE = 2;
const int AUX_MODE = 4;


TTUI tui;
Laser laser;
Light light;
Sound mic; 
Aux aux;
TimeLapse timeLapse;



/***********************************************************
 * 
 * Setup
 * 
 ***********************************************************/
void setup() {   

  #ifdef SERIAL_DEBUG
  Serial.begin(9600);  
  #endif
  
  #ifndef SERIAL_DEBUG
      // USB Serial port configure as input, disable pullup
      pinMode (0, INPUT);
      pinMode (1, INPUT);
      digitalWrite(0, LOW);
      digitalWrite(1, LOW);
  #endif
  
  analogReference(INTERNAL);

  tui.setup(laser,mic,light,timeLapse,aux); //setup touch input buttons, and lcd menus
  
  interrupts();

}

/***********************************************************
 * 
 * loop
 * 
 ***********************************************************/
void loop() {

  
  
  tui.update(); //update the UI

  if(tui.trapActive() == true) //start button pressed
  {
    
    switch (tui.trigger()) //which trigger are we using.
  {
  case TIMELAPSE_MODE:


    	timeLapseTrigger();

    break;

  case LASER_MODE:

	laserTrigger();
    break;

  case SOUND_MODE:
      soundTrigger();
      
    break;
  case LIGHT_MODE:
    lightTrigger();
  break;  

  case AUX_MODE:

    break;

  default: //no default option.
    break;
  }
  }


}



void laserTrigger()
{

	laser.setShutters(false,true); //no focus, only shutter
	laser.trigger();
	
	
}

void timeLapseTrigger()
{
  
  timeLapse.setShutters(false,true);
  if(timeLapse.trigger() == true)
  {
    #ifdef SERIAL_DEBUG
    Serial.print(timeLapse.shotCount());
    #endif
   
  }



}

void soundTrigger()
{
        mic.setShutters(true,true); //no focus, only shutter
	if(mic.trigger() == true) //returns true if sound changes based on current mode type
	{
                #ifdef SERIAL_DEBUG
                Serial.println("Focus");
                #endif
	}

}

void lightTrigger()
{
      
        light.setShutters(true,false); //no focus, only shutter
	if(light.trigger() == true) //returns true if sound changes based on current mode type
	{

                #ifdef SERIAL_DEBUG
                Serial.println(light.sensorLevel());  
                #endif

	}

}





