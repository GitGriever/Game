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
#include <stm32f2xx.h>

U8 feat;

void usbd_hid_init (void) {

  /* clock enable for port_A, port_C, port_G and port_I                       */
  RCC->AHB1ENR   |= 1 | (1 <<  2) | (1 <<  6) | (1 << 8);
  
  
  /* LED pins config: Set pins G6, G8, I9 and C7 as 50MHz push-pull outputs   */
  GPIOG->MODER    =   (GPIOG->MODER  & ~(3 << 12)) | (1 << 12);
  GPIOG->MODER    =   (GPIOG->MODER  & ~(3 << 16)) | (1 << 16);
  GPIOI->MODER    =   (GPIOI->MODER  & ~(3 << 18)) | (1 << 18);
  GPIOC->MODER    =   (GPIOC->MODER  & ~(3 << 14)) | (1 << 14);
  GPIOG->OTYPER  &=  ~(1 <<  6);
  GPIOG->OTYPER  &=  ~(1 <<  8);
  GPIOI->OTYPER  &=  ~(1 <<  9);
  GPIOC->OTYPER  &=  ~(1 <<  7);
  GPIOH->OSPEEDR  =   (GPIOH->OSPEEDR & ~(3 << 12)) | (2 << 12);
  GPIOH->OSPEEDR  =   (GPIOH->OSPEEDR & ~(3 << 16)) | (2 << 16);
  GPIOI->OSPEEDR  =   (GPIOI->OSPEEDR & ~(3 << 18)) | (2 << 18);
  GPIOC->OSPEEDR  =   (GPIOC->OSPEEDR & ~(3 << 14)) | (2 << 14);

  /* Push buttons config: Set pins A0, C13, G15 as 50MHz inputs               */
  GPIOA->MODER   &=  ~(1 <<  0);
  GPIOC->MODER   &=  ~(1 << 26);
  GPIOG->MODER   &=  ~(1 << 30);
  GPIOA->OSPEEDR  =   (GPIOA->OSPEEDR & ~(3   <<  0)) | (2   <<  0);
  GPIOC->OSPEEDR  =   (GPIOC->OSPEEDR & ~(3   << 26)) | (2   << 26);
  GPIOG->OSPEEDR  =   (GPIOG->OSPEEDR & ~(3UL << 30)) | (2UL << 30);
  GPIOA->PUPDR    =   (GPIOA->PUPDR   & ~(3   <<  0)) | (2   <<  0);
  GPIOC->PUPDR    =   (GPIOC->PUPDR   & ~(3   << 26)) | (1   << 26);
  GPIOG->PUPDR    =   (GPIOG->PUPDR   & ~(3UL << 30)) | (1   << 30);
}

int usbd_hid_get_report (U8 rtype, U8 rid, U8 *buf, U8 req) {

  switch (rtype) {
    case HID_REPORT_INPUT:
      switch (rid) {
         case 0:
          switch (req) {
            case USBD_HID_REQ_EP_CTRL:
            case USBD_HID_REQ_PERIOD_UPDATE:
              /* read wakeup, tamper and key push buttons                     */
              *buf = (((GPIOA->IDR & (1 <<  0)) >>  0) & 0x01) |
                     (((GPIOC->IDR ^ (1 << 13)) >> 12) & 0x02) |                                        
                     (((GPIOG->IDR ^ (1 << 15)) >> 13) & 0x04) ;                                        
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
      ((*buf & 1) ? (GPIOG->BSRRL |= (1 << 6)) : (GPIOG->BSRRH |= (1 << 6))); 
      ((*buf & 2) ? (GPIOG->BSRRL |= (1 << 8)) : (GPIOG->BSRRH |= (1 << 8))); 
      ((*buf & 4) ? (GPIOI->BSRRL |= (1 << 9)) : (GPIOI->BSRRH |= (1 << 9))); 
      ((*buf & 8) ? (GPIOC->BSRRL |= (1 << 7)) : (GPIOC->BSRRH |= (1 << 7))); 
      break;
    case HID_REPORT_FEATURE:
      feat = buf[0];
      break;
  }
}
