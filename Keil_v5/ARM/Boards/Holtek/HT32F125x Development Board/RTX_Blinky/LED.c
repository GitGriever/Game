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
 * Copyright (c) 2011 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include "HT32F125x.h"
#include "LED.h"

const unsigned long led_mask[] = { 1UL <<  5, 1UL <<  6, 1UL << 12};


/*----------------------------------------------------------------------------
  initialize LED Pins
 *----------------------------------------------------------------------------*/
void LED_Init (void) {

  HT_CKCU->APBCCR0 |=  ((1UL <<  14 ) |      /* enable AFIO Clock             */
                        (1UL <<  16 ) |      /* enable GPIO Port A Clock      */
                        (1UL <<  17 ) );     /* enable GPIO Port B Clock      */

  HT_AFIO->GPACFGR &= ~((3UL <<  5*2) |      /* configure PA5  as GPIO        */
                        (3UL <<  6*2) );     /* configure PA5  as GPIO        */
  HT_GPIOA->DIRCR  |=  ((1UL <<  5  ) |      /* configure PA5  as Output      */
                        (1UL <<  6  ) );     /* configure PA6  as Output      */
  HT_GPIOA->DOUTR  |=  ((1UL <<  5  ) |      /* Switch    PA5  LED off        */
                        (1UL <<  6  ) );     /* Switch    PA6  LED off        */

  HT_AFIO->GPBCFGR &= ~((3UL << 12*2) );     /* configure PB12 as GPIO        */
  HT_GPIOB->DIRCR  |=  ((1UL << 12  ) );     /* configure PB12 as Output      */
  HT_GPIOB->DOUTR  |=  ((1UL << 12  ) );     /* Switch    PB12 LED off        */
}

/*----------------------------------------------------------------------------
  Function that turns on requested LED
 *----------------------------------------------------------------------------*/
void LED_On (unsigned int num) {

  if (num < LED_NUM) {
    if (num < 2) HT_GPIOA->DOUTR &= ~led_mask[num];
    else         HT_GPIOB->DOUTR &= ~led_mask[num];
  }
}

/*----------------------------------------------------------------------------
  Function that turns off requested LED
 *----------------------------------------------------------------------------*/
void LED_Off (unsigned int num) {

  if (num < LED_NUM) {
    if (num < 2) HT_GPIOA->DOUTR |=  led_mask[num];
    else         HT_GPIOB->DOUTR |=  led_mask[num];
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
