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
 * Copyright (c) 2004-2013 KEIL - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <MKL25Z4.H>
#include "LED.h"

#define NUM_LEDS  2                     /* Number of LEDs                     */

/*-----------------------------------------------------------------------------
 *      LED_Init:  Initialize LEDs
 *
 * Parameters: (none)
 * Return:     (none)
 *----------------------------------------------------------------------------*/
void LED_Init (void) {

  SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK |  /* Enable Port B Clock                */
                SIM_SCGC5_PORTA_MASK;   /* Enable Port A Clock                */
  PORTA->PCR[17] = (1UL <<  8);         /* PTA17 is GPIO pin                  */
  PORTB->PCR[ 8] = (1UL <<  8);         /* PTB8 is GPIO pin                   */

  /* Switch LEDs off and enable output                                        */
  PTA->PDDR = (1UL << 17);
  PTB->PDDR = (1UL <<  8);
  LED_Val (0);
}


/*-----------------------------------------------------------------------------
 *      LED_UnInit:  Uninitialize LEDs; pins are set to reset state
 *
 * Parameters: (none)
 * Return:     (none)
 *----------------------------------------------------------------------------*/
void LED_UnInit(void) {
  PORTA->PCR[17] = 0;                   /* PTA17  is at reset state           */
  PORTB->PCR[ 8] = 0;                   /* PTB8  is at reset state            */
}

/*-----------------------------------------------------------------------------
 *      LED_On: Turns on requested LED
 *
 * Parameters:  num - LED number
 * Return:     (none)
 *----------------------------------------------------------------------------*/
void LED_On (uint32_t num) {

  if (num < NUM_LEDS) {
    switch (num) {
      case 0:
        PTA->PCOR = (1UL << 17);
      break;

      case 1:
        PTB->PCOR = (1UL <<  8);
      break;
    }
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
    switch (num) {
      case 0:
        PTA->PSOR = (1UL << 17);
      break;
       
      case 1:
        PTB->PSOR = (1UL <<  8);
      break;
    }
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
