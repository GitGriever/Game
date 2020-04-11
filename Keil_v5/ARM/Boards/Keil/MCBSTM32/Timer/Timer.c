/*----------------------------------------------------------------------------
 * Name:    Timer.c
 * Purpose: Timer usage for STM32
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


int ledPos = 0;                                   /* led position (0..7)      */
int ledDir = 1;


/*----------------------------------------------------------------------------
  Timer1 Update Interrupt Handler
 *----------------------------------------------------------------------------*/
void TIM1_UP_IRQHandler (void) {

  if (TIM1->SR & (TIM_SR_UIF)) {                  /* UIF set?                 */

    if (ledPos == 0)                              /* lower limit reached ?    */
      ledDir = 1;                                 /* move up                  */

    if (ledPos == 7)                              /* upper limit reached ?    */
      ledDir = -1;                                /* move down                */

    GPIOB->ODR &= ~(1UL << (ledPos+8));           /* switch off old LED pos.  */
    ledPos += ledDir;
    GPIOB->ODR |=  (1UL << (ledPos+8));           /* switch on  new LED pos.  */

    TIM1->SR &= ~(TIM_SR_UIF);                    /* clear UIF flag           */
  }
}


/*----------------------------------------------------------------------------
  initialize GPIO Pins
 *----------------------------------------------------------------------------*/
void GPIO_Init (void) {

  /* configure GPIOs used for LEDs */
  RCC->APB2ENR |=  RCC_APB2ENR_IOPBEN;            /* Enable GPIOB clock       */
  GPIOB->ODR   &= ~(0x0000FF00);                  /* switch off LEDs          */
  GPIOB->CRH    =  (0x33333333);                  /* PB8..15 GPIO             */

}


/*----------------------------------------------------------------------------
  initialize TIM1
 *----------------------------------------------------------------------------*/
void TIM1_Init (void) {

  RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;             /* enable clock for TIM4    */

  TIM1->PSC   = ( 7200 - 1);                      /* set prescaler   = 10KHz  */
  TIM1->ARR   = ( 2500 - 1);                      /* set auto-reload = 250 ms */
  TIM1->RCR   =  0;;                              /* set repetition counter   */

  TIM1->DIER = TIM_DIER_UIE;                      /* Update Interrupt enable  */
  NVIC_EnableIRQ(TIM1_UP_IRQn);                   /* TIM1   Interrupt enable  */

  TIM1->CR1  |= TIM_CR1_CEN;                      /* timer enable             */
}


/*----------------------------------------------------------------------------
  MAIN function
 *----------------------------------------------------------------------------*/
int main (void) {

  GPIO_Init ();
  TIM1_Init ();

  GPIOB->ODR |=  (1UL << (ledPos+8));             /* lit initial LED position */

  while (1) {
    ;
  }
}
