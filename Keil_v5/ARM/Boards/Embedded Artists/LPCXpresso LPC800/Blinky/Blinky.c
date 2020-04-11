/*----------------------------------------------------------------------------
 * Name:    Blinky.c
 * Purpose: LED Flasher
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

#include <LPC8xx.h>
#include "LED.h"
#include "CMP.h"


volatile uint32_t msTicks = 0;                        /* counts 1ms timeTicks */
/*----------------------------------------------------------------------------
  SysTick_Handler
 *----------------------------------------------------------------------------*/
void SysTick_Handler(void) {
  msTicks++;                        /* increment counter necessary in Delay() */
}

/*----------------------------------------------------------------------------
  delays number of tick Systicks (happens every 1 ms)
 *----------------------------------------------------------------------------*/
void Delay (uint32_t dlyTicks) {
  uint32_t curTicks;

  curTicks = msTicks;
  while ((msTicks - curTicks) < dlyTicks) { __NOP(); }
}

/*----------------------------------------------------------------------------
  Main Program
 *----------------------------------------------------------------------------*/
int main (void) {
  uint32_t leds = 0x01;
  uint16_t dlyVal = 0;

  LED_Init();                                   /* LED Initialization         */
  CMP_Init();                                   /* CMP Initialization         */

  SystemCoreClockUpdate();
  SysTick_Config(SystemCoreClock/1000);         /* Systick interrupt each 1ms */

  while(1) {                                    /* Loop forever               */

    dlyVal = CMP_GetLad();                      /* get Ladder value for delay */
    dlyVal = dlyVal << 5;                       /* scale delay value          */

    leds     <<= 1;
    if (leds >= (1 << LED_NUM)) leds = 0x01;
    LED_Out(leds);                              /* lit LEDs                   */
    Delay(dlyVal);                              /* Delay                      */
  }

}
