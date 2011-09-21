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

#define SERIAL_DEBUG

#include <LiquidCrystalFast.h>
#include <Wire.h>
#include <AtTouch.h>
#include <Sleep.h>
#include "globals.h"  //where all the pin name defines and variables are declared
//#include <TTSensor.h>
#include <TTUI.h>
#include <laser.h>
#include <light.h>
#include <trigger.h>
#include <sound.h>
#include <timeLapse.h>
#include <auxiliary.h>



TTUI tui;
Sleep sleep;
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

  Serial.begin(9600);  
  analogReference(INTERNAL);

 // pinMode(INFRARED_SENDER, OUTPUT);     // infrared sender
//  pinMode(CAMERA_TRIGGER_A, OUTPUT);     // Camera Trigger A
//  pinMode(CAMERA_TRIGGER_B, OUTPUT);     // Camera Trigger B
//  digitalWrite(CAMERA_TRIGGER_A,HIGH);
  
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
	int currentMode = laser.select();
	laser.setShutters(false,true); //no focus, only shutter
	laser.trigger();
	
	
}

void timeLapseTrigger()
{
 
  timeLapse.setShutters(true,false);
  if(timeLapse.trigger() == true)
  {
    
    Serial.print(timeLapse.shotCount());

   
  }
  else
  {
     //print remaining seconds, count down to shutter 
     Serial.println(timeLapse.countDown());
  }


}

void soundTrigger()
{
        mic.setShutters(true,false); //no focus, only shutter
	if(mic.trigger() == true) //returns true if sound changes based on current mode type
	{
                Serial.println("Focus");
	}

}

void lightTrigger()
{
        //Serial.println(light.lightLevel());   
       // light.setThreshold(800);
        light.setShutters(true,false); //no focus, only shutter
	if(light.trigger() == true) //returns true if sound changes based on current mode type
	{

		//digitalWrite(CAMERA_TRIGGER_A,HIGH); //trigger camera
                Serial.println(light.sensorLevel());  
                //Serial.println("CAMERA TRIGGER A");
	}

}





