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
 * Copyright (c) 2004-2013 KEIL - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <stm32f2xx.h>                  /* STM32F2xx Definitions              */
#include "LED.h"

#define NUM_LEDS  4                     /* Number of LEDs                     */

/*-----------------------------------------------------------------------------
 *      LED_Init:  Initialize LEDs
 *
 * Parameters: (none)
 * Return:     (none)
 *----------------------------------------------------------------------------*/
void LED_Init (void) {
  /* Enable clock for GPIOC, GPIOG and GPIOI                                  */
  RCC->AHB1ENR |= (1UL << 2) | (1UL << 6) | (1UL << 8);

  /* Configure LED (PG6, PG8, PI9, PC7) pins as push-pull outputs */
  GPIOG->MODER  &= ~((3UL << 2 * 6) | (3UL << 2 * 8));
  GPIOG->MODER  |=   (1UL << 2 * 6) | (1UL << 2 * 8);
  GPIOG->OTYPER &= ~((1UL <<     6) | (1UL <<     8));

  GPIOI->MODER  &= ~(3UL << 2 * 9);
  GPIOI->MODER  |=  (1UL << 2 * 9);
  GPIOI->OTYPER &= ~(1UL <<     9);

  GPIOC->MODER  &= ~(3UL << 2 * 7);
  GPIOC->MODER  |=  (1UL << 2 * 7);
  GPIOC->OTYPER &= ~(1UL <<     7);
}


/*-----------------------------------------------------------------------------
 *      LED_UnInit:  Uninitialize LEDs; pins are set to reset state
 *
 * Parameters: (none)
 * Return:     (none)
 *----------------------------------------------------------------------------*/
void LED_UnInit(void) {
  /* Configure Port G pins PG6, PG8:                                          */
  GPIOG->MODER   &= ~((3UL << 2*6) | (3UL << 2*8));
  GPIOG->OTYPER  &= ~((1UL <<   6) | (1UL <<   8));
  GPIOG->OSPEEDR &= ~((3UL << 2*6) | (3UL << 2*8));
  GPIOG->PUPDR   &= ~((3UL << 2*6) | (3UL << 2*8));

  /* Reset Port I: PI9                                                        */
  GPIOI->MODER   &= ~(3UL << 2*9);
  GPIOI->OTYPER  &= ~(1UL <<   9);
  GPIOI->OSPEEDR &= ~(3UL << 2*9);
  GPIOI->PUPDR   &= ~(3UL << 2*9);

  /* Reset Port C: PC7                                                        */
  GPIOC->MODER   &= ~(3UL << 2*7);
  GPIOC->OTYPER  &= ~(1UL <<   7);
  GPIOC->OSPEEDR &= ~(3UL << 2*7);
  GPIOC->PUPDR   &= ~(3UL << 2*7);
}

/*-----------------------------------------------------------------------------
 *      LED_On: Turns on requested LED
 *
 * Parameters:  num - LED number
 * Return:     (none)
 *----------------------------------------------------------------------------*/
void LED_On (uint32_t num) {

  switch (num) {
    case 0: GPIOC->BSRRL |= (1 << 7); break;
    case 1: GPIOI->BSRRL |= (1 << 9); break;
    case 2: GPIOG->BSRRL |= (1 << 8); break;
    case 3: GPIOG->BSRRL |= (1 << 6); break;
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
    case 0: GPIOC->BSRRH |= (1 << 7); break;
    case 1: GPIOI->BSRRH |= (1 << 9); break;
    case 2: GPIOG->BSRRH |= (1 << 8); break;
    case 3: GPIOG->BSRRH |= (1 << 6); break;
  }
}

/*-----------------------------------------------------------------------------
 *       LED_Val: Write value to LEDs
 *
 * Parameters:  val - value to be displayed on LEDs
 * Return:     (none)
 *----------------------------------------------------------------------------*/
void LED_Val (uint32_t val) {
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
