/************************************************************************************
 * 	
 * 	Name    : Trigger Trap touchUI.h                        
 * 	Author  : Noah Shibley, NoMi Design                         
 * 	Date    : July 10th 2011                                    
 * 	Version : 0.1                                              
 * 	Notes   :  All the interface stuff for trigger trap, buttons and screen menus                 
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

#include <TTUI.h>

  //for the ISR
  TTUI* TTUI::pTTUI = 0; 

  //call the parent class constructor as well
  TTUI::TTUI():LiquidCrystalFast(A3,4,5, 6, 7, 8, 9) 
  {
	
	pTTUI = this;	//the ptr points to this object
	trapActive_ = false; 
  }

  /***********************************************************
   * 
   * begin
   * 
   ***********************************************************/
  void TTUI::setup(Trigger& laser, Trigger& sound, Trigger& light,Trigger& timeLapse, Trigger& aux)
  {

	//set triggers into array
	triggers[0] = &laser;
	triggers[1] = &sound;
	triggers[2] = &light;
	triggers[3] = &timeLapse;
	triggers[4] = &aux;
	
	for(int i=0;i<NUM_OF_SENSORS;++i)
	{
		triggers[i]->restoreState();
	}
	
	trapActive_ = false;
	startBttnHold = false; 	
	currentTrigger = 0;

	//configure start button
	pinMode(START_BUTTON, INPUT);       // Start Button
    digitalWrite(START_BUTTON, HIGH);   // turn on pullup resistor for Start button
	attachInterrupt(0,startDownHandler,FALLING); //trigger ISR function on start button press.
	
	//set UI Power
	 DDRB |= (1<<PORTB7);   //pinMode(KEY_PAD_LEDS, OUTPUT);      // LED on UI
	 DDRB |= (1<<PORTB6);   //pinMode(POWER_UI,OUTPUT);
	 state_UIPower = false; 
	 uiPowerOn(); //turn on power at startup
	 previousMillis_UIPower = 0; 
     touch.begin(KEY_CHANGE);  //init touch UI with key change interrupt
	 

    //restoreSettings(); //restore all menu settings from eeprom

	//LCD Stuff
	TCCR1B = TCCR1B & 0b11111000 | 0x01; //-- sets the pwm base
	
	pinMode (10, OUTPUT); // lcd contrast output 
	
	if(batteryPower() == true)
	{
		long lcdContrast = analogRead(A1);
		lcdContrast = 175 - ((lcdContrast * 5)  / 32);
		analogWrite (10, lcdContrast);
		
	}
	else
	{
		byte lcdContrast = 25; 
		analogWrite (10, lcdContrast);
		
	}
	

	//this class inherits from LCD, so call lcd functions as part of this class
    begin(8, 2);
  	// Print a message to the LCD.
  	print("Trigger");
	setCursor(0,1);
	print("Trap0v39");

    #ifdef SERIAL_DEBUG
	Serial.println("TT 0v39");
    #endif
  }

  /***********************************************************
   * 
   * checkBttnStatus
   * 
   ***********************************************************/
void TTUI::update()
{
	
	boolean keyDown = false;
	boolean hold = false; 
	int hitKeyVal = 0;
  
	if(trapActive_ == false) //waiting for UI input
	{

  	 	if(touch.hold() == true) //press and hold key pad
		{
			keyDown = true;
			hold = true;  
			hitKeyVal = touch.getKey();
		}

		if(touch.hit() == true) //just press key pad
		{ 
	   	 	keyDown = true;
		 	hitKeyVal = touch.readActiveKey(); //read which key was hit
		} 
	

		//call a specific function based on which key is pressed
	    if(keyDown == true)
	    {
		  previousMillis_UIPower = millis(); //time key was active
		  keyDown = false;
	      switch (hitKeyVal)
	      {		
	      	  case MODE_BTTN:
		        bttnMode();
		        break;
		      case SELECT_BTTN:
		        bttnSelect();		
		        break;
		      case DOWN_BTTN:
		        bttnDown(hold); 
		        break;
		      case UP_BTTN:
		        bttnUp(hold);
		        break;
		      default: //no default option, just here for compiler
		        break;
	      }

	    }
		
		if(batteryPower() == true)
		{
		 	uiPowerTimeOut(); //if there has been no activity turn off lcd power
		}
		
		resetCheck();
	
		
	}
	else if(trapActive_ == true) //start button was pressed, active
	{
		updateActive();
	}

   

}
/***********************************************************
* 
* resetCheck
* 
***********************************************************/
void TTUI::resetCheck()
{
		if(startBttnHold == true) //user is holding down the start button for reset
		{
			
				//only update LCD every 300ms
				int now = millis()/1000; //100 ms 
				int elapsed = now - holdBttnStart;

				if(elapsed > 10)  //10 sec
				{
					startBttnHold = false; //even if its still held we only want to reset once, not many times
					//clear eeprom
					//reset all sensor states to 0
					clear();
					setCursor(0,0);
					print("reset");
					setCursor(0,1);
					print("device");
					
					for(int i=0;i<NUM_OF_SENSORS;++i)
					{
						triggers[i]->initState();
					}
					
					
				}
			
		}
		else if(startBttnHold == false && trapActive_ == true )
		{
			//if(batteryPower() == true && )
						 detachInterrupt(0);
						 attachInterrupt(0,startDownHandler,FALLING); //trigger ISR function on start button press.
		}		
		
		
}

