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


U8 InReport;                                      /* HID Input Report         */
                                                  /*   Bit0..7: Buttons       */

U8 OutReport;                                     /* HID Out Report           */
                                                  /*   Bit0..7: LEDs          */


/*----------------------------------------------------------------------------
  Get HID Input Report -> InReport
 *----------------------------------------------------------------------------*/
void GetInReport (void) {

  InReport = 0x00;
  if ((GPIOE->IDR & SW1) == 0) InReport |= 0x01;    // SW1 pressed means 0
  if ((GPIOE->IDR & SW2) == 0) InReport |= 0x02;    // SW2 pressed means 0
  if ((GPIOE->IDR & SW3) == 0) InReport |= 0x04;    // SW3 pressed means 0
  if ((GPIOE->IDR & SW4) == 0) InReport |= 0x08;    // SW4 pressed means 0
  if ((GPIOE->IDR & SW5) == 0) InReport |= 0x10;    // SW5 pressed means 0
  if ((GPIOE->IDR & SW6) == 0) InReport |= 0x20;    // SW6 pressed means 0
  if ((GPIOE->IDR & SW7) == 0) InReport |= 0x40;    // SW7 pressed means 0
  if ((GPIOE->IDR & SW8) == 0) InReport |= 0x80;    // SW8 pressed means 0
}


/*----------------------------------------------------------------------------
  Set HID Output Report <- OutReport
 *----------------------------------------------------------------------------*/
void SetOutReport (void) {

  if (OutReport & 0x01) GPIOD->BSRR |= (1<<15); else GPIOD->BRR = (1<<15); 
  if (OutReport & 0x02) GPIOD->BSRR |= (1<< 8); else GPIOD->BRR = (1<< 8); 
  if (OutReport & 0x04) GPIOE->BSRR |= (1<<14); else GPIOE->BRR = (1<<14); 
  if (OutReport & 0x08) GPIOD->BSRR |= (1<<11); else GPIOD->BRR = (1<<11); 
  if (OutReport & 0x10) GPIOC->BSRR |= (1<<11); else GPIOC->BRR = (1<<11); 
  if (OutReport & 0x20) GPIOD->BSRR |= (1<< 2); else GPIOD->BRR = (1<< 2); 
  if (OutReport & 0x40) GPIOD->BSRR |= (1<< 7); else GPIOD->BRR = (1<< 7); 
  if (OutReport & 0x80) GPIOD->BSRR |= (1<<12); else GPIOD->BRR = (1<<12); 
}


/*----------------------------------------------------------------------------
  MAIN function
 *----------------------------------------------------------------------------*/
int main (void) {

  /* configure LEDs */
  RCC->APB2ENR |= ((1UL << 4) |                   /* Enable GPIOC clock       */
                   (1UL << 5) |                   /* Enable GPIOD clock       */
                   (1UL << 6)  );                 /* Enable GPIOE clock       */

  GPIOC->ODR   &= ~0x00000800;                    /* switch off LEDs          */
  GPIOC->CRH   &= ~0x0000F000;                    /* PC.11 clear              */
  GPIOC->CRH   |=  0x00003000;                    /* PC.11 is output          */

  GPIOD->ODR   &= ~0x00009984;                    /* switch off LEDs          */
  GPIOD->CRL   &= ~0xF0000F00;                    /* PD.2,7 clear             */
  GPIOD->CRL   |=  0x30000300;                    /* PD.2,7 is output         */
  GPIOD->CRH   &= ~0xF00FF00F;                    /* PD.8,11,12,15 clear      */
  GPIOD->CRH   |=  0x30033003;                    /* PD.8,11,12,15 is output  */

  GPIOE->ODR   &= ~0x00004000;                    /* switch off LEDs          */
  GPIOE->CRH   &= ~0x0F000000;                    /* PE.14 clear              */
  GPIOE->CRH   |=  0x03000000;                    /* PE.14 is output          */

  /* configure Push Buttons */
  RCC->APB2ENR |= (1UL << 6);                     /* enable GPIOE             */

  GPIOE->CRL    =  0x44444444;                    /* PE.0..7 is Floating Input*/

  USB_Init();                                     /* USB Initialization       */
  USB_Connect(__TRUE);                            /* USB Connect              */

  while (1);                                      /* Loop forever             */
}
