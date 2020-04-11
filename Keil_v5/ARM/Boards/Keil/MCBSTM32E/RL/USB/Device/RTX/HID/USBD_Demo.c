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
#include <stm32f10x.h>
#include "GLCD.h"

/* Push Button Definitions                                                    */
#define UP             0x8000                     /* PG15: Joystick up        */
#define LEFT           0x4000                     /* PG14: Joystick left      */
#define RIGHT          0x2000                     /* PG13: Joystick right     */
#define SELECT         0x0080                     /* PG07: Joystick select    */
#define DOWN           0x0008                     /* PD03: Joystick down      */

__task void init (void) {
  static U8 but_ex;
         U8 but;
         U8 buf[1];

  GLCD_Init          ();
  GLCD_Clear         (Blue);
  GLCD_SetBackColor  (Blue);
  GLCD_SetTextColor  (White);
  GLCD_DisplayString (4, 0, 1, "  USB Device Demo   ");
  GLCD_DisplayString (5, 0, 1, "   www.keil.com     ");

  os_tsk_prio_self(100);
  usbd_init();                          /* USB Device Initialization          */
  usbd_connect(__TRUE);                 /* USB Device Connect                 */
  os_tsk_prio_self(1);
  while (1) {                           /* Loop forever                       */
    but = (((GPIOG->IDR ^ UP    ) >> 15) & 0x01) |
          (((GPIOG->IDR ^ LEFT  ) >> 13) & 0x02) |
          (((GPIOG->IDR ^ RIGHT ) >> 11) & 0x04) |
          (((GPIOG->IDR ^ SELECT) >>  4) & 0x08) |
          (((GPIOD->IDR ^ DOWN  ) <<  1) & 0x10) ;
    if (but ^ but_ex) {
      buf[0] = but;
      usbd_hid_get_report_trigger(0, buf, 1);
      but_ex = but;
    }
  };
}

int main (void) {

  os_sys_init(init);                    /* Init RTX and start 'init'          */
}
