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

#include <stdio.h>
#include <em_device.h>
#include "em_chip.h"
#include "em_cmu.h"
#include "dvk.h"
#include "trace.h"
#include "Serial.h"
#include "LED.h"


uint8_t  clock_1s;                              /* Flag activated each second */

volatile uint32_t msTicks;                      /* counts 1ms timeTicks       */
/*----------------------------------------------------------------------------
  SysTick_Handler
 *----------------------------------------------------------------------------*/
void SysTick_Handler(void) {

  if ((msTicks++ % 1000) == 0) {                /* Set Clock1s to 1 every sec */
    clock_1s = 1;
  }
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
  int32_t idx = -1, dir = 1;

  CHIP_Init();
  TRACE_ProfilerSetup();
  DVK_init(DVK_Init_EBI);         /* Initialize DVK board register access     */

  DVK_enablePeripheral(DVK_RS232_UART);  /* Enable access to Serial Port      */

  SER_Init ();
  LED_Init();

  SystemCoreClockUpdate();                      /* Get Core Clock Frequency   */
  if (SysTick_Config(SystemCoreClock / 1000)) { /* SysTick 1 msec interrupts  */
    while (1) __NOP();                          /* Capture error              */
  }

  while(1) {                                    /* Loop forever               */
    /* Calculate 'idx': 0,1,...,LED_NUM-1,LED_NUM-1,...,1,0,0,...             */
    idx += dir;
    if (idx == LED_NUM) { dir = -1; idx =  LED_NUM-1; }
    else if   (idx < 0) { dir =  1; idx =  0;         }

    LED_On (idx);                               /* Turn on LED 'idx'          */
    Delay(50);                                  /* Delay 50ms                 */
    LED_Off(idx);                               /* Turn off LED 'idx'         */
    Delay(150);                                 /* Delay 150ms                */

    if (clock_1s) {
      clock_1s = 0;
      printf ("Hello World\n\r");
		}
  }

}
