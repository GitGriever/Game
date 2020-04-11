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
 * Copyright (c) 2009-2013 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <LPC15xx.h>                         /* LPC15xx definitions           */
#include "LED.h"

const unsigned long led_port[] = {       1,       1,       0,       1 };
const unsigned long led_mask[] = { 1UL<< 2, 1UL<< 3, 1UL<< 0, 1UL<< 0 };


/*----------------------------------------------------------------------------
  initialize LED Pins (P1.2, P1.3, P0.0, P1.0)   Rev 1.1
 *----------------------------------------------------------------------------*/
void LED_Init (void) {

  LPC_SYSCON->SYSAHBCLKCTRL0 |= (1UL << 14) | (1UL << 15);   /* enable clock for GPIO0 and GPIO1 port */

  /* configure GPIO as output */
  LPC_GPIO_PORT->DIR[0]  |= led_mask[2];
  LPC_GPIO_PORT->SET[0]  |= led_mask[2];
  LPC_GPIO_PORT->DIR[1]  |= (led_mask[0] | led_mask[1] | led_mask[3]);
  LPC_GPIO_PORT->SET[1]  |= (led_mask[0] | led_mask[1] | led_mask[3]);
}

/*----------------------------------------------------------------------------
  Function that turns on requested LED
 *----------------------------------------------------------------------------*/
void LED_On (unsigned int num) {

  if (num < LED_NUM) {
    LPC_GPIO_PORT->CLR[led_port[num]] |= led_mask[num];
  }
}

/*----------------------------------------------------------------------------
  Function that turns off requested LED
 *----------------------------------------------------------------------------*/
void LED_Off (unsigned int num) {

  if (num < LED_NUM) {
    LPC_GPIO_PORT->SET[led_port[num]]  = led_mask[num];
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
