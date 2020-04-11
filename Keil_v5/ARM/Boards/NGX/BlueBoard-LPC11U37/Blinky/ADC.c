/*----------------------------------------------------------------------------
 * Name:    ADC.c
 * Purpose: low level ADC functions
 * Note(s): possible defines select the used ADC interface:
 *            __ADC_IRQ   - ADC works in Interrupt mode
 *                        - ADC works in polling mode (default)
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

#include <LPC11Uxx.h>
#include "ADC.h"

uint16_t AD_last;                            /* Last converted value          */
uint8_t  AD_done = 0;                        /* AD conversion done flag       */

/*----------------------------------------------------------------------------
  Function that initializes ADC
 *----------------------------------------------------------------------------*/
void ADC_Init (void) {
  /* NOTE: AD0(PIO0_11) multiplexed with JTAG pin (TDI) */
  
  /* configure PINs GPIO0.11 for AD0 */
  LPC_SYSCON->SYSAHBCLKCTRL |= ((1UL <<  6) |   /* enable clock for GPIO0     */
                                (1UL << 16) );  /* enable clock for IOCON     */

  LPC_IOCON->TDI_PIO0_11 &= ~(7UL <<  0);       /* clear FUNC bits            */
  LPC_IOCON->TDI_PIO0_11 |=  (1UL <<  1);       /* P0.11 is AD0               */
  LPC_IOCON->TDI_PIO0_11 &= ~(1UL <<  7);       /* Analog input mode          */

  LPC_SYSCON->PDRUNCFG      &= ~(1UL <<  4);    /* Enable power to ADC        */
  LPC_SYSCON->SYSAHBCLKCTRL |=  (1UL << 13);    /* Enable clock to ADC        */

  LPC_ADC->CR          =  ( 1 <<  0) |          /* select AD0 pin             */
                          (10 <<  8) |          /* ADC clock is 48MHz/11      */
                          ( 0 << 17);           /* 11 clocks / 10 bits        */

#ifdef __ADC_IRQ
  LPC_ADC->INTEN     =  ( 1 <<  8);             /* global enable interrupt    */

  NVIC_EnableIRQ(ADC_IRQn);                     /* enable ADC Interrupt       */
#endif
}


/*----------------------------------------------------------------------------
  start AD Conversion
 *----------------------------------------------------------------------------*/
void ADC_StartCnv (void) {
  LPC_ADC->CR &= ~( 7 << 24);                   /* stop conversion            */
  LPC_ADC->CR |=  ( 1 << 24);                   /* start conversion           */
}


/*----------------------------------------------------------------------------
  stop AD Conversion
 *----------------------------------------------------------------------------*/
void ADC_StopCnv (void) {

  LPC_ADC->CR &= ~( 7 << 24);                   /* stop conversion            */
}


/*----------------------------------------------------------------------------
  get converted AD value
 *----------------------------------------------------------------------------*/
uint16_t ADC_GetCnv (void) {

#ifndef __ADC_IRQ
  while (!(LPC_ADC->GDR & ( 1UL << 31)));       /* Wait for Conversion end    */
  AD_last = (LPC_ADC->GDR >> 6) & ADC_VALUE_MAX;   /* Store converted value   */

  AD_done = 1;
#endif

  return(AD_last);
}


/*----------------------------------------------------------------------------
  A/D IRQ: Executed when A/D Conversion is done
 *----------------------------------------------------------------------------*/
#ifdef __ADC_IRQ
void ADC_IRQHandler(void) {
  volatile uint32_t adstat;

  adstat = LPC_ADC->STAT;		                    /* Read ADC clears interrupt  */

  AD_last = (LPC_ADC->GDR >> 6) & ADC_VALUE_MAX;   /* Store converted value   */

  AD_done = 1;
}
#endif
