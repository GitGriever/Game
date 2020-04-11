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

#include <MK60N512MD100.h>              /* MK60N512MD100 Definitions          */

#include <RTL.h>
#include <File_Config.h>
#include <rl_usb.h>

#include "Memory.h"
#include "LED.h"

FAT_VI *mc0;                                        /* Media Control Block    */
Media_INFO info;


/*-----------------------------------------------------------------------------
  Main Program
 *----------------------------------------------------------------------------*/
int main (void) {

  LED_Init   ();                            /* Init LEDs                      */

  mc0 = ioc_getcb (NULL);
  if (ioc_init (mc0) == 0) {
    ioc_read_info (&info, mc0);
    usbd_init();                            /* USB Device Initialization      */
    usbd_connect(__TRUE);                   /* USB Device Connect             */
  } else {
    /* Memory Failure! */
  }

  while (1);                                /* Loop forever                   */
}
