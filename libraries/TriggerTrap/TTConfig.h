/************************************************************************************
 * 	
 * 	Name    : Triggertrap TTConfig.h
 * 	Author  : Noah Shibley, NoMi Design
 * 	Date    : July 10th 2011
 * 	Version : 0.1
 * 	Notes   : Configuration Files
 * 
 * 			  Code for Triggertrap device (TTv1) and Triggertrap Shield (TT-K)                      
 * 
 ***********************************************************************************/

#ifndef TT_CONFIG_H
#define TT_CONFIG_H

/*** CHOOSE YOUR VERSION OF THE TRIGGERTRAP ***************************************
 * 	
 * 	There are two different versions of the Triggertrap. If you are developing for 
 *  the Arduino-compatible Shield version, un-comment the  //#define TT_SHIELD 
 *  statement below. 
 * 
 *  If you are developing for the Triggertrap Device (with the touch-sensitive 
 *  buttons on the front), leave it commented.          
 * 
 ***********************************************************************************/

  //#define TT_SHIELD

/*** SERIAL DEBUGGING ***************************************************************
 * 	
 * 	If you need debugging information over the Serial interface, uncomment the below 
 *  #Define. Keep in mind that serial debugging makes the Triggertrap slower, so 
 *  remember to turn it off when you are done!
 * 
 ***********************************************************************************/

  //#define SERIAL_DEBUG 

/*** HARDWARE QA TESTING ***********************************************************
 * 	
 * 	If you need to do some QA testing to verify quickly and easily that all sensors
 *  are working, and all buttons are working, uncomment below. 
 *  
 * 
 ***********************************************************************************/

 // #define HARDWARE_TEST

/*** CHOOSE CAMERAS TO TRIGGER OVER INFRA-RED ***************************************
 * 	
 * 	The Triggertrap supports camera triggering over IR, but the IR protocol is very
 *  slow indeed. By default, the Triggertrap will transmit all the IR codes, one 
 *  after another. If your camera is the last one that is transmitted, that means a 
 *  delay of approximately 1 second. 
 * 
 *  If there are cameras you know you will never use, you can safely 'comment them 
 *  out' by adding two slashes at the beginning of the line. Cameras that are 
 * 'commented out' will be skipped by the Triggertrap - this makes triggering faster.
 * 
 ***********************************************************************************/
 #define CANON   
 #define FUJI
 #define NIKON	
 #define OLYMPUS
 #define PENTAX
 #define SONYCAM
 #define WLDC100

/*** FOCUS, SHUTTER, AND SLEEP SETTINGS ********************************************
 * 	
 * 	The Triggertrap lets you choose how long you want to send the 'trigger' or
 * 'focus' signals to your camera for. Some cameras need the shutter signal to be
 *  sent for a little bit longer than others. We've found that 150 ms is safe for 
 *  all cameras we tested with - but you can reduce this if needed. 
 * 
 * 	The 'focus' signal is sent before the 'shutter' signal. The camera will only try
 *  to focus as long as the Focus signal is sent, so if your camera takes 1 second to 
 *  focus, you may have to increase this to 1000 ms (i.e. 1 second). 
 * 
 *  The 'sleep' mode is set based on how long it takes between your last user input
 *  and the Triggertrap going into sleep mode. 
 * 
 ***********************************************************************************/
 	
 const int DEFAULT_SHUTTER_TIME = 150;   // duration of shutter signal, in ms. Default: 150 
 const int DEFAULT_FOCUS_TIME = 500;     // duration of focus signal, in ms. Default: 500 
 const unsigned int UI_SLEEP_MS = 30000; // time before device goes to sleep, in ms. Default: 30000
 
 

/*** ADVANCED SETTINGS **************************************************************
 * 	
 * 	Below, we're defining the pins we are using etc. In normal use, you shouldn't 
 *  have to change any of these, so it's probably a good idea to leave them alone
 *  unless you know what you're doing! 
 * 
 ***********************************************************************************/
    #define FIRMWARE_VERSION "0.3.30"               //The current firmware version of this TT

 #ifdef TT_SHIELD
	const byte START_BUTTON = 7;				// D7 = Digital in - Start button
	const byte FOCUS_TRIGGER_PIN = 9;		// D13 = Digital out - Camera Trigger A
    const byte SHUTTER_TRIGGER_PIN = 8;		// D12 = Digital out - Camera Trigger B
	const byte LASER_SENSOR = A3; 
	const byte AMBIENT_LIGHT_SENSOR = A3;
	const byte SOUND_IN = A5;
	const byte AUX = A0;  //Aux Pin
 #else //Normal TT
	const byte POWER_UI = PORTB6;				// A3 = Digital out - UI power mosfet switch
	const byte KEY_PAD_LEDS = PORTB7;				// D4 = Digital out - LED on keypad
	const byte START_BUTTON = 2;				// D2 = Digital in - Start button 
	const byte KEY_CHANGE = 3;				// D3 = Digital in - key change interrupt for touch ic
	const byte FOCUS_TRIGGER_PIN = 13;		// D13 = Digital out - Camera Trigger A
    const byte SHUTTER_TRIGGER_PIN = 12;		// D12 = Digital out - Camera Trigger B
	const byte LASER_SENSOR = A6; 	//Laser Sensor
	const byte AMBIENT_LIGHT_SENSOR = A0;
	const byte SOUND_IN = A7;
	const byte AUX = A2;  //Aux Pin
 #endif

 
 const byte NUM_OF_SENSORS = 5; //the number of sensor triggers



#endif //TT_CONFIG_H
