/*********************************************************************
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2017  SEGGER Microcontroller GmbH & Co. KG       *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

** emWin V5.46 - Graphical user interface for embedded applications **
All  Intellectual Property rights  in the Software belongs to  SEGGER.
emWin is protected by  international copyright laws.  Knowledge of the
source code may not be used to write a similar product.  This file may
only be used in accordance with the following terms:

The software has been licensed to  ARM LIMITED whose registered office
is situated at  110 Fulbourn Road,  Cambridge CB1 9NJ,  England solely
for  the  purposes  of  creating  libraries  for  ARM7, ARM9, Cortex-M
series,  and   Cortex-R4   processor-based  devices,  sublicensed  and
distributed as part of the  MDK-ARM  Professional  under the terms and
conditions  of  the   End  User  License  supplied  with  the  MDK-ARM
Professional. 
Full source code is available at: www.segger.com

We appreciate your understanding and fairness.
----------------------------------------------------------------------
Licensing information
Licensor:                 SEGGER Software GmbH
Licensed to:              ARM Ltd, 110 Fulbourn Road, CB1 9NJ Cambridge, UK
Licensed SEGGER software: emWin
License number:           GUI-00181
License model:            LES-SLA-20007, Agreement, effective since October 1st 2011 
Licensed product:         MDK-ARM Professional
Licensed platform:        ARM7/9, Cortex-M/R4
Licensed number of seats: -
----------------------------------------------------------------------
File        : GUI_X.C
Purpose     : Config / System dependent externals for GUI
---------------------------END-OF-HEADER------------------------------
*/

#include <stm32f2xx.h>
#include "GUI.h"
#include "I2C.h"
#include "TSC.h"
#include "JOY.h"

/*********************************************************************
*
*       Global data
*/
volatile int OS_TimeMS;

/*********************************************************************
*
*      Timing:
*                 GUI_X_GetTime()
*                 GUI_X_Delay(int)

  Some timing dependent routines require a GetTime
  and delay function. Default time unit (tick), normally is
  1 ms.
*/

int GUI_X_GetTime(void) { 
  return OS_TimeMS; 
}

void GUI_X_Delay(int ms) { 
  int tEnd = OS_TimeMS + ms;
  while ((tEnd - OS_TimeMS) > 0);
}

/*********************************************************************
*
*       GUI_X_Init()
*
* Note:
*     GUI_X_Init() is called from GUI_Init is a possibility to init
*     some hardware which needs to be up and running before the GUI.
*     If not required, leave this routine blank.
*/

void GUI_X_Init(void) {
  I2C_Init();
  TSC_Init();
  JOY_Init();
  SysTick_Config(SystemCoreClock/1000);  /* SysTick IRQ each 1 ms */
}

/*********************************************************************
*
*       GUI_X_ExecIdle
*
* Note:
*  Called if WM is in idle state
*/

void GUI_X_ExecIdle(void) {}

/*********************************************************************
*
*      Logging: OS dependent

Note:
  Logging is used in higher debug levels only. The typical target
  build does not use logging and does therefor not require any of
  the logging routines below. For a release build without logging
  the routines below may be eliminated to save some space.
  (If the linker is not function aware and eliminates unreferenced
  functions automatically)

*/

void GUI_X_Log     (const char *s) { GUI_USE_PARA(s); }
void GUI_X_Warn    (const char *s) { GUI_USE_PARA(s); }
void GUI_X_ErrorOut(const char *s) { GUI_USE_PARA(s); }

/*********************************************************************
*
*      Multitasking:
*
*                 GUI_X_InitOS()
*                 GUI_X_GetTaskId()
*                 GUI_X_Lock()
*                 GUI_X_Unlock()
*
* Note:
*   The following routines are required only if emWin is used in a
*   true multi task environment, which means you have more than one
*   thread using the emWin API.
*   In this case the
*                       #define GUI_OS 1
*  needs to be in GUIConf.h
*/

