/*----------------------------------------------------------------------------
 * Name:    LED.c
 * Purpose: low level LED functions
 * Note(s):
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2013 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <XMC1200.h>
#include "LED.h"

/* LEDs : P0.0, P0.2, P0.5, P0.6, P0,7 */
const uint32_t led_mask[] = {1UL << 0, 1UL << 2, 1UL << 5, 1UL << 6, 1UL << 7};

/*----------------------------------------------------------------------------
  initialize LED Pins
 *----------------------------------------------------------------------------*/
void LED_Init (void) {

  /* configure ports as output push pull*/
  PORT0->IOCR0  &=  ~((0xFFUL <<  0));       /* clear P0.0 settings           */
  PORT0->IOCR0  |=    (0x80UL <<  0);        /* P0.0 output push pull         */

  PORT0->IOCR0  &=  ~((0xFFUL << 16));       /* clear P0.2 settings           */
  PORT0->IOCR0  |=    (0x80UL << 16);        /* P0.2 output push pull         */

  PORT0->IOCR4  &=  ~((0xFFUL <<  8));       /* clear P0.5 settings           */
  PORT0->IOCR4  |=    (0x80UL <<  8);        /* P0.5 output push pull         */

  PORT0->IOCR4  &=  ~((0xFFUL << 16));       /* clear P0.6 settings           */
  PORT0->IOCR4  |=    (0x80UL << 16);        /* P0.6 output push pull         */

  PORT0->IOCR4  &=  ~((0xFFUL << 24));       /* clear P0.7 settings           */
  PORT0->IOCR4  |=    (0x80UL << 24);        /* P0.7 output push pull         */

  LED_Out (0);                               /* switch LEDs off               */
}

/*----------------------------------------------------------------------------
  Function that turns on requested LED
 *----------------------------------------------------------------------------*/
void LED_On (unsigned int num) {

  if (num < LED_NUM) {
    PORT0->OMR = (led_mask[num] << 16);
  }
}

/*----------------------------------------------------------------------------
  Function that turns off requested LED
 *----------------------------------------------------------------------------*/
void LED_Off (unsigned int num) {

  if (num < LED_NUM) {
    PORT0->OMR = (led_mask[num]      );
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
