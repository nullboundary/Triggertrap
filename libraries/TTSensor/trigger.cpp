/************************************************************************************
 * 	
 * 	Name    : Trigger Trap trigger.h                        
 * 	Author  : Noah Shibley, NoMi Design nomidesign.net                       
 * 	Date    : July 10th 2011                                    
 * 	Version : 0.1                                              
 * 	Notes   :  The base class for all sensors. 
 *			   DO NOT EDIT - EDITING WILL CHANGE ALL SENSORS BEHAVIOR
 *			   ,inherit instead to a sub class and change there.            
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

#include "trigger.h"

//default active status message
const prog_char activeMessage[] PROGMEM= {"Armed"};

 //Option Menu default
const prog_char trigThreshold[] PROGMEM = "threshld";
const prog_char trigDelay[] PROGMEM="delay";
const prog_char trigType[] PROGMEM="type";

const prog_char * selectMenu[] PROGMEM  = 	   //options menu
{   
trigThreshold,	
trigDelay,
trigType,
};


 //Value setting titles for values that are not just numbers (ex. type option)
const prog_char onRise[] PROGMEM="On Rise";
const prog_char onFall[] PROGMEM="On Fall";	
const prog_char onChange[] PROGMEM="On Chnge";


const prog_char * settingMenu[] PROGMEM  = 	   // select menu options
{   
onRise,onFall,onChange
};


/***********************************************************
 * 
 * setSelect
 *
 *  
 *   
 ***********************************************************/
  void Trigger::setSelect(int select)
  {
    select_ = select; 

  }
/***********************************************************
 * 
 * select
 *
 * 
 *   
 ***********************************************************/
  const int Trigger::select()
  {
    return select_;
  }

/***********************************************************
 * 
 * incSelect
 *
 * 
 * 
 ***********************************************************/
  void Trigger::incSelect()
  {
	select_++;
	select_ = select_ % maxOptionMenu;  //TODO hard coded needs to be set somewhere..
  }

/***********************************************************
 * 
 * option
 *
 * 
 * 
 ***********************************************************/
const unsigned int Trigger::option(int menuOption)
{
	return optionValues[menuOption];
}

/***********************************************************
 * 
 * setOption
 *
 * 
 * 
 ***********************************************************/
void Trigger::setOption(int menuOption,unsigned int value)
{
	optionValues[menuOption] = value;
}

/***********************************************************
 * 
 * incOption
 *
 * 
 * 
 ***********************************************************/
void Trigger::incOption(int menuOption, unsigned int maxValue, int inc)
{
	unsigned int mOpt = optionValues[menuOption];
	mOpt=mOpt+inc;
	if(mOpt > maxValue) { mOpt = 0; } //limits

	optionValues[menuOption] = mOpt;
}

/***********************************************************
 * 
 * decOption
 *
 * 
 * 
 ***********************************************************/
void Trigger::decOption(int menuOption, unsigned int maxValue, int dec)
{
	unsigned int mOpt = optionValues[menuOption];
	mOpt=mOpt-dec;
	if(mOpt > maxValue) { mOpt = maxValue; }  //limits, unsigned so < 0 is 65535

	optionValues[menuOption] = mOpt;
}

void Trigger::setShutters(boolean cameraA, boolean cameraB, boolean IRShutter, int shutterPulseTime) 
{
	cameraA_ = cameraA;
	cameraB_ = cameraB;
	IRShutter_ = IRShutter;
	shutterPulseTime_ = shutterPulseTime;
}

/***********************************************************
 * 
 * trigger
 *
 * pure virtual, must override in sub class!
 * 
 
 boolean Trigger::trigger()
 {


 }
***********************************************************/


/***********************************************************
 * 
 * shutter
 *
 * 
 * 
 ***********************************************************/
