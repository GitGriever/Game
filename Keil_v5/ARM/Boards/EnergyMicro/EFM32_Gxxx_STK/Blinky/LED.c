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

#include "em_cmu.h"
#include "em_gpio.h"
#include "LED.h"

const uint32_t led_mask[] = { 0, 1, 2, 3 };

/*----------------------------------------------------------------------------
  initialize LED Pins
 *----------------------------------------------------------------------------*/
void LED_Init (void) {

  /* Enable clock and init GPIO outputs */
  CMU_ClockEnable(cmuClock_GPIO, true);
  GPIO_PinModeSet(gpioPortC, 0, gpioModePushPull, 1);
  GPIO_PinModeSet(gpioPortC, 1, gpioModePushPull, 1);
  GPIO_PinModeSet(gpioPortC, 2, gpioModePushPull, 1);
  GPIO_PinModeSet(gpioPortC, 3, gpioModePushPull, 1);

  LED_Out(0);                                         /* switch all LEDs Off */

}

/*----------------------------------------------------------------------------
  Function that turns on requested LED
 *----------------------------------------------------------------------------*/
void LED_On (unsigned int num) {

  if (num < LED_NUM) {
    GPIO_PinOutSet(gpioPortC, led_mask[num]);
  }
}

/*----------------------------------------------------------------------------
  Function that turns off requested LED
 *----------------------------------------------------------------------------*/
void LED_Off (unsigned int num) {

  if (num < LED_NUM) {
    GPIO_PinOutClear(gpioPortC, led_mask[num]);
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
