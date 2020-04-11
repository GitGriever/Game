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

#include <LPC18xx.H>

#include <RTL.h>
#include <File_Config.h>
#include <rl_usb.h>

#include "Memory.h"
#include "LED.h"
#include "GLCD.h"

FAT_VI *mc0;                                        /* Media Control Block    */
Media_INFO info;

/*-----------------------------------------------------------------------------
  init task
 *----------------------------------------------------------------------------*/
__task void init (void) {

  LED_Init();

  GLCD_Init ();
  GLCD_Clear (Blue);
  GLCD_SetBackColor (Blue);
  GLCD_SetTextColor (White);
  GLCD_DisplayString (1, 0, 1, "      MCB1800       ");
  GLCD_DisplayString (2, 0, 1, "       RL-ARM       ");
  GLCD_DisplayString (3, 0, 1, "    MSD example     ");

  os_tsk_prio_self(100);
  mc0 = ioc_getcb (NULL);
  if (ioc_init (mc0) == 0) {
    ioc_read_info (&info, mc0);
    usbd_init();                            /* USB Device Initialization      */
    usbd_connect(__TRUE);                   /* USB Device Connect             */
  } else {
    GLCD_DisplayString (5, 0, 1, "  Memory Failure!   ");
  }
  os_tsk_delete_self();
}

/*-----------------------------------------------------------------------------
  Main Program
 *----------------------------------------------------------------------------*/
int main (void) {

  os_sys_init(init);                    /* Init RTX and start 'init'          */
}
