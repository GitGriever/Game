/*----------------------------------------------------------------------------
 * Name:    LED.c
 * Purpose: MCB1000 low level LED functions
 * Note(s):
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2010-2013 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include "LPC11xx.h"                    /* LPC11xx definitions                */
#include "LED.h"

const unsigned long led_mask[] = {1UL << 0, 1UL << 1, 1UL << 2, 1UL << 3, 
                                  1UL << 4, 1UL << 5, 1UL << 6, 1UL << 7 };

/*----------------------------------------------------------------------------
  initialize LED Pins
 *----------------------------------------------------------------------------*/
void LED_init (void) {

  LPC_SYSCON->SYSAHBCLKCTRL |= (1UL <<  6);     /* enable clock for GPIO      */

  /* configure GPIO as output */
  LPC_GPIO2->DIR  |=  (led_mask[0] | led_mask[1] | led_mask[2] | led_mask[3] |
                       led_mask[4] | led_mask[5] | led_mask[6] | led_mask[7] );

  LPC_GPIO2->DATA &= ~(led_mask[0] | led_mask[1] | led_mask[2] | led_mask[3] |
                       led_mask[4] | led_mask[5] | led_mask[6] | led_mask[7] );
}


/*----------------------------------------------------------------------------
  Function that turns on requested LED
 *----------------------------------------------------------------------------*/
void LED_on (uint8_t num) {

  LPC_GPIO2->DATA |=  led_mask[num];
}

/*----------------------------------------------------------------------------
  Function that turns off requested LED
 *----------------------------------------------------------------------------*/
void LED_off (uint8_t num) {

  LPC_GPIO2->DATA &= ~led_mask[num];
}

/*----------------------------------------------------------------------------
  Output value to LEDs
 *----------------------------------------------------------------------------*/
void LED_out(uint8_t value) {
  int i;

  for (i = 0; i < LED_NUM; i++) {
    if (value & (1<<i)) {
      LED_on (i);
    } else {
      LED_off(i);
    }
  }
}
