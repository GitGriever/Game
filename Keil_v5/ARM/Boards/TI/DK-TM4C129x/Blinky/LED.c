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

#include <TM4C129.h>
#include "LED.h"

const uint32_t led_mask[] = { 1UL << 7 };

/*----------------------------------------------------------------------------
  initialize LED Pins (GPIOQ.7)
 *----------------------------------------------------------------------------*/
void LED_Init (void) {

  /* Enable clock and init GPIO outputs */
  SYSCTL->RCGCGPIO |= (1UL << 14);                /* enable clock for GPIO    */
  GPIOQ->DIR = led_mask[0];                       /* GPIOQ.7 is output        */
  GPIOQ->DEN = led_mask[0];                       /* GPIOQ.7 is digital func. */

}

/*----------------------------------------------------------------------------
  Function that turns on requested LED
 *----------------------------------------------------------------------------*/
void LED_On (unsigned int num) {

  if (num < LED_NUM) {
    GPIOQ->DATA |=  led_mask[num];
  }
}

/*----------------------------------------------------------------------------
  Function that turns off requested LED
 *----------------------------------------------------------------------------*/
void LED_Off (unsigned int num) {

  if (num < LED_NUM) {
    GPIOQ->DATA &= ~led_mask[num];
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
