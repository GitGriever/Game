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
#include <LPC13xx.h>

/* Push Button Definitions */
#define KBD_S3          0x00000010      /* P1.4                               */
#define KBD_S4          0x00000020      /* P1.5                               */

__task void init (void) {
  static U8 but_ex;
         U8 but;
         U8 buf[1];

  os_tsk_prio_self(100);
  usbd_init();                          /* USB Device Initialization          */
  usbd_connect(__TRUE);                 /* USB Device Connect                 */
  os_tsk_prio_self(1);
  while (1) {                           /* Loop forever                       */
    but = (((LPC_GPIO1->DATA ^ KBD_S3) >> 4) & 0x01) |
          (((LPC_GPIO1->DATA ^ KBD_S4) >> 4) & 0x02) ;
    if (but ^ but_ex) {
      buf[0] = but;
      usbd_hid_get_report_trigger(0, buf, 1);
      but_ex = but;
    }
  };
}

int main (void) {

  SystemInit ();                        /* Initialize Clocks                  */
  os_sys_init(init);                    /* Init RTX and start 'init'          */
}
