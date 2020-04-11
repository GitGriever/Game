/*-----------------------------------------------------------------------------
 * Name:    CM4P1_Blinky.c
 * Purpose: CM4P1 Core LED Flasher for SAM4C
 * Note(s):
 *-----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2013 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <sam4c.h>
#include "LED.h"

uint32_t LEDOn, LEDOff, LEDToggle;


/*-----------------------------------------------------------------------------
  SysTick IRQ Handler @ 50ms
 *----------------------------------------------------------------------------*/
void SysTick_Handler (void) {
  static uint32_t ticks;

  switch (ticks++) {
    case  0: LEDOn  = 1; break;
    case  5: LEDOff = 1; break;
    case  9: ticks  = 0; break;

    default:
      if (ticks > 10) {
        ticks = 0;
      }
  }
}


/*-----------------------------------------------------------------------------
  IPC1 IRQ Handler
 *----------------------------------------------------------------------------*/
void IPC1_Handler (void) {
  volatile uint32_t ipr = IPC1->IPC_IPR;
  volatile uint32_t isr = IPC1->IPC_ISR;

  IPC1->IPC_ICCR = (1ul << 0);

  LEDToggle = 1;
}


/*-----------------------------------------------------------------------------
  Main function
 *----------------------------------------------------------------------------*/
int main (void) {

  SysTick_Config(8000000ul / 20);           /* Generate interrupt each 50 ms  */

  IPC1->IPC_IECR = (1ul << 0);
  NVIC_EnableIRQ (IPC1_IRQn);               /* Enable IRQ from the CM4P0 Core */

  while (1) {
    if (LEDOn) {
      LEDOn = 0;
      LED_On (2);                           /* Turn on LED 2                  */
    }

    if (LEDOff) {
      LEDOff = 0;
      LED_Off (2);                          /* Turn off LED 2                 */
    }

    if (LEDToggle) {                        /* Toggle LED 3                   */
      LEDToggle = 0;

      IPC0->IPC_ISCR = (1ul << 0);          /* Send response signal to CM4P0  */
    }
  }
}