/***********************************************************
* 
* updateActive
* 
***********************************************************/
void TTUI::updateActive()
{
		if(batteryPower() == false) //USB connected
		{
			//only update LCD every 300ms
			int now = millis()/100; //100 ms 
			int elapsed = now - activeRefreshTime;
			
			if(elapsed > 3)  //300ms
			{
				activeRefreshTime = now;
				
				clear();
				
				printMode(0);
			
				char printBuffer[9];
				triggers[currentTrigger]->getActiveMessage(printBuffer);
				setCursor(0,1);
				print(printBuffer);
			}	
		}
	
}

/***********************************************************
* 
* initStart
* 
***********************************************************/
void TTUI::initStart(unsigned long startTime)
{

	trapActive_ = !trapActive_; //flip boolean status

	if(trapActive_ == true)
	{
		#ifdef SERIAL_DEBUG
		Serial.println("Active");
		#endif
		
		activeRefreshTime = startTime/100; //100ms
		triggers[currentTrigger]->saveState(); //save the values of active trigger to eeprom
		triggers[currentTrigger]->start(startTime); //set start time for the active trigger 
		uiPowerOff();
		
	}
	else if(trapActive_ == false) 
	{
	
		uiPowerOn();
		//restore screen to so current select menu and value, better to show mode+select?
		//set the value title in line 1
		clear();
		printSelect(0);
		printInc(1,0); //inc 0 so just display, don't actually increment

	}
	

			
}

/***********************************************************
* 
* bttnMode
* 
***********************************************************/
  void TTUI::bttnMode()  
  {
    
    currentTrigger+=1; //mode button has been pressed, advance the mode option to next
	currentTrigger = currentTrigger % NUM_OF_SENSORS;
	
	clear();
	printMode(0);
	printSelect(1);

  }

/***********************************************************
* 
* bttnSelect
* 
***********************************************************/
  void TTUI::bttnSelect()
  {
	char printBuffer[9];

	triggers[currentTrigger]->incSelect(); //set sensor to next select mode
	
	clear();
	printMode(0);
	printSelect(1);
	
  }

/***********************************************************
* 
* bttnUp
* 
***********************************************************/
void TTUI::bttnUp(boolean hold)
{
	int incVal = 1; 
	
	if(hold == true)
	{
		//speed up increment if held down for a long time
		unsigned long holdTime = millis() - touch.getStartTime();
		if(holdTime > 5000) { incVal = 5; } //increase after 5sec
		if(holdTime > 15000) { incVal = 10; } //increase after 15sec
	}	

	clear();
	printSelect(0);
	printInc(1,incVal);
	
}
   
/***********************************************************
* 
* bttnUp
* 
***********************************************************/
void TTUI::bttnDown(boolean hold)
{
	int decVal = 1; 
	
	if(hold == true)
	{
		//speed up increment if held down for a long time
		unsigned long holdTime = millis() - touch.getStartTime();
		if(holdTime > 10000) { decVal = 5; } //increase after 10sec
	}

	clear();
	printSelect(0);
	printDec(1,decVal);
}

/***********************************************************
* 	   
* uiPowerOn
*  
***********************************************************/
void TTUI::uiPowerOn()
{
    if(state_UIPower == false) //if ui power off
    {
          state_UIPower = true;     

          PORTB &= ~ (1<<PORTB6);    // enable Vsw power to touch IC and LCD
          delay (150);             // wait for power to stabilize and hw to start
          touch.begin(KEY_CHANGE);         //re init touch keys

          previousMillis_UIPower = millis();  //clock countdown start time

          if(onBatteryPower == false) //USB connected
          {
                 #ifdef SERIAL_DEBUG
                  Serial.println("USB");
                  #endif
          }
          else //battery power
          {

              // restart the LCD     
            LiquidCrystalFast(A3,4,5, 6, 7, 8, 9);
            begin(8,2);    
            display(); 
            // test battery level and set LCD contrast PWM
            long lcdContrast = analogRead(A1);
            lcdContrast = 175 - ((lcdContrast * 5)  / 32);
            analogWrite (10, lcdContrast);

           }
          #ifdef SERIAL_DEBUG
          Serial.println("UI On");
          #endif
    }
}

