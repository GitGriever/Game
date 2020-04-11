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
 * Copyright (c) 2011 Keil - An ARM Company. All rights reserved.
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

  /* Enable clock for GPIOC, GPIOD                                            */
  RCC->AHBENR |= (1UL << 19) | (1UL << 20);

  /* Configure LED (PC.10..12, PD.2) pins as push-pull outputs */
  GPIOC->BSRR   |=  ((1UL <<   10) | (1UL <<   11) | (1UL <<   12));
  GPIOC->MODER  &= ~((3UL << 2*10) | (3UL << 2*11) | (3UL << 2*12));
  GPIOC->MODER  |=  ((1UL << 2*10) | (1UL << 2*11) | (1UL << 2*12));
  GPIOC->OTYPER &= ~((1UL <<   10) | (1UL <<   11) | (1UL <<   12));

  GPIOD->BSRR   |=  (1UL <<    2);
  GPIOD->MODER  &= ~(3UL << 2* 2);
  GPIOD->MODER  |=  (1UL << 2* 2);
  GPIOD->OTYPER &= ~(1UL <<    2);
}


/*-----------------------------------------------------------------------------
 *      LED_On: Turns on requested LED
 *
 * Parameters:  num - LED number
 * Return:     (none)
 *----------------------------------------------------------------------------*/
void LED_On (uint32_t num) {

  switch (num) {
    case 0: GPIOC->BSRR |= ((1UL << 10) << 16); break;
    case 1: GPIOC->BSRR |= ((1UL << 11) << 16); break;
    case 2: GPIOC->BSRR |= ((1UL << 12) << 16); break;
    case 3: GPIOD->BSRR |= ((1UL <<  2) << 16); break;
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
    case 0: GPIOC->BSRR |= (1UL << 10); break;
    case 1: GPIOC->BSRR |= (1UL << 11); break;
    case 2: GPIOC->BSRR |= (1UL << 12); break;
    case 3: GPIOD->BSRR |= (1UL <<  2); break;
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
