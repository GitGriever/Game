/*----------------------------------------------------------------------------
 * Name:    BTN.c
 * Purpose: low Level Push Button functions
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


/*----------------------------------------------------------------------------
  initialize Push Button Pins
 *----------------------------------------------------------------------------*/
void BTN_Init(void) {

  PMC->PMC_WPMR = 0x504D4300;             /* Disable write protect            */
  PMC->PMC_PCER0 = (1UL << ID_PIOA);      /* enable clock for push button     */
  PMC->PMC_WPMR = 0x504D4301;             /* Enable write protect             */

  PIOA->PIO_WPMR = 0x50494F00;            /* Disable write protect            */
  PIOA->PIO_PUER =
  PIOA->PIO_IDR  =
  PIOA->PIO_ODR  =
  PIOA->PIO_PER  = (PIO_PA15 | PIO_PA16); /* Setup PA15..PA16 for user button */
  PIOA->PIO_WPMR = 0x50494F01;            /* Enable write protect             */

}


/*----------------------------------------------------------------------------
  Get Push Button status
 *----------------------------------------------------------------------------*/
unsigned int BTN_Get (void) {

  return (~(PIOA->PIO_PDSR) & (PIO_PA15 | PIO_PA16));
}
