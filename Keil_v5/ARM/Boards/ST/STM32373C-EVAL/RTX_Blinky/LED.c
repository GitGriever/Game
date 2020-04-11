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

#include <stm32f3xx.h>
#include "LED.h"

const unsigned long led_mask[] = { 1UL << 0, 1UL << 1, 1UL << 2, 1UL << 3 };


/*----------------------------------------------------------------------------
  initialize LED Pins
 *----------------------------------------------------------------------------*/
void LED_Init (void) {

  RCC->AHBENR     |=  ((1UL << 19) );                 /* Enable GPIOC clock            */

  GPIOC->BSRRL     =  ((1UL <<   0) | (1UL <<   1) |
                       (1UL <<   2) | (1UL <<   3)  ); /* switch LEDs off)             */

  GPIOC->MODER    &= ~((3UL << 2*0) | (3UL << 2*1) |
                       (3UL << 2*2) | (3UL << 2*3)  ); /* PC.0..3 is output            */
  GPIOC->MODER    |=  ((1UL << 2*0) | (1UL << 2*1) |
                       (1UL << 2*2) | (1UL << 2*3)  );
  GPIOC->OTYPER   &= ~((1UL <<   0) | (1UL <<   1) |
                       (1UL <<   2) | (1UL <<   3)  ); /* PC.0..3 is output Push-Pull  */
  GPIOC->OSPEEDR  &= ~((3UL << 2*0) | (3UL << 2*1) |
                       (3UL << 2*2) | (3UL << 2*3)  ); /* PC.0..3 is 50MHz Fast Speed  */
  GPIOC->OSPEEDR  |=  ((2UL << 2*0) | (2UL << 2*1) |
                       (2UL << 2*2) | (2UL << 2*3)  );
  GPIOC->PUPDR    &= ~((3UL << 2*0) | (3UL << 2*1) |
                       (3UL << 2*2) | (3UL << 2*3)  ); /* PC.0..3 is Pull up           */
  GPIOC->PUPDR    |=  ((1UL << 2*0) | (1UL << 2*1) |
                       (1UL << 2*2) | (1UL << 2*3)  );
}

/*----------------------------------------------------------------------------
  Function that turns on requested LED
 *----------------------------------------------------------------------------*/
void LED_On (unsigned int num) {

  if (num < LED_NUM) {
    GPIOC->BSRRH = led_mask[num];
  }
}

/*----------------------------------------------------------------------------
  Function that turns off requested LED
 *----------------------------------------------------------------------------*/
void LED_Off (unsigned int num) {

  if (num < LED_NUM) {
    GPIOC->BSRRL = led_mask[num];
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
