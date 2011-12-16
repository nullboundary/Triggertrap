#include "triggerTrap.h"


TriggerTrap::TriggerTrap()
{
	
}

void TriggerTrap::setup()
{
  tui.setup(laser,mic,light,timeLapse,aux); //pass the sensors to the UI
}

void TriggerTrap::update()
{
	 tui.update(); //update the UI
}

boolean TriggerTrap::trigger()
{	
	boolean triggerStatus = false;
	 
  	if(tui.trapActive() == true) //if start button pressed
    {
	   switch (tui.trigger()) //which trigger are we using.
	   {
	   case TIMELAPSE_TRIG:
		triggerStatus = timeLapse.trigger();
	   break;

	   case LASER_TRIG:
	   triggerStatus = laser.trigger();
	   break;

	   case SOUND_TRIG:
	   triggerStatus = mic.trigger();
	   break;

	   case LIGHT_TRIG:
		triggerStatus = light.trigger();
	   break;  

	   case AUX_TRIG:
	   triggerStatus = aux.trigger();
	   break;

	   default: //no default option.
	   break;
	   }
	}
	
	return triggerStatus;
	
}

int TriggerTrap::triggerMode()
{
	return tui.trigger();	
}



