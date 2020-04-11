/*----------------------------------------------------------------------------
 * Name:    KBD.c
 * Purpose: low level PushButton functions
 * Note(s):
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2009-2013 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <sam3u.h>
#include "KBD.h"


/*----------------------------------------------------------------------------
  initialize LED Pins
 *----------------------------------------------------------------------------*/
void KBD_Init (void) {

  PMC->PMC_WPMR   = 0x504D4300;                  /* Disable write protect     */
  PMC->PMC_PCER0 |= (1UL << ID_PIOA);            /* enable clock for PIOA     */  
  PMC->PMC_WPMR   = 0x504D4301;                  /* Enable write protect      */

  PIOA->PIO_IDR   = 
  PIOA->PIO_ODR   = 
  PIOA->PIO_PER   = (PIO_PA19 | PIO_PA18);       /* Setup Pins for Buttons    */

}


/*----------------------------------------------------------------------------
  Get Push Button Values value
 *----------------------------------------------------------------------------*/
uint32_t KBD_Get  (void) {
  uint32_t kbd_val;

  kbd_val = (PIOA->PIO_PDSR >> 18) & KBD_MASK;   /* 0 = pressed, 1 = released */
  return (kbd_val);
}