void GUI_X_InitOS(void)    {}
void GUI_X_Unlock(void)    {}
void GUI_X_Lock(void)      {}
U32  GUI_X_GetTaskId(void) { return 1; }

/*********************************************************************
*
*      Event driving (optional with multitasking)
*
*                 GUI_X_WaitEvent()
*                 GUI_X_WaitEventTimed()
*                 GUI_X_SignalEvent()
*/

void GUI_X_WaitEvent(void)            {}
void GUI_X_WaitEventTimed(int Period) {}
void GUI_X_SignalEvent(void)          {}

/*********************************************************************
*
*       Touch screen support (override default library functions)
*
*/

#define GUI_TOUCH_X_MIN 0x0000
#define GUI_TOUCH_X_MAX 0x0FFF
#define GUI_TOUCH_Y_MIN 0x0000
#define GUI_TOUCH_Y_MAX 0x0FFF

static unsigned int TouchOrientation;

static int TouchPhysX;
static int TouchPhysY;

static struct {
  int Min;
  int Max;
} TouchPhysLim[2] = {
  { GUI_TOUCH_X_MIN, GUI_TOUCH_X_MAX},
  { GUI_TOUCH_Y_MIN, GUI_TOUCH_Y_MAX} 
};

void GUI_TOUCH_SetOrientation (unsigned Orientation) {
  TouchOrientation = Orientation;
}

int  GUI_TOUCH_GetxPhys (void) {
  return TouchPhysX;
}

int  GUI_TOUCH_GetyPhys (void) {
  return TouchPhysY;
}

int  GUI_TOUCH_Calibrate (int Coord, int Log0, int Log1, int Phys0, int Phys1) {
  int size;

  if ((Log0 == Log1) || (Phys0 == Phys1)) return (1);

  switch (Coord) {
    case GUI_COORD_X:
      size = (TouchOrientation & GUI_SWAP_XY) ? LCD_GetYSize() : LCD_GetXSize();
      break;
    case GUI_COORD_Y:
      size = (TouchOrientation & GUI_SWAP_XY) ? LCD_GetXSize() : LCD_GetYSize();
      break;
    default:
      return (1);
  }

  TouchPhysLim[Coord].Min = Phys0 + ((0      - Log0)*(Phys1-Phys0))/(Log1-Log0);
  TouchPhysLim[Coord].Max = Phys0 + ((size-1 - Log0)*(Phys1-Phys0))/(Log1-Log0);

  return (0);
}

void GUI_TOUCH_GetCalData (int Coord, int* pMin, int* pMax) {
  *pMin = TouchPhysLim[Coord].Min;
  *pMax = TouchPhysLim[Coord].Max;
}

void GUI_TOUCH_Exec (void) {
  TSC_DATA   tscd;
  static U8  PressedOld = 0;
  static int xOld = 0;
  static int yOld = 0;
         int x, y;
         int xDiff, yDiff;
         int xSize, ySize;

  if (TSC_TouchDet()) {
    // Touch screen is pressed

    // Read current position
    TSC_GetData(&tscd);
    if (TouchOrientation & GUI_SWAP_XY) {
      TouchPhysX = 0x0FFF - tscd.y;
      TouchPhysY = 0x0FFF - tscd.x;
    } else {
      TouchPhysX = 0x0FFF - tscd.x;
      TouchPhysY = 0x0FFF - tscd.y;
    }

    // Convert touch values to pixels
    x = TouchPhysX - TouchPhysLim[0].Min;
    y = TouchPhysY - TouchPhysLim[1].Min;
    if (TouchOrientation & GUI_SWAP_XY) {
      xSize = LCD_GetYSize();
      ySize = LCD_GetXSize(); 
    } else {
      xSize = LCD_GetXSize();
      ySize = LCD_GetYSize(); 
    }
    x *= xSize - 1;
    x /= TouchPhysLim[0].Max - TouchPhysLim[0].Min;
    y *= ySize - 1;
    y /= TouchPhysLim[1].Max - TouchPhysLim[1].Min;

    if (PressedOld == 1) {
      // Touch screen has already been pressed

      // Calculate difference between new and old position
      xDiff = (x > xOld) ? (x - xOld) : (xOld - x);
      yDiff = (y > yOld) ? (y - yOld) : (yOld - y);

      // Store state if new position differs significantly
      if (xDiff + yDiff > 2) {
        GUI_TOUCH_StoreState(x, y);
        xOld = x;
        yOld = y;
      }
    }
    else {
      // Touch screen was previously not pressed

      // Store state regardless of position
      GUI_TOUCH_StoreState(x, y);
      xOld = x;
      yOld = y;
      PressedOld = 1;
    }
  }
  else {
    // Touch screen is not pressed

    // Store state if it was released recently
    if (PressedOld == 1) {
      PressedOld = 0;
      GUI_TOUCH_StoreState(-1, -1);
    }
  }
}


