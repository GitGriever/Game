/*------------------------------------------------------------------------------
 * Name:    Blinky.c
 * Purpose: Freescale MK64FN1M0VMD12 LED Flasher
 *------------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2013 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <MK64F12.H>

#define NUM_LEDS  4                     /* Number of LEDs                     */

const uint32_t led_mask[] = { 1UL << 6,
                              1UL << 7,
                              1UL << 8,
                              1UL << 9 };
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

  SIM->SCGC5    |= (1UL <<  13);       /* Enable Port E Clock                */
  PORTE->PCR[6] = (1UL <<  8);         /* PTE6 is GPIO pin                  */
  PORTE->PCR[7] = (1UL <<  8);         /* PTE7 is GPIO pin                  */
  PORTE->PCR[8] = (1UL <<  8);         /* PTE8 is GPIO pin                  */
  PORTE->PCR[9] = (1UL <<  8);         /* PTE9 is GPIO pin                  */

  /* Switch LEDs off and enable output                                        */
  PTE->PDOR = (led_mask[3] | led_mask[2] | led_mask[1] | led_mask[0]);
  PTE->PDDR = (led_mask[3] | led_mask[2] | led_mask[1] | led_mask[0]);
}

/*------------------------------------------------------------------------------
 * Turn a LED on
 *----------------------------------------------------------------------------*/

__INLINE static void LED_On (uint32_t idx) {

  if (idx < NUM_LEDS) {
    PTE->PCOR = led_mask[idx];
  }
}


/*------------------------------------------------------------------------------
 * Turn a LED off
 *----------------------------------------------------------------------------*/

__INLINE static void LED_Off (uint32_t idx) {

  if (idx < NUM_LEDS) {
    PTE->PSOR = led_mask[idx];
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

  while(1) {
    /* Calculate 'idx': 0,1,...,NUM_LEDS-1,NUM_LEDS-1,...,1,0,0,...           */
    idx += dir;
    if (idx == NUM_LEDS) { dir = -1; idx =  NUM_LEDS-1; }
    else if    (idx < 0) { dir =  1; idx =  0;          }

    LED_On  (idx);
    Delay_ms(250);
    LED_Off (idx);
    Delay_ms(250);
  }
}
