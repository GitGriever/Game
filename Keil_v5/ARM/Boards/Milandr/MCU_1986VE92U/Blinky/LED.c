/*----------------------------------------------------------------------------
 * Name:    LED.c
 * Purpose: Low Level LED functions
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

#include "MDR32F9x.h"                      /* MDR32F9x definitions            */
#include "LED.h"

const unsigned long led_mask[] = {1UL << 0, 1UL << 1 };

/*----------------------------------------------------------------------------
  initialize LED Pins
 *----------------------------------------------------------------------------*/
void LED_Init (void) {

  MDR_RST_CLK->PER_CLOCK |= (1UL << 23);     /* enable clock for PORT C       */

  /* configure PORT C pins as output */
  MDR_PORTC->OE       |=  ((1 << 0  ) | (1 << 1  ));       /* Output          */
  MDR_PORTC->FUNC     &= ~((3 << 0*2) | (3 << 1*2));       /* Port            */
  MDR_PORTC->ANALOG   |=  ((1 << 0  ) | (1 << 1  ));       /* Digital         */
  MDR_PORTC->PWR      &= ~((3 << 0*2) | (3 << 1*2));
  MDR_PORTC->PWR      |=  ((1 << 0*2) | (1 << 1*2));       /* Speed Slow      */

  LED_Out (0);                               /* switch LEDs off               */
}


/*----------------------------------------------------------------------------
  Function that turns on requested LED
 *----------------------------------------------------------------------------*/
void LED_On (unsigned int num) {

  if (num < LED_NUM) {
    MDR_PORTC->RXTX |= led_mask[num];
  }
}

/*----------------------------------------------------------------------------
  Function that turns off requested LED
 *----------------------------------------------------------------------------*/
void LED_Off (unsigned int num) {

  if (num < LED_NUM) {
    MDR_PORTC->RXTX &= ~led_mask[num];
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
