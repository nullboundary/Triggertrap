#include "timelapse.h"

//mode menu title
const prog_char timeMenu[] PROGMEM= {"TimeLaps"};

 //option Menu titles
const prog_char delta[] PROGMEM="interval";
const prog_char startdelay[] PROGMEM="delay1st";
const prog_char numShots[] PROGMEM = "#shots";

const prog_char * timeSelectMenu[] PROGMEM  = 	   //options menu order
{   
delta,
startdelay,
numShots,
};

//option order should match timeSelectMenu order
const int TIME_DELTA = 0;
const int TIME_DELAY = 1; 
const int TIME_NUMSHOTS = 2;

/***********************************************************
 * 
 * TimeLapse
 *
 *  
 * 
 ***********************************************************/
TimeLapse::TimeLapse(){
	
		maxOptionMenu = 3;
	    triggerState_ = false; //off
		abortTrigger = false; 
		setOption(TIME_DELTA,0);    //set time delta to 0
		setOption(TIME_DELAY,0);    //set time delay to 0
		setOption(TIME_NUMSHOTS,0); //set #shots to 0
		delayCount = 0; 
		select_ = 0;  //set 

	  	
	
}

/***********************************************************
 * 
 * delayFirstShot
 *
 *  
 * 
 ***********************************************************/
boolean TimeLapse::delayFirstShot()
{	
	if(shotCounter_ < 1) //first shot happens RIGHT away on start button press, unless delayed
	{
			if(delayCount == 0) { delayCount = millis(); } //set this only the first time through the loop
			shutterReady = true; //if delay is up, take the first shot, 
			shutter(true,false); //set delay to true, we want delay, set delay time unit false to seconds
			return true; 
	}
	else
	{
			return false; 
	}
}

/***********************************************************
 * 
 * trigger
 *
 *  
 * 
 ***********************************************************/
boolean TimeLapse::trigger()
{

	unsigned long remainTime;
	//don't allow zero time delta. Will crash the device
	if(option(TIME_DELTA) == 0) { incOption(TIME_DELTA,1); }
	
	//----------------NumShot
	//if you set a shot limit, stop take pictures when it is reached
	if(shotCounter_ >= option(TIME_NUMSHOTS) && option(TIME_NUMSHOTS) != 0) 
	{
		if(shutterStateA_ == false && shutterStateB_ == false) //don't abort if shutter is triggered
		{
		 	abortTrigger = true; return false;
		}
   }

    remainTime = countDown(); //get the remaining time to next shot

    //-------------Delay
	if(delayFirstShot() == true)
	{
		return true;
	}


	shutter(false,false); //trigger shutter if shutterReady = true, set delay to false, false time unit seconds
			
	
	//----------Sleep		
	if(batteryPower() == true)
	{
		if(shutterStateA_ == false && shutterStateB_ == false) //don't sleep if shutter is triggered
		{
	
			sleepNow(remainTime); //sleep now if there is time left to sleep
			remainTime = 0;
	
		}	
	}
			
		
	 //---------- Shutter	
	  if ((long) remainTime < 0 ) {remainTime = 0;} //check for unsigned underflow, by converting to signed	
	  
	  if (remainTime <= 0) 
	  {
			//times up,take a shot
			delayCount = millis(); //start counting till delay is up

			startBttnTime = delayCount; //don't call millis twice, just use delayCount, reset startButtnTime
		//	timelapseCountDown = startBttnTime; 
			shutterReady = true; //set shutter ready, will activate shutter next time through loop
			return shutterReady;
	  }
	  else
	  {
		return false; 
	  }



	
}

/***********************************************************
 * 
 * countDown
 *
 *  
 * 
 ***********************************************************/
unsigned long TimeLapse::countDown()
{

	if(shotCounter_ < 1)
	{
		startBttnTime = millis(); //reset startBttnTime, so interval starts here
				
	}
	  unsigned long currentTime = millis();
			
      unsigned long elapsedTime = currentTime - startBttnTime;
	  unsigned long deltaTime =	(unsigned long) option(TIME_DELTA);
	  deltaTime = deltaTime*1000;
	  unsigned long remainTime = deltaTime - elapsedTime;
	
	return remainTime;

}

unsigned int TimeLapse::countDownInt()
{   
	unsigned long longCount = countDown()/1000;
	unsigned int countInt = (unsigned int) longCount;
	return countInt;
}

