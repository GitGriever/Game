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
#include <LPC11Uxx.H>

U8 feat;

void usbd_hid_init (void) {

  LPC_SYSCON->SYSAHBCLKCTRL |= (1UL <<  6);     /* enable clock for GPIO      */

  /* configure GPIO as output */
  LPC_GPIO->DIR[1]  |=  (0xFF << 19);
  LPC_GPIO->CLR[1]  |=  (0xFF << 19);
  LPC_GPIO->DIR[0]  &= ~((1UL << 16) | (1UL << 17));
}

int usbd_hid_get_report (U8 rtype, U8 rid, U8 *buf, U8 req) {

  switch (rtype) {
    case HID_REPORT_INPUT:
      switch (rid) {
         case 0:
          switch (req) {
            case USBD_HID_REQ_EP_CTRL:
            case USBD_HID_REQ_PERIOD_UPDATE:
              *buf = (((LPC_GPIO->PIN[0] ^ (1UL << 16)) >> 16) & 0x01) |
                     (((LPC_GPIO->PIN[0] ^ (1UL << 17)) >> 16) & 0x02) ;
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
      LPC_GPIO->SET[1] = (( ((U32)buf[0])) & 0xFF) << 19;
      LPC_GPIO->CLR[1] = ((~((U32)buf[0])) & 0xFF) << 19;
      break;
    case HID_REPORT_FEATURE:
      feat = buf[0];
      break;
  }
}
