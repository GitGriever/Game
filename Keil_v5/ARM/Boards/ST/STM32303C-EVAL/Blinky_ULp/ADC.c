/*----------------------------------------------------------------------------
 * Name:    ADC.c
 * Purpose: low level ADC functions
 * Note(s): possible defines select the used ADC interface:
 *            __ADC_IRQ   - ADC works in Interrupt mode
 *                        - ADC works in DMA mode (default)
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2014 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <stm32f3xx.h>
#include "ADC.h"

uint16_t AD_last;                         /* Last converted value             */
uint8_t  AD_done = 0;                     /* AD conversion done flag          */

/*----------------------------------------------------------------------------
  Function that initializes ADC
 *----------------------------------------------------------------------------*/
void ADC_Init (void) {

  RCC->AHBENR  |= ( 1UL <<   19);         /* Enable GPIOC clock               */
  GPIOC->MODER |= ( 3UL << 2* 1);         /* PC.1  is Analog mode             */

#ifndef __ADC_IRQ
  /* DMA1 Channel1 configuration ---------------------------------------------*/
  RCC->AHBENR  |= (1UL <<  0);            /* enable DMA1 clock                */

  DMA1_Channel1->CMAR  = (uint32_t)&AD_last;    /* Set memory     address     */
  DMA1_Channel1->CPAR  = (uint32_t)&(ADC1->DR); /* Set peripheral address     */
  DMA1_Channel1->CNDTR = 1;               /* Transmit 1 data item             */
  DMA1_Channel1->CCR   = 0x00002522;      /* configure DMA channel            */
  NVIC_EnableIRQ(DMA1_Channel1_IRQn);     /* enable DMA1 Channel Interrupt    */
  DMA1_Channel1->CCR  |= (1UL << 0);      /* enable DMA channel 1             */
#endif

  /* Setup and initialize ADC converter                                       */
  RCC->CR      |= ( 1UL <<  0);           /* Enable Internal high-speed clock */
  RCC->CFGR2   &= (0x1F <<  4);           /* clear ADC12PRES                  */
  RCC->CFGR2   |= (0x12 <<  4);           /* ADC12PRES = PLL clock / 4        */
  RCC->AHBENR  |= ( 1UL << 28);           /* enable periperal clock for ADC1/2*/

  ADC1->CR      = (1UL <<  28);           /* enable ADC VREG                  */
  __NOP();__NOP();__NOP();__NOP();__NOP();__NOP();

  ADC1->CR     |= ( 1UL << 31);           /* start Calibration */
  while (ADC1->CR & ( 1UL << 31)) __NOP();
  ADC1->CR     |= (1UL << 30);            /* enable differential mode */
  ADC1->CR     |= ( 1UL << 31);           /* start Calibration */
  while (ADC1->CR & ( 1UL << 31)) __NOP();

  ADC1->SQR1    = ( 7UL <<  6);           /* 1. conversion = channel 7 */
  ADC1->SQR2    =  0;                     /* Clear register                   */
  ADC1->SQR3    =  0;                     /* Clear register                   */
  ADC1->SQR4    =  0;                     /* Clear register                   */
  ADC1->SMPR1   = ( 4UL << 21);           /* Chn 7 sample time is 19.5 cyc    */
  ADC1->SMPR2   =  0;                     /* Clear register                   */

#ifndef __ADC_IRQ
  ADC1->CFGR   |= ( 3UL <<  0);           /* DMA mode enable, cont. DMA req.  */
#else
  ADC1->IER    |= ( 1UL <<  2);           /* enable for EOC Interrupt         */
  NVIC_EnableIRQ(ADC1_2_IRQn);            /* enable ADC Interrupt             */
#endif

  ADC1->CR     |= ( 1UL <<  0);           /* ADC enable                       */
}


/*----------------------------------------------------------------------------
  start AD Conversion
 *----------------------------------------------------------------------------*/
void ADC_StartCnv (void) {

  AD_done = 0;
  ADC1->CR     |=  (1UL <<  2);           /* Start A/D conversion             */
}


/*----------------------------------------------------------------------------
  stop AD Conversion
 *----------------------------------------------------------------------------*/
void ADC_StopCnv (void) {

  ADC1->CR     |=  (1UL <<  4);           /* Stop  A/D conversion             */
}


/*----------------------------------------------------------------------------
  get converted AD value
 *----------------------------------------------------------------------------*/
uint16_t ADC_GetCnv (void) {

  return(AD_last);
}


#ifndef __ADC_IRQ
/*----------------------------------------------------------------------------
  DMA IRQ: Executed when a transfer is completet
 *----------------------------------------------------------------------------*/
void DMA1_Channel1_IRQHandler(void) {

  if (DMA1->ISR & (1 << 1)) {             /* TCIF interrupt?                  */
    AD_done = 1;

    DMA1->IFCR  = (1 << 1);               /* clear TCIF interrupt             */
  }
}
#endif


#ifdef __ADC_IRQ
/*----------------------------------------------------------------------------
  A/D IRQ: Executed when A/D Conversion is done
 *----------------------------------------------------------------------------*/
void ADC1_2_IRQHandler(void) {

  if (ADC1->ISR & (1 << 2)) {              /* ADC1 EOC interrupt?              */
    AD_last = ADC1->DR;
    AD_done = 1;

    ADC1->ISR |=  (1 << 2);                /* clear EOC interrupt              */
  }

}
#endif
