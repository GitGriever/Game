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
 * Copyright (c) 2011-2013 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <sam3s.h>
#include "LED.h"

#ifdef __REV_A
const unsigned long led_mask[] = { PIO_PC20, PIO_PC21 };
Pio *pPIO = PIOC; 
#else
const unsigned long led_mask[] = { PIO_PA19, PIO_PA20 };
Pio *pPIO = PIOA; 
#endif

/*----------------------------------------------------------------------------
  initialize LED Pins
 *----------------------------------------------------------------------------*/
void LED_Init (void) {

  PMC->PMC_WPMR = 0x504D4300;             /* Disable write protect              */
#ifdef __REV_A
  PMC->PMC_PCER0 = (1 << ID_PIOC);
#else
  PMC->PMC_PCER0 = (1 << ID_PIOA);
#endif

  pPIO->PIO_IDR  =
  pPIO->PIO_PER  =
  pPIO->PIO_OER  =  (led_mask[0] | led_mask[1]);

  LED_Out (0);                            /* switch LEDs off                  */

  PMC->PMC_WPMR = 0x504D4301;             /* Enable write protect             */
}

/*----------------------------------------------------------------------------
  Function that turns on requested LED
 *----------------------------------------------------------------------------*/
void LED_On (unsigned int num) {

  if (num < LED_NUM) {
    pPIO->PIO_CODR = led_mask[num];
  }
}

/*----------------------------------------------------------------------------
  Function that turns off requested LED
 *----------------------------------------------------------------------------*/
void LED_Off (unsigned int num) {

  if (num < LED_NUM) {
    pPIO->PIO_SODR = led_mask[num];
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
