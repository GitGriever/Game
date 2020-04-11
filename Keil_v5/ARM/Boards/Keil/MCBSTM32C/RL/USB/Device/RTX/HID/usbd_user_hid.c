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
#include <stm32f10x_cl.h>

const long led_mask[] = { 1<<15, 1<<14, 1<<13, 1<<12, 1<<11, 1<<10, 1<<9, 1<<8 };

U8 feat;

void usbd_hid_init (void) {
  U32 i;

  RCC->APB2ENR |=  1 <<  6;             /* Enable GPIOE clock                 */
  RCC->APB2ENR |=  1 <<  5;             /* Enable GPIOD clock                 */
  GPIOE->CRH    = 0x33333333;           /* Configure the GPIO for LEDs        */
  for (i = 0; i < 8; i++)               /* Clear leds                         */
    GPIOE->BSRR = led_mask[i] << 16;

  GPIOD->CRH  = (GPIOD->CRL & ~0xfffff000) | 0x88888000;  /* PD11-PD15 inputs */
  GPIOD->ODR |= (0x1f << 11);                             /* PD11-PD15 pull-up*/

}

int usbd_hid_get_report (U8 rtype, U8 rid, U8 *buf, U8 req) {

  switch (rtype) {
    case HID_REPORT_INPUT:
      switch (rid) {
         case 0:
          switch (req) {
            case USBD_HID_REQ_EP_CTRL:
            case USBD_HID_REQ_PERIOD_UPDATE:
             *buf = ~(((GPIOD->IDR >> 11) & 0x1F ) | 0xE0);
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
  U32 i;

  switch (rtype) {
    case HID_REPORT_OUTPUT:
      for (i = 0; i < 8; i++) {
        if(*buf & (1 << i))
          GPIOE->BSRR = led_mask[i];
        else
          GPIOE->BSRR = led_mask[i] << 16;
      }
      break;
    case HID_REPORT_FEATURE:
      feat = buf[0];
      break;
  }
}
