#include "scanningButtons.h"
#include "FreeRTOS.h"
#include "task.h"
//
//
//=====================================================================================================================================
//
SB_ButtonCondition_t buttonUp;
SB_ButtonCondition_t buttonDown;
SB_ButtonCondition_t buttonLeft;
SB_ButtonCondition_t buttonRight;
SB_ButtonCondition_t buttonEnter;
SB_ButtonCondition_t buttonFunction;
//
//
//=====================================================================================================================================
//

_Bool SB_ScanningButton ( SB_ButtonCondition_t* currentButton, _Bool PinState )
{
  if ( PinState )
    {
      currentButton->counter++;
      if ( currentButton->counter>3	&& ( !currentButton->buttonPressed ) )
        {
          currentButton->buttonPressed =1;
          return 1;
        }
    }
  else 
    {
      //currentButton->counter--;
			currentButton->counter =0;
      currentButton->buttonPressed =0;

    }


  return 0;
}

/*
_Bool SB_ScanningButton ( SB_ButtonCondition_t* currentButton, _Bool PinState )
{
  if ( PinState )
    {
      currentButton->counter ++;
    }
  else
    {
      currentButton->counter =0;
      currentButton->buttonPressed =0;
    }
		
		
  if ( currentButton->counter>2	&& (!currentButton->buttonPressed) )
    {
			currentButton->counter =0;
      currentButton->buttonPressed =0;
			
      return 1;
    }
  else
    {
      return 0;
    }
}
*/

