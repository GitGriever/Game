/******************************************************************************/
/* Demo.c: Cortex-Mx System Design Kit Demo Example                                                      */
/******************************************************************************/
/* This file is part of the uVision/ARM development tools.                    */
/* Copyright (c) 2011 Keil - An ARM Company. All rights reserved.             */
/* This software may only be used under the terms of a valid, current,        */
/* end user licence from KEIL for a compatible version of KEIL software       */
/* development tools. Nothing else gives you the right to use this software.  */
/******************************************************************************/
#include <stdio.h>
#include "CMSDK_CM0.h"
#include "Timer.h"
volatile uint32_t msTicks = 0;                            /* counts 1ms timeTicks */
uint32_t PeripheralClock = 100000000UL;                   /* PCLK (Peipheral Clock Frequency) */


extern void SER_init(void);

/*----------------------------------------------------------------------------
  Initialize GPIO pins
 *----------------------------------------------------------------------------*/
static void GPIO_init(void)
{
  CMSDK_GPIO0->OUTENABLESET = 1;	/* Set GPIO0 Pin 0 as "Output Enable" */
  CMSDK_GPIO1->OUTENABLESET = 1;	/* Set GPIO1 Pin 0 as "Output Enable" */
  return;
}

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


int main (void)
{
  PeripheralClock = SystemCoreClock;       /* PCLK = HCLK     */
  SER_init ();                             /* Initialize UART */              

  printf(" Cortex-M0 System Design Kit Demo:  \n");
  printf(" This Demo Example demonstrates the \n");
  printf(" functionality of 32-bit Timer and GPIO \n");

  SysTick_Config(SystemCoreClock/1000);    /* SysTick: Generate interrupt each 1 ms      */
  GPIO_init();                             /* Initialize GPIO */              
  Delay(100);

  Timer0_Init(PeripheralClock/1000);       /* Timer 0: Generate interrupt each 1 ms      */
  Timer1_Init(PeripheralClock/500);        /* Timer 1: Generate interrupt each 2 ms      */

  while(1) {
  }

}
