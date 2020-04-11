/*-----------------------------------------------------------------------------
 *      RL-ARM - USB
 *-----------------------------------------------------------------------------
 *      Name:    Memory.c
 *      Purpose: USB Memory Storage Demo
 *      Rev.:    V4.70
 *-----------------------------------------------------------------------------
 *      This code is part of the RealView Run-Time Library.
 *      Copyright (c) 2004-2013 KEIL - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <stm32f2xx.h>                  /* STM32F2xx definitions              */

#include <RTL.h>
#include <File_Config.h>
#include <rl_usb.h>

#include "SD_File.h"
#include "Memory.h"
#include "Serial.h"
#include "GLCD.h"
#include "LED.h"
#include "KBD.h"

#define WAKEUP  1

FAT_VI *mc0;                            /* FAT Media Control Block            */
Media_INFO info;                        /* Media information                  */ 

extern void sd_main (void);             /* Terminal mode main loop            */

/*-----------------------------------------------------------------------------
  WKUP Key
 *----------------------------------------------------------------------------*/
volatile BOOL WakeUp   = __FALSE;                   /* WakeUp Event           */
volatile BOOL WKUP_Key = __FALSE;                   /* WakeUp Key Active      */


/*-----------------------------------------------------------------------------
  Systick Interrupt Handler (happens every 50ms)
 *----------------------------------------------------------------------------*/
void SysTick_Handler (void) {
  BOOL key;

  key = ((KBD_GetKeys() & WAKEUP) == 0);
  if (key && !WKUP_Key) {
    WakeUp = __TRUE;
  }
  WKUP_Key = key;
}


/*-----------------------------------------------------------------------------
  Setup GLCD and print out example title
 *----------------------------------------------------------------------------*/
static void GLCD_Setup (void) {
  GLCD_Init ();
  GLCD_Clear (Blue);
  GLCD_SetBackColor (Blue);
  GLCD_SetTextColor (White);
  GLCD_DisplayString (0, 0, 1, "    MCBSTM32F200    ");
  GLCD_DisplayString (1, 0, 1, "       RL-ARM       ");
  GLCD_DisplayString (2, 0, 1, "    MSD example     ");
  GLCD_DisplayString (4, 0, 1, " Current:           ");
}

/*-----------------------------------------------------------------------------
  Main Program
 *----------------------------------------------------------------------------*/
int main (void) {
  BOOL usb_initialized = __FALSE;

  SER_Init   ();                                    /* Init serial interface  */
  LED_Init   ();                                    /* Init LEDs              */
  KBD_Init   ();                                    /* Init Keyboard          */
  GLCD_Setup ();                                    /* Init and setup GLCD    */

  SysTick_Config (SystemCoreClock / 1000);

  while (1) {
    if (WakeUp) {
      WakeUp = __FALSE;
      usbd_connect(__FALSE);                        /* USB Device Disconnect  */
      LED_Val (LED_MSK_OFF);
      GLCD_DisplayString (5, 0, 1, "   Term Interface   ");
      sd_main();
    }
    else {
      mc0 = ioc_getcb (NULL);
      if (ioc_init (mc0) == 0) {
        ioc_read_info (&info, mc0);
        if (usb_initialized == __FALSE) {
          usbd_init();                              /* Init USB Device        */
          usb_initialized = __TRUE;
        } else {
          usbd_msc_init();
        }
        usbd_connect(__TRUE);                       /* USB Device Connect     */
        GLCD_DisplayString (5, 0, 1, "   PC Interface     ");
      }
      else {
        LED_Val (LED_MSK_ALL);
        GLCD_DisplayString (5, 0, 1, "  Memory Failure!   ");
      }
      while (!WakeUp);
    }
  }
}
