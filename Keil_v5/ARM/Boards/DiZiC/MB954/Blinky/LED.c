/*----------------------------------------------------------------------------
 * Name:    LED.c
 * Purpose: Low level LED functions
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

#include <stm32w108xx.h>                        /* STM32F0xx Definitions      */
#include "LED.h"

const unsigned long led_mask[] = {1UL << 5, 1UL << 0};

/*-----------------------------------------------------------------------------
 *      LED_Init:  Initialize LEDs
 *
 * Parameters: (none)
 * Return:     (none)
 *----------------------------------------------------------------------------*/
void LED_Init (void) {

  /* Configure LED (PA.05, PA.00) pins as push-pull outputs */
  GPIOA->CRL  &= ~(15UL << (4 * 0));
  GPIOA->CRL  |=  ( 1UL << (4 * 0));
  GPIOA->CRH  &= ~(15UL << (4 * 1));
  GPIOA->CRH  |=  ( 1UL << (4 * 1));
  GPIOA->BSR   =  (led_mask[0] | led_mask[1]);  /* switch LEDs off            */

}


/*-----------------------------------------------------------------------------
 *      LED_On: Turns on requested LED
 *
 * Parameters:  num - LED number
 * Return:     (none)
 *----------------------------------------------------------------------------*/
void LED_On (uint32_t num) {

  if (num < LED_NUM) {
    GPIOA->BRR = (led_mask[num]);
  }
}

/*-----------------------------------------------------------------------------
 *       LED_Off: Turns off requested LED
 *
 * Parameters:  num - LED number
 * Return:     (none)
 *----------------------------------------------------------------------------*/
void LED_Off (uint32_t num) {

  if (num < LED_NUM) {
    GPIOA->BSR = (led_mask[num]);
  }
}

/*-----------------------------------------------------------------------------
 *       LED_Val: Write value to LEDs
 *
 * Parameters:  val - value to be displayed on LEDs
 * Return:     (none)
 *----------------------------------------------------------------------------*/
void LED_Out (uint32_t val) {
  int i;

  for (i = 0; i < LED_NUM; i++) {
    if (val & (1<<i)) {
      LED_On (i);
    } else {
      LED_Off(i);
    }
  }
}
