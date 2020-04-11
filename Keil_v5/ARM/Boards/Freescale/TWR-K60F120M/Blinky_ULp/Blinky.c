/*------------------------------------------------------------------------------
 * Name:    Blinky.c
 * Purpose: Freescale MK60FN1M0VLQ12 LED Flasher
 *------------------------------------------------------------------------------
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
#include <MK60F12.H>

#define NUM_LEDS  3                     /* Number of LEDs                     */

const uint32_t led_mask[] = { 1UL << 11,
                              1UL << 28,
                              1UL << 29 };
volatile uint32_t msTicks;              /* 1ms time ticks counter             */

/*------------------------------------------------------------------------------
 * SysTick_Handler
 *----------------------------------------------------------------------------*/

void SysTick_Handler(void) {
  msTicks++;                            /* Increment 1m time tick counter     */
}

/*------------------------------------------------------------------------------
 * Delays number of milliseconds
 *----------------------------------------------------------------------------*/

__INLINE static void Delay_ms (uint32_t ms) {
  uint32_t curTicks;

  curTicks = msTicks;
  while ((msTicks - curTicks) < ms);
}

/*------------------------------------------------------------------------------
 * Initialize LEDs (pins and initial states)
 *----------------------------------------------------------------------------*/

__INLINE static void LED_Init (void) {

  SIM->SCGC5    |= (1UL <<  9);         /* Enable Port A Clock                */
  PORTA->PCR[11] = (1UL <<  8);         /* PTA11 is GPIO pin                  */
  PORTA->PCR[28] = (1UL <<  8);         /* PTA28 is GPIO pin                  */
  PORTA->PCR[29] = (1UL <<  8);         /* PTA29 is GPIO pin                  */

  /* Switch LEDs off and enable output                                        */
  PTA->PDOR = (led_mask[2] | led_mask[1] | led_mask[0]);
  PTA->PDDR = (led_mask[2] | led_mask[1] | led_mask[0]);
}

/*------------------------------------------------------------------------------
 * Turn a LED on
 *----------------------------------------------------------------------------*/

__INLINE static void LED_On (uint32_t idx) {

  if (idx < NUM_LEDS) {
    PTA->PCOR = led_mask[idx];
  }
}


/*------------------------------------------------------------------------------
 * Turn a LED off
 *----------------------------------------------------------------------------*/

__INLINE static void LED_Off (uint32_t idx) {

  if (idx < NUM_LEDS) {
    PTA->PSOR = led_mask[idx];
  }
}

/*------------------------------------------------------------------------------
  Main function
 *----------------------------------------------------------------------------*/

int main (void) {
  int idx = -1; 
  int dir =  1;
  
  SystemCoreClockUpdate();              /* Update Core Clock Frequency        */
  SysTick_Config(SystemCoreClock/1000); /* Generate interrupt each 1 ms       */

  LED_Init();
  printf("Hello World\n");

  while(1) {
    /* Calculate 'idx': 0,1,...,NUM_LEDS-1,NUM_LEDS-1,...,1,0,0,...           */
    idx += dir;
    if (idx == NUM_LEDS) { dir = -1; idx =  NUM_LEDS-1; }
    else if    (idx < 0) { dir =  1; idx =  0;          }

    LED_On  (idx);
    printf("LED%d ON\n",  idx);
    Delay_ms(1000);
    LED_Off (idx);
    printf("LED%d OFF\n", idx);
    Delay_ms(1000);
  }
}
