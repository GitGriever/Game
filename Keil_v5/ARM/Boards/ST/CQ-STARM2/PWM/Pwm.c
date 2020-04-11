/*----------------------------------------------------------------------------
 * Name:    Pwm.c
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
  Timer3 Update Interrupt Handler
 *----------------------------------------------------------------------------*/
void TIM3_IRQHandler(void) {

  if (TIM3->SR & (TIM_SR_UIF)) {                  /* UIF set?                 */

    if (Ccr ==   0) Dir =  1;                     /* lower limit ? increase   */
    if (Ccr == 100) Dir = -1;                     /* upper limit ? decrease   */
    Ccr = Ccr + Dir;   
    TIM3->CCR1 = Ccr;                             /* change ccr chn1          */

    TIM3->SR &= ~(TIM_SR_UIF);                    /* clear UIF flag           */
  }
}


/*----------------------------------------------------------------------------
  initialize GPIO Pins
 *----------------------------------------------------------------------------*/
void GPIO_Init (void) {

  /* configure GPIOs used for LEDs */
  RCC->APB2ENR |=  RCC_APB2ENR_IOPCEN;            /* Enable GPIOV clock       */
  GPIOC->ODR   &= ~0x00000040;                    /* switch off LEDs          */
  GPIOC->CRL   &= ~0x0F000000;                    /* PC.6     clear           */
  GPIOC->CRL   |=  0x0B000000;                    /* PC.6     is output       */
}


/*----------------------------------------------------------------------------
  initialize Timer 3 for PWM
 *----------------------------------------------------------------------------*/
void TIM3_Init (void) {


  /* configure full remap for TIM3 */
  RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;             /* enable clock for AFIO    */
  AFIO->MAPR |= AFIO_MAPR_TIM3_REMAP_FULLREMAP;   /* set used bits            */

  /* configure TIM4 used for PWM */
  RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;             /* enable clock for TIM3    */

  TIM3->PSC   = ( 7200 - 1);                      /* set prescaler   = 10 KHz */
  TIM3->ARR   = (  100 - 1);                      /* set auto-reload = 10 ms  */

  TIM3->CCMR1 = (6 <<  4);                        /* PWM mode 1 channel 1     */
  TIM3->CCER  = (1 <<  0);                        /* CCE channel 1            */

  TIM3->DIER  = TIM_DIER_UIE;                     /* update interrupt enable  */
  NVIC_EnableIRQ(TIM3_IRQn);                      /* TIM3   interrupt enable  */

  TIM3->CR1  |= TIM_CR1_CEN;                      /* timer enable             */
}


/*----------------------------------------------------------------------------
  MAIN function
 *----------------------------------------------------------------------------*/
int main (void) {

  GPIO_Init ();
  TIM3_Init ();

  while (1) {
    ;
  }
}
