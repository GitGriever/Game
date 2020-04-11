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
 * Copyright (c) 2012 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <LPC11Uxx.h>
#include "LED.h"

const unsigned long led_mask[] = { 1UL << 22, 1UL << 23 };


/*----------------------------------------------------------------------------
  initialize LED Pins
 *----------------------------------------------------------------------------*/
void LED_Init (void) {

  LPC_SYSCON->SYSAHBCLKCTRL |= (1UL <<  6);     /* enable clock for GPIO      */

  /* configure GPIO as output */
  LPC_GPIO->DIR[0]  |= (led_mask[0] | led_mask[1]);
  LPC_GPIO->SET[0]   = (led_mask[0] | led_mask[1]);
}

/*----------------------------------------------------------------------------
  Function that turns on requested LED
 *----------------------------------------------------------------------------*/
void LED_On (unsigned int num) {

  if (num < LED_NUM) {
    LPC_GPIO->CLR[0]  = led_mask[num];
  }
}

/*----------------------------------------------------------------------------
  Function that turns off requested LED
 *----------------------------------------------------------------------------*/
void LED_Off (unsigned int num) {

  if (num < LED_NUM) {
    LPC_GPIO->SET[0]  = led_mask[num];
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
