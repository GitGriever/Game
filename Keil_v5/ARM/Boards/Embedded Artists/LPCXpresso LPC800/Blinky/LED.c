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

#include <LPC8xx.h>
#include "LED.h"

const uint32_t led_mask[] = { 1UL << 7, 1UL << 16, 1UL << 17 };

/*----------------------------------------------------------------------------
  initialize LED Pins
 *----------------------------------------------------------------------------*/
void LED_Init(void) {

  LPC_SYSCON->SYSAHBCLKCTRL |= (1UL << 6);               /* enable GPIO clock */

	LPC_GPIO_PORT->DIR0 |= (led_mask[0] |           /* configure GPIO as output */
                          led_mask[1] |
                          led_mask[2]  );

  LED_Out (0);                                    /* switch LEDs off          */
}

/*----------------------------------------------------------------------------
  Function that turns on requested LED
 *----------------------------------------------------------------------------*/
void LED_On (uint32_t num) {

  if (num < LED_NUM) {
    LPC_GPIO_PORT->CLR0 = led_mask[num];          /* LED On                   */
  }
}

/*----------------------------------------------------------------------------
  Function that turns off requested LED
 *----------------------------------------------------------------------------*/
void LED_Off (uint32_t num) {

  if (num < LED_NUM) {
    LPC_GPIO_PORT->SET0 = led_mask[num];          /* LED Off                  */
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