void Trigger::shutter(boolean delayActive,boolean delayUnitMs)
{
	int elapsed;
	shutterPulseTime_ = SHUTTER_PULSE_TIME;
	
	resetShutter(); //make the shutter close after 50ms delay
	
	//don't take a picture until its done taking a picture already!
	if(shutterStateA_ == true || shutterStateB_ == true)
	{
		shutterReady = false;
	}
	
	if(shutterReady == true) //trigger() set this to ready
	{
	
		if(delayUnitMs)
		{
				unsigned long currentTime = millis();
				unsigned long delaySec = delayCount;
				elapsed = (int) (currentTime - delaySec);
		}
		else
		{
			int currentTime = millis()/1000;
			int delaySec = delayCount/1000;
			elapsed = currentTime - delaySec;
		}	
		
	    //ready, but need to wait for delay timer, unless delayActive is false, then skip the delay
		if(elapsed > option(TRIG_DELAY) || delayActive == false) 
		{
			shotCounter_++; 
			shutterReady = false; 
			
		
	
			if(cameraA_ == true) //use cameraA?
			{
				PORTB |= (1<<PORTB7);		    //digitalWrite(KEY_PAD_LEDS,HIGH); turn on keypad LEDs
				//focusDelay = millis();
				shutterStateA_ = true;
				digitalWrite(CAMERA_TRIGGER_A,LOW); //trigger camera
					
				delay(shutterPulseTime_); //if you are going to use focus you need to delay before the shutter anyway.
				digitalWrite(CAMERA_TRIGGER_A,HIGH);
				PORTB &= ~ (1<<PORTB7);        //digitalWrite(KEY_PAD_LEDS,LOW); //turn off led
				shutterStateA_ = false;
			
				
				#ifdef SERIAL_DEBUG
				Serial.println("Focus");
				#endif
			
			}
	
			if(cameraB_ == true) //or use CameraB?
			{
				
				PORTB |= (1<<PORTB7);		    //digitalWrite(KEY_PAD_LEDS,HIGH); turn on keypad LEDs
				shutterDelay = millis(); 
				shutterStateB_ = true;
			 	digitalWrite(CAMERA_TRIGGER_B,LOW);
	
				
	
				#ifdef SERIAL_DEBUG
				Serial.println("Shutter");
				#endif
			}
			
			if(IRShutter_ == true)
			{
				PORTB |= (1<<PORTB7);		    //digitalWrite(KEY_PAD_LEDS,HIGH); turn on keypad LEDs
				IRTransmit();
				PORTB &= ~ (1<<PORTB7);        //digitalWrite(KEY_PAD_LEDS,LOW); //turn off led
			}
			
			
		}
	}
}


/***********************************************************
 * 
 * rise
 *
 * if trigger just went high, trigger the camera. (return true)
 * 
 ***********************************************************/
boolean Trigger::rise()
{
    boolean changed = change();

    if(changed == true) //trigger changed state
    {
      	if(sensorLevel_ > option(TRIG_THRESHOLD)) //sesnor data being received, any value above threshold 
	      {
	        return true;
			
	      }
	      else //sensor, 0 value
	      {
	        return false;
	      }
    }
    else //sensor didn't change state, still high or low...
    {
      return false; 
    }

}

/***********************************************************
 * 
 * fall
 *
 * if sensor just went low, trigger the camera. (return true)
 * 
 ***********************************************************/
   boolean Trigger::fall()
  {
    boolean changed = change();

    if(changed == true) //sensor changed state
    {
     	 if(sensorLevel_ <= option(TRIG_THRESHOLD)) //sensor low, stopped
	      {
	        return true;
			
	      }
	      else //sensor recieving data
	      {
	        return false;
	      }
    }
    else //sensor didn't change state
    {
      return false; 
    }

  }

/***********************************************************
 * 
 * change
 *
 * if sensor changed state (high/low), trigger the camera. (return true)
 * 
 ***********************************************************/
boolean Trigger::change()
{
    boolean state = triggerState_; 
	int threshold = option(TRIG_THRESHOLD); //get setting option 2 threshold

	sensorLevel_ = analogRead(sensorPin_) >> 2; //shift 1024 to 255
	
	
    //state is high, or state is low, depending on change above or below threshold
    if(sensorLevel_ > threshold)  //true above
    {
	      	state = true; 
		  
    }
    else if(sensorLevel_ <= threshold) //false below
    {
	      	state = false; 
    }

    //trigger either on or off, not measuring soundValue
    if(state != triggerState_) 
    {
      triggerState_ = state;
      return true; //changed its status above or below threshold
	  
    }
    else //nothing changing here
    {
      return false; 
    }
}

