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

#include <stdint.h>
#include <LM4Fxxxx.H>
#include "LED.h"

const uint32_t led_mask[] = { GPIO_PIN_1, GPIO_PIN_2, GPIO_PIN_3 };

#define LED_MASK  (led_mask[0] | led_mask[1] | led_mask[2])

/*----------------------------------------------------------------------------
  initialize LED Pins (PF.1..3)
 *----------------------------------------------------------------------------*/
void LED_Init (void) {

  /* Enable clock and init GPIO outputs */
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

  GPIOPadConfigSet(GPIO_PORTF_BASE, LED_MASK, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD);
  GPIODirModeSet  (GPIO_PORTF_BASE, LED_MASK, GPIO_DIR_MODE_OUT);

  GPIOPinWrite    (GPIO_PORTF_BASE, LED_MASK, 0);
}

/*----------------------------------------------------------------------------
  Function that turns on requested LED
 *----------------------------------------------------------------------------*/
void LED_On (unsigned int num) {

  if (num < LED_NUM) {
    GPIOPinWrite(GPIO_PORTF_BASE, led_mask[num], led_mask[num]);
  }
}

/*----------------------------------------------------------------------------
  Function that turns off requested LED
 *----------------------------------------------------------------------------*/
void LED_Off (unsigned int num) {

  if (num < LED_NUM) {
    GPIOPinWrite(GPIO_PORTF_BASE, led_mask[num], 0);
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
