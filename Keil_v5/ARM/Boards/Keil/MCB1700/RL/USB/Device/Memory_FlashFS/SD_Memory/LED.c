/*------------------------------------------------------------------------------
 * Name:    LED.c
 * Purpose: LED functions for the 
 *          Keil MCB1700 evaluation board
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

#include "LPC17xx.H"                         /* LPC17xx definitions           */
#include "LED.h"

#define NUM_LEDS  8                     /* Number of LEDs                     */

const uint32_t led_mask[] = { 1<<28, 1<<29, 1UL<<31, 1<<2, 1<<3, 1<<4, 1<<5, 1<<6 };


/*------------------------------------------------------------------------------
 * LED_Init:        Initialize LEDs
 *----------------------------------------------------------------------------*/

void LED_Init (void) {

  LPC_SC->PCONP     |=  (1 << 15);      /* Enable power to GPIO & IOCON       */
  LPC_GPIO1->FIODIR |=  0xB0000000;     /* LED pins on PORT1 are output       */
  LPC_GPIO2->FIODIR |=  0x0000007C;     /* LED pins on PORT2 are output       */
}


/*------------------------------------------------------------------------------
 * LED_Uninit:      Uninitialize LEDs; pins are set to reset state
 *----------------------------------------------------------------------------*/

void LED_Uninit (void) {

  LPC_GPIO1->FIODIR &= ~0xB0000000;
  LPC_GPIO2->FIODIR &= ~0x0000007C;
}


/*------------------------------------------------------------------------------
 * LED_On:          Turns on requested LED
 *----------------------------------------------------------------------------*/

void LED_On (uint32_t idx) {

  if (idx < 3) LPC_GPIO1->FIOPIN |=  led_mask[idx];
  else         LPC_GPIO2->FIOPIN |=  led_mask[idx];
}


/*------------------------------------------------------------------------------
 * LED_Off:         Turns off requested LED
 *----------------------------------------------------------------------------*/

void LED_Off (uint32_t idx) {

  if (idx < 3) LPC_GPIO1->FIOPIN &= ~led_mask[idx];
  else         LPC_GPIO2->FIOPIN &= ~led_mask[idx];
}


/*------------------------------------------------------------------------------
 * LED_Val:         Write value to LEDs
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


/*------------------------------------------------------------------------------
 * LED_Num:         Get number of available LEDs
 *----------------------------------------------------------------------------*/

uint32_t LED_Num (void) {

  return (NUM_LEDS);
}


/*------------------------------------------------------------------------------
 * End of file
 *----------------------------------------------------------------------------*/
