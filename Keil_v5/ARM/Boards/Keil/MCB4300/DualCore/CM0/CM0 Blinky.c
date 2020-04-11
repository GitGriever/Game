/*-----------------------------------------------------------------------------
 * Name:    CM0 Blinky.c
 * Purpose: Cortex M0 Core LED Flasher for MCB4300
 * Note(s):
 *-----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2004-2012 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <LPC43xx.h>                    /* LPC43xx Definitions                */
#include "LED.h"

uint32_t LEDOn, LEDOff, LEDToggle; 

/*-----------------------------------------------------------------------------
  Repetitive Interrupt Timer IRQ Handler @ 50ms
 *----------------------------------------------------------------------------*/
void M0_RITIMER_OR_WWDT_IRQHandler (void) {
  static uint32_t ticks;

  LPC_RITIMER->CTRL |= 1;
  
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
  M4 IRQ Handler
 *----------------------------------------------------------------------------*/
void M0_M4CORE_IRQHandler (void) {
  LPC_CREG->M4TXEVENT = 0;
  LEDToggle = 1;
}


#define CLOCK_M4 180000000

/*-----------------------------------------------------------------------------
  Repetitive Interrupt Timer Init
 *----------------------------------------------------------------------------*/
void RIT_Init (void) {
  LPC_RITIMER->COMPVAL = CLOCK_M4/20-1; /* Set compare value (50ms)           */
  LPC_RITIMER->COUNTER = 0;
  LPC_RITIMER->CTRL    = (1 << 3) |
                         (1 << 2) |
                         (1 << 1) ;

  NVIC_EnableIRQ (M0_RITIMER_OR_WWDT_IRQn);                           
}

/*-----------------------------------------------------------------------------
  Main function
 *----------------------------------------------------------------------------*/
int main (void) {
  uint32_t LEDState = 0;
  
  RIT_Init();                           /* Repetitive Interrupt Timer Init    */
  LED_Init();                           /* LED Initialization                 */
  NVIC_EnableIRQ (M0_M4CORE_IRQn);      /* Enable IRQ from the M4 Core        */

  while (1) {
    if (LEDOn) {
      LEDOn = 0;
      LED_On (0);                       /* Turn on LED 0                      */
    }

    if (LEDOff) {
      LEDOff = 0;
      LED_Off (0);                      /* Turn off LED 0                     */
    }

    if (LEDToggle) {                    /* Toggle LED 3                       */
      LEDToggle = 0;
              
      (LEDState ^= 1) ? LED_On (3) : LED_Off(3);
      
      __sev();
    }
  }
}
