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
 * Copyright (c) 2010-2013 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <sam3n.h>
#include "LED.h"

const unsigned long led_mask[] = {PIO_PA23,  PIO_PB14, PIO_PA25 };

/*----------------------------------------------------------------------------
  initialize LED Pins
 *----------------------------------------------------------------------------*/
void LED_Init(void) {

  PMC->PMC_WPMR  = 0x504D4300;            /* Disable write protect            */
  PMC->PMC_PCER0 = ((1UL << ID_PIOA) |    /* enable PIOA clock                */
                    (1UL << ID_PIOB)  );  /* enable PIOB clock                */
  PMC->PMC_WPMR  = 0x504D4301;            /* Enable write protect             */

  PIOB->PIO_WPMR = 0x50494F00;            /* Disable write protect            */
  PIOB->PIO_PER  =
  PIOB->PIO_OER  =
  PIOB->PIO_PUDR =
  PIOB->PIO_OWER = (PIO_PB14);            /* Setup pin PB14       for LED     */
  PIOB->PIO_WPMR = 0x50494F01;            /* Enable write protect             */

  PIOA->PIO_WPMR = 0x50494F00;            /* Disable write protect            */
  PIOA->PIO_PER  =
  PIOA->PIO_OER  =
  PIOA->PIO_PUDR =
  PIOA->PIO_OWER = (PIO_PA23 | PIO_PA25); /* Setup pins PA23 PA25 for LEDs    */
  PIOA->PIO_WPMR = 0x50494F01;            /* Enable write protect             */

  LED_Out (0);                            /* switch LEDs off                  */
}

/*----------------------------------------------------------------------------
  Function that turns on requested LED
 *----------------------------------------------------------------------------*/
void LED_On (uint32_t num) {

  if (num < LED_NUM) {
    if (num == 1) PIOB->PIO_CODR = led_mask[num];
    else          PIOA->PIO_CODR = led_mask[num];
  }
}

/*----------------------------------------------------------------------------
  Function that turns off requested LED
 *----------------------------------------------------------------------------*/
void LED_Off (uint32_t num) {

  if (num < LED_NUM) {
    if (num == 1) PIOB->PIO_SODR = led_mask[num];
    else          PIOA->PIO_SODR = led_mask[num];
  }
}

/*----------------------------------------------------------------------------
  Function that outputs value to LEDs
 *----------------------------------------------------------------------------*/
void LED_Out(uint32_t value) {
  int i;

  for (i = 0; i < LED_NUM; i++) {
    if (value & (1<<i)) {
      LED_On (i);
    } else {
      LED_Off(i);
    }
  }
}

