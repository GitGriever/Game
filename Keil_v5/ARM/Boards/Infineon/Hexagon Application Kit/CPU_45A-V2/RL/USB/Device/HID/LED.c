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

#include "XMC4500.h"
#include "LED.h"

#define NUM_LEDS  1                     /* Number of LEDs                     */

const uint32_t led_mask[] = { 
                             1UL << 9
                            };
/*-----------------------------------------------------------------------------
 *      LED_Init:  Initialize LEDs
 *
 * Parameters: (none)
 * Return:     (none)
 *----------------------------------------------------------------------------*/
void LED_Init (void) {

   /* configure PORT P3.9 pin as output */
   PORT3->IOCR8  &=  ~(0xFFUL << 8);          /* clear P3.9 settings           */
   PORT3->IOCR8  |=   (0x80UL << 8);          /* P3.9 output push pull         */
   PORT3->PDR1   &=  ~(0x07UL << 4);          /* clear P3.9 output settings    */
   PORT3->PDR1   |=   (0x02UL << 4);          /* P3.9 output strong            */

   LED_Val (0);                               /* switch LEDs off               */
}


/*-----------------------------------------------------------------------------
 *      LED_UnInit:  Uninitialize LEDs; pins are set to reset state
 *
 * Parameters: (none)
 * Return:     (none)
 *----------------------------------------------------------------------------*/
void LED_UnInit(void) {
   PORT3->IOCR8  &=  ~(0xFFUL << 8);          /* clear P3.9 settings           */
   PORT3->PDR1   &=  ~(0x07UL << 4);          /* clear P3.9 output settings    */
}

/*-----------------------------------------------------------------------------
 *      LED_On: Turns on requested LED
 *
 * Parameters:  num - LED number
 * Return:     (none)
 *----------------------------------------------------------------------------*/
void LED_On (uint32_t num) {

  if (num < NUM_LEDS) {
    PORT3->OMR = (led_mask[num] << 16);
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
     PORT3->OMR = (led_mask[num]      );
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
     if (val & (1 << i)) {
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
