/*----------------------------------------------------------------------------
 *      RL-ARM - USB
 *----------------------------------------------------------------------------
 *      Name:    USBD_Demo.c
 *      Purpose: USB Device Demonstration
 *      Rev.:    V4.70
 *----------------------------------------------------------------------------
 *      This code is part of the RealView Run-Time Library.
 *      Copyright (c) 2004-2013 KEIL - An ARM Company. All rights reserved.
 *---------------------------------------------------------------------------*/

#include <RTL.h>
#include <rl_usb.h>
#include "GLCD.h"
#include <sam3s.h>

int main (void) {

  SystemCoreClockUpdate ();

  GLCD_Init          ();
  GLCD_Clear         (Blue);
  GLCD_SetBackColor  (Blue);
  GLCD_SetTextColor  (White);
  GLCD_DisplayString (4, 0, 1, "  USB Device Demo   ");
  GLCD_DisplayString (5, 0, 1, "   www.keil.com     ");

  usbd_init();                          /* USB Device Initialization          */
  usbd_connect(__TRUE);                 /* USB Device Connect                 */

  while (1);                            /* Loop forever                       */
}
