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
OS_TID bt_tsk;

__task void button_sim_tsk (void) {
  /* 7 button simulation... binary counter                                    */
  while (1) {
    os_dly_wait (100);
    InReportVal = (InReportVal + 1) & 0xFF;
  }
}

__task void init (void) {
  static U8 but_ex;
         U8 but;
         U8 buf[1];

  os_tsk_prio_self(100);
  usbd_init();                          /* USB Device Initialization          */
  usbd_connect(__TRUE);                 /* USB Device Connect                 */
  os_tsk_prio_self(1);
  bt_tsk = os_tsk_create (button_sim_tsk, 1);
  while (1) {                           /* Loop forever                       */
    but = InReportVal;
    if (but ^ but_ex) {
      buf[0] = but;
      usbd_hid_get_report_trigger(0, buf, 1);
      but_ex = but;
    }  
  }
}

int main (void) {

  os_sys_init(init);                    /* Init RTX and start 'init'          */
}
