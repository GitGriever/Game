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
 * Copyright (c) 2008-2013 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <LPC15xx.h>                            /* LPC15xx definitions        */
#include "ADC.h"

uint16_t AD_last;                               /* Last converted value       */
uint8_t  AD_done = 0;                           /* AD conversion done flag    */

/*----------------------------------------------------------------------------
  Function that initializes ADC
 *----------------------------------------------------------------------------*/
void ADC_Init (void) {

  /* configure PIN P1.1 for ADC1 */
  LPC_SYSCON->SYSAHBCLKCTRL0 |= ((1UL << 15) |  /* enable clock for GPIO1     */
                                 (1UL << 12)  );/* enable clock for SWM       */

  LPC_SWM->PINENABLE0   &= ~(1UL << 12);        /* P1.1 is ADC1_0             */

  LPC_GPIO_PORT->DIR[1] &= ~(1UL <<  1);        /* configure GPIO as input    */

  /* configure ADC */
  LPC_SYSCON->PDRUNCFG       &= ~( 1UL << 11);  /* Enable power to ADC block  */
  LPC_SYSCON->SYSAHBCLKCTRL0 |=  ( 1UL << 28);  /* Enable clock to ADC1 block */
  LPC_SYSCON->ADCASYNCCLKSEL  = 0;              /* select IRC as async. clock */
  LPC_SYSCON->ADCASYNCCLKDIV  = 1;

  LPC_ADC1->CTRL |= (1UL << 30);                /* start calibration          */
 __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();

  LPC_ADC1->CTRL      = (1UL <<  8);            /* asynchronous mode          */
  LPC_ADC1->SEQA_CTRL = (1UL <<  0);            /* select channel 0           */

#ifdef __ADC_IRQ
  LPC_ADC1->INTEN  =  (1UL <<  0);              /* enable Seq. A interrupt    */

  NVIC_EnableIRQ(ADC1_SEQA_IRQn);               /* enable ADC1 SEQA interrupt */
#endif
}


/*----------------------------------------------------------------------------
  start AD Conversion
 *----------------------------------------------------------------------------*/
void ADC_StartCnv (void) {

  LPC_ADC1->SEQA_CTRL |= ((1UL << 26) |         /* start conversion           */
                          (1UL << 31)  );       /* enable Sequence            */
}


/*----------------------------------------------------------------------------
  stop AD Conversion
 *----------------------------------------------------------------------------*/
void ADC_StopCnv (void) {

  LPC_ADC1->SEQA_CTRL &= ~(1UL << 31);          /* disable Sequence           */
}


/*----------------------------------------------------------------------------
  get converted AD value
 *----------------------------------------------------------------------------*/
uint16_t ADC_GetCnv (void) {

#ifndef __ADC_IRQ
  unsigned int ADC_reg;

  do {
    ADC_reg = LPC_ADC1->SEQA_GDAT;              /* read conversion result     */
    AD_last = (ADC_reg >> 4) & ADC_VALUE_MAX;   /* store converted value      */
  } while ((ADC_reg & (1UL << 31)) == 0);

  AD_done = 1;
#endif

  return(AD_last);
}


/*----------------------------------------------------------------------------
  A/D IRQ: Executed when A/D Conversion is done
 *----------------------------------------------------------------------------*/
#ifdef __ADC_IRQ
void ADC1_SEQA_IRQHandler(void) {
  unsigned int ADC_reg;

  ADC_reg = LPC_ADC1->SEQA_GDAT;                /* read conversion result     */
  AD_last = (ADC_reg >> 4) & ADC_VALUE_MAX;     /* store converted value      */

  AD_done = 1;
}
#endif
