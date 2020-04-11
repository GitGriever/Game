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

#include <stdio.h>
#include <em_device.h>
#include "em_chip.h"
#include "em_cmu.h"
#include "bsp.h"
#include "bsp_trace.h"
#include "Serial.h"


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

  BSP_Init(BSP_INIT_DK_SPI);         /* Initialize DVK board register access  */
  BSP_PeripheralAccess(BSP_RS232_UART, true);  /* Enable access to Serial Port*/
  BSP_TraceSwoSetup();

  SER_Init ();
  BSP_LedsInit();

  SystemCoreClockUpdate();                      /* Get Core Clock Frequency   */
  if (SysTick_Config(SystemCoreClock / 1000)) { /* SysTick 1 msec interrupts  */
    while (1) __NOP();                          /* Capture error              */
  }

  while(1) {                                    /* Loop forever               */
    /* Calculate 'idx': 0,1,...,LED_NUM-1,LED_NUM-1,...,1,0,0,...             */
    idx += dir;
    if (idx == BSP_NO_OF_LEDS) { dir = -1; idx =  BSP_NO_OF_LEDS-1; }
    else if   (idx < 0) { dir =  1; idx =  0;         }

    BSP_LedSet (idx);                           /* Turn on LED 'idx'          */
    Delay(50);                                  /* Delay 50ms                 */
    BSP_LedClear (idx);                         /* Turn off LED 'idx'         */
    Delay(150);                                 /* Delay 150ms                */

    if (clock_1s) {
      clock_1s = 0;
      printf ("Hello World\n\r");
		}
  }

}
