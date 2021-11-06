#ifndef scanningButtons
#define scanningButtons
//
//
//=====================================================================================================================================
//
#include "main.h"
//
//
//=====================================================================================================================================
//
typedef enum
{
  KEYS_ARE_REALESED =0,
  KEY_ENTER =1,
  KEY_UP =2,
  KEY_RIGHT =3,
  KEY_FUNCTION=4,
  KEY_LEFT =5,
  KEY_DOWN =6
} SB_Key_t;
//
//
typedef const struct
{
  SB_Key_t keyEnter;
  SB_Key_t keyUp;
  SB_Key_t keyRight;
  SB_Key_t keyFunction;
  SB_Key_t keyLeft;
  SB_Key_t keyDown;
} SB_Keyboard_t;
//
//
typedef struct
{
  _Bool buttonPressed;
  //_Bool button_holded;
  //uint32_t holdtime;
  uint32_t counter;
} SB_ButtonCondition_t;
//
//
//=====================================================================================================================================
//
extern SB_ButtonCondition_t buttonUp;
extern SB_ButtonCondition_t buttonDown;
extern SB_ButtonCondition_t buttonLeft;
extern SB_ButtonCondition_t buttonRight;
extern SB_ButtonCondition_t buttonEnter;
extern SB_ButtonCondition_t buttonFunction;
//
//
//=====================================================================================================================================
//
_Bool SB_ScanningButton ( SB_ButtonCondition_t* button, _Bool PinState );
//
//
//=====================================================================================================================================
//

#endif
