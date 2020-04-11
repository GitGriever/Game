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

#include "demo.h"


U8 InReport;                                         /* HID Input Report      */
                                                     /*   Bit0..1: Buttons    */
                                                     /*   Bit2..7: Reserved   */

U8 OutReport;                                        /* HID Out Report        */
                                                     /*   Bit0..7: LEDs       */


/*----------------------------------------------------------------------------
  Get HID Input Report -> InReport
 *----------------------------------------------------------------------------*/
void GetInReport (void) {

  InReport = 0x00;
  if ((GPIOG->IDR & UP)     == 0) InReport |= 0x01; /* up     pressed means 0 */
  if ((GPIOG->IDR & LEFT)   == 0) InReport |= 0x02; /* left   pressed means 0 */
  if ((GPIOG->IDR & RIGHT)  == 0) InReport |= 0x04; /* right  pressed means 0 */
  if ((GPIOG->IDR & SELECT) == 0) InReport |= 0x08; /* select pressed means 0 */
  if ((GPIOD->IDR & DOWN)   == 0) InReport |= 0x10; /* down   pressed means 0 */
}


/*----------------------------------------------------------------------------
  Set HID Output Report <- OutReport
 *----------------------------------------------------------------------------*/
void SetOutReport (void) {

  GPIOB->ODR = ((OutReport &0x3F) << 8);	   /* LED Bits PB8..PB13 are set  */
}


/*----------------------------------------------------------------------------
  MAIN function
 *----------------------------------------------------------------------------*/
int main (void) {

  /* configure LEDs */
  RCC->APB2ENR |= (1UL << 3);                  /* Enable GPIOB clock          */

  GPIOB->ODR   &= ~0x0000BF00;                 /* switch off LEDs             */
  GPIOB->CRH   &= ~0xF0FFFFFF;                 /* PB.8..13 clear              */
  GPIOB->CRH   |=  0x30333333;                 /* PB.8..13 is output          */

  /* configure Joystick */
  RCC->APB2ENR |= ((1UL << 5) | (1UL << 8));   /* enable GPIOD, GPIOG clock   */

  GPIOG->CRH   &= ~0xFFF00000;                 /* PG.13..15 clear             */
  GPIOG->CRH   |=  0x44400000;                 /* PG.13..15 is Floating Input */
  GPIOG->CRL   &= ~0xF0000000;                 /* PG.7 clear                  */
  GPIOG->CRL   |=  0x40000000;                 /* PG.7 is Floating Input      */

  GPIOD->CRL   &= ~0x0000F000;                 /* PD.3 clear                  */
  GPIOD->CRL   |=  0x00004000;                 /* PD.3 is Floating Input      */

  USB_Init();                                  /* USB Initialization          */
  USB_Connect(__TRUE);                         /* USB Connect                 */

  while (1);                                   /* Loop forever                */
}