/***********************************************************
 * 
 * decSetting
 *
 * 
 * 
 ***********************************************************/
void Trigger::decSetting(char buffer[], int dec)
{

	
		switch (select_)
	    {
	     case TRIG_TYPE:
	 	  decOption(TRIG_TYPE, 2,dec);
		  getSettingMenu(buffer); 
	      break;
	    case TRIG_DELAY:
	      decOption(TRIG_DELAY, 9999,dec);
	      if(option(TRIG_DELAY) == 0) //delay 0 is infinity 
		  {
				buffer[0] = 0;
				strcat(buffer,"Off");
				strcat(buffer,"\0");
		  }
		  else
		  {
	      	formatMSString(option(TRIG_DELAY),buffer);
 	  	  }
		  //itoa (option(TRIG_DELAY),buffer,10);
	      break;
	    case TRIG_THRESHOLD:
	      decOption(TRIG_THRESHOLD, 255,dec); 
		  formatThresholdString(option(TRIG_THRESHOLD),buffer);
	      break;
	    default: 
	      break;
	    }

}

/***********************************************************
 * 
 * incSetting
 *
 * 
 * 
 ***********************************************************/
void Trigger::incSetting(char buffer[], int inc)
{
	
	
		switch (select_)
	    {
	     case TRIG_TYPE:
	 	  incOption(TRIG_TYPE, 2,inc);
		  getSettingMenu(buffer); 
	      break;
	    case TRIG_DELAY:
	      incOption(TRIG_DELAY, 9999,inc);
		  if(option(TRIG_DELAY) == 0) //delay 0 is infinity 
		  {
				buffer[0] = 0;
				strcat(buffer,"Off");
				strcat(buffer,"\0");
		  }
		  else
		  {
		  	formatMSString(option(TRIG_DELAY),buffer);
	 	  }
		  //itoa (option(TRIG_DELAY),buffer,10);
	      break;
	    case TRIG_THRESHOLD:
	      incOption(TRIG_THRESHOLD, 255,inc); 
		  formatThresholdString(option(TRIG_THRESHOLD),buffer);
	      break;
	    default: 
	      break;
	    }
		
}

/***********************************************************
 * 
 * formatTimeString
 *
 * 
 * 
 ***********************************************************/
void Trigger::formatTimeString(unsigned int data, char buffer[])
{
	//setting the first char to 0 causes str functions to think 
	//of the buffer as empty, like a clear buffer.
	buffer[0] = 0;
	char tempBuffer[5];
	//transform delay seconds into min and add to tempbuffer
	utoa (data/60,tempBuffer,10);
	//add minute data to buffer
	strcat(buffer, tempBuffer);
	//add minute symbol to buffer
	strcat(buffer,":");
	//transform delay seconds into remainder seconds
	if(data%60 < 10) strcat(buffer,"0");
	utoa(data%60,tempBuffer,10);
	//add second data to buffer
	strcat(buffer,tempBuffer);
	//add second symbol to buffer
	strcat(buffer,'\0');
		
}

void Trigger::formatMSString(unsigned int data,char buffer[])
{
	//setting the first char to 0 causes str functions to think 
	//of the buffer as empty, like a clear buffer.
	buffer[0] = 0;
	char tempBuffer[5];
	//transform delay ms into seconds
	utoa (data/1000,tempBuffer,10);
	//add second data to buffer
	strcat(buffer, tempBuffer);
	//add second delimiter symbol to buffer
	strcat(buffer,".");
	//transform delay seconds into remainder seconds
	if(data%1000 < 100) strcat(buffer,"0");
	if(data%1000 < 10) strcat(buffer,"0");
	utoa(data%1000,tempBuffer,10);
	//add second data to buffer
	strcat(buffer,tempBuffer);
	strcat(buffer,"sec");
	//add second symbol to buffer
	strcat(buffer,'\0');
}

/***********************************************************
 * 
 * formatThresholdString
 *
 * 
 * 
 ***********************************************************/
void Trigger::formatThresholdString(unsigned int data, char buffer[])
{
	buffer[0] = 0;
	char tempBuffer[5];
	int level = sensorLevel();
	
	utoa(data,tempBuffer,10);
	
	strcat(buffer,tempBuffer);
	
	strcat(buffer,"|");
	
	itoa (level,tempBuffer,10);
	
	strcat(buffer,tempBuffer);
	
	strcat(buffer,'\0');
	
}

