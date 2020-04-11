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
 * Copyright (c) 20009-2013 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <sam3xa.h>
#include "LED.h"

const unsigned long led_mask[] = { PIO_PA12, PIO_PB12, PIO_PB13 };

/*----------------------------------------------------------------------------
  initialize LED Pins
 *----------------------------------------------------------------------------*/
void LED_Init (void) {
  PMC->PMC_WPMR = 0x504D4300;             /* Disable write protect            */

  PMC->PMC_PCER0 = ((1UL << ID_PIOA) |    /* enable PIOA clock                */
                    (1UL << ID_PIOB)  );  /* enable PIOB clock                */

  PIOB->PIO_PER  =
  PIOB->PIO_OER  =
  PIOB->PIO_PUDR =
  PIOB->PIO_OWER = (PIO_PB12 | PIO_PB13); /* Setup PIO_PB12..13 for LEDs      */

  PIOA->PIO_PER  =
  PIOA->PIO_OER  =
  PIOA->PIO_PUDR =
  PIOA->PIO_OWER = (PIO_PA12);            /* Setup PIO_PA12     for LED       */

  PMC->PMC_WPMR = 0x504D4301;             /* Enable write protect             */

  LED_Out (0);                            /* switch LEDs off                  */
}

/*----------------------------------------------------------------------------
  Function that turns on requested LED
 *----------------------------------------------------------------------------*/
void LED_On (unsigned int num) {

  if (num < LED_NUM) {
    if (num == 0) PIOA->PIO_CODR = led_mask[num];
    else          PIOB->PIO_CODR = led_mask[num];
  }
}

/*----------------------------------------------------------------------------
  Function that turns off requested LED
 *----------------------------------------------------------------------------*/
void LED_Off (unsigned int num) {

  if (num < LED_NUM) {
    if (num == 0) PIOA->PIO_SODR = led_mask[num];
    else          PIOB->PIO_SODR = led_mask[num];
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
