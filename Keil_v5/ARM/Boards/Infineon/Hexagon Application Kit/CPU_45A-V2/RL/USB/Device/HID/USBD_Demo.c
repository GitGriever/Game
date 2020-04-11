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
#include <XMC4500.h>

U8 InReportVal;

void SysTick_Handler (void) {
  /* 7 button simulation... binary counter                                    */
  static int ms = 0;
  if (ms == 1000) {
    InReportVal = (InReportVal + 1) & 0xFF;	
    ms = 0;
  }
  ms++;
}

int main (void) {
  static U8 but_ex;
         U8 but;
         U8 buf[1];

  SystemCoreClockUpdate();              /* update SystemCoreClock             */

  SysTick_Config (120000);              /* Tick timer interrupr every ms      */
  usbd_init();                          /* USB Device Initialization          */
  usbd_connect(__TRUE);                 /* USB Device Connect                 */

  while (1) {                           /* Loop forever                       */
    but = InReportVal;
    if (but ^ but_ex) {
      buf[0] = but;
      usbd_hid_get_report_trigger(0, buf, 1);
      but_ex = but;
    }  
  }
}
