/*----------------------------------------------------------------------------
 *      RL-ARM - USB
 *----------------------------------------------------------------------------
 *      Name:    usbd_user_hid.c
 *      Purpose: Human Interface Device Class User module
 *      Rev.:    V4.70
 *----------------------------------------------------------------------------
 *      This code is part of the RealView Run-Time Library.
 *      Copyright (c) 2004-2013 KEIL - An ARM Company. All rights reserved.
 *---------------------------------------------------------------------------*/

#include <RTL.h>
#include <rl_usb.h>
#include <LPC13xx.H>

/* Push Button Definitions */
#define KBD_MASK        0x00000030      /* P1.4..5                            */
#define KBD_S3          0x00000010      /* P1.4                               */
#define KBD_S4          0x00000020      /* P1.5                               */

/* LED Definitions */
#define LED_NUM         8               /* Number of user LEDs                */
#define LED_MASK        0x000000FF      /* P2.0..7                            */

U8 feat;

void usbd_hid_init (void) {

  LPC_SYSCON->SYSAHBCLKCTRL |= 1 << 6;  /* Enable clock for GPIO              */
  LPC_GPIO1->DIR  &= ~KBD_MASK;         /* Push Button on P0.1 is input       */
  LPC_GPIO2->DIR  |=  LED_MASK;         /* LEDs on PORT2 defined as Output    */
  LPC_GPIO2->DATA &= ~LED_MASK;         /* LEDs on PORT2 off                  */
}

int usbd_hid_get_report (U8 rtype, U8 rid, U8 *buf, U8 req) {

  switch (rtype) {
    case HID_REPORT_INPUT:
      switch (rid) {
         case 0:
          switch (req) {
            case USBD_HID_REQ_EP_CTRL:
            case USBD_HID_REQ_PERIOD_UPDATE:
              *buf = (((LPC_GPIO1->DATA ^ KBD_S3) >> 4) & 0x01) |
                     (((LPC_GPIO1->DATA ^ KBD_S4) >> 4) & 0x02) ;
              return (1);
            case USBD_HID_REQ_EP_INT:
              break;
          }
           break;
      }
      break;
    case HID_REPORT_FEATURE:
      buf[0] = feat;
      return (1);
  }
  return (0);
}

void usbd_hid_set_report (U8 rtype, U8 rid, U8 *buf, int len, U8 req) {

  switch (rtype) {
    case HID_REPORT_OUTPUT:
      LPC_GPIO2->DATA |=   *buf;
      LPC_GPIO2->DATA &= ~(*buf);
      break;
    case HID_REPORT_FEATURE:
      feat = buf[0];
      break;
  }
}
