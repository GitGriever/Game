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

//#include <stdio.h>
#include <stm32w108xx.h>                        /* STM32F0xx Definitions      */
#include "LED.h"



volatile uint32_t msTicks;                      /* counts 1ms timeTicks       */
/*----------------------------------------------------------------------------
  SysTick_Handler
 *----------------------------------------------------------------------------*/
void SysTick_Handler(void) {
  msTicks++;
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
  Function that initializes Button pins
 *----------------------------------------------------------------------------*/
void BTN_Init(void) {

  GPIOB->CRL  &= ~(15UL << (4 * 3));
  GPIOB->CRL  |=  ( 4UL << (4 * 3));            /* PB.03 is input  (no Pull)  */
}

/*----------------------------------------------------------------------------
  Function that read Button pins
 *----------------------------------------------------------------------------*/
uint32_t BTN_Get(void) {

 return ((~GPIOB->IDR) & (1UL << 3));
}


/*----------------------------------------------------------------------------
  MAIN function
 *----------------------------------------------------------------------------*/
int main (void) {
  int32_t num = -1; 
  int32_t dir =  1;
 uint32_t btns = 0;

  SystemCoreClockUpdate();                      /* Get Core Clock Frequency   */
  if ((CLK->CPUCR & 0x01) != 0x01) 
     SystemCoreClock /= 2;                      /* system runs at 12Mhz       */

  if (SysTick_Config(SystemCoreClock / 1000)) { /* SysTick 1 msec interrupt   */
    while (1);                                  /* Capture error              */
  }

  LED_Init();
  BTN_Init();                             
 
  while(1) {                                    /* Loop forever               */
    btns = BTN_Get();                           /* Read button states         */

    if (btns != (1UL << 3)) {
      /* Calculate 'num': 0,1,...,LED_NUM-1,LED_NUM-1,...,1,0,0,...  */
      num += dir;
      if (num == LED_NUM) { dir = -1; num =  LED_NUM-1; } 
      else if   (num < 0) { dir =  1; num =  0;         }

      LED_On (num);
      Delay( 50);                               /* Delay 50ms                 */
      LED_Off(num);
      Delay(450);                               /* Delay 450ms                */
    }
    else {
      LED_Out (0x03);
      Delay(10);                                /* Delay 10ms                 */
    }

  }
  
}

