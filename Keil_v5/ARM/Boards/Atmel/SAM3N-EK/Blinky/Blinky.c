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
 * Copyright (c) 2010-2013 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <stdio.h>
#include <sam3n.h>
#include "Serial.h"
#include "LED.h"
#include "BTN.h"


volatile uint32_t msTicks;                      /* counts timeTicks           */
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
  int32_t idx  = -1, dir = 1;
 uint32_t btns = 0;

  SystemCoreClockUpdate();                      /* Get Core Clock Frequency   */

  SER_Init();
  LED_Init();
  BTN_Init();

  if (SysTick_Config(SystemCoreClock / 100)) {  /* SysTick 10 msec interrupts */
    while (1) __NOP();                          /* Capture error              */
  }

  while(1) {                                    /* Loop forever               */
    btns = BTN_Get();                           /* Read button states         */


    if (btns == 0) {                            /* no push button pressed     */
      /* Calculate 'idx': 0,1,...,LED_NUM-1,LED_NUM-1,...,1,0,0,...           */
      idx += dir;
      if (idx == LED_NUM) { dir = -1; idx =  LED_NUM-1; }
      else if   (idx < 0) { dir =  1; idx =  0;         }

      LED_On (idx);                             /* Turn on LED 'idx'          */
      Delay(20);                                /* Delay 200ms                */
      LED_Off(idx);                             /* Turn off LED 'idx'         */
      Delay(20);                                /* Delay 200ms                */
    }
    else {
      LED_Out(0x07);
      Delay(20);                                /* Delay 200ms                */
      LED_Out(0x00);
    }

    printf ("Hello World\n\r");
  }

}
