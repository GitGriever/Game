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

#include <XMC4500.h>                    /* XMC4500 Definitions                */
#include "LED.h"
#include "ADC.h"

volatile uint32_t tick_cnt;

/*----------------------------------------------------------------------------
  Systick Interrupt Handler
 *----------------------------------------------------------------------------*/

void SysTick_Handler (void) {
  if (tick_cnt) {
    tick_cnt--;
  }
}

/*----------------------------------------------------------------------------
  Delay function in milliseconds
 *----------------------------------------------------------------------------*/

static void delay (uint32_t ms) {

  tick_cnt = ms;
  while (tick_cnt);
}

/*----------------------------------------------------------------------------
  Main Program
 *----------------------------------------------------------------------------*/

int main (void) {
  uint32_t n,leds = 0x01;

  SystemCoreClockUpdate();                   /* update SystemCoreClock        */

  ADC_Init ();
  LED_Init ();

  SysTick_Config(SystemCoreClock/1000);      /* Generate interrupt each ms    */

  while (1) {                                /* Loop forever                  */
    leds <<= 1;
    if (leds == 0x08) leds = 0x01;
    LED_Val (leds);

    /* AD converter input                                                     */
    n = ADC_GetVal () >> 2;
    delay (n + 20);
  }
}

