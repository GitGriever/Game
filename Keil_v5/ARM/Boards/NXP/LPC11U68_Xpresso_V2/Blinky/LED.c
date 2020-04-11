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
 * Copyright (c) 2014 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include "LPC11U6x.h"                        /* LPC11U6x definitions          */
#include "LED.h"

const unsigned long led_mask[] = {1UL << 16, 1UL << 17, 1UL << 18 };


/*----------------------------------------------------------------------------
  initialize LED Pins (P2.16..18)
 *----------------------------------------------------------------------------*/
void LED_Initialize (void) {

  LPC_SYSCON->SYSAHBCLKCTRL |= (1UL <<  6);     /* enable clock for GPIO      */

  /* configure GPIO as output */
  LPC_GPIO_PORT->SET[2]  |= (led_mask[0] | led_mask[1] |led_mask[2]  );
  LPC_GPIO_PORT->DIR[2]  |= (led_mask[0] | led_mask[1] |led_mask[2]  );
}

/*----------------------------------------------------------------------------
  Function that turns on requested LED
 *----------------------------------------------------------------------------*/
void LED_On (unsigned int num) {

  if (num < LED_NUM) {
    LPC_GPIO_PORT->CLR[2] |= led_mask[num];
  }
}

/*----------------------------------------------------------------------------
  Function that turns off requested LED
 *----------------------------------------------------------------------------*/
void LED_Off (unsigned int num) {

  if (num < LED_NUM) {
    LPC_GPIO_PORT->SET[2]  = led_mask[num];
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
