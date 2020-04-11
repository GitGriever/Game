/*----------------------------------------------------------------------------
 * Name:    Blinky.c
 * Purpose: SAMSUNG S3FN429 LED Flasher
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

#include <S3FN429.H>

  /* GPIO Output Configuration for LED
   |-------------------------|
   |EVM Board LED | GPIO Port|
   |-------------------------|  
   |D7 (LED1)     | P0.8     |	
   |D8 (LED2)     | P0.9     |
   |D9 (LED3)     | P0.10    |
   |D10(LED4)     | P0.11    |
   |-------------------------|
  */ 


#define LED_NUM     4                                 /* Number of user LEDs  */
const uint32_t led_mask[] = {GPIO_P8, GPIO_P9, GPIO_P10, GPIO_P11};
volatile uint32_t msTicks;                            /* counts 1ms timeTicks */

/*----------------------------------------------------------------------------
  SysTick_Handler
 *----------------------------------------------------------------------------*/
void SysTick_Handler(void) {
  msTicks++;                            /* increment counter necessary in Delay() */
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

  CM0->PCSR     = CM_IOCLK;			         /* Enable IOCONF Clock */
  GPIO0->CEDR   = GPIO_CLKEN;            /* Enable GPIO0  Clock */ 

  GPIO0->OER    = (led_mask[0] |        
                   led_mask[1] |
                   led_mask[2] |
                   led_mask[3] );        /* enable Output */

  GPIO0->CODR   = (led_mask[0] |        
                   led_mask[1] |
                   led_mask[2] |
                   led_mask[3] );        /* switch LEDs off  */

}

/*------------------------------------------------------------------------------
  Switch on LEDs
 *------------------------------------------------------------------------------*/
__INLINE static void LED_On (uint32_t led) {
  GPIO0->SODR  =led_mask[led];
}


/*------------------------------------------------------------------------------
  Switch off LEDs
 *------------------------------------------------------------------------------*/
__INLINE static void LED_Off (uint32_t led) {
  GPIO0->CODR  =led_mask[led];
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
    Delay(250);
    LED_Off(num);
    Delay(250);
  }
  
}

