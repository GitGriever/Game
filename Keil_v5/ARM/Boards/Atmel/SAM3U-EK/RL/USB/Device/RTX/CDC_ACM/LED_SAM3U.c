/*------------------------------------------------------------------------------
 * Name:    LED_SAM3U.c
 * Purpose: LED functions for the Atmel SAM3U-EK evaluation board
 * Note(s): 
 *------------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2004-2013 KEIL - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <sam3u.h>
#include "LED.h"

#define LED_NUM   2                     /* Number of LEDs                     */


/*------------------------------------------------------------------------------
 * LED_Initialize:  Initialize LEDs
 *----------------------------------------------------------------------------*/

void LED_Initialize (void) {

  PMC->PMC_WPMR = 0x504D4300;           /* Disable write protect              */
  PMC->PMC_PCER0  =  (1 << ID_PIOB);    /* Enable clock for LEDs              */
  PMC->PMC_WPMR = 0x504D4301;           /* Enable write protect               */

  PIOB->PIO_IDR  =  (PIO_PB1 | PIO_PB0);
  PIOB->PIO_OER  =  (PIO_PB1 | PIO_PB0);
  PIOB->PIO_PER  =  (PIO_PB1 | PIO_PB0);/* Setup Pins PB0..PB1 for LEDs       */

  LED_Value (0);                        /* Turn all LEDs off                  */
}


/*------------------------------------------------------------------------------
 * LED_Uninitialize: Ininitialize LEDs; pins are set to reset state
 *----------------------------------------------------------------------------*/

void LED_Uninitialize (void) {

  LED_Value (0);                        /* Turn all LEDs off                  */

  PIOB->PIO_OER &= ~(PIO_PB1 | PIO_PB0);
  PIOB->PIO_PER &= ~(PIO_PB1 | PIO_PB0);/* Reset state of PB0..PB1 pins       */
}


/*------------------------------------------------------------------------------
 * LED_On:          Turns on requested LED
 *----------------------------------------------------------------------------*/

void LED_On (uint32_t idx) {

  if (idx < LED_NUM) 
    PIOB->PIO_CODR = 1 << idx;
}


/*------------------------------------------------------------------------------
 * LED_Off:         Turns off requested LED
 *----------------------------------------------------------------------------*/

void LED_Off (uint32_t idx) {

  if (idx < LED_NUM) 
    PIOB->PIO_SODR = 1 << idx;
}


/*------------------------------------------------------------------------------
 * LED_Value:       Write value to LEDs
 *----------------------------------------------------------------------------*/

void LED_Value (uint32_t val) {

  PIOB->PIO_SODR = (~val) & 3;
  PIOB->PIO_CODR =   val  & 3;
}


/*------------------------------------------------------------------------------
 * LED_GetNumber:   Get number of available LEDs
 *----------------------------------------------------------------------------*/

uint32_t LED_GetNumber (void) {

  return (LED_NUM);
}


/*------------------------------------------------------------------------------
 * End of file
 *----------------------------------------------------------------------------*/