boolean TimeLapse::batteryPower()
{
	if(digitalRead(0) == HIGH || digitalRead(1) == HIGH) //USB connected
	{
		return false; //USB
	}
	else
	{
		return true; //battery
	}
}


/***********************************************************
 * 
 * SleepNow
 *
 *  
 * 
 ***********************************************************/
void TimeLapse::sleepNow(unsigned long remainTime)
{

	sleep.pwrDownMode(); //deepest sleep
	sleep.sleepDelay(remainTime,shotCounter_);
	  
}

/***********************************************************
 * 
 * decSetting
 *
 *  
 * 
 ***********************************************************/
void TimeLapse::decSetting(char buffer[],int dec)
{

	
		switch (select_)
	    {
	     case TIME_DELAY:
	 	  decOption(TIME_DELAY, 54000,dec); //max secs
		  if(option(TRIG_DELAY) == 0) //delay 0 is infinity 
		  {
				buffer[0] = 0;
				strcat(buffer,"Off");
				strcat(buffer,"\0");
		  }
		  else
		  {	
		   		formatTimeString(option(TIME_DELAY),buffer); //format and save string in buffer
		  }
	      break;
	    case TIME_DELTA:
	      decOption(TIME_DELTA, 54000,dec);
		  if(option(TIME_DELTA) == 0) //delay 0 is none 
		  {
				buffer[0] = 0;
				strcat(buffer,"-");
				strcat(buffer,"\0");
		  }
		  else
		  {
	 	  	formatTimeString(option(TIME_DELTA),buffer);
      	  }
	      break;
	    case TIME_NUMSHOTS:
	      decOption(TIME_NUMSHOTS, 50000,dec);
	 	  if(option(TIME_NUMSHOTS) == 0) //numshots 0 is infinity 
		  {
				buffer[0] = 0;
				char inf[2];
				inf[0] = (char)243;
				inf[1] = '\0';
				strcat(buffer,inf);
				strcat(buffer,"\0");
		  }
		  else
		  {
	 	  	utoa (option(TIME_NUMSHOTS),buffer,10);
      	  }	
		  
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
void TimeLapse::incSetting(char buffer[],int inc)
{
	
	
		switch (select_)
	    {
	     case TIME_DELAY:
	 	   incOption(TIME_DELAY, 54000,inc); //max secs
		  if(option(TRIG_DELAY) == 0) //delay 0 is infinity 
		  {
				buffer[0] = 0;
				strcat(buffer,"Off");
				strcat(buffer,"\0");
		  }
		  else
		  {	
		   		formatTimeString(option(TIME_DELAY),buffer); 
		  }
	      break;
	    case TIME_DELTA:
	      incOption(TIME_DELTA, 54000,inc);
		  if(option(TIME_DELTA) == 0) //delay 0 is none
		  {
				buffer[0] = 0;
				strcat(buffer,"-");
				strcat(buffer,"\0");
		  }
		  else
		  { 
	 	  	formatTimeString(option(TIME_DELTA),buffer);
	      }
		  break;
	    case TIME_NUMSHOTS:
	      incOption(TIME_NUMSHOTS, 50000,inc); 
		  if(option(TIME_NUMSHOTS) == 0) //numshots 0 is infinity 
		  {
				buffer[0] = 0;
				char inf[2];
				inf[0] = (char)243;
				inf[1] = '\0';
				strcat(buffer,inf);
				strcat(buffer,"\0");
		  }
		  else
		  {
	 	  	utoa (option(TIME_NUMSHOTS),buffer,10);
      	  }
	      break;
	    default: 
	      break;
	    }
		
}

/***********************************************************
 * 
 * getModeMenu
 *
 *  
 * 
 ***********************************************************/
void TimeLapse::getModeMenu(char buffer[])
{
	 //reads the timeMenu title from flash
	 strcpy_P(buffer, timeMenu); 
	
}

/***********************************************************
 * 
 * getSelectMenu
 *
 *  
 * 
 ***********************************************************/
void TimeLapse::getSelectMenu(char buffer[])
{
	 //reads the timeSelectMenu options from flash memory
	 strcpy_P(buffer, (const prog_char *)pgm_read_word(&(timeSelectMenu[select_])));
}

/***********************************************************
 * 
 * getActiveMessage
 *
 * get the current sensors LCD message to print during trap Active mode.
 * 
 ***********************************************************/
void TimeLapse::getActiveMessage(char buffer[])
{
	//buffer[0] = 0;
	
	//itoa (countDownInt(),buffer,10);
	formatTimeString(countDownInt(),buffer);
	
	
}