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
#include <LPC17xx.H>

#define LED_NUM 8

/* Push Button Definitions */
#define KBD_SELECT      0x01
#define KBD_LEFT        0x08
#define KBD_UP          0x10
#define KBD_RIGHT       0x20
#define KBD_DOWN        0x40
#define KBD_MASK        0x79

const unsigned long led_mask[] = { 1<<28, 1<<29, 1UL<<31, 1<<2, 1<<3, 1<<4, 1<<5, 1<<6 };

U8 feat;

void usbd_hid_init (void) {
  int i;

  for (i = 0; i < LED_NUM; i++) {
    if (i < 3) { 
      LPC_GPIO1->FIODIR |= led_mask[i]; /* output                             */
      LPC_GPIO1->FIOCLR |= led_mask[i]; /* Initially all LED turn off         */
    }
    else {
      LPC_GPIO2->FIODIR |= led_mask[i]; /* output                             */
      LPC_GPIO2->FIOCLR |= led_mask[i]; /* Initially all LED turn off         */
    }
  }
}

int usbd_hid_get_report (U8 rtype, U8 rid, U8 *buf, U8 req) {

  switch (rtype) {
    case HID_REPORT_INPUT:
      switch (rid) {
         case 0:
          switch (req) {
            case USBD_HID_REQ_EP_CTRL:
            case USBD_HID_REQ_PERIOD_UPDATE:
              *buf = (((LPC_GPIO1->FIOPIN ^ (1 << 20)) >> 20) & 0x01) |
                     (((LPC_GPIO1->FIOPIN ^ (1 << 23)) >> 22) & 0x02) |
                     (((LPC_GPIO1->FIOPIN ^ (1 << 24)) >> 22) & 0x04) |
                     (((LPC_GPIO1->FIOPIN ^ (1 << 25)) >> 22) & 0x08) |
                     (((LPC_GPIO1->FIOPIN ^ (1 << 26)) >> 22) & 0x10) ;            
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
      ((*buf & 0x01) ? (LPC_GPIO1->FIOPIN |= led_mask[0]) : (LPC_GPIO1->FIOPIN &= ~led_mask[0])); 
      ((*buf & 0x02) ? (LPC_GPIO1->FIOPIN |= led_mask[1]) : (LPC_GPIO1->FIOPIN &= ~led_mask[1])); 
      ((*buf & 0x04) ? (LPC_GPIO1->FIOPIN |= led_mask[2]) : (LPC_GPIO1->FIOPIN &= ~led_mask[2])); 
      ((*buf & 0x08) ? (LPC_GPIO2->FIOPIN |= led_mask[3]) : (LPC_GPIO2->FIOPIN &= ~led_mask[3])); 
      ((*buf & 0x10) ? (LPC_GPIO2->FIOPIN |= led_mask[4]) : (LPC_GPIO2->FIOPIN &= ~led_mask[4])); 
      ((*buf & 0x20) ? (LPC_GPIO2->FIOPIN |= led_mask[5]) : (LPC_GPIO2->FIOPIN &= ~led_mask[5])); 
      ((*buf & 0x40) ? (LPC_GPIO2->FIOPIN |= led_mask[6]) : (LPC_GPIO2->FIOPIN &= ~led_mask[6])); 
      ((*buf & 0x80) ? (LPC_GPIO2->FIOPIN |= led_mask[7]) : (LPC_GPIO2->FIOPIN &= ~led_mask[7])); 
      break;
    case HID_REPORT_FEATURE:
      feat = buf[0];
      break;
  }
}
