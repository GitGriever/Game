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
#include <SAM3S.H>

#define LED_NUM 2

U8 feat;

void usbd_hid_init (void) {

  PMC->PMC_WPMR = 0x504D4300;           /* Disable write protect              */

  /* Enable Clock for PIO                                                     */
  PMC->PMC_PCER0 = ((1 << ID_PIOB) |    /* Enable clock for USRPB1            */
                    (1 << ID_PIOC) );   /* Enable clock for USRPB2            */

  PIOB->PIO_IDR  = 
  PIOB->PIO_ODR  = 
  PIOB->PIO_PER  =   PIO_PB3;           /* Setup Pins PB3 for Button USRPB1   */

  PIOC->PIO_IDR  = 
  PIOC->PIO_ODR  = 
  PIOC->PIO_PER  =   PIO_PC12;          /* Setup Pins PC12 for Button USRPB2  */

#ifdef __REV_A
  PMC->PMC_PCER0 = (1 << ID_PIOC);      /* Enable clock for LEDs              */

  PIOC->PIO_IDR   =                     /* Setup Pins PC20..PC21 for LEDs     */
  PIOC->PIO_OER   =
  PIOC->PIO_PER   = PIO_PC20 | PIO_PC21;
  PIOC->PIO_SODR  = PIO_PC20 | PIO_PC21;/* Turn LEDs Off                      */
#else
  PMC->PMC_PCER0 = (1 << ID_PIOA);      /* Enable clock for LEDs              */

  PIOA->PIO_IDR   =                     /* Setup Pins PC20..PC21 for LEDs     */
  PIOA->PIO_OER   =
  PIOA->PIO_PER   = PIO_PA19 | PIO_PA20;
  PIOA->PIO_SODR  = PIO_PA19 | PIO_PA20;/* Turn LEDs Off                      */
#endif
}

int usbd_hid_get_report (U8 rtype, U8 rid, U8 *buf, U8 req) {

  switch (rtype) {
    case HID_REPORT_INPUT:
      switch (rid) {
         case 0:
          switch (req) {
            case USBD_HID_REQ_EP_CTRL:
            case USBD_HID_REQ_PERIOD_UPDATE:
              *buf = (((PIOB->PIO_PDSR ^ PIO_PB3 ) >>  3) & 0x01) |
                     (((PIOC->PIO_PDSR ^ PIO_PC12) >> 11) & 0x02) ;
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
#ifdef __REV_A
      PIOC->PIO_CODR =  (*buf << 20) & (PIO_PC21 | PIO_PC20);
      PIOC->PIO_SODR = ~(*buf << 20) & (PIO_PC21 | PIO_PC20);
#else
      PIOA->PIO_CODR =  (*buf << 19) & (PIO_PA20 | PIO_PA19);
      PIOA->PIO_SODR = ~(*buf << 19) & (PIO_PA20 | PIO_PA19);
#endif
      break;
    case HID_REPORT_FEATURE:
      feat = buf[0];
      break;
  }
}