/***********************************************************
* 	   
* uiPowerOff
*  
***********************************************************/
void TTUI::uiPowerOff()
{

    if(state_UIPower == true) //power currently on
    {    
      state_UIPower = false; 
      
      detachInterrupt(1);    // disable the touch key interrupt
      I2c.end();              // stop i2c

        //if(digitalRead(0) == HIGH || digitalRead(1) == HIGH) //USB connected
        if(batteryPower() == false) //USB connected
        {
          //do nothing
        }
        else //battery power
        {

            // Prevent leakage current: Bring all pins connected to devices on Vsw to ground.
            // Shutdown LCD
            noDisplay();                 // clear output
            digitalWrite(A3,LOW);        // LCD RS
            digitalWrite(4,LOW);        // LCD RW
            digitalWrite(5,LOW);        // LCD EN
            digitalWrite(6,LOW);        // LCD DB4
            digitalWrite(7,LOW);        // LCD DB5
            digitalWrite(8,LOW);        // LCD DB6
            digitalWrite(9,LOW);        // LCD DB7
            analogWrite (10, 0);        // LCD Contrast PWM
            // Shutdown main power to LCD and Touch IC
            PORTB |= (1<<PORTB6);            // turn off Vsw_SW
            // i2c pins SDA and SCL are already input with internal pullups disabled
            // KEY_CHG interrupt is already input without pullup
        }
    
      #ifdef SERIAL_DEBUG
      Serial.println("UI Off");
      #endif
     

    }
}


/***********************************************************
* 	   
* uiPowerTimeOut
*  
***********************************************************/
  void TTUI::uiPowerTimeOut()
  {
    if(state_UIPower == true)
    {
      

      if(millis() - previousMillis_UIPower > UI_SLEEP_MS) 
      {
		uiPowerOff();
      }
    }
  }

/***********************************************************
* 
* batteryPower
* 
***********************************************************/
boolean TTUI::batteryPower()  
{
		
	  if(digitalRead(0) == HIGH || digitalRead(1) == HIGH) //USB connected
	  {
		onBatteryPower = false; 
		return false;
	  }
	  else
	  {
		onBatteryPower = true; 
		return true;
	  }
		
}

/***********************************************************
* 
* printMode
* 
***********************************************************/
void TTUI::printMode(int row)
{
	char printBuffer[9];
	triggers[currentTrigger]->getModeMenu(printBuffer); //load printBuffer with string to print
	setCursor(0,row);
	print(printBuffer);
	
	#ifdef SERIAL_DEBUG
	Serial.println(printBuffer);
	#endif
}

/***********************************************************
* 
* printSelect
* 
***********************************************************/
void TTUI::printSelect(int row)
{
		char printBuffer[9];
		triggers[currentTrigger]->getSelectMenu(printBuffer); //load printBuffer with string to print
		setCursor(0,row);
		print(printBuffer);
		
		#ifdef SERIAL_DEBUG
		Serial.println(printBuffer);
		#endif
}

/***********************************************************
* 
* printInc
* 
***********************************************************/
void TTUI::printInc(int row,int incVal)
{
	char printBuffer[9];
	triggers[currentTrigger]->incSetting(printBuffer,incVal); //increment the current selected value, pass char buffer
	//set the value in line 2	
	setCursor(0,row);
	print(printBuffer);
	
	#ifdef SERIAL_DEBUG
	Serial.println(printBuffer);
	#endif
}

/***********************************************************
* 
* printDec
* 
***********************************************************/
void TTUI::printDec(int row,int decVal)
{
	char printBuffer[9];
	triggers[currentTrigger]->decSetting(printBuffer,decVal); //increment the current selected value, pass char buffer
	//set the value in line 2	
	setCursor(0,row);
	print(printBuffer);
	
	#ifdef SERIAL_DEBUG
	Serial.println(printBuffer);
	#endif
}

/***********************************************************
* 	   
* startDownHandler ISR
*  
***********************************************************/
void startDownHandler(void)
{
	  //maybe most of this stuff should happen in loop? (speed ok..but maybe better outside?)
	  unsigned long currentTime = millis();
	  // debounce that button! 
	  if (currentTime - TTUI::pTTUI->prevIntTime > 130) //debounce 130ms
	  {
	    detachInterrupt(0);
		attachInterrupt(0,startUpHandler,RISING); //trigger ISR function on start button press.
		TTUI::pTTUI->startBttnHold = true;
		TTUI::pTTUI->holdBttnStart = currentTime/1000;
	
	  }
	    TTUI::pTTUI->prevIntTime = currentTime;
	
	
	
	
}

/***********************************************************
* 
* startUpHandler ISR
* 
***********************************************************/
void startUpHandler(void)
{

	  unsigned long currentTime = millis();
	  // debounce that button! 
	  if (currentTime - TTUI::pTTUI->prevIntTime > 130) //debounce 130ms
	  {
		detachInterrupt(0);
		attachInterrupt(0,startDownHandler,FALLING); //trigger ISR function on start button press.
		
		if(TTUI::pTTUI->startBttnHold == true) //press&hold reset wasn't done, so count it as a normal press
		{
			TTUI::pTTUI->initStart(currentTime);
		}
		TTUI::pTTUI->startBttnHold = false;
		
	  }
	    TTUI::pTTUI->prevIntTime = currentTime;
	
	
}




