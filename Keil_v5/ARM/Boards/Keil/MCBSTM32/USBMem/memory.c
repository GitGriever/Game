/*----------------------------------------------------------------------------
 * Name:    memory.c
 * Purpose: USB Memory Storage Demo
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
#include "mscuser.h"

#include "LCD.h"
#include "memory.h"

extern U8 Memory[MSC_MemorySize];                 /* MSC Memory in RAM        */

/*----------------------------------------------------------------------------
  Switch on LEDs
 *----------------------------------------------------------------------------*/
void LED_On (unsigned int value) {

  GPIOB->BSRR = (value);                          /* Turn On  LED             */
}


/*----------------------------------------------------------------------------
  Switch off LEDs
 *----------------------------------------------------------------------------*/
void LED_Off (unsigned int value) {

  GPIOB->BRR  = (value);                          /* Turn Off LED             */
}


/*----------------------------------------------------------------------------
  Main Program
 *----------------------------------------------------------------------------*/
int main (void) {
  U32 n;

  for (n = 0; n < MSC_ImageSize; n++) {           /* Copy Initial Disk Image  */
    Memory[n] = DiskImage[n];                     /*   from Flash to RAM      */
  }

  /* configure LEDs */
  RCC->APB2ENR |= (1UL << 3);                     /* Enable GPIOB clock       */

  GPIOB->ODR   &= ~0x0000FF00;                    /* switch off LEDs          */
  GPIOB->CRH   &= ~0xFFFFFFFF;                    /* PB.8..15 clear           */
  GPIOB->CRH   |=  0x33333333;                    /* PB.8..15 is output       */


  lcd_init();
  lcd_clear();
  lcd_print ("  MCBSTM32 MSD  ");
  set_cursor (0, 1);
  lcd_print ("  www.keil.com  ");

  USB_Init();                                     /* USB Initialization       */
  USB_Connect(__TRUE);                            /* USB Connect              */

  while (1);                                      /* Loop forever             */
}
