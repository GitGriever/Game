/*----------------------------------------------------------------------------
 *      RL-ARM - USB
 *----------------------------------------------------------------------------
 *      Name:    USBD_Demo.c
 *      Purpose: USB Device Demonstration
 *      Rev.:    V4.60
 *----------------------------------------------------------------------------
 *      This code is part of the RealView Run-Time Library.
 *      Copyright (c) 2004-2012 KEIL - An ARM Company. All rights reserved.
 *---------------------------------------------------------------------------*/

#include <RTL.h>
#include <rl_usb.h>
#include <MK60D10.H>

int main (void) {
  SystemCoreClockUpdate();
  usbd_init();                          /* USB Initialization                 */
  usbd_connect(__TRUE);                 /* USB Connect                        */

  while (1);                            /* Loop forever                       */
}
