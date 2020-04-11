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

#include <samd20.h>
#include "LED.h"

const uint32_t led_mask[] = { PORT_PA14 };

/*----------------------------------------------------------------------------
  initialize LED Pins
 *----------------------------------------------------------------------------*/
void LED_Init(void) {

  PORT->Group[0].DIRSET.reg = led_mask[0];            /* Setup Pins  for LEDs */

  LED_Out (0);                                        /* switch LEDs off      */
}

/*----------------------------------------------------------------------------
  Function that turns on requested LED
 *----------------------------------------------------------------------------*/
void LED_On (uint32_t num) {

  if (num < LED_NUM) {
    PORT->Group[0].OUTCLR.reg = led_mask[num];
  }
}

/*----------------------------------------------------------------------------
  Function that turns off requested LED
 *----------------------------------------------------------------------------*/
void LED_Off (uint32_t num) {

  if (num < LED_NUM) {
    PORT->Group[0].OUTSET.reg = led_mask[num];
  }
}

/*----------------------------------------------------------------------------
  Function that outputs value to LEDs
 *----------------------------------------------------------------------------*/
void LED_Out(uint32_t value) {
  int i;

  for (i = 0; i < LED_NUM; i++) {
    if (value & (1<<i)) {
      LED_On (i);
    } else {
      LED_Off(i);
    }
  }
}

