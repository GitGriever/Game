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
 * Copyright (c) 2013 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <stm32f0xx.h>                  /* STM32F0xx Definitions              */
#include "LED.h"

#define NUM_LEDS  4                     /* Number of LEDs                     */

/*-----------------------------------------------------------------------------
 *      LED_Init:  Initialize LEDs
 *
 * Parameters: (none)
 * Return:     (none)
 *----------------------------------------------------------------------------*/
void LED_Init (void) {

  /* Enable clock for GPIOD                                                   */
  RCC->AHBENR |= (1UL << 20);

  /* Configure LED pins (PD.8..11) as push-pull outputs */
  GPIOD->BSRR   |=  ((1UL <<    8) | (1UL <<    9) | (1UL <<   10) | (1UL <<   11));
  GPIOD->MODER  &= ~((3UL << 2* 8) | (3UL << 2* 9) | (3UL << 2*10) | (3UL << 2*11));
  GPIOD->MODER  |=  ((1UL << 2* 8) | (1UL << 2* 9) | (1UL << 2*10) | (1UL << 2*11));
  GPIOD->OTYPER &= ~((1UL <<    8) | (1UL <<    9) | (1UL <<   10) | (1UL <<   11));
}


/*-----------------------------------------------------------------------------
 *      LED_On: Turns on requested LED
 *
 * Parameters:  num - LED number
 * Return:     (none)
 *----------------------------------------------------------------------------*/
void LED_On (uint32_t num) {

  switch (num) {
    case 0: GPIOD->BSRR |= ((1UL <<  8) << 16); break;
    case 1: GPIOD->BSRR |= ((1UL <<  9) << 16); break;
    case 2: GPIOD->BSRR |= ((1UL << 10) << 16); break;
    case 3: GPIOD->BSRR |= ((1UL << 11) << 16); break;
  }
}

/*-----------------------------------------------------------------------------
 *       LED_Off: Turns off requested LED
 *
 * Parameters:  num - LED number
 * Return:     (none)
 *----------------------------------------------------------------------------*/
void LED_Off (uint32_t num) {

  switch (num) {
    case 0: GPIOD->BSRR |= (1UL <<  8); break;
    case 1: GPIOD->BSRR |= (1UL <<  9); break;
    case 2: GPIOD->BSRR |= (1UL << 10); break;
    case 3: GPIOD->BSRR |= (1UL << 11); break;
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

  for (i = 0; i < NUM_LEDS; i++) {
    if (val & (1<<i)) {
      LED_On (i);
    } else {
      LED_Off(i);
    }
  }
}

/*-----------------------------------------------------------------------------
 *       LED_Num: Get number of available LEDs
 *
 * Parameters: (none)
 * Return:      number of available LEDs
 *----------------------------------------------------------------------------*/
uint32_t LED_Num (void) {
  return (NUM_LEDS);
}

/*-----------------------------------------------------------------------------
 * End of file
 *----------------------------------------------------------------------------*/
