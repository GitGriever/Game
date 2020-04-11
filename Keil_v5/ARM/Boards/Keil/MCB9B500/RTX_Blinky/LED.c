/*----------------------------------------------------------------------------
 * Name:    LED.c
 * Purpose: low level LED functions
 * Note(s):
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2011 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include "mcu.h"
#include "LED.h"

const unsigned long led_mask[] = { 1UL <<  8, 1UL <<  9, 1UL << 10, 1UL << 11,
                                   1UL << 12, 1UL << 13, 1UL << 14, 1UL << 15};


/*----------------------------------------------------------------------------
  initialize LED Pins
 *----------------------------------------------------------------------------*/
void LED_Init (void) {

  FM3_GPIO->PFR3  &= ~(0xFF00);         /* LEDs on port 3 defined as GPIO     */
  FM3_GPIO->PDOR3 |=  (0xFF00);         /* switch off LEDs                    */
  FM3_GPIO->DDR3  |=  (0xFF00);         /* LEDs on port 3 defined as output   */
  FM3_GPIO->PCR3  &= ~(0xFF00);         /* LEDs on port 3 do not use pull-up  */
}

/*----------------------------------------------------------------------------
  Function that turns on requested LED
 *----------------------------------------------------------------------------*/
void LED_On (unsigned int num) {

  if (num < LED_NUM) {
    FM3_GPIO->PDOR3 &= ~led_mask[num];
  }
}

/*----------------------------------------------------------------------------
  Function that turns off requested LED
 *----------------------------------------------------------------------------*/
void LED_Off (unsigned int num) {

  if (num < LED_NUM) {
    FM3_GPIO->PDOR3 |=  led_mask[num];
  }
}

/*----------------------------------------------------------------------------
  Function that outputs value to LEDs
 *----------------------------------------------------------------------------*/
void LED_Out(unsigned int value) {
  int i;

  for (i = 0; i < LED_NUM; i++) {
    if (value & (1<<i)) {
      LED_On (i);
    } else {
      LED_Off(i);
    }
  }
}
