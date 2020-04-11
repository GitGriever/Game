/*----------------------------------------------------------------------------
 * Name:    LED.c
 * Purpose: Low Level LED functions
 * Note(s): 
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2012 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include "XMC4500.h"                         /* XMC4500 definitions           */
#include "LED.h"

const unsigned long led_mask[] = {1UL << 9 };              /* GPIO P3.9       */

/*----------------------------------------------------------------------------
  initialize LED Pins
 *----------------------------------------------------------------------------*/
void LED_Init (void) {

  /* configure PORT P3.9 pin as output */
  PORT3->IOCR8  &=  ~(0xFFUL << 8);          /* clear P3.9 settings           */
  PORT3->IOCR8  |=   (0x80UL << 8);          /* P3.9 output push pull         */
  PORT3->PDR1   &=  ~(0x07UL << 4);          /* clear P3.9 output settings    */
  PORT3->PDR1   |=   (0x02UL << 4);          /* P3.9 output strong            */

  LED_Out (0);                               /* switch LEDs off               */
}


/*----------------------------------------------------------------------------
  Function that turns on requested LED
 *----------------------------------------------------------------------------*/
void LED_On (unsigned int num) {

  if (num < LED_NUM) {
    PORT3->OMR = (led_mask[num] << 16);
  }
}

/*----------------------------------------------------------------------------
  Function that turns off requested LED
 *----------------------------------------------------------------------------*/
void LED_Off (unsigned int num) {

  if (num < LED_NUM) {
    PORT3->OMR = (led_mask[num]      );
  }
}

/*----------------------------------------------------------------------------
  Function that outputs value to LEDs
 *----------------------------------------------------------------------------*/
void LED_Out(unsigned int value) {
  int i;

  for (i = 0; i < LED_NUM; i++) {
    if (value & (1 << i)) {
      LED_On (i);
    } else {
      LED_Off(i);
    }
  }
}
