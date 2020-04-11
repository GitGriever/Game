/*-----------------------------------------------------------------------------
 * Name:    Blinky.c
 * Purpose: Dual Core LED Flasher Example for MCB4300
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

const                                   /* Place the CM0 image in CM4 ROM     */
#include "CM0_Image.c"                  /* Cortex M0 image reference          */

uint32_t LEDOn, LEDOff, LEDToggle;

uint32_t M4_EventTimer, M0_EventResp;

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
  if (M4_EventTimer) {
    M4_EventTimer -= 50;
  }
}


/*-----------------------------------------------------------------------------
  M0 Core IRQ Handler
 *----------------------------------------------------------------------------*/
void M0APP_IRQHandler (void) {
  LPC_CREG->M0APPTXEVENT = 0;
  M0_EventResp = 1;
}


/*-----------------------------------------------------------------------------
  Load Cortex M0 Application Image
 *----------------------------------------------------------------------------*/
void Load_CM0_Image (uint32_t DestAddr, const uint8_t *Image, uint32_t Sz) {
  uint32_t i;
  uint8_t *dp = (uint8_t *)DestAddr;

  /* Copy application image */
  for (i = 0; i < Sz; i++) {
    dp[i] = Image[i];
  }
  /* Set shadow pointer to beginning of the CM0 application */
  LPC_CREG->M0APPMEMMAP = DestAddr;
}


/*-----------------------------------------------------------------------------
  Main function
 *----------------------------------------------------------------------------*/
int main (void) {
  uint32_t LEDState    = 0;
  uint32_t M4_EventSet = 1;

  SystemCoreClockUpdate ();                 /* Update system core clock       */
  SysTick_Config(SystemCoreClock/20-1);     /* Generate interrupt each 50 ms  */
  LED_Init();                               /* LED Initialization             */
  NVIC_EnableIRQ (M0APP_IRQn);              /* Enable IRQ from the M0 Core    */

  /* Stop CM0 core */
  LPC_RGU->RESET_CTRL1 = (1 << 24);

  /* Download CM0 application */
  Load_CM0_Image (0x10080000, LR0, sizeof (LR0)); 
  
  /* Start CM0 core */
  LPC_RGU->RESET_CTRL1 = 0;

  while (1) {
    if (LEDOn) {
      LEDOn = 0;
      LED_On (7);                       /* Turn on LED 7                      */
    }

    if (LEDOff) {
      LEDOff = 0;
      LED_Off (7);                      /* Turn off LED 7                     */
    }
    
    if (M4_EventSet) {
      M4_EventSet = 0;
      __sev();                          /* Send event signal to the CM0 core  */
    }

    if (M4_EventTimer == 0) {           /* Wait for timer timeout (250ms)     */
      if (M0_EventResp) {
        M0_EventResp = 0;               /* M0 response received, toggle LED 4 */

        (LEDState ^= 1) ? LED_On (4) : LED_Off (4);

        M4_EventSet =   1;
        M4_EventTimer  = 250;
      }
    }
  }
}
