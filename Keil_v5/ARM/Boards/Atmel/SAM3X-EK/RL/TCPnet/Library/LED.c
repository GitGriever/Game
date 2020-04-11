/*-----------------------------------------------------------------------------
 * Name:    LED.c
 * Purpose: Low level LED functions
 * Note(s):
 *-----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2004-2013 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <sam3xa.h>
#include "LED.h"

#define NUM_LEDS  3                     /* Number of LEDs                     */

const uint32_t led_mask[] = { PIO_PA12, PIO_PB12, PIO_PB13 };


/* PMC Write Protect Mode Keys */
#define PMC_WPEN_KEY        0x504D4301
#define PMC_WPDIS_KEY       0x504D4300

/* PIO Write Protect Mode Keys */
#define PIO_WPEN_KEY        0x50494F01
#define PIO_WPDIS_KEY       0x50494F00

/*-----------------------------------------------------------------------------
 *      LED_Init:  Initialize LEDs
 *
 * Parameters: (none)
 * Return:     (none)
 *----------------------------------------------------------------------------*/
void LED_Init (void) {

  PMC->PMC_WPMR  = PMC_WPDIS_KEY;       /* Disable write protection           */
  PMC->PMC_PCER0 = ((1UL << ID_PIOA) |  /* Enable PIOA clock                  */
                    (1UL << ID_PIOB) ); /* Enable PIOB clock                  */  
  PMC->PMC_WPMR  = PMC_WPEN_KEY;        /* Enable write protection            */

  PIOB->PIO_WPMR = PIO_WPDIS_KEY;       /* Disable PIO write protection       */
  PIOB->PIO_PER  =
  PIOB->PIO_OER  =
  PIOB->PIO_PUDR =
  PIOB->PIO_OWER = (PIO_PB12 | PIO_PB13); /* Setup PIO_PB12..13 for LEDs      */
  PIOB->PIO_WPMR = PIO_WPEN_KEY;        /* Enable PIO write protection        */

  PIOA->PIO_WPMR = PIO_WPDIS_KEY;       /* Disable PIO write protection       */
  PIOA->PIO_PER  =
  PIOA->PIO_OER  =
  PIOA->PIO_PUDR =
  PIOA->PIO_OWER = (PIO_PA12);            /* Setup PIO_PA12     for LED       */
  PIOA->PIO_WPMR = PIO_WPEN_KEY;        /* Enable PIO write protection        */

  LED_Val (0);                            /* switch LEDs off                  */
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
    if (num == 0) PIOA->PIO_CODR = led_mask[num];
    else          PIOB->PIO_CODR = led_mask[num];
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
    if (num == 0) PIOA->PIO_SODR = led_mask[num];
    else          PIOB->PIO_SODR = led_mask[num];
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
