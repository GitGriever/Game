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
 * Copyright (c) 2013 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <TM4C123.h>
#include "LED.h"


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
  MAIN function
 *----------------------------------------------------------------------------*/
int main (void) {
  
  SystemCoreClockUpdate();                        /* Get Core Clock Frequency */

  LED_Init ();

  SysTick_Config(SystemCoreClock / 1000ul);       /* Setup SysTick for 1 msec */

  while(1) {
    LED_On (0);                                   /* Turn on LED 'idx'        */
    Delay(200);                                   /* Delay 200ms              */
    LED_Off(0);                                   /* Turn off LED 'idx'       */
    Delay(200);                                   /* Delay 200ms              */

  }

}
