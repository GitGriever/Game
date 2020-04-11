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

#include <LPC177x_8x.h>

#include <RTL.h>
#include <File_Config.h>
#include <rl_usb.h>
#include <..\..\RL\USB\INC\usb.h>

#include "Terminal.h"

#include "Memory.h"
#include "Serial.h"
#include "KBD.h"
#include "I2C.h"

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
  Systick Interrupt Handler
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

  SER_Init   ();                                    /* Init serial interface  */
  I2C_Init   ();                                    /* Init I2C0              */
  KBD_Init   ();                                    /* Init Keyboard          */
  
  usbd_init();
  SysTick_Config (SystemCoreClock / 100);

  while (1) {
    if (WakeUp) {
      WakeUp = __FALSE;
      usbd_connect(__FALSE);                        /* USB Device Disconnect  */
      sd_main();
    }
    else {
      mc0 = ioc_getcb (NULL);
      if (ioc_init (mc0) == 0) {
        ioc_read_info (&info, mc0);
        usbd_msc_init();
        usbd_connect(__TRUE);                       /* USB Device Connect     */
      }
      while (!WakeUp);
    }
  }
}
