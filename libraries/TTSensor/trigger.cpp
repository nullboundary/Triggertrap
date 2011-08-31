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


 //Option Menu default
 const prog_char trigType[] PROGMEM="type";
 const prog_char trigDelay[] PROGMEM="delay";
const prog_char trigThreshold[] PROGMEM = "threshold";

const prog_char * selectMenu[] PROGMEM  = 	   //options menu
{   
trigType,
trigDelay,
trigThreshold,
};


 //Value setting titles for values that are not just numbers (ex. type option)
 const prog_char onMake[] PROGMEM="On Make";
 const prog_char onBreak[] PROGMEM="On Break";	
 const prog_char onChange[] PROGMEM="On Change";
 const prog_char updown[] PROGMEM="^v";	

 const prog_char * settingMenu[] PROGMEM  = 	   // select menu options
{   
onMake,onBreak,onChange,
updown
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
  int Trigger::select()
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
	select_ = select_ % 3;  //TODO hard coded needs to be set somewhere..
  }

/***********************************************************
 * 
 * option
 *
 * 
 * 
 ***********************************************************/
const int Trigger::option(int menuOption)
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
void Trigger::setOption(int menuOption,int value)
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
void Trigger::incOption(int menuOption, int maxValue)
{
	int mOpt = optionValues[menuOption];
	mOpt++;
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
void Trigger::decOption(int menuOption, int maxValue)
{
	int mOpt = optionValues[menuOption];
	mOpt--;
	if(mOpt < 0) { mOpt = maxValue; }  //limits
	optionValues[menuOption] = mOpt;
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
void Trigger::shutter(boolean noDelay)
{
	resetShutter(); //make the shutter close after 10ms delay
	
	if(shutterReady == true) //trigger() set this to ready
	{
	
		int currentTime = millis()/1000;
		int delaySec = delayCount/1000;
		int elapsed = currentTime - delaySec;

	    //ready, but need to wait for delay timer, unless noDelay is true, then skip the delay
		if(elapsed > option(TRIG_DELAY) || noDelay == true) 
		{
			shotCounter_++; 
			shutterReady = false; 
	
			if(cameraA_ == true) //use cameraA?
			{
				shutterDelay = millis();
				shutterStateA_ = true;
				digitalWrite(CAMERA_TRIGGER_A,LOW); //trigger camera
		
				#ifdef SERIAL_DEBUG
				Serial.println("CAMERA TRIGGER A");
				#endif
			}
	
			if(cameraB_ == true) //or use CameraB?
			{
				shutterDelay = millis(); 
				shutterStateB_ == true;
			 	digitalWrite(CAMERA_TRIGGER_B,LOW);
	
				#ifdef SERIAL_DEBUG
				Serial.println("CAMERA TRIGGER B");
				#endif
			}
		}
	}
}


/***********************************************************
 * 
 * high
 *
 * if trigger just went high, trigger the camera. (return true)
 * 
 ***********************************************************/
boolean Trigger::high()
{
    boolean changed = change();

    if(changed == true) //trigger changed state
    {
      	if(sensorLevel_ > option(TRIG_THRESHOLD)) //sesnor data being received, any value above threshold 
	      {
	        return true;
			delayCount = millis(); //start counting till delay is up
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
 * low
 *
 * if sensor just went low, trigger the camera. (return true)
 * 
 ***********************************************************/
   boolean Trigger::low()
  {
    boolean changed = change();

    if(changed == true) //sensor changed state
    {
     	 if(sensorLevel_ <= option(TRIG_THRESHOLD)) //sensor low, stopped
	      {
	        return true;
			delayCount = millis(); //start counting till delay is up
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
    int state = triggerState_; 
	int threshold = option(TRIG_THRESHOLD); //get setting option 2 threshold

    sensorLevel_ = analogRead(sensorPin_); 
	
    //state is high, or state is low, depending on change above or below threshold
    if(sensorLevel_ > threshold) 
    {
	
      state = 1; 
    }
    else if(sensorLevel_ <= threshold) 
    {
      state = 0; 
    }

    //trigger either on or off, not measuring soundValue
    if(state != triggerState_) 
    {
      triggerState_ = state;
      return true; //changed its status above or below threshold
	  delayCount = millis(); //start counting till delay is up
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
void Trigger::decSetting(char buffer[])
{

	
		switch (select_)
	    {
	     case TRIG_TYPE:
	 	  decOption(TRIG_TYPE, 2);
		  getSettingMenu(buffer); 
	      break;
	    case TRIG_DELAY:
	      decOption(TRIG_DELAY, 18600);
	      formatString(option(TRIG_DELAY),buffer);
	 	 // itoa (option(TRIG_DELAY),buffer,10);
	      break;
	    case TRIG_THRESHOLD:
	      decOption(TRIG_THRESHOLD, 1023); 
		  itoa (option(TRIG_THRESHOLD),buffer,10);
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
void Trigger::incSetting(char buffer[])
{
	
	
		switch (select_)
	    {
	     case TRIG_TYPE:
	 	  incOption(TRIG_TYPE, 2);
		  getSettingMenu(buffer); 
	      break;
	    case TRIG_DELAY:
	      incOption(TRIG_DELAY, 18600);
		  formatString(option(TRIG_DELAY),buffer);
	 	  //itoa (option(TRIG_DELAY),buffer,10);
	      break;
	    case TRIG_THRESHOLD:
	      incOption(TRIG_THRESHOLD, 1023); 
		  itoa (option(TRIG_THRESHOLD),buffer,10);
	      break;
	    default: 
	      break;
	    }
		
}

/***********************************************************
 * 
 * formatString
 *
 * 
 * 
 ***********************************************************/
void Trigger::formatString(int data, char buffer[])
{
		//TODO: optimize this, could be done more simply aka faster/smaller
		//don't use String class if you can..
		String minute =  String(data/60, DEC);
		String minUnit = String(minute + "\'");
		
		String second =  String(data%60, DEC);
		String secUnit = String(second + "\"");
		
		String final = String(minUnit + secUnit);
		final.toCharArray(buffer, final.length()+1); //need to add +1 to length..seems like bug?
	
	
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
	//reset trigger low after small delay
  if(shutterStateA_ == true || shutterStateB_ == true)
  {
	 unsigned long currentTime = millis();
	 if(currentTime - shutterDelay > 10) 
	 {
	    // save the last time you took a photo
	    shutterDelay = currentTime;
		
		#ifdef SERIAL_DEBUG
		Serial.println("clear Trigger");
		#endif
		
		shutterStateA_ = false;
		digitalWrite(CAMERA_TRIGGER_A,HIGH);
		shutterStateB_ = false; 
		digitalWrite(CAMERA_TRIGGER_B,HIGH);
	 }	
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
}

