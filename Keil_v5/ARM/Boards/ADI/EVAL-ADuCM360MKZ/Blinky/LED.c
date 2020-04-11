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

#include <ADuCM360.H>
#include "LED.h"

const uint32_t led_mask[] = { 1UL << 3 };

/*----------------------------------------------------------------------------
  initialize LED Pins (GPIO2.0..1)
 *----------------------------------------------------------------------------*/
void LED_Init (void) {

  /* configure GPIO as output */
  pADI_GP1->GPOEN  |=  (led_mask[0] );              /* GPIO1.3 is output      */

  pADI_GP1->GPSET   =  led_mask[0];                 /* switch LED off         */

}

/*----------------------------------------------------------------------------
  Function that turns on requested LED
 *----------------------------------------------------------------------------*/
void LED_On (unsigned int num) {

  if (num < LED_NUM) {
    pADI_GP1->GPCLR = led_mask[0];
  }
}

/*----------------------------------------------------------------------------
  Function that turns off requested LED
 *----------------------------------------------------------------------------*/
void LED_Off (unsigned int num) {

  if (num < LED_NUM) {
    pADI_GP1->GPSET = led_mask[0];
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
