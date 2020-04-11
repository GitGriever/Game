/*----------------------------------------------------------------------------
 * Name:    Blinky.c
 * Purpose: Freescale MK20DX128xxx5 LED Flasher
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

#include <MK20D5.H>

#define LED_NUM     3                   /* Number of user LEDs                */
const uint32_t led_mask[] = {1UL << 4, 1UL << 3, 1UL << 2};
volatile uint32_t msTicks;                            /* counts 1ms timeTicks */
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

/*------------------------------------------------------------------------------
  configer LED pins
 *------------------------------------------------------------------------------*/
__INLINE static void LED_Config(void) {

  SIM->SCGC5   |= (1UL << 9) | (1UL << 11) | (1UL << 12); /* Enable Clock to Port A,C,D */ 
  PORTD->PCR[4]  = (1UL <<  8);                           /* PTD.4 is GPIO */
  PORTC->PCR[3]  = (1UL <<  8);                           /* PTC.3 is GPIO */
  PORTA->PCR[2]  = (1UL <<  8);                           /* PTA.2 is GPIO */

  /* Switch LEDs off */
  PTD->PDOR = led_mask[0];
  PTC->PDOR = led_mask[1];
  PTA->PDOR = led_mask[2];
  /* Enable Output */
  PTD->PDDR = led_mask[0];
  PTC->PDDR = led_mask[1];
  PTA->PDDR = led_mask[2];
}

/*------------------------------------------------------------------------------
  Switch on LEDs
 *------------------------------------------------------------------------------*/
__INLINE static void LED_On (uint32_t led) {
  if (led == 0)       PTD->PCOR   = led_mask[led];
  else if (led == 1)  PTC->PCOR   = led_mask[led]; 
  else if (led == 2)  PTA->PCOR   = led_mask[led]; 
  
}


/*------------------------------------------------------------------------------
  Switch off LEDs
 *------------------------------------------------------------------------------*/
__INLINE static void LED_Off (uint32_t led) {

  if (led == 0)       PTD->PSOR   = led_mask[led];
  else if (led == 1)  PTC->PSOR   = led_mask[led]; 
  else if (led == 2)  PTA->PSOR   = led_mask[led]; 
}

/*----------------------------------------------------------------------------
  MAIN function
 *----------------------------------------------------------------------------*/
int main (void) {
  int num     = -1; 
  int dir     =  1;

  SystemCoreClockUpdate();                      /* Get Core Clock Frequency */
  SysTick_Config(SystemCoreClock/1000);         /* Generate interrupt each 1 ms    */
  
  LED_Config();                             
 
  while(1) {
    /* Calculate 'num': 0,1,...,LED_NUM-1,LED_NUM-1,...,1,0,0,...             */
    num += dir;
    if (num == LED_NUM) { dir = -1; num =  LED_NUM-1; } 
    else if   (num < 0) { dir =  1; num =  0;         }
    LED_On (num);
    Delay(500);
    LED_Off(num);
    Delay(500);
  }
  
}