/***********************************************************
 * 
 * resetShutter
 *
 * 
 * 
 ***********************************************************/
void Trigger::resetShutter()
{
	
	/*
  //reset trigger low after small delay
  if(shutterStateA_ == true)
  {
	 if(millis() - focusDelay > shutterPulseTime_) 
	 {
	    // save the last time you took a photo
	    focusDelay = millis();

		#ifdef SERIAL_DEBUG
		Serial.println("clear A");
		#endif

		PORTB &= ~ (1<<PORTB7);        //digitalWrite(KEY_PAD_LEDS,LOW); //turn off led
		shutterStateA_ = false;
		digitalWrite(CAMERA_TRIGGER_A,HIGH);
		
	 }	
  }		*/
  //reset trigger low after small delay
  if(shutterStateB_ == true)
  {
	 if(millis() - shutterDelay > shutterPulseTime_) 
	 {
	    // save the last time you took a photo
	    shutterDelay = millis();
		
		#ifdef SERIAL_DEBUG
		Serial.println("clear B");
		#endif
		
		
		PORTB &= ~ (1<<PORTB7);        //digitalWrite(KEY_PAD_LEDS,LOW); //turn off led
		shutterStateB_ = false; 
		digitalWrite(CAMERA_TRIGGER_B,HIGH);
	 }	
  }
}

/***********************************************************
 * 
 * IRShutter
 *
 * 
 * 
 ***********************************************************/
void Trigger::IRTransmit()
{
	irsend.sendNEC(0x61DC807F,32); //RM-2 (olympus) is probably NEC protocol
	
	unsigned int MLL3Buffer[] = {2000,27830,390,1580,410,3580,400}; //shutter from: http://www.bigmike.it/ircontrol/
	for (int i = 0; i < 2; ++i) { //send twice
		irsend.sendRaw(MLL3Buffer,7,38); //ML-L3 (Nikon), suppose to be 38.4khz
		delay(63);
	}
	
	unsigned int PentaxBuffer[] = {13000,3000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000}; //shutter
		irsend.sendRaw(PentaxBuffer,16,38); //found here, not sure which pentax, all? http://sourceforge.net/project/showfiles.php?group_id=131383
	
	//we better read this: http://www.arcfn.com/2010/12/64-bit-rc6-codes-arduino-and-xbox.html we probably have mistakes and probably need to track the toggle bit
	for (int i = 0; i < 3; ++i) {
    	irsend.sendRC6(0x0, 20); // Cannon RC-5 code for shutter, MAY work for RC-6
    	delay(100);
	}
	
	for (int i = 0; i < 3; ++i) {
		irsend.sendSony(0xB4B8F,20); // or maybe 0x7478F both found here: http://sebastian.setz.name/arduino/my-libraries/multi-camera-ir-control#comment-110
		delay(100);
	}
}

/***********************************************************
 * 
 * getSelectMenu
 *
 * 
 * 
 ***********************************************************/
  void Trigger::getSelectMenu(char buffer[])
  {
	 strcpy_P(buffer, (const prog_char *)pgm_read_word(&(selectMenu[select_])));
  }

/***********************************************************
 * 
 * getSettingMenu
 *
 * 
 * 
 ***********************************************************/
  void Trigger::getSettingMenu(char buffer[])
  {
	strcpy_P(buffer, (const prog_char *)pgm_read_word(&(settingMenu[option(TRIG_TYPE)]))); 
  }

/***********************************************************
 * 
 * getModeMenu
 *
 * 
 * 
 ***********************************************************/
  	void Trigger::getModeMenu(char buffer[])
  {
		//	strcpy_P(buffer, (char*)pgm_read_word(&(menu))); 
  }

/***********************************************************
 * 
 * getActiveMessage
 *
 * 
 * 
 ***********************************************************/
void Trigger::getActiveMessage(char buffer[])
{
	strcpy_P(buffer,activeMessage); //default active message
}

/***********************************************************
 * 
 * saveState
 *
 * 
 * 
 ***********************************************************/
