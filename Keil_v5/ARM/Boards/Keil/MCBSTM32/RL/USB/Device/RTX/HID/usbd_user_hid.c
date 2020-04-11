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
#include <stm32f10x.h>                         /* STM32F10x Definitions      */

/* Push Button Definitions                                                    */
#define S3              0x2000          /* PC13: S3                           */
#define S2              0x0001          /* PA0 : S2                           */

U8 feat;

void usbd_hid_init (void) {
  /* configure LEDs */
  RCC->APB2ENR |= (1UL << 3);                 /* Enable GPIOB clock          */

  GPIOB->ODR   &= ~(0xFF << 8);
  GPIOB->CRH   &= ~0xFFFFFFFF;                /* PB.8..15 clear              */
  GPIOB->CRH   |=  0x33333333;                /* PB.8..15 is output          */

  /* configure Push Buttons */
  RCC->APB2ENR |= ((1UL << 2) | (1UL << 4));  /* enable GPIOA, GPIOC clock   */

  GPIOA->CRL   &= ~0x0000000F;                /* PA.0 clear                  */
  GPIOA->CRL   |=  0x00000004;                /* PA.0 is Floating Input      */

  GPIOC->CRH   &= ~0x00F00000;                /* PC.13 clear                 */
  GPIOC->CRH   |=  0x00400000;                /* PC.13 is Floating Input     */
}

int usbd_hid_get_report (U8 rtype, U8 rid, U8 *buf, U8 req) {

  switch (rtype) {
    case HID_REPORT_INPUT:
      switch (rid) {
         case 0:
          switch (req) {
            case USBD_HID_REQ_EP_CTRL:
            case USBD_HID_REQ_PERIOD_UPDATE:
              *buf = (((GPIOA->IDR ^ S2)      ) & 0x01) |
                     (((GPIOC->IDR ^ S3) >> 12) & 0x02) ;
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
      GPIOB->ODR = (*buf << 8) | (GPIOB->ODR & (~(0xFF << 8)));
      break;
    case HID_REPORT_FEATURE:
      feat = buf[0];
      break;
  }
}
