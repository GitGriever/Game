/*----------------------------------------------------------------------------
 * Name:    LED.c
 * Purpose: Low level LED functions
 * Note(s):
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2004-2013 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <MK60D10.H>
#include "LED.h"

#define NUM_LEDS  4                     /* Number of LEDs                     */

const uint32_t led_mask[] = { 1UL << 11,
                              1UL << 28,
                              1UL << 29,
                              1UL << 10 };

/*-----------------------------------------------------------------------------
 *      LED_Init:  Initialize LEDs
 *
 * Parameters: (none)
 * Return:     (none)
 *----------------------------------------------------------------------------*/
void LED_Init (void) {

  SIM->SCGC5    |= (1UL <<  9);         /* Enable Port A Clock                */
  PORTA->PCR[10] = (1UL <<  8);         /* PTA10 is GPIO pin                  */
  PORTA->PCR[11] = (1UL <<  8);         /* PTA11 is GPIO pin                  */
  PORTA->PCR[28] = (1UL <<  8);         /* PTA28 is GPIO pin                  */
  PORTA->PCR[29] = (1UL <<  8);         /* PTA29 is GPIO pin                  */

  /* Switch LEDs off and enable output                                        */
  PTA->PDOR = (led_mask[3] | led_mask[2] | led_mask[1] | led_mask[0]);
  PTA->PDDR = (led_mask[3] | led_mask[2] | led_mask[1] | led_mask[0]);
}


/*-----------------------------------------------------------------------------
 *      LED_UnInit:  Uninitialize LEDs; pins are set to reset state
 *
 * Parameters: (none)
 * Return:     (none)
 *----------------------------------------------------------------------------*/
void LED_UnInit(void) {
  PORTA->PCR[10] = 0;                   /* PTA10 is at reset state            */
  PORTA->PCR[11] = 0;                   /* PTA11 is at reset state            */
  PORTA->PCR[28] = 0;                   /* PTA28 is at reset state            */
  PORTA->PCR[29] = 0;                   /* PTA29 is at reset state            */
}

/*-----------------------------------------------------------------------------
 *      LED_On: Turns on requested LED
 *
 * Parameters:  num - LED number
 * Return:     (none)
 *----------------------------------------------------------------------------*/
void LED_On (uint32_t num) {

  if (num < NUM_LEDS) {
    PTA->PCOR = led_mask[num];
  }
}

/*-----------------------------------------------------------------------------
 *       LED_Off: Turns off requested LED
 *
 * Parameters:  num - LED number
 * Return:     (none)
 *----------------------------------------------------------------------------*/
void LED_Off (uint32_t num) {

  if (num < NUM_LEDS) {
    PTA->PSOR = led_mask[num];
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
