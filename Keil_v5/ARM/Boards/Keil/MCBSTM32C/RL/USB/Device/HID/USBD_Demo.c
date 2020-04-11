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
#include <stm32f10x_cl.h>

int main (void) {
  static U8 but_ex;
         U8 but;
         U8 buf[1];

  GLCD_Init          ();
  GLCD_Clear         (Blue);
  GLCD_SetBackColor  (Blue);
  GLCD_SetTextColor  (White);
  GLCD_DisplayString (4, 0, 1, "  USB Device Demo   ");
  GLCD_DisplayString (5, 0, 1, "   www.keil.com     ");

  usbd_init();                          /* USB Device Initialization          */
  usbd_connect(__TRUE);                 /* USB Device Connect                 */

  while (1) {                           /* Loop forever                       */
    but = ~(((GPIOD->IDR >> 11) & 0x1F ) | 0xE0);
    if (but ^ but_ex) {
      buf[0] = but;
      usbd_hid_get_report_trigger(0, buf, 1);
      but_ex = but;
    }
  };
}
