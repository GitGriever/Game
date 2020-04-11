/*-----------------------------------------------------------------------------
 * Name:    CM4P0_Blinky.c
 * Purpose: CM4P0 Core LED Flasher for SAM4C
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

const                                   /* Place the CM0 image in CMP40 ROM   */
#include "CM4P1_Image.c"                /* CM4P1 image reference              */

uint32_t LEDOn, LEDOff, LEDToggle;

uint32_t CM4P0_EventTimer, CM4P1_EventResp;


/*-----------------------------------------------------------------------------
  SysTick IRQ Handler @ 50ms
 *----------------------------------------------------------------------------*/
void SysTick_Handler (void) {
  static uint32_t ticks;

  switch (ticks++) {
    case  0: LEDOn  = 1; break;
    case  2: LEDOff = 1; break;
    case  4: ticks  = 0; break;

    default:
      if (ticks > 10) {
        ticks = 0;
      }
  }
  if (CM4P0_EventTimer) {
    CM4P0_EventTimer -= 50;
  }
}


/*-----------------------------------------------------------------------------
  IPC0 IRQ Handler
 *----------------------------------------------------------------------------*/
void IPC0_Handler (void) {
  volatile uint32_t ipr = IPC0->IPC_IPR;
  volatile uint32_t isr = IPC0->IPC_ISR;

  IPC0->IPC_ICCR = (1ul << 0);

  CM4P1_EventResp = 1;
}


/*-----------------------------------------------------------------------------
  Load CM4P1 Application Image
 *----------------------------------------------------------------------------*/
void Load_CM4P1_Image (uint32_t DestAddr, const uint8_t *Image, uint32_t Sz) {
  uint32_t i;
  uint8_t *dp = (uint8_t *)DestAddr;

  /* Copy application image */
  for (i = 0; i < Sz; i++) {
    dp[i] = Image[i];
  }

}


/*-----------------------------------------------------------------------------
  Main function
 *----------------------------------------------------------------------------*/
int main (void) {
  uint32_t LEDState       = 0;
  uint32_t CM4P1_EventSet = 1;

  SystemCoreClockUpdate ();                 /* Update system core clock       */
  SysTick_Config(SystemCoreClock / 20);     /* Generate interrupt each 50 ms  */

  LED_Init();                               /* LED Initialization             */

  IPC0->IPC_IECR = (1ul << 0);
  NVIC_EnableIRQ (IPC0_IRQn);               /* Enable IRQ from the CM4P1 Core */


  /* Download CM4P1 application */
  Load_CM4P1_Image (0x20080000, LR0, sizeof (LR0));

  /* Start CM4P1 core */
  RSTC->RSTC_CPMR |= RSTC_CPMR_CPROCEN | RSTC_CPMR_CPKEY(0x5A);

  while (1) {
    if (LEDOn) {
      LEDOn = 0;
      LED_On (0);                           /* Turn on LED 0                  */
    }

    if (LEDOff) {
      LEDOff = 0;
      LED_Off (0);                          /* Turn off LED 0                 */
    }

    if (CM4P1_EventSet) {
      CM4P1_EventSet = 0;
      IPC1->IPC_ISCR = (1ul << 0);          /* Send event signal to CM4P1     */
    }

    if (CM4P0_EventTimer == 0) {            /* Wait for timer timeout (250ms) */
      if (CM4P1_EventResp) {
        CM4P1_EventResp = 0;                /* CM4P1 response received        */

        (LEDState ^= 1) ? LED_On (1) : LED_Off (1);     /* toggle LED 1       */

        CM4P1_EventSet   =   1;
        CM4P0_EventTimer = 250;
      }
    }
  }
}
