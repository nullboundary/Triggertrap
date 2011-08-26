/************************************************************************************
 * 	
 * 	Name    : Trigger Trap globals.h                         
 * 	Author  : Noah Shibley, NoMi Design                         
 * 	Date    : July 10th 2011                                    
 * 	Version : 0.1                                              
 * 	Notes   : Where all the constants (defines) are located           
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

#ifndef GLOBALS_H
#define GLOBALS_H

#include <WProgram.h>  //Analog #defines A0,A1 etc. 

// PIN usage and setup

//const int LASER_SENSOR = A0; 			// A0 = Analong in - Laser Sensor
//const int AMBIENT_LIGHT_SENSOR = A1;	// A1 = Analong in - Ambient Light Sensor
const int AUX = A2;						// A2 = Analong in - AUX in
//const int POWER_UI = A3;				// A3 = Digital out - UI power mosfet switch
const int SDA_TOUCH = A4;				// A4 = I2C bus - SDA - touch controller  
const int SCL_TOUCH = A5;				// A5 = I2C bus - SCL - touch controller  
//const int SOUND_IN = A6;				// A6 = Analong in - Sound in    
const int BATT_VOLTS = A7;				// A7 = Analong in - Battery voltage in    

const int SERIAL_RX = 0;				// D0 = Serial RXD
const int SERIAL_TXD = 1;				// D1 = Serial TXD
//const int START_BUTTON = 2;				// D2 = Digital in - Start button 
//const int KEY_CHANGE = 3; 				// D3 = Digital in - key change interrupt for touch ic
//const int KEY_PAD_LEDS = 4;				// D4 = Digital out - LED on keypad
const int LCD_ENABLE = 5; 				// D5 = Digital out - LCD Enable 
const int LCD_DATALINE4 = 6;			// D6 = Digital out - LCD Dataline 4
const int LCD_DATALINE5 = 7;			// D7 = Digital out - LCD Dataline 5
const int LCD_DATALINE6 = 8;			// D8 = Digital out - LCD Dataline 6 
const int LCD_DATALINE7 = 9; 			// D9 = Digital out - LCD Dataline 7 
const int SPEAKER_OUT = 10;				// D10 = Digital out - Speaker
const int INFRARED_SENDER = 11;			// D11 = Digital out - Infrared sender
//const int CAMERA_TRIGGER_A = 12;		// D12 = Digital out - Camera Trigger A
//const int CAMERA_TRIGGER_B = 13;		// D13 = Digital out - Camera Trigger B

int triggerMode = 0; //which function is being used now

const int NUM_MODES = 5; //number of trigger modes
const int TIMELAPSE_MODE = 3; 
const int LASER_MODE = 0;
const int SOUND_MODE = 1;
const int LIGHT_MODE = 2;
const int AUX_MODE = 4;
const int MAN_MODE = 5;





boolean trapActive = false;

	int timeLapseInterval = 0;
	unsigned long prevTime = 2147483647;  //prevent triggering, if start button not pressed for 25 days..

unsigned long const MAX_NUM_SHOTS = 2147483647; //many more then your SD card can do
unsigned int const MAX_INTERVAL = 86400; //24 hours





// PB6 = Digital in - LCD RS
// PB7 = Digital out - LCD RW

int freq =100; 
int start =0;


#endif
