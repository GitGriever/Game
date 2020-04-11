/*----------------------------------------------------------------------------
 * Name:    Blinky.c
 * Purpose: TI TMS470Mx LED Flasher
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

#include <TMS470Mx.H>

#define LED_NUM     6                   /* Number of user LEDs                */
const uint32_t led_mask[] = { 1UL<< 0, 1UL<< 1, 1UL<< 2, 1UL<< 3, 1UL<< 4, 1UL<< 5};
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
  hetREG->DCLR = 0x0000003F;       /* Set pins for LEDs to low          */
  hetREG->DIR |= 0x0000003F;       /* Configure pins for LEDs as output */
}


/*------------------------------------------------------------------------------
  Switch on LEDs
 *------------------------------------------------------------------------------*/
__INLINE static void LED_On (uint32_t led) {
  hetREG->DSET = led_mask[led];
}

/*------------------------------------------------------------------------------
  Switch off LEDs
 *------------------------------------------------------------------------------*/
__INLINE static void LED_Off (uint32_t led) {
  hetREG->DCLR = led_mask[led];
}
extern uint32_t SystemCoreClock;
int main (void)
{
  int num     = -1; 
  int dir     =  1;
  
  SysTick_Config(SystemCoreClock/1000);      /* Generate interrupt each 1 ms    */
  
  LED_Config();

  while(1) {
    /* Calculate 'num': 0,1,...,LED_NUM-1,LED_NUM-1,...,1,0,0,...             */
    num += dir;
    if (num == LED_NUM) { dir = -1; num =  LED_NUM-1; } 
    else if   (num < 0) { dir =  1; num =  0;         }

    LED_On (num);
    Delay(250);
    LED_Off(num);
    Delay(250);
  }
}
