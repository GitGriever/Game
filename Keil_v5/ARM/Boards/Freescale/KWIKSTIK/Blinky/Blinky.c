/*----------------------------------------------------------------------------
 * Name:    Blinky.c
 * Purpose: Freescale MK40X256VMD100 SLCD Flasher
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2011 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include "MK40N512MD100.h"
#include "SLCD_Driver.h"

unsigned char * _BLINKY   = {"BLINKY"};
volatile uint32_t msTicks;                            /* counts 1ms timeTicks */
/*----------------------------------------------------------------------------
  SysTick_Handler
 *----------------------------------------------------------------------------*/
void SysTick_Handler(void) {
  msTicks++;                        /* increment counter necessary in Delay() */
}

/*------------------------------------------------------------------------------
  delays number of tick Systicks (happens every 1 ms)
 *------------------------------------------------------------------------------*/
__INLINE static void Delay (uint32_t dlyTicks) {
  uint32_t curTicks;

  curTicks = msTicks;
  while ((msTicks - curTicks) < dlyTicks);
}
/*----------------------------------------------------------------------------
  MAIN function
 *----------------------------------------------------------------------------*/

int main (void) {
  uint8_t phase = 1;
  
  SystemCoreClockUpdate();                      /* Get Core Clock Frequency */
  SysTick_Config(SystemCoreClock/1000);         /* Generate interrupt each 1 ms    */

  SLCD_Configuration();
  SLCD_SegmentsAllOff ();
  SLCD_PrintString(_BLINKY, 0);
  while(1) 
  {
    if (phase)   {phase =  0; SLCD_FreescaleLogoOff(); SLCD_JLinkSignOn();    } 
    else         {phase =  1; SLCD_FreescaleLogoOn();  SLCD_JLinkSignOff();   }
    Delay(3000);
  }
}
