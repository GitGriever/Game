/*----------------------------------------------------------------------------
 * Name:    Hello.c
 * Purpose: Serial Output
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

#include <stdio.h>
#include <SN32F100.h>
#include "Serial.h"



volatile uint32_t msTicks;                      /* counts 10ms timeTicks      */
/*----------------------------------------------------------------------------
  SysTick_Handler
 *----------------------------------------------------------------------------*/
void SysTick_Handler(void) {
  msTicks++;
}

/*----------------------------------------------------------------------------
  delays number of tick Systicks (happens every 10 ms)
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

  SystemCoreClockUpdate();                      /* Get Core Clock Frequency   */

  SER_Init ();

  if (SysTick_Config(SystemCoreClock / 1000)) { /* SysTick 10 msec interrupts */
    while (1) __NOP();                          /* Capture error              */
  }

  while(1) {                                    /* Loop forever               */
    Delay(50);                                  /* Delay 500ms                */

    printf ("Hello World\n\r");
  }

}
