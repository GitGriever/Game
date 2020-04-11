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

#include <HT32F175x_275x.h>
#include "LED.h"

const unsigned long led_mask[] = { 1UL << 15, 1UL <<  0, 1UL <<  1};


/*----------------------------------------------------------------------------
  initialize LED Pins
 *----------------------------------------------------------------------------*/
void LED_Init (void) {

  HT_CKCU->APBCCR0 |=  ((1UL <<  14 ) |      /* enable AFIO Clock             */
                        (1UL <<  16 ) |      /* enable GPIO Port A Clock      */
                        (1UL <<  17 ) );     /* enable GPIO Port B Clock      */

  HT_AFIO->GPBCFGR &= ~((3UL << 15*2) );     /* configure PA15 as GPIO        */
  HT_GPIOA->DIRCR  |=  ((1UL << 15  ) );     /* configure PA15 as Output      */
  HT_GPIOA->DOUTR  |=  ((1UL << 15  ) );     /* Switch    PA15 LED off        */

  HT_AFIO->GPACFGR &= ~((3UL <<  0*2) |      /* configure PB0  as GPIO        */
                        (3UL <<  1*2) );     /* configure PB1  as GPIO        */
  HT_GPIOB->DIRCR  |=  ((1UL <<  0  ) |      /* configure PB0  as Output      */
                        (1UL <<  1  ) );     /* configure PB1  as Output      */
  HT_GPIOB->DOUTR  |=  ((1UL <<  0  ) |      /* Switch    PB0  LED off        */
                        (1UL <<  1  ) );     /* Switch    PB1  LED off        */
}

/*----------------------------------------------------------------------------
  Function that turns on requested LED
 *----------------------------------------------------------------------------*/
void LED_On (unsigned int num) {

  if (num < LED_NUM) {
    if (num == 0) HT_GPIOA->DOUTR &= ~led_mask[num];
    else          HT_GPIOB->DOUTR &= ~led_mask[num];
  }
}

/*----------------------------------------------------------------------------
  Function that turns off requested LED
 *----------------------------------------------------------------------------*/
void LED_Off (unsigned int num) {

  if (num < LED_NUM) {
    if (num == 0) HT_GPIOA->DOUTR |=  led_mask[num];
    else          HT_GPIOB->DOUTR |=  led_mask[num];
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
