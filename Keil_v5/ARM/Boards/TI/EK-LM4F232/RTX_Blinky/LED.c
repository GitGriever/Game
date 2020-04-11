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

#include <TM4C123.h>
#include "LED.h"

const uint32_t led_mask[] = { 1UL << 2 };

/*----------------------------------------------------------------------------
  initialize LED Pins (GPIOG.2)
 *----------------------------------------------------------------------------*/
void LED_Init (void) {

  /* Enable clock and init GPIO outputs */
  SYSCTL->RCGCGPIO |= (1UL <<  6);                /* enable clock for GPIO    */
  GPIOG->DIR = led_mask[0];                       /* GPIOG.2 is output        */
  GPIOG->DEN = led_mask[0];                       /* GPIOG.2 is digital func. */

}

/*----------------------------------------------------------------------------
  Function that turns on requested LED
 *----------------------------------------------------------------------------*/
void LED_On (unsigned int num) {

  if (num < LED_NUM) {
    GPIOG->DATA |=  led_mask[num];
  }
}

/*----------------------------------------------------------------------------
  Function that turns off requested LED
 *----------------------------------------------------------------------------*/
void LED_Off (unsigned int num) {

  if (num < LED_NUM) {
    GPIOG->DATA &= ~led_mask[num];
  }
}

/*----------------------------------------------------------------------------
  Function that outputs value to LEDs
 *----------------------------------------------------------------------------*/
void LED_Out(unsigned int value) {
  int i;

  for (i = 0; i < LED_NUM; i++) {
    if (value & (1<<i)) {
      LED_On (i);
    } else {
      LED_Off(i);
    }
  }
}
