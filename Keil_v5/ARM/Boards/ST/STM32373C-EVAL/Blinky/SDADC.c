/*----------------------------------------------------------------------------
 * Name:    SDADC.c
 * Purpose: low level SDADC functions
 * Note(s): possible defines select the used ADC interface:
 *            __SDADC_IRQ   - SDADC works in Interrupt mode
 *                          - SDADC works in DMA mode (default)
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
#include "SDADC.h"

uint16_t AD_last;                         /* Last converted value             */
uint8_t  AD_done = 0;                     /* AD conversion done flag          */

/*----------------------------------------------------------------------------
  Function that initializes SDADC
    6 MHz operation
 *----------------------------------------------------------------------------*/
void ADC_Init (void) {

  RCC->AHBENR  |= ( 1UL <<   18);         /* Enable GPIOB clock               */
  GPIOB->MODER |= ( 3UL << 2* 1);         /* PB.1  is Analog mode             */

#ifndef __SDADC_IRQ
  /* DMA1 Channel1 configuration ---------------------------------------------*/
  RCC->AHBENR  |= (1UL <<  1);            /* enable DMA2 clock                */

  DMA2_Channel3->CMAR  = (uint32_t)&AD_last;          /* Set memory     addr  */
  DMA2_Channel3->CPAR  = (uint32_t)&(SDADC1->JDATAR); /* Set peripheral addr  */
  DMA2_Channel3->CNDTR = 1;               /* Transmit 1 data item             */
  DMA2_Channel3->CCR   = ((1UL <<  1) |   /* enable Transfer complete IRQ     */
                          (0UL <<  4) |   /* destination read from peripheral */
                          (1UL <<  5) |   /* enable Circular mode             */
                          (1UL <<  8) |   /* set PSIZE to 16Bit               */
                          (1UL << 10) |   /* set MSIZE to 16Bit               */
                          (1UL << 12)  ); /* channel priority medium          */
  NVIC_EnableIRQ(DMA2_Channel3_IRQn);     /* enable DMA2 Channel Interrupt    */
  DMA2_Channel3->CCR  |=  (1UL << 0);     /* enable DMA channel 3             */
#endif

  /* Setup and initialize ADC converter                                       */
  RCC->CFGR &= ~(0x1FUL << 27);
  RCC->CFGR |=  (0x15UL << 27);           /* SDADC1 clock is 6MHz @ 72 MZ     */

  RCC->APB1ENR |= (1UL << 28);            /* enable Power interface clock     */
  PWR->CR      |= (1UL <<  9);            /* enable SD1                       */
  RCC->APB2ENR |= (1UL << 24);            /* enable SDADC1 clock              */

  SDADC1->CR1 &= ~(3UL <<  8);            /* select external reference        */

  SDADC1->CR1 |=  (1UL << 31);            /* Initialization mode request      */
  while ((SDADC1->CR1 & (1UL << 31)) == 0) __NOP(); /* wait for init. mode    */

  SDADC1->CONF0R = (3UL << 26);           /* single ended 0-volt reference    */
  SDADC1->CONFCHR1 &= ~(3UL << (4*5));    /* chn5 uses SDADC_CONFR0           */

  SDADC1->JCHGR  = (1UL << 5);            /* chn5 part of injected group      */

  SDADC1->CR1 &= ~(1UL << 31);            /* exit initialization mode         */

#ifndef __SDADC_IRQ
  SDADC1->CR1  |= (1UL << 16);            /* DMA mode enable                  */
#else
  SDADC1->CR1  |= ( 1UL <<  1);           /* enable injected EOC Interrupt    */
  NVIC_EnableIRQ(SDADC1_IRQn);            /* enable SDADC Interrupt           */
#endif

  SDADC1->CR2 |= (1UL <<  0);             /* enable SDADC                     */
  SDADC1->CR2 |= (1UL <<  4);             /* start calibration                */
  while ((SDADC1->ISR & (1UL <<  0)) == 0) __NOP(); /* wait for CAL finished  */
}


/*----------------------------------------------------------------------------
  start AD Conversion
 *----------------------------------------------------------------------------*/
void ADC_StartCnv (void) {

  AD_done = 0;
  SDADC1->CR2 |=  (1UL << 15);            /* Start A/D conversion             */
}


/*----------------------------------------------------------------------------
  get converted AD value
 *----------------------------------------------------------------------------*/
uint16_t ADC_GetCnv (void) {

  return(AD_last);
}


#ifndef __SDADC_IRQ
/*----------------------------------------------------------------------------
  DMA IRQ: Executed when a transfer is completet
 *----------------------------------------------------------------------------*/
void DMA2_Channel3_IRQHandler(void) {

  if (DMA2->ISR & (1 << 9)) {             /* TCIF interrupt?                  */
    AD_last = (AD_last + 0x8000);         /* scale value to 0x0 .. 0xFFFF     */

    AD_done = 1;

    DMA2->IFCR  = (1 << 9);               /* clear TCIF interrupt             */
  }
}
#endif


#ifdef __SDADC_IRQ
/*----------------------------------------------------------------------------
  A/D IRQ: Executed when A/D Conversion is done
 *----------------------------------------------------------------------------*/
void SDADC1_IRQHandler(void) {
  uint32_t tmp;

  if (SDADC1->ISR & (1 << 1)) {           /* SDADC1 injected EOC interrupt?   */
    tmp = SDADC1->JDATAR & 0xFFFF;
    AD_last = (uint16_t)(tmp + 0x8000);   /* scale value to 0x0 .. 0xFFFF     */

    AD_done = 1;
  }

}
#endif
