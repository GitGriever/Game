/*----------------------------------------------------------------------------
 * Name:    ADC.c
 * Purpose: low level ADC functions
 * Note(s):
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2004-2013 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include "stm32f0xx.h"                  /* STM32F4xx Definitions              */
#include "ADC.h"

uint16_t AD_last;                       /* Last converted value               */
uint8_t  AD_done;                       /* AD conversion done flag            */


/*----------------------------------------------------------------------------
  Function that initializes ADC
 *----------------------------------------------------------------------------*/
void ADC_Init (void) {

  /* Setup and initialize ADC converter                                       */
  RCC->APB2ENR |= (1UL <<   9);         /* Enable ADC clock                   */
  RCC->AHBENR  |= (1UL <<  19);         /* Enable GPIOC clock                 */

  GPIOC->MODER |= (3UL << 2*0);         /* PC0 is in Analog mode              */

  RCC->APB2RSTR |=  (1UL << 9);          /* start ADC reset                   */
  RCC->APB2RSTR &= ~(1UL << 9);          /* stop  ADC reset                   */

  ADC1->CR    |=  ( 1UL << 31);         /* Start calibration                  */
  while (ADC1->CR  & (1UL << 31));      /* Wait for calibration to finish     */

  ADC1->CHSELR =  ( 1UL << 10);         /* AIN10 selected                     */
  ADC1->SMPR   =  ( 7UL <<  0);         /* sample time: 239.5 ADC Clock Cyc.  */
  ADC1->CFGR1  =  0;                    /* use default configuration          */

  ADC1->IER   |=  ( 1UL <<  2);         /* enable EOC interrupt               */
  NVIC_EnableIRQ(ADC1_COMP_IRQn);       /* enable ADC Interrupt               */
  ADC1->CR    |=  ( 1UL <<  0);         /* ADC enable                         */
}


/*-----------------------------------------------------------------------------
  ADC Interrupt Handler
 *----------------------------------------------------------------------------*/
void ADC1_COMP_IRQHandler (void) {

  if (ADC1->ISR & (1UL << 2)) {         /* ADC EOC interrupt?                 */
    AD_last  = (ADC1->DR & ADC_VALUE_MAX);
    AD_done  = 1;

    ADC1->ISR |=  (1UL << 2);           /* Clear EOC interrupt                */
  }
}


/*-----------------------------------------------------------------------------
  Start AD Conversion
 *----------------------------------------------------------------------------*/
void ADC_StartCnv (void) {

  AD_done = 0;

  ADC1->CR |= (1UL << 2);               /* Start conversion                   */
}
