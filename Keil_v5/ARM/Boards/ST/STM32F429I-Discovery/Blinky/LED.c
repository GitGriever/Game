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
 * Copyright (c) 2014 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include "STM32F4xx.h"
#include "LED.h"

const unsigned long led_mask[] = {1UL << 13, 1UL << 14};

void LED_Initialize (void) {

RCC->AHB1ENR  |= (1UL << 6);           /* Enable GPIOG clock             */

	/* Configure LED (PG13, PG14) pins as push-pull outputs */
GPIOG->MODER  &= ~((3UL << 2 * 13) | (3UL << 2 * 14));
GPIOG->MODER  |=   (1UL << 2 * 13) | (1UL << 2 * 14);
GPIOG->OTYPER &= ~((1UL <<     13) | (1UL <<     14));
}

/*----------------------------------------------------------------------------
  Function that turns on requested LED
 *----------------------------------------------------------------------------*/
void LED_On (unsigned int num) {

  if (num < LED_NUM) {
    GPIOG->BSRRL = led_mask[num];
  }
}

/*----------------------------------------------------------------------------
  Function that turns off requested LED
 *----------------------------------------------------------------------------*/
void LED_Off (unsigned int num) {

  if (num < LED_NUM) {
    GPIOG->BSRRH = led_mask[num];
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
