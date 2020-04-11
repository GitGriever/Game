/*----------------------------------------------------------------------------
 * Name:    demo.c
 * Purpose: USB HID Demo
 * Note(s): 
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2009-2011 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include "STM32F10x.h"                            /* STM32F10x definitions    */

#include "type.h"

#include "usb.h"
#include "usbcfg_STM32F10x.h"
#include "usbhw_STM32F10x.h"

#include "LCD.h"
#include "demo.h"


U8 InReport;                                      /* HID Input Report         */
                                                  /*   Bit0..1: Buttons       */
                                                  /*   Bit2..7: Reserved      */

U8 OutReport;                                     /* HID Out Report           */
                                                  /*   Bit0..7: LEDs          */


/*----------------------------------------------------------------------------
  Get HID Input Report -> InReport
 *----------------------------------------------------------------------------*/
void GetInReport (void) {

  InReport = 0x00;
  if ((GPIOA->IDR & S2) == 0) InReport |= 0x01;   /* S2 pressed means 0       */
  if ((GPIOC->IDR & S3) == 0) InReport |= 0x02;   /* S3 pressed means 0       */
}


/*----------------------------------------------------------------------------
  Set HID Output Report <- OutReport
 *----------------------------------------------------------------------------*/
void SetOutReport (void) {

  GPIOB->ODR = (OutReport << 8);                  /* All LED Bits are set     */
}


/*----------------------------------------------------------------------------
  MAIN function
 *----------------------------------------------------------------------------*/
int main (void) {

  /* configure LEDs */
  RCC->APB2ENR |= (1UL << 3);                     /* Enable GPIOB clock       */

  GPIOB->ODR   &= ~0x0000FF00;                    /* switch off LEDs          */
  GPIOB->CRH   &= ~0xFFFFFFFF;                    /* PB.8..15 clear           */
  GPIOB->CRH   |=  0x33333333;                    /* PB.8..15 is output       */

  /* configure Push Buttons */
  RCC->APB2ENR |= ((1UL << 2) | (1UL << 4));      /* enable GPIOA, GPIOC clock*/

  GPIOA->CRL   &= ~0x0000000F;                    /* PA.0 clear               */
  GPIOA->CRL   |=  0x00000004;                    /* PA.0 is Floating Input   */

  GPIOC->CRH   &= ~0x00F00000;                    /* PC.13 clear              */
  GPIOC->CRH   |=  0x00400000;                    /* PC.13 is Floating Input  */

  lcd_init();
  lcd_clear();
  lcd_print ("  MCBSTM32 HID  ");
  set_cursor (0, 1);
  lcd_print ("  www.keil.com  ");

  USB_Init();                                     /* USB Initialization       */
  USB_Connect(__TRUE);                            /* USB Connect              */

  while (1);                                      /* Loop forever             */
}
