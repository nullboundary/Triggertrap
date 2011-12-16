#ifndef TT_CONFIG_H
#define TT_CONFIG_H

  //#define TT_SHIELD
  //#define SERIAL_DEBUG //comment out if you don't want serial statements

//IR Defines. If a camera is defined the IR code will be transmitted by the IR led
 #define CANON_RC6   
 #define CANON_RC5
 #define FUJI
 #define NIKON	
 #define OLYMPUS
 #define PENTAX
 #define SONY
 #define WLDC100

 	
 const int DEFAULT_SHUTTER_TIME = 150; //The time the shutter remains open in MS
 const int DEFAULT_FOCUS_TIME = 500; //The time given to focus the camera in MS
 const unsigned int UI_SLEEP_MS = 30000; //time before leds and screen turn off, with no activity
 

//-------------------------Stuff you probably don't want to change --------------------------------

 #ifdef TT_SHIELD
	const byte START_BUTTON = 7;				// D7 = Digital in - Start button
	const int FOCUS_TRIGGER_PIN = 9;		// D13 = Digital out - Camera Trigger A
    const int SHUTTER_TRIGGER_PIN = 8;		// D12 = Digital out - Camera Trigger B
	const int LASER_SENSOR = A3; 
	const int AMBIENT_LIGHT_SENSOR = A3;
	const int SOUND_IN = A5;
	const int AUX = A0;  //Aux Pin
 #else //Normal TT
	const byte POWER_UI = PORTB6;				// A3 = Digital out - UI power mosfet switch
	const byte KEY_PAD_LEDS = PORTB7;				// D4 = Digital out - LED on keypad
	const byte START_BUTTON = 2;				// D2 = Digital in - Start button 
	const byte KEY_CHANGE = 3;				// D3 = Digital in - key change interrupt for touch ic
	const int FOCUS_TRIGGER_PIN = 13;		// D13 = Digital out - Camera Trigger A
    const int SHUTTER_TRIGGER_PIN = 12;		// D12 = Digital out - Camera Trigger B
	const int LASER_SENSOR = A6; 	//Laser Sensor
	const int AMBIENT_LIGHT_SENSOR = A0;
	const int SOUND_IN = A7;
	const int AUX = A2;  //Aux Pin
 #endif

 
 const byte NUM_OF_SENSORS = 5; //the number of sensor triggers



#endif //TT_CONFIG_H