void Trigger::saveState()
{
	#ifdef SERIAL_DEBUG
	Serial.println("saveState");
	#endif
	
	   //each trigger object will save to a seperate eeprom memory space
	    int objectMemoryOffset = triggerIndex*16;
	
	   eeprom_write(select_, optionSelect,objectMemoryOffset);
	   eeprom_write(optionValues[0], optionVal[0],objectMemoryOffset);
	   eeprom_write(optionValues[1], optionVal[1],objectMemoryOffset);
	   eeprom_write(optionValues[2], optionVal[2],objectMemoryOffset);
	   eeprom_write(cameraA_,cameraFocus,objectMemoryOffset);
	   eeprom_write(cameraB_,cameraShutter,objectMemoryOffset);
	   eeprom_write(IRShutter_,cameraIR,objectMemoryOffset);
	
/*	
  unsigned int optionValues[3];	
  byte select_; //trigger on START,STOP or CHANGE
  
  boolean cameraA_;  //camera A on
  boolean cameraB_; //camera B on
  
 */
}
/***********************************************************
 * 
 * restoreState
 *
 * 
 * 
 ***********************************************************/
void Trigger::restoreState()
{
	#ifdef SERIAL_DEBUG
	Serial.println("restoreState");
	#endif
	
	//each trigger object will save to a seperate eeprom memory space
	int objectMemoryOffset = triggerIndex*16;
	
	eeprom_read(select_, optionSelect,objectMemoryOffset);
	if(select_ > 3 || select_ < 0) select_ = 0;
	eeprom_read(optionValues[0], optionVal[0],objectMemoryOffset);
	eeprom_read(optionValues[1], optionVal[1],objectMemoryOffset);
	eeprom_read(optionValues[2], optionVal[2],objectMemoryOffset);
	eeprom_read(cameraA_,cameraFocus,objectMemoryOffset);
	eeprom_read(cameraB_,cameraShutter,objectMemoryOffset);
	eeprom_read(IRShutter_,cameraIR,objectMemoryOffset);
	
}

/***********************************************************
 * 
 * initState
 *
 * 
 * 
 ***********************************************************/
void Trigger::initState()
{
	#ifdef SERIAL_DEBUG
	Serial.println("initState");
	#endif
	
	//each trigger object will save to a seperate eeprom memory space
	int objectMemoryOffset = triggerIndex*16;
	
	   eeprom_write(0, optionSelect,objectMemoryOffset);
	   eeprom_write(0, optionVal[0],objectMemoryOffset);
	   eeprom_write(0, optionVal[1],objectMemoryOffset);
	   eeprom_write(0, optionVal[2],objectMemoryOffset);
	   eeprom_write(false,cameraFocus,objectMemoryOffset);
	   eeprom_write(true,cameraShutter,objectMemoryOffset);
	   eeprom_write(false,cameraIR,objectMemoryOffset);
}
/*
void eeprom_read_to(dst_p, eeprom_field, dst_size,blockOffset)
{
	eeprom_read_block(dst_p, (void *)offsetof(__eeprom_data, eeprom_field)+blockOffset, MIN(dst_size, sizeof((__eeprom_data*)0)->eeprom_field));
}

void eeprom_read(dst, eeprom_field,blockOffset) eeprom_read_to(&dst, eeprom_field, sizeof(dst),blockOffset)

void eeprom_write_from(src_p, eeprom_field, src_size,blockOffset)
{
	eeprom_write_block(src_p, (void *)offsetof(__eeprom_data, eeprom_field)+blockOffset, MIN(src_size, sizeof((__eeprom_data*)0)->eeprom_field));
}

void eeprom_write(src, eeprom_field,blockOffset) 
{ 
	typeof(src) x = src; eeprom_write_from(&x, eeprom_field, sizeof(x),blockOffset); 
}
*/

/***********************************************************
 * 
 * start
 *
 * 
 * 
 ***********************************************************/
void Trigger::start(unsigned long startTime)
{ 
	
	shotCounter_ = 0; //reset shot count. 
	startBttnTime = startTime; 
	delayCount = 0; 
	abortTrigger = false; 
}
/***********************************************************
 * 
 * stop
 *
 * 
 * 
 ***********************************************************/
boolean Trigger::stop()
{
	return abortTrigger;  
}

