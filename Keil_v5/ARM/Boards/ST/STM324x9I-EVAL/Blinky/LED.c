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
 * Copyright (c) 2004-2013 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <stm32f4xx.h>                  /* STM32F4xx Definitions              */
#include "LED.h"

#define NUM_LEDS  4                     /* Number of LEDs                     */

/*-----------------------------------------------------------------------------
 *      LED_Init:  Initialize LEDs
 *
 * Parameters: (none)
 * Return:     (none)
 *----------------------------------------------------------------------------*/
void LED_Init (void) {

  /* Enable clock for GPIOG                                                   */
  RCC->AHB1ENR |= (1UL << 6);

  /* Configure LED (PG6, PG7, PG10, PG12) pins as push-pull outputs */
  GPIOG->BSRRL   =   (1   <<    6) | (1   <<    7) | (1   <<   10) | (1   <<   12);

  GPIOG->MODER  &= ~((3UL << 2* 6) | (3UL << 2* 7) | (3UL << 2*10) | (3UL << 2*12));
  GPIOG->MODER  |=   (1UL << 2* 6) | (1UL << 2* 7) | (1UL << 2*10) | (1UL << 2*12);
  GPIOG->OTYPER &= ~((1UL <<    6) | (1UL <<    7) | (1UL <<   10) | (1UL <<   12));

}


/*-----------------------------------------------------------------------------
 *      LED_UnInit:  Uninitialize LEDs; pins are set to reset state
 *
 * Parameters: (none)
 * Return:     (none)
 *----------------------------------------------------------------------------*/
void LED_UnInit(void) {

  /* Configure Port G pins PG6, PG7, PG10, PG12:                              */
  GPIOG->MODER   &= ~((3UL << 2* 6) | (3UL << 2* 7) | (3UL << 2*10) | (3UL << 2*12));
  GPIOG->OTYPER  &= ~((1UL <<    6) | (1UL <<    7) | (1UL <<   10) | (1UL <<   12));
  GPIOG->OSPEEDR &= ~((3UL << 2* 6) | (3UL << 2* 7) | (3UL << 2*10) | (3UL << 2*12));
  GPIOG->PUPDR   &= ~((3UL << 2* 6) | (3UL << 2* 7) | (3UL << 2*10) | (3UL << 2*12));

}

/*-----------------------------------------------------------------------------
 *      LED_On: Turns on requested LED
 *
 * Parameters:  num - LED number
 * Return:     (none)
 *----------------------------------------------------------------------------*/
void LED_On (uint32_t num) {

  switch (num) {
    case 0: GPIOG->BSRRH = (1 <<  6); break;
    case 1: GPIOG->BSRRH = (1 <<  7); break;
    case 2: GPIOG->BSRRH = (1 << 10); break;
    case 3: GPIOG->BSRRH = (1 << 12); break;
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
    case 0: GPIOG->BSRRL = (1 <<  6); break;
    case 1: GPIOG->BSRRL = (1 <<  7); break;
    case 2: GPIOG->BSRRL = (1 << 10); break;
    case 3: GPIOG->BSRRL = (1 << 12); break;
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
