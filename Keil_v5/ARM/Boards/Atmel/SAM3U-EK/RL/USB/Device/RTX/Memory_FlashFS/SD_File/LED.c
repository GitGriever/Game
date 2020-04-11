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

#include <SAM3U.h>                      /* SAM3U definitions                  */
#include "LED.h"

#define NUM_LEDS  2                     /* Number of LEDs                     */

/* LEDs for SAM3U-EK */
#define LED_D2 0
#define LED_D3 1

const unsigned long led_mask[] = { PIO_PB0, PIO_PB1 };

/*-----------------------------------------------------------------------------
 *      LED_Init:  Initialize LEDs
 *
 * Parameters: (none)
 * Return:     (none)
 *----------------------------------------------------------------------------*/
void LED_Init (void) {

  PMC->PMC_WPMR  = 0x504D4300;          /* Disable write protect              */
  PMC->PMC_PCER0 = (1 << ID_PIOB);      /* Enable clock for LEDs              */
  PMC->PMC_WPMR  = 0x504D4301;          /* Enable write protect               */

  PIOB->PIO_IDR  = PIO_PB1 | PIO_PB0;
  PIOB->PIO_OER  = PIO_PB1 | PIO_PB0;
  PIOB->PIO_PER  = PIO_PB1 | PIO_PB0;   /* Setup Pins PB0..PB1 for LEDs       */

  LED_Val (0);                          /* Switch LEDs off                    */
}


/*-----------------------------------------------------------------------------
 *      LED_UnInit:  Uninitialize LEDs; pins are set to reset state
 *
 * Parameters: (none)
 * Return:     (none)
 *----------------------------------------------------------------------------*/
void LED_UnInit(void) {

}

/*-----------------------------------------------------------------------------
 *      LED_On: Turns on requested LED
 *
 * Parameters:  num - LED number
 * Return:     (none)
 *----------------------------------------------------------------------------*/
void LED_On (uint32_t num) {

  if (num < NUM_LEDS) {
    PIOB->PIO_CODR = led_mask[num];
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
    PIOB->PIO_SODR = led_mask[num];
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
