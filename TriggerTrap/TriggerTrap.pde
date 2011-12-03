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
#include <TTIRremote.h>
//#include <TTSensor.h>
#include <TTUI.h>
#include <laser.h>
#include <light.h>
#include <trigger.h>
#include <sound.h>
#include <timeLapse.h>
#include <auxiliary.h>

//#define SERIAL_DEBUG //do you want to use Serial for debugging? uncomment this

const int NUM_MODES = 5;  //number of trigger modes
const int LASER_MODE = 0;
const int SOUND_MODE = 1;
const int LIGHT_MODE = 2;
const int TIMELAPSE_MODE = 3; 
const int AUX_MODE = 4;

//const int FOCUS_PULSE_TIME = 500; //500ms focus pin goes HIGH. 
//const int SHUTTER_PULSE_TIME = 50; //50ms shutter pin goes HIGH. 


//the UI object, and the sensor objects
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

  tui.setup(laser,mic,light,timeLapse,aux); //pass the sensors to the UI

  interrupts(); //make sure interrupts are on

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
      auxTrigger();
      break;

    default: //no default option.
      break;
    }
  }


}


/***********************************************************
 * 
 * laserTrigger
 *
 * 
 * 
 ***********************************************************/
void laserTrigger()
{

 // laser.setTriggerPulseTime(FOCUS_PULSE_TIME,SHUTTER_PULSE_TIME);
  laser.trigger(); //check to see if TT should take a picture

}

/***********************************************************
 * 
 * timeLapseTrigger
 *
 * 
 * 
 ***********************************************************/
void timeLapseTrigger()
{

  //timeLapse.setTriggerPulseTime(FOCUS_PULSE_TIME,SHUTTER_PULSE_TIME);
  if(timeLapse.trigger() == true)
  {
//#ifdef SERIAL_DEBUG
//    Serial.print(timeLapse.shotCount());
//#endif

  }


}
/***********************************************************
 * 
 * soundTrigger
 *
 * 
 * 
 ***********************************************************/
void soundTrigger()
{
  
 // mic.setTriggerPulseTime(FOCUS_PULSE_TIME,SHUTTER_PULSE_TIME);
  if(mic.trigger() == true) //returns true if sound changes based on current mode type
  {
#ifdef SERIAL_DEBUG
    Serial.println("Trigger");
#endif
  }

}
/***********************************************************
 * 
 * lightTrigger
 *
 * 
 * 
 ***********************************************************/
void lightTrigger()
{

  //light.setTriggerPulseTime(FOCUS_PULSE_TIME,SHUTTER_PULSE_TIME);
  if(light.trigger() == true) //returns true if sound changes based on current mode type
  {

#ifdef SERIAL_DEBUG
    Serial.println(light.sensorLevel());  
#endif

  }

}
/***********************************************************
 * 
 * auxTrigger
 *
 * 
 * 
 ***********************************************************/
void auxTrigger()
{

//  aux.setTriggerPulseTime(FOCUS_PULSE_TIME,SHUTTER_PULSE_TIME);
  aux.trigger(); //returns a boolean but you don't need to use it


}








