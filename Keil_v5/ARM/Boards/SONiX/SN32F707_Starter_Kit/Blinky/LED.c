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

#include <SN32F700.h>
#include "LED.h"

const uint32_t led_mask[] = { 1UL << 0, 1UL << 1 };

/*----------------------------------------------------------------------------
  initialize LED Pins (GPIO2.0..1)
 *----------------------------------------------------------------------------*/
void LED_Init (void) {

  /* Enable clock and init GPIO outputs */
  SN_SYS1->AHBCLKEN |= (1UL << 3);                  /* enable clock for GPIO */
  SN_GPIO2->MODE |= (led_mask[0] | led_mask[1]);    /* GPIO2.0..1 are output */

}

/*----------------------------------------------------------------------------
  Function that turns on requested LED
 *----------------------------------------------------------------------------*/
void LED_On (unsigned int num) {

  if (num < LED_NUM) {
    SN_GPIO2->BSET = led_mask[num];
  }
}

/*----------------------------------------------------------------------------
  Function that turns off requested LED
 *----------------------------------------------------------------------------*/
void LED_Off (unsigned int num) {

  if (num < LED_NUM) {
    SN_GPIO2->BCLR = led_mask[num];
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