/*********************************************************************
*
*       Joystick support
*
*/

#ifdef  JOYSTICK_ROTATE
#define JOYSTICK_LEFT   JOY_DOWN
#define JOYSTICK_RIGHT  JOY_UP
#define JOYSTICK_UP     JOY_LEFT
#define JOYSTICK_DOWN   JOY_RIGHT
#define JOYSTICK_CENTER JOY_CENTER
#else
#define JOYSTICK_LEFT   JOY_LEFT
#define JOYSTICK_RIGHT  JOY_RIGHT
#define JOYSTICK_UP     JOY_UP
#define JOYSTICK_DOWN   JOY_DOWN
#define JOYSTICK_CENTER JOY_CENTER
#endif

static void Joystick_Exec (void) {
  GUI_PID_STATE state;
  static  U8    hold = 0;
  static  U8    prevkeys = 0;
          U32   keys;
          I32   diff, max;

  // Read Joystick keys
  keys = JOY_GetKeys();

  // Dynamic pointer acceleration
  if (keys == prevkeys) {
    if (hold < (40+3)) hold++;
    diff = (hold > 3) ? hold - 3 : 0; 
  } else {
    hold = 0;
    diff = 1;
  }

  // Change State if keys are pressed or have changed
  if (keys || (keys != prevkeys)) {
    GUI_PID_GetState(&state);
    if (keys & JOYSTICK_LEFT) {
      state.x -= diff;
      if (state.x < 0) state.x = 0;
    }
    if (keys & JOYSTICK_RIGHT) {
      state.x += diff;
      max = LCD_GetXSize() - 1;
      if (state.x > max) state.x = max; 
    }
    if (keys & JOYSTICK_UP) {
      state.y -= diff;
      if (state.y < 0) state.y = 0;
    }
    if (keys & JOYSTICK_DOWN) {
      state.y += diff;
      max = LCD_GetYSize() - 1;
      if (state.y > max) state.y = max; 
    } 
    state.Pressed = (keys & JOYSTICK_CENTER) ? 1 : 0;
    GUI_PID_StoreState(&state);
    prevkeys = keys;
  }
}


/*********************************************************************
*
*       SysTick_Handler
*
* Function decription:
*   Systick Interrupt Handler which is called periodically each 1ms.
*/
void SysTick_Handler (void) {
  static U8 ttick = 0;          // Touch Screen time tick
  static U8 ytick = 0;          // Joystick time tick

  OS_TimeMS++;                  // Increment 1ms time tick

  if (ttick++ == 20) {          // Touch Screen update period is 20ms
    ttick = 0;
    GUI_TOUCH_Exec();           // Execute Touch Screen function
  }
  if (ytick++ == 50) {          // Joystick update period is 50ms
    ytick = 0;
    Joystick_Exec();            // Execute Joystick function
  }
}

/*************************** End of file ****************************/
