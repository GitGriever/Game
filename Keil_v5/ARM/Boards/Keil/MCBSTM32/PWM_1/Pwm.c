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
  TIM4->ARR   = (10000 - 1);                      /* set auto-reload =  1 s   */          

  TIM4->CCR3  = 5000;                             /* Duty cicle channel 3     */
  TIM4->CCR4  = 2500;                             /* Duty cicle channel 4     */
  TIM4->CCMR2 = ((6 <<  4) |                      /* PWM mode 1 channel 3     */
                 (6 << 12)  );                    /* PWM mode 1 channel 4     */
  TIM4->CCER  = ((1 <<  8) |                      /* CCE channel 3            */
                 (1 << 12)  );                    /* CCE channel 4            */

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
