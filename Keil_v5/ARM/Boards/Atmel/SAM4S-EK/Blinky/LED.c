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
 * Copyright (c) 20009-2011 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include "SAM4S.h"                        /* SAM4S definitions                */
#include "LED.h"

const unsigned long led_mask[] = { PIO_PC20, PIO_PA19, PIO_PA20 };

/*----------------------------------------------------------------------------
  initialize LED Pins
 *----------------------------------------------------------------------------*/
void LED_Init(void) {

  PMC->PMC_WPMR  = 0x504D4300;            /* Disable write protect            */
  PMC->PMC_PCER0 = ((1UL << ID_PIOA) |    /* enable PIOA clock                */
                    (1UL << ID_PIOC)  );  /* enable PIOC clock                */
  PMC->PMC_WPMR  = 0x504D4301;            /* Enable write protect             */

  PIOA->PIO_WPMR = 0x50494F00;
  PIOA->PIO_PER  =
  PIOA->PIO_OER  =
  PIOA->PIO_PUDR =
  PIOA->PIO_OWER = (PIO_PA19 | PIO_PA20); /* Setup PIO_PA19..20 for LEDs      */

  PIOC->PIO_WPMR = 0x50494F00;
  PIOC->PIO_PER  =
  PIOC->PIO_OER  =
  PIOC->PIO_PUDR =
  PIOC->PIO_OWER = (PIO_PC20);            /* Setup PIO_PC20     for LED       */


  LED_Out (0);                            /* switch LEDs off                  */
}

/*----------------------------------------------------------------------------
  Function that turns on requested LED
 *----------------------------------------------------------------------------*/
void LED_On (unsigned int num) {

  if (num < LED_NUM) {
    if (num == 0) PIOC->PIO_SODR  =  led_mask[num];
    else          PIOA->PIO_CODR  =  led_mask[num];
  }
}

/*----------------------------------------------------------------------------
  Function that turns off requested LED
 *----------------------------------------------------------------------------*/
void LED_Off (unsigned int num) {

  if (num < LED_NUM) {
    if (num == 0) PIOC->PIO_CODR = led_mask[num];
    else          PIOA->PIO_SODR = led_mask[num];
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

