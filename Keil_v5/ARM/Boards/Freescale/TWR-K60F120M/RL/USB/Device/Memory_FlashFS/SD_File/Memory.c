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

#include <MK60F12.H>

#include <RTL.h>
#include <File_Config.h>
#include <rl_usb.h>

#include "Terminal.h"
#include "Memory.h"
#include "SER.h"
#include "LED.h"
#include "KBD.h"

#define WAKEUP  2

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
  Main Program
 *----------------------------------------------------------------------------*/
int main (void) {
  BOOL usb_initialized = __FALSE;

  SER_Init   ();                                    /* Init serial interface  */
  LED_Init   ();                                    /* Init LEDs              */
  KBD_Init   ();                                    /* Init Keyboard          */

  SysTick_Config (SystemCoreClock / 1000);

  while (1) {
    if (WakeUp) {
      WakeUp = __FALSE;
      usbd_connect(__FALSE);                        /* USB Device Disconnect  */
      LED_Val (LED_MSK_OFF);
      /* Terminal Interface Selected */
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
        /* PC Interface Selected */
      }
      else {
        LED_Val (LED_MSK_ALL);
        /* Memory Failure! */
      }
      while (!WakeUp);
    }
  }
}
