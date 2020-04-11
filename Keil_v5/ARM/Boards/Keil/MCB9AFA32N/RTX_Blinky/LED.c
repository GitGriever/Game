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
 * Copyright (c) 2012 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <mb9aa30n.h>
#include "LED.h"

/* LEDs : P40..P43*/
const uint32_t led_mask[] = {1UL << 0, 1UL << 1, 1UL << 2, 1UL << 3};
#define  LED_ALL   (led_mask[0] | led_mask[1] | led_mask[2] | led_mask[3])

/*----------------------------------------------------------------------------
  initialize LED Pins
 *----------------------------------------------------------------------------*/
void LED_Init (void) {

  FM3_GPIO->PFR4  &= ~(LED_ALL);        /* LEDs on port 4 defined as GPIO     */
  FM3_GPIO->PDOR4 &= ~(LED_ALL);        /* switch off LEDs                    */
  FM3_GPIO->DDR4  |=  (LED_ALL);        /* LEDs on port 4 defined as output   */
  FM3_GPIO->PCR4  &= ~(LED_ALL);        /* LEDs on port 4 do not use pull-up  */
}

/*----------------------------------------------------------------------------
  Function that turns on requested LED
 *----------------------------------------------------------------------------*/
void LED_On (unsigned int num) {

  if (num < LED_NUM) {
    FM3_GPIO->PDOR4 |=  led_mask[num];
  }
}

/*----------------------------------------------------------------------------
  Function that turns off requested LED
 *----------------------------------------------------------------------------*/
void LED_Off (unsigned int num) {

  if (num < LED_NUM) {
    FM3_GPIO->PDOR4 &= ~led_mask[num];
  }
}

/*----------------------------------------------------------------------------
  Function that outputs value to LEDs
 *----------------------------------------------------------------------------*/
void LED_Out(unsigned int value) {
  int i;

  for (i = 0; i < LED_NUM; i++) {
    if (value & (1 << i)) {
      LED_On (i);
    } else {
      LED_Off(i);
    }
  }
}
