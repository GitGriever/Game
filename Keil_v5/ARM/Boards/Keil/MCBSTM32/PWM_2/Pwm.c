/*----------------------------------------------------------------------------
 * Name:    PWM.c
 * Purpose: PWM Timer usage for STM32
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

#include "STM32F10x.h"


unsigned short Ccr = 0;                     /* Capture , Compare value        */
           int Dir = 1;                     /* in- decrease CCR               */

/*----------------------------------------------------------------------------
  Timer4 Update Interrupt Handler
 *----------------------------------------------------------------------------*/
void TIM4_IRQHandler(void) {

  if (TIM4->SR & (TIM_SR_UIF)) {                  /* UIF set?                 */

    if (Ccr ==   0) Dir =  1;                     /* lower limit ? increase   */
    if (Ccr == 100) Dir = -1;                     /* upper limit ? decrease   */
    Ccr = Ccr + Dir;   
    TIM4->CCR3 = Ccr;                             /* change ccr chn3          */
    TIM4->CCR4 = 100 - Ccr;                       /* change ccr chn4          */

    TIM4->SR &= ~(TIM_SR_UIF);                    /* clear UIF flag           */
  }
}


/*----------------------------------------------------------------------------
  initialize GPIO Pins
 *----------------------------------------------------------------------------*/
void GPIO_Init (void) {

  /* configure GPIOs used for LEDs */
  RCC->APB2ENR |=  RCC_APB2ENR_IOPBEN;            /* Enable GPIOB clock       */
  GPIOB->ODR   &= ~(0x0000FF00);                  /* switch off LEDs          */
  GPIOB->CRH    =  (0x333333BB);                  /* PB8..9 AF Push Pull      */
}


/*----------------------------------------------------------------------------
  initialize Timer 4 for PWM
 *----------------------------------------------------------------------------*/
void TIM4_Init (void) {

  /* configure TIM4 used for PWM */
  RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;             /* enable clock for TIM4    */

  TIM4->PSC   = ( 7200 - 1);                      /* set prescaler   = 10 KHz */
  TIM4->ARR   = (  100 - 1);                      /* set auto-reload = 10 ms  */

  TIM4->CCMR2 = ((6 <<  4) |                      /* PWM mode 1 channel 3     */
                 (6 << 12)  );                    /* PWM mode 1 channel 4     */
  TIM4->CCER  = ((1 <<  8) |                      /* CCE channel 3            */
                 (1 << 12)  );                    /* CCE channel 4            */

  TIM4->DIER  = TIM_DIER_UIE;                     /* update interrupt enable  */
  NVIC_EnableIRQ(TIM4_IRQn);                      /* TIM4   interrupt enable  */

  TIM4->CR1  |= TIM_CR1_CEN;                      /* timer enable             */
}


/*----------------------------------------------------------------------------
  MAIN function
 *----------------------------------------------------------------------------*/
int main (void) {

  GPIO_Init ();
  TIM4_Init ();

  while (1) {
    ;
  }
}
