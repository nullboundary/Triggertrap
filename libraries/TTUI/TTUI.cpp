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


TTUI* TTUI::pTTUI = 0; 

  TTUI::TTUI()
  {
	pTTUI = this;	//the ptr points to this object
	trapActive_ = false; 
  }

  /***********************************************************
   * 
   * begin
   * 
   ***********************************************************/
  void TTUI::begin(Trigger& laser, Trigger& sound, Trigger& light,Trigger& timeLapse )
  {

	triggers[0] = &laser;
	triggers[1] = &sound;
	triggers[2] = &light;
	triggers[3] = &timeLapse;

			

	pinMode(START_BUTTON, INPUT);       // Start Button
    digitalWrite(START_BUTTON, HIGH);   // turn on pullup resistor for Start button
	attachInterrupt(0,startHandler,FALLING); //trigger ISR function on start button press.
	
	 DDRB = _BV (PORTB7);   //pinMode(KEY_PAD_LEDS, OUTPUT);      // LED on UI
	 DDRB = _BV (PORTB6);   //pinMode(POWER_UI,OUTPUT);
	 state_UIPower = false; 
	 uiPowerOn(); //turn on power at startup

	 

    //restoreSettings(); //restore all menu settings from eeprom

    previousMillis_UIPower = 0; 
    interval_UIPower = 30000;  //30 seconds till sleep mode with no use of device
    touch.begin(KEY_CHANGE);  //init touch UI with key change interrupt
   
	trapActive_ = false; 	
	currentTrigger = 0;


	//LCD Stuff
	TCCR1B = TCCR1B & 0b11111000 | 0x01; //-- sets the pwm base
	// initialize the library with the numbers of the interface pins
	pinMode (4, OUTPUT); // RW Pin 
	digitalWrite (4, LOW); // ouput low - slow mode - write delay
	pinMode (10, OUTPUT); // lcd contrast output 
	analogWrite (10, LCD_CONTRAST); // ouput low pwm - negative voltage...

    // initialize the library with the numbers of the interface pins
    LiquidCrystal _lcd(A3, 4, 5, 6, 7, 8, 9);
    lcd = &_lcd; //assign the adress of _lcd to the lcd pointer 

    // set up the LCD's number of columns and rows:
    lcd->begin(8, 2);
  	// Print a message to the LCD.
  	lcd->print("Trigger");
	lcd->setCursor(0,1);
	lcd->print("Trap0v34");

/*
    
    // set up the LCD's number of columns and rows: 
    lcd->begin(8, 2);
    // Print a message to the LCD.
    lcd->autoscroll();
    lcd->print("Trigger");
    lcd->print("Trap");
    lcd->print("v0.01");
*/
    #ifdef SERIAL_DEBUG
	Serial.println("Trigger Trap v.02");
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

   // uiPowerTimeOut(); //if there has been no activity turn off led power

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
		Serial.println("trapActive");
		triggers[currentTrigger]->start(startTime); //set start time for the active trigger 
		//uiPowerOff();
		
	}
	else if(trapActive_ == false) 
	{
		uiPowerOn();
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
	currentTrigger = currentTrigger % 4;//TODO assign length based on number of objects triggers.length();
	char printBuffer[10];

	triggers[currentTrigger]->getModeMenu(printBuffer);

	//lcd->println(printBuffer);
	analogWrite (10, LCD_CONTRAST);
	lcd->setCursor(0,0);
	lcd->print("this");

	#ifdef SERIAL_DEBUG
	Serial.println(printBuffer);
	#endif
	
	//lcd->print(printBuffer);
	
	

  }

/***********************************************************
* 
* bttnSelect
* 
***********************************************************/
  void TTUI::bttnSelect()
  {
	char printBuffer[30];

	triggers[currentTrigger]->incSelect(); //set sensor to next select mode
	triggers[currentTrigger]->getSelectMenu(printBuffer); //load printBuffer with string to print
	
	#ifdef SERIAL_DEBUG
	Serial.println(printBuffer);
	#endif

  }

/***********************************************************
* 
* bttnUp
* 
***********************************************************/
void TTUI::bttnUp()
{

	char printBuffer[30];
	triggers[currentTrigger]->incSetting(printBuffer); //increment the current selected value, pass printBuffer

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
	char printBuffer[30];

	triggers[currentTrigger]->decSetting(printBuffer); //increment the current selected value, pass char buffer

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
    if(state_UIPower == false)
    {
      state_UIPower = true; 	
      unsigned long currentMillis = millis();

	  Serial.println("ui Power UP");
      previousMillis_UIPower = currentMillis;  //clock countdown start time
	  PORTB &= ~ _BV(PORTB6);        //digitalWrite(POWER_UI,LOW);
      PORTB |= _BV(PORTB7);		    //digitalWrite(KEY_PAD_LEDS,HIGH); turn on keypad LEDs
	 
      //lcd->display(); //turn on LCD
    }
}

/***********************************************************
* 	   
* uiPowerOff
*  
***********************************************************/
void TTUI::uiPowerOff()
{
    if(state_UIPower == true)
    {
      state_UIPower = false; 	
      PORTB |= _BV(PORTB6);	               //digitalWrite(POWER_UI,HIGH);
      PORTB &= ~ _BV(PORTB7);              //digitalWrite(KEY_PAD_LEDS,LOW); // turn off keypad LEDs
	  Serial.println("ui Power Down");
	 
      //lcd->display(); //turn on LCD
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
      
      unsigned long currentMillis = millis();

      if(currentMillis - previousMillis_UIPower > interval_UIPower) 
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
    EEPROM.write(0,currentTrigger);
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
    currentTrigger = EEPROM.read(0);
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




