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
#include <SAM3U.H>

U8 feat;

void usbd_hid_init (void) {

  /* Enable Clock for PIO                                                     */
  PMC->PMC_WPMR  = 0x504D4300;              /* Disable write protect          */
  PMC->PMC_PCER0 = (1 << ID_PIOA);          /* Push Buttons                   */
  PMC->PMC_PCER0 = (1 << ID_PIOB);          /* LEDs                           */
  PMC->PMC_WPMR  = 0x504D4301;              /* Enable write protect           */

  PIOA->PIO_IDR  = 
  PIOA->PIO_ODR  = 
  PIOA->PIO_PER  = (PIO_PA19 | PIO_PA18);   /* Setup Pins PA18..PA19 for Butn */

  PIOB->PIO_IDR  = 
  PIOB->PIO_OER  =
  PIOB->PIO_PER  = (PIO_PB1  | PIO_PB0 );   /* Setup Pins PB0..PB1 for LEDs   */
  PIOB->PIO_SODR = (PIO_PB1  | PIO_PB0 );   /* Turn LEDs Off                  */
}

int usbd_hid_get_report (U8 rtype, U8 rid, U8 *buf, U8 req) {

  switch (rtype) {
    case HID_REPORT_INPUT:
      switch (rid) {
         case 0:
          switch (req) {
            case USBD_HID_REQ_EP_CTRL:
            case USBD_HID_REQ_PERIOD_UPDATE:
              *buf = ((PIOA->PIO_PDSR ^ (PIO_PA18 | PIO_PA19)) >> 18) & 0x03;
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
      PIOB->PIO_CODR =  (*buf) & (PIO_PB0 | PIO_PB1);
      PIOB->PIO_SODR = ~(*buf) & (PIO_PB0 | PIO_PB1);
      break;
    case HID_REPORT_FEATURE:
      feat = buf[0];
      break;
  }
}
