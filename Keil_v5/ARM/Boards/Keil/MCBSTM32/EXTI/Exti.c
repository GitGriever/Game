/*----------------------------------------------------------------------------
 * Name:    Exti.c
 * Purpose: EXTI usage for STM32
 * Note(s): 
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2008-2011 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include "STM32F10x.h"

unsigned int ledPosExti = 0;                      /* led position (from 0..7) */
unsigned int ledExti    = 0;


/*----------------------------------------------------------------------------
  initialize GPIO Pins
 *----------------------------------------------------------------------------*/
void GPIO_Init (void) {

  /* configure GPIOs used for LEDs */
  RCC->APB2ENR |=  RCC_APB2ENR_IOPBEN;            /* Enable GPIOB clock       */
  GPIOB->ODR   &= ~(0x0000FF00);                  /* switch off LEDs          */
  GPIOB->CRH    =  (0x33333333);                  /* PB8..15 GPIO             */

  /* configure GPIOs for PushButtons */
  RCC->APB2ENR |= (RCC_APB2ENR_IOPAEN |           /* Enable GPIOA clock       */
                   RCC_APB2ENR_IOPCEN  );         /* Enable GPIOC clock       */
  GPIOA->CRL   &= ~0x0000000F;                    /* PA.0 clear               */
  GPIOA->CRL   |=  0x00000004;                    /* PA.0 is Floating Input   */

  GPIOC->CRH   &= ~0x00F00000;                    /* PC.13 clear              */
  GPIOC->CRH   |=  0x00400000;                    /* PC.13 is Floating Input  */
}


/*----------------------------------------------------------------------------
  External Interrupt initialisation
 *----------------------------------------------------------------------------*/
void EXTI_Init(void) {

    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;           /* enable clock for AFIO    */

    AFIO->EXTICR[0] &= ~AFIO_EXTICR1_EXTI0;       /* clear used pin           */
    AFIO->EXTICR[0] |=  AFIO_EXTICR1_EXTI0_PA;    /* set PA.0 to use          */

    EXTI->IMR       |=  EXTI_IMR_MR0;             /* unmask interrupt         */
    EXTI->EMR       &= ~EXTI_EMR_MR0;             /* no event                 */
    EXTI->RTSR      &= ~EXTI_RTSR_TR0;            /* no rising edge trigger   */
    EXTI->FTSR      |=  EXTI_FTSR_TR0;            /* set falling edge trigger */

    NVIC_EnableIRQ(EXTI0_IRQn);                   /* enable IRQ EXTI 0        */


    AFIO->EXTICR[3] &= ~AFIO_EXTICR4_EXTI13;      /* clear used pin           */
    AFIO->EXTICR[3] |=  AFIO_EXTICR4_EXTI13_PC;   /* set PC.13 to use         */

    EXTI->IMR       |=  EXTI_IMR_MR13;            /* unmask interrupt         */
    EXTI->EMR       &= ~EXTI_EMR_MR13;            /* unmask event             */
    EXTI->RTSR      &= ~EXTI_RTSR_TR13;           /* no rising edge trigger   */
    EXTI->FTSR      |=  EXTI_FTSR_TR13;           /* set falling edge trigger */

    NVIC_EnableIRQ(EXTI15_10_IRQn);               /* enable IRQ EXTI 10..15   */
}


/*----------------------------------------------------------------------------
  EXTI0 Interrupt Handler      for WKUP button connected to GPIOA.0
 *----------------------------------------------------------------------------*/
void EXTI0_IRQHandler(void) {

  if (EXTI->PR & (1UL << 0)) {                    /* EXTI13 interrupt pending?*/
    EXTI->PR |= (1UL << 0);                       /* clear pending interrupt  */

    if ((ledExti ^=1) == 0) 
      GPIOB->ODR &= ~(1UL << (ledPosExti+8));     /* switch off LED           */
    else
      GPIOB->ODR |=  (1UL << (ledPosExti+8));     /* switch on  LED           */
  }
}


/*----------------------------------------------------------------------------
  EXTI15..10 Interrupt Handler for TAMP button connected to GPIOC.13
 *----------------------------------------------------------------------------*/
void EXTI15_10_IRQHandler(void) {

  if (EXTI->PR & (1UL << 13)) {                   /* EXTI0 interrupt pending? */
    EXTI->PR |= (1UL << 13);                      /* clear pending interrupt  */

    if ((ledExti ^=1) == 0) 
      GPIOB->ODR &= ~(1UL << (ledPosExti+8));     /* switch off LED           */
    else
      GPIOB->ODR |=  (1UL << (ledPosExti+8));     /* switch on  LED           */
  }
}


/*----------------------------------------------------------------------------
  MAIN function
 *----------------------------------------------------------------------------*/
int main (void) {

  GPIO_Init ();
  EXTI_Init();
    
  while (1) {
    ;
  }
}
