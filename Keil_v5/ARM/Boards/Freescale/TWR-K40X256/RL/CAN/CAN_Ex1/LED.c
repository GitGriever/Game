/*------------------------------------------------------------------------------
 * Name:    LED.c
 * Purpose: Low level LED functions for Kinetis MK40 Device Series
 * Note(s):
 *------------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2004-2013 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <MK40N512MD100.h>
#include "LED.h"

#define NUM_LEDS  4                     /* Number of LEDs                     */

const uint32_t led_mask[] = { 1UL <<  7,
                              1UL <<  8,
                              1UL <<  9,
                              1UL << 11 };

/*-----------------------------------------------------------------------------
 *      LED_Init:  Initialize LEDs
 *
 * Parameters: (none)
 * Return:     (none)
 *----------------------------------------------------------------------------*/
void LED_Init (void) {

  SIM->SCGC5    |= (1UL << 10);         /* Enable Port B Clock                */
  SIM->SCGC5    |= (1UL << 11);         /* Enable Port C Clock                */
  PORTC->PCR[ 7] = (1UL <<  8);         /* PTC7  is GPIO pin                  */
  PORTC->PCR[ 8] = (1UL <<  8);         /* PTC8  is GPIO pin                  */
  PORTC->PCR[ 9] = (1UL <<  8);         /* PTC9  is GPIO pin                  */
  PORTB->PCR[11] = (1UL <<  8);         /* PTB11 is GPIO pin                  */

  /* Switch LEDs off and enable output                                        */
  PTC->PDOR = (led_mask[2] | led_mask[1] | led_mask[0]);
  PTC->PDDR = (led_mask[2] | led_mask[1] | led_mask[0]);
  PTB->PDOR = (led_mask[3]);
  PTB->PDDR = (led_mask[3]);
}


/*-----------------------------------------------------------------------------
 *      LED_UnInit:  Uninitialize LEDs; pins are set to reset state
 *
 * Parameters: (none)
 * Return:     (none)
 *----------------------------------------------------------------------------*/
void LED_UnInit(void) {
  PORTC->PCR[ 7] = 0;                   /* PTC7  is at reset state            */
  PORTC->PCR[ 8] = 0;                   /* PTC8  is at reset state            */
  PORTC->PCR[ 9] = 0;                   /* PTC9  is at reset state            */
  PORTB->PCR[11] = 0;                   /* PTB11 is at reset state            */
}

/*-----------------------------------------------------------------------------
 *      LED_On: Turns on requested LED
 *
 * Parameters:  num - LED number
 * Return:     (none)
 *----------------------------------------------------------------------------*/
void LED_On (uint32_t num) {

  if (num < 3) {
    PTC->PCOR = led_mask[num];
  } else {
    PTB->PCOR = led_mask[num];
  }
}

/*-----------------------------------------------------------------------------
 *       LED_Off: Turns off requested LED
 *
 * Parameters:  num - LED number
 * Return:     (none)
 *----------------------------------------------------------------------------*/
void LED_Off (uint32_t num) {

  if (num < 3) {
    PTC->PSOR = led_mask[num];
  } else {
    PTB->PSOR = led_mask[num];
  }
}

/*-----------------------------------------------------------------------------
 *       LED_Val: Write value to LEDs
 *
 * Parameters:  val - value to be displayed on LEDs
 * Return:     (none)
 *----------------------------------------------------------------------------*/
void LED_Val (uint32_t val) {
  int i;

  for (i = 0; i < NUM_LEDS; i++) {
    if (val & (1<<i)) {
      LED_On (i);
    } else {
      LED_Off(i);
    }
  }
}

/*-----------------------------------------------------------------------------
 *       LED_Num: Get number of available LEDs
 *
 * Parameters: (none)
 * Return:      number of available LEDs
 *----------------------------------------------------------------------------*/
uint32_t LED_Num (void) {
  return (NUM_LEDS);
}

/*-----------------------------------------------------------------------------
 * End of file
 *----------------------------------------------------------------------------*/
