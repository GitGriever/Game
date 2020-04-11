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
 * Copyright (c) 2004-2012 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <stm32f4xx.h>                  /* STM32F4xx Definitions              */
#include "LED.h"

#define NUM_LEDS  2                     /* Number of LEDs                     */

const uint32_t led_mask[] = { 1UL << 10,
                              1UL << 11 };

/*-----------------------------------------------------------------------------
 *      LED_Init:  Initialize LEDs
 *
 * Parameters: (none)
 * Return:     (none)
 *----------------------------------------------------------------------------*/
void LED_Init (void) {
  /* Enable clock for GPIOI                                                   */
  RCC->AHB1ENR   |=  (1UL << 8) ;

  /* Configure Port I pins: PI10, PI11                                        */
  /* Pins in Open-drain output mode, 50 MHz Fast Speed, no Pull-up/down resist*/
  GPIOI->MODER   &= ~((3UL << 2*10) | (3UL << 2*11));
  GPIOI->MODER   |=  ((1UL << 2*10) | (1UL << 2*11));
  GPIOI->OTYPER  |=  ((1UL <<   10) | (1UL <<   11));
  GPIOI->OSPEEDR &= ~((3UL << 2*10) | (3UL << 2*11));
  GPIOI->OSPEEDR |=  ((2UL << 2*10) | (2UL << 2*11));
  GPIOI->PUPDR   &= ~((3UL << 2*10) | (3UL << 2*11));
  GPIOI->ODR     |=  ((1UL <<   10) | (1UL <<   11));
}


/*-----------------------------------------------------------------------------
 *      LED_UnInit:  Uninitialize LEDs; pins are set to reset state
 *
 * Parameters: (none)
 * Return:     (none)
 *----------------------------------------------------------------------------*/
void LED_UnInit(void) {
  /* Unconfigure Port I pins: PI10, PI11                                      */
  GPIOI->MODER   &= ~((3UL << 2*10) | (3UL << 2*11));
  GPIOI->OTYPER  &= ~((1UL <<   10) | (1UL <<   11));
  GPIOI->OSPEEDR &= ~((3UL << 2*10) | (3UL << 2*11));
  GPIOI->PUPDR   &= ~((3UL << 2*10) | (3UL << 2*11));
}

/*-----------------------------------------------------------------------------
 *      LED_On: Turns on requested LED
 *
 * Parameters:  num - LED number
 * Return:     (none)
 *----------------------------------------------------------------------------*/
void LED_On (uint32_t num) {

  GPIOI->BSRRH = led_mask[num];
}

/*-----------------------------------------------------------------------------
 *       LED_Off: Turns off requested LED
 *
 * Parameters:  num - LED number
 * Return:     (none)
 *----------------------------------------------------------------------------*/
void LED_Off (uint32_t num) {

  GPIOI->BSRRL = led_mask[num];
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
