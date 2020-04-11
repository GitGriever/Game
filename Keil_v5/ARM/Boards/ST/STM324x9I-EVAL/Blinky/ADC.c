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

#include <stm32f4xx.h>                  /* STM32F4xx Definitions              */
#include "ADC.h"

uint16_t AD_last;                       /* Last converted value               */
uint8_t  AD_done;                       /* AD conversion done flag            */


/*----------------------------------------------------------------------------
  Function that initializes ADC
 *----------------------------------------------------------------------------*/
void ADC_Init (void) {

#define ADCx ADC3

  /* Setup and initialize ADC converter                                       */
  RCC->APB2ENR |= (1UL <<   10);        /* Enable ADC3 clock                  */
  RCC->AHB1ENR |= (1UL <<    5);        /* Enable GPIOF clock                 */
  GPIOF->MODER |= (3UL << 2*10);        /* PF10 is in Analog mode             */

  ADC3->SQR1   =   0;
  ADC3->SQR2   =   0;
  ADC3->SQR3   =  (8UL << 0);           /* SQ1 = channel 8                   */
  ADC3->SMPR1  =  (8UL << 6);           /* Channel 8 sample time is 480 cyc. */
  ADC3->SMPR2  =   0;                   /* Clear register                     */
  ADC3->CR1    =  (1UL << 8);           /* Scan mode on                       */

  ADC3->CR2   |=  (1UL << 3);           /* Initialize calibration registers   */
  while (ADC3->CR2 & (1UL << 3));       /* Wait for initialization to finish  */
  ADC3->CR2   |=  (1UL << 2);           /* Start calibration                  */
  while (ADC3->CR2 & (1UL << 2));       /* Wait for calibration to finish     */

  ADC3->CR1   |=  (1UL << 5);           /* Enable EOC interrupt               */
  NVIC_EnableIRQ(ADC_IRQn);             /* Enable ADC Interrupt               */
  ADC3->CR2   |=  (1UL << 0);           /* ADC enable                         */

}


/*-----------------------------------------------------------------------------
  ADC Interrupt Handler
 *----------------------------------------------------------------------------*/
void ADC_IRQHandler (void) {

  if (ADCx->SR & (1<<1)) {              /* ADC EOC interrupt?                 */
    AD_last  = (ADCx->DR & ADC_VALUE_MAX);
    AD_done  = 1;
    ADCx->SR &= ~(1<<1);                /* Clear EOC interrupt                */
  }
}


/*-----------------------------------------------------------------------------
  Start AD Conversion
 *----------------------------------------------------------------------------*/
void ADC_StartCnv (void) {
  AD_done = 0;
  ADCx->CR2 |= 1 << 30;                 /* Start conversion                   */
}


/*-----------------------------------------------------------------------------
  Is AD conversion done
 *----------------------------------------------------------------------------*/
uint32_t ADC_DoneCnv (void) {
  return (AD_done);
}

/*-----------------------------------------------------------------------------
  Get converted AD value
 *----------------------------------------------------------------------------*/
uint16_t ADC_GetCnv (void) {
  return(AD_last);
}
