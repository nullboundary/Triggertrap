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

	triggers[0] = &laser;
	triggers[1] = &sound;
	triggers[2] = &light;
	triggers[3] = &timeLapse;
	triggers[4] = &aux;

			

	pinMode(START_BUTTON, INPUT);       // Start Button
    digitalWrite(START_BUTTON, HIGH);   // turn on pullup resistor for Start button
	attachInterrupt(0,startHandler,FALLING); //trigger ISR function on start button press.
	
	 DDRB |= (1<<PB7);   //pinMode(KEY_PAD_LEDS, OUTPUT);      // LED on UI
	 DDRB |= (1<<PB6);   //pinMode(POWER_UI,OUTPUT);
	 state_UIPower = false; 
	 uiPowerOn(); //turn on power at startup

	 

    //restoreSettings(); //restore all menu settings from eeprom

    previousMillis_UIPower = 0; 
    touch.begin(KEY_CHANGE);  //init touch UI with key change interrupt
   
	trapActive_ = false; 	
	currentTrigger = 0;

	//LCD Stuff
	TCCR1B = TCCR1B & 0b11111000 | 0x01; //-- sets the pwm base
	// initialize the library with the numbers of the interface pins
//	pinMode (4, OUTPUT); // RW Pin 
//	digitalWrite (4, LOW); // ouput low - slow mode - write delay
	pinMode (10, OUTPUT); // lcd contrast output 
	analogWrite (10, LCD_CONTRAST); // ouput low pwm - negative voltage...

	//this class inherits from LCD, so call lcd functions as part of this class
	// set up the LCD's number of columns and rows:
	
    begin(8, 2);
  	// Print a message to the LCD.
  	print("Trigger");
	setCursor(0,1);
	print("Trap0v34");

    #ifdef SERIAL_DEBUG
	Serial.println("TT 0v34");
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
	int hitKeyVal;
  
	if(trapActive_ == false) //waiting for UI input
	{

  	 	if(touch.hold() == true) //press and hold
		  {
			keyDown = true; 
			hitKeyVal = touch.getKey();
		  }

  

		   if(touch.hit() == true) //just press
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
			        bttnDown(); 
			        break;
			      case UP_BTTN:
			        bttnUp();
			        break;
			      default: //no default option, just here for compiler
			        break;
		      }

		    }
		
		 //uiPowerTimeOut(); //if there has been no activity turn off led power
		
	}
	else if(trapActive_ == true) //start button was pressed, active
	{
		updateActive();
	}

   

}

