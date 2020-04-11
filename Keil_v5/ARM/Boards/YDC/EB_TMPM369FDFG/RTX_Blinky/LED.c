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

#include <TMPM369.H>
#include "LED.h"

const unsigned long led_mask[] = {1 << 4, 1 << 5, 1 << 6, 1 << 7 };
#define  LED_ALL   0xF0

/*----------------------------------------------------------------------------
  initialize LED Pins
 *----------------------------------------------------------------------------*/
void LED_Init (void) {

  TSB_PC->CR    =  LED_ALL;             /* Set LED pins to outputs            */
  TSB_PC->FR1   = ~LED_ALL;             /* Set LED pins function as "Port"    */
  TSB_PC->FR2   = ~LED_ALL;             /* Set LED pins function as "Port"    */
  TSB_PC->OD    = ~LED_ALL;             /* Set LED pins as CMOS               */
  TSB_PC->PUP   = ~LED_ALL;             /* Disable  LED pins pull ups         */
  TSB_PC->PDN   = ~LED_ALL;             /* Disable  LED pins pull downs       */
  TSB_PC->IE    = ~LED_ALL;             /* Disable LED pins input             */
  TSB_PC->DATA  =  LED_ALL;             /* Turn all LEDs initially off        */
}

/*----------------------------------------------------------------------------
  Function that turns on requested LED
 *----------------------------------------------------------------------------*/
void LED_On (unsigned int num) {

  if (num < LED_NUM) {
    TSB_PC->DATA &= ~(led_mask[num]);
  }
}

/*----------------------------------------------------------------------------
  Function that turns off requested LED
 *----------------------------------------------------------------------------*/
void LED_Off (unsigned int num) {

  if (num < LED_NUM) {
    TSB_PC->DATA |=  (led_mask[num]);
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
