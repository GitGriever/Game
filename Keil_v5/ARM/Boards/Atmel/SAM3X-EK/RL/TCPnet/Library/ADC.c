/*-----------------------------------------------------------------------------
 * Name:    ADC.c
 * Purpose: Low level ADC functions
 * Note(s): possible defines select the used ADC interface:
 *            __ADC_IRQ   - ADC works in Interrupt mode
 *                        - ADC works in DMA mode (default)
 *-----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2004-2013 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <sam3xa.h>
#include "ADC.h"

#define ADC_BITS      12                /* Number of A/D converter bits       */
#define ADC_TOUT 2000000                /* Approx. A/D conversion timeout     */

static volatile uint16_t AD_last;       /* Last converted value               */
static volatile uint8_t  AD_done;       /* AD conversion done flag            */


/*-----------------------------------------------------------------------------
 *      ADC_Init: Initialize Analog to Digital Converter
 *                5 MHz operation, 20µs startup time, 600ns track and hold time
 * Parameters:  (none)
 * Return:      (none)
 *----------------------------------------------------------------------------*/
void ADC_Init (void) {

  PMC->PMC_WPMR = 0x504D4300;             /* Disable write protect            */

  /* Setup and initialize ADC pins                                            */
  PMC->PMC_PCER0 = (1 << ID_PIOA);        /* enable clock for PA (ADC input)  */

  PIOA->PIO_IDR  = 
  PIOA->PIO_ODR  = 
  PIOA->PIO_PER  = (PIO_PA3);             /* Setup PIO_PA3 for ADC  CHN1      */

  /* Setup and initialize ADC converter                                       */
  PMC->PMC_PCER1 = ( 1 << (ID_ADC - 32)); /* enable clock for12-bit ADC       */

  ADC->ADC_CR   =  ( 1 <<  0);            /* reset ADC12B                     */
  ADC->ADC_MR   =  ( 0 <<  0) |           /* Starting conversion by software  */
                   ( 0 <<  1) |           /* external trigger                 */
                   ( 0 <<  4) |           /* 12-bit resolution                */
                   ( 0 <<  5) |           /* Normal Mode                      */
                   ( 7 <<  8) |           /* ADC clck =   5.25 MHz            */
                   (12 << 16) |           /* startup  =  19 us                */
                   ( 2 << 24);            /* shtim    = 570 ns                */

  ADC->ADC_CHER =  ( 1 <<  1);            /* enable channel 1                 */


#ifndef __ADC_IRQ
  /* Peripheral DMA (PDC) configuration --------------------------------------*/
  ADC->ADC_RPR  = (uint32_t)&(AD_last); 
  ADC->ADC_RCR  = 1; 
  ADC->ADC_PTCR = (  1 <<  0);            /* enable RX transfer               */

  ADC->ADC_IER  =  ( 1 << 27);            /* enable End of Receive Buffer irq */
#else
  ADC->ADC_IER  =  ( 1 <<  1);            /* enable channel 1  EOC irq        */
#endif

  NVIC_EnableIRQ(ADC_IRQn);               /* enable ADC Interrupt             */

  PMC->PMC_WPMR = 0x504D4301;             /* Enable write protect             */
}


/*-----------------------------------------------------------------------------
 *      ADC_UnInit: Uninitialize Analog to Digital Converter
 *                  (set hardware to reset state)
 *
 * Parameters:  (none)
 * Return:      (none)
 *----------------------------------------------------------------------------*/
void ADC_UnInit (void) {
}


/*-----------------------------------------------------------------------------
  ADC Interrupt Handler
 *----------------------------------------------------------------------------*/
void ADC_IRQHandler (void) {
  uint32_t adc_isr;

  adc_isr = (ADC->ADC_ISR & ADC->ADC_IMR);
  if (adc_isr & (1 << 1)) {               /* ADC chn1 EOC interrupt?          */
    AD_last = ADC->ADC_CDR[1];
    AD_done = 1;
  }

  if (adc_isr & (1 << 27)) {              /* ADC End of RX Buffer interrupt?  */
    ADC->ADC_RPR  = (uint32_t)&(AD_last);
    ADC->ADC_RCR  = 1;

    AD_done = 1;
  }
}

/*-----------------------------------------------------------------------------
 *      ADC_StartCnv:  Start A/D conversion 
 *
 * Parameters:  (none)
 * Return:      (none)
 *----------------------------------------------------------------------------*/
void ADC_StartCnv (void) {
  AD_done = 0;
  ADC->ADC_CR   |=  ( 1 <<  1);           /* Start A/D conversion             */
}


/*-----------------------------------------------------------------------------
 *      ADC_GetVal:  Get converted value from the ADC
 *
 * Parameters:  (none)
 * Return:      converted value or -1 if conversion in progress/failed
 *----------------------------------------------------------------------------*/
int32_t ADC_GetVal (void) {
  int32_t i;

  for (i = ADC_TOUT; i; i--) {
    if (AD_done) {
      return (AD_last);                 /* Return converted value             */
    }
  }
  return -1;                            /* Conversion timeout expired         */
}


/*-----------------------------------------------------------------------------
 *      ADC_Bits: Get number of ADC bits
 *
 * Parameters:  (none)
 * Return:      number of ADC bits
 *----------------------------------------------------------------------------*/
int32_t ADC_Bits (void) {
  return (ADC_BITS);
}

/*-----------------------------------------------------------------------------
 * End of file
 *----------------------------------------------------------------------------*/