/***********************************************************
* 
* updateActive
* 
***********************************************************/
void TTUI::updateActive()
{
		if(digitalRead(0) == HIGH || digitalRead(1) == HIGH) //USB connected
		{
			//only update LCD every 300ms
			int now = millis()/100; //100 ms 
			int elapsed = now - activeRefreshTime;
			
			if(elapsed > 3)  //300ms
			{
				activeRefreshTime = now;
				char printBuffer[9];
				triggers[currentTrigger]->getModeMenu(printBuffer);
				
				clear();
				setCursor(0,0);
				print(printBuffer);
			
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
		triggers[currentTrigger]->start(startTime); //set start time for the active trigger 
		//uiPowerOff();
		
	}
	else if(trapActive_ == false) 
	{
		//uiPowerOn();
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
	currentTrigger = currentTrigger % 5;//TODO assign length based on number of objects triggers.length();
	char printBuffer[9];

	triggers[currentTrigger]->getModeMenu(printBuffer);

	#ifdef SERIAL_DEBUG
	Serial.println(printBuffer);
	#endif
	
	//LCD
	clear();
	setCursor(0,0);
	print(printBuffer);


	triggers[currentTrigger]->getSelectMenu(printBuffer);
	
	setCursor(0,1);
	print(printBuffer);
	

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
	triggers[currentTrigger]->getSelectMenu(printBuffer); //load printBuffer with string to print
	
	#ifdef SERIAL_DEBUG
	Serial.println(printBuffer);
	#endif
	
	//LCD
	clear();
	setCursor(0,1);
	print(printBuffer);

	triggers[currentTrigger]->getModeMenu(printBuffer);
	
	setCursor(0,0);
	print(printBuffer);

  }

/***********************************************************
* 
* bttnUp
* 
***********************************************************/
void TTUI::bttnUp()
{

	char printBuffer[9];

	//set the value title in line 1
	triggers[currentTrigger]->getSelectMenu(printBuffer); //load printBuffer with string to print
	clear();
	setCursor(0,0);
	print(printBuffer);


	triggers[currentTrigger]->incSetting(printBuffer); //increment the current selected value, pass char buffer

	//set the value in line 2	
	setCursor(0,1);
	print(printBuffer);

	#ifdef SERIAL_DEBUG
	Serial.println(printBuffer);
	#endif
	
}
   
/***********************************************************
* 
* bttnUp
* 
***********************************************************/
void TTUI::bttnDown()
{
	char printBuffer[9];

	//set the value title in line 1
	triggers[currentTrigger]->getSelectMenu(printBuffer); //load printBuffer with string to print
	clear();
	setCursor(0,0);
	print(printBuffer);


	
	triggers[currentTrigger]->decSetting(printBuffer); //increment the current selected value, pass char buffer

	//set the value in line 2	
	setCursor(0,1);
	print(printBuffer);

	#ifdef SERIAL_DEBUG
	Serial.println(printBuffer);
	#endif
	
	

	


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
		
		  PORTB &= ~ (1<<PB6);        //digitalWrite(POWER_UI,LOW);
	      //PORTB |= (1<<PB7);		    //digitalWrite(KEY_PAD_LEDS,HIGH); turn on keypad LEDs
		  touch.begin(KEY_CHANGE); 		//re init touch keys
	      previousMillis_UIPower = millis();  //clock countdown start time

		  if(digitalRead(0) == HIGH || digitalRead(1) == HIGH) //USB connected
		  {
				 #ifdef SERIAL_DEBUG
				  Serial.println("USB");
				  #endif
		  }
		  else //battery power
		  {
	  
      		//init(1, A3, 4, 5, 6, 7, 8, 9, 0, 0, 0, 0);
    
		  	//begin(8,2);	
	      	//analogWrite (10, LCD_CONTRAST);
	  		//    display(); //turn on LCD
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
      PORTB |= (1<<PB6);	               //digitalWrite(POWER_UI,HIGH); //turn off keypad
	  detachInterrupt(1);	//disable the touch key interrupt

		if(digitalRead(0) == HIGH || digitalRead(1) == HIGH) //USB connected
		{
			clear();
			print("active");
		}
		else //battery power
		{
			PORTB &= ~ (1<<PB7);              //digitalWrite(KEY_PAD_LEDS,LOW); // turn off keypad LEDs
		
			 /* LCD SHUTDOWN
				digitalWrite(A3,LOW);
				digitalWrite(5,LOW);
				digitalWrite(6,LOW);
				digitalWrite(7,LOW);
				digitalWrite(8,LOW);
				digitalWrite(9,LOW);
				analogWrite (10, 0);
			*/
		
		    //     noDisplay(); //turn off LCD	
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
* saveSettings
*  
***********************************************************/
  void TTUI::saveSettings()
  {
    //EEPROM.write(0,currentTrigger);
   // EEPROM.write(1,timeLapseAdjState);
   // EEPROM.write(2,laserAdjState);
   // EEPROM.write(3,soundAdjState);
   // EEPROM.write(4,auxAdjState); 

  }

/***********************************************************
* 	   
* restoreSettings
*  
***********************************************************/
  void TTUI::restoreSettings()
  {
    //currentTrigger = EEPROM.read(0);
   // timeLapseAdjState = EEPROM.read(1);
   // laserAdjState = EEPROM.read(2);
  //  soundAdjState = EEPROM.read(3);
  //  auxAdjState = EEPROM.read(4);

    //if the device is run the first time ever, all values will be 255, so set them to 0 (option1)
    if(currentTrigger == 255){ currentTrigger = 0; }
   // if(timeLapseAdjState == 255){  timeLapseAdjState = 0; }
   // if(laserAdjState == 255){ laserAdjState = 0; }
   // if(soundAdjState == 255){ soundAdjState = 0; }
   // if(auxAdjState == 255){ auxAdjState = 0; }

  }

void TTUI::printCmp(char newBuffer[], char oldBuffer[],int line)
{
	if( strcmp(newBuffer,oldBuffer) != 0)
	{
		print("        ");
		setCursor(0,line);
		print(newBuffer);
	}
	
}

/***********************************************************
* 	   
* startHandler ISR
*  
***********************************************************/
void startHandler(void)
{
	  //maybe most of this stuff should happen in loop? (speed ok..but maybe better outside?)
	  unsigned long currentTime = millis();
	  // debounce that button! 
	  if (currentTime - TTUI::pTTUI->prevIntTime > 200)
	  {
	    //TTUI::pTTUI->trapActive_ = 	!TTUI::pTTUI->trapActive_;
		TTUI::pTTUI->initStart(millis());
	  }
	  TTUI::pTTUI->prevIntTime = currentTime;
	
	
	
	
}




