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
#include <stm32f10x.h>

/* Push Button Definitions                                                    */
#define UP             0x8000                     /* PG15: Joystick up        */
#define LEFT           0x4000                     /* PG14: Joystick left      */
#define RIGHT          0x2000                     /* PG13: Joystick right     */
#define SELECT         0x0080                     /* PG07: Joystick select    */
#define DOWN           0x0008                     /* PD03: Joystick down      */

U8 feat;

void usbd_hid_init (void) {
  /* configure LEDs */
  RCC->APB2ENR |= (1UL << 3);                     /* Enable GPIOB clock       */
                                                                            
  GPIOB->ODR   &= ~0x0000BF00;                    /* switch off LEDs          */
  GPIOB->CRH   &= ~0xF0FFFFFF;                    /* PB.8..13 clear           */
  GPIOB->CRH   |=  0x30333333;                    /* PB.8..13 is output       */

  /* configure Joystick */
  RCC->APB2ENR |= ((1UL << 5) | (1UL << 8));      /* enable GPIOD, GPIOG clock*/

  GPIOG->CRH   &= ~0xFFF00000;                    /* PG.13..15 clear          */
  GPIOG->CRH   |=  0x44400000;                    /* PG.13..15 Floating Input */
  GPIOG->CRL   &= ~0xF0000000;                    /* PG.7 clear               */
  GPIOG->CRL   |=  0x40000000;                    /* PG.7 Floating Input      */

  GPIOD->CRL   &= ~0x0000F000;                    /* PD.3 clear               */
  GPIOD->CRL   |=  0x00004000;                    /* PD.3 Floating Input      */
}

int usbd_hid_get_report (U8 rtype, U8 rid, U8 *buf, U8 req) {

  switch (rtype) {
    case HID_REPORT_INPUT:
      switch (rid) {
         case 0:
          switch (req) {
            case USBD_HID_REQ_EP_CTRL:
            case USBD_HID_REQ_PERIOD_UPDATE:
              *buf = (((GPIOG->IDR ^ UP    ) >> 15) & 0x01) |
                     (((GPIOG->IDR ^ LEFT  ) >> 13) & 0x02) |
                     (((GPIOG->IDR ^ RIGHT ) >> 11) & 0x04) |
                     (((GPIOG->IDR ^ SELECT) >>  4) & 0x08) |
                     (((GPIOD->IDR ^ DOWN  ) <<  1) & 0x10) ;
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
      GPIOB->ODR = ((*buf &0x3F) << 8);	          /* LED Bits PB8..PB13       */
      break;
    case HID_REPORT_FEATURE:
      feat = buf[0];
      break;
  }
}
