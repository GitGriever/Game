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

#include <TSX1001.h>
#include "LED.h"

const unsigned long led_mask[] = { 1UL <<  8, 1UL <<  9, 1UL << 10, 1UL << 11,
                                   1UL << 12, 1UL << 13, 1UL << 14, 1UL << 15};

/*----------------------------------------------------------------------------
  initialize LED Pins
 *----------------------------------------------------------------------------*/
void LED_Init (void) {

  GPIO0->DATA[0].WORD |=  (0xFF << 8);  /* switch of LEDs */
}

/*----------------------------------------------------------------------------
  Function that turns on requested LED
 *----------------------------------------------------------------------------*/
void LED_On (unsigned int num) {

  if (num < LED_NUM) {
    GPIO0->DATA[0].WORD &= ~led_mask[num];
  }
}

/*----------------------------------------------------------------------------
  Function that turns off requested LED
 *----------------------------------------------------------------------------*/
void LED_Off (unsigned int num) {

  if (num < LED_NUM) {
    GPIO0->DATA[0].WORD |=  led_mask[num];
  }
}

/*----------------------------------------------------------------------------
  Function that outputs value to LEDs
 *----------------------------------------------------------------------------*/
void LED_Out(unsigned int value) {
  int i;
  uint32_t leds = 0;

  for (i = 0; i < LED_NUM; i++) {
    if ((value & (1<<i)) == 0) {
      leds |=  led_mask[i];
    }
  }

  GPIO0->DATA[0].WORD =  (GPIO0->DATA[0].WORD & ~(0xFF << 8)) | leds;
}
