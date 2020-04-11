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
 * Copyright (c) 2011 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <stdio.h>
#include "MDR32F9x.h"                      /* MDR32F9x definitions            */
#include "Serial.h"
#include "LED.h"



volatile uint32_t msTicks;                 /* counts 1ms timeTicks            */
/*----------------------------------------------------------------------------
  SysTick_Handler
 *----------------------------------------------------------------------------*/
void SysTick_Handler(void) {
  msTicks++;                               /* inc. counter necessary in Delay*/
}

/*----------------------------------------------------------------------------
  delays number of tick Systicks (happens every 1 ms)
 *----------------------------------------------------------------------------*/
void Delay (uint32_t dlyTicks) {                                              
  uint32_t curTicks;

  curTicks = msTicks;
  while ((msTicks - curTicks) < dlyTicks);
}

/*----------------------------------------------------------------------------
  MAIN function
 *----------------------------------------------------------------------------*/
int main (void) {
  int num     = -1; 
  int dir     =  1;

  /* use HSE 8MHz as core clock */
  MDR_RST_CLK->HS_CONTROL = 0x1;                      /* Enable HSE oscillator     */
  while (MDR_RST_CLK->CLOCK_STATUS == 0x00) __NOP();  /* wait while HSE startup    */
  MDR_RST_CLK->CPU_CLOCK = 0x102;                     /* switch to HSE (8 MHz)     */

  SystemCoreClockUpdate();                            /* Get Core Clock Frequency  */
  if (SysTick_Config(8000000UL / 1000)) {             /* Setup SysTick Timer for 1 msec interrupts  */
    while (1);                                        /* Capture error */
  }
  
  LED_Init();
  SER_Init();                             

  while(1) {
    /* Calculate 'num': 0,1,...,LED_NUM-1,LED_NUM-1,...,1,0,0,...             */
    num += dir;
    if (num == LED_NUM) { dir = -1; num =  LED_NUM-1; } 
    else if   (num < 0) { dir =  1; num =  0;         }

    LED_On (num);
    Delay(250);
    LED_Off(num);
    Delay(250);
 
    printf ("\n\rHello World\n\r");
  }
  
}

