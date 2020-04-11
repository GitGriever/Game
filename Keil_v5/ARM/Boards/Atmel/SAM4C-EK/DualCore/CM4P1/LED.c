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
 * Copyright (c) 2013 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <sam4c.h>
#include "LED.h"

const unsigned long led_mask[] = { PIO_PC8, PIO_PC7, PIO_PC6 };

/*----------------------------------------------------------------------------
  initialize LED Pins
 *----------------------------------------------------------------------------*/
void LED_Init(void) {

  PMC->PMC_WPMR = 0x504D4300;             /* Disable write protect            */
  PMC->PMC_PCER1 = (1UL << (ID_PIOC-32)); /* enable PIOC clock                */
  PMC->PMC_WPMR = 0x504D4301;             /* Enable write protect             */

  PIOC->PIO_WPMR = 0x50494F00;            /* Disable write protect            */
  PIOC->PIO_PER  =
  PIOC->PIO_OER  =
  PIOC->PIO_PUDR =
  PIOC->PIO_OWER = (PIO_PC6 |
                    PIO_PC7 |
                    PIO_PC8  );           /* Setup PIO_PC6..7 for LEDs        */
  PIOC->PIO_WPMR = 0x50494F01;            /* Disable write protect            */

  LED_Out (0);                            /* switch LEDs off                  */
}

/*----------------------------------------------------------------------------
  Function that turns on requested LED
 *----------------------------------------------------------------------------*/
void LED_On (unsigned int num) {

  if (num < LED_NUM) {
    PIOC->PIO_CODR = led_mask[num];
  }
}

/*----------------------------------------------------------------------------
  Function that turns off requested LED
 *----------------------------------------------------------------------------*/
void LED_Off (unsigned int num) {

  if (num < LED_NUM) {
    PIOC->PIO_SODR = led_mask[num];
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

