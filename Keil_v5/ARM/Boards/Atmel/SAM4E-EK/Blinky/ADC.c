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
 * Copyright (c) 2013 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <SAM4E.h>                        /* SAM4E definitions                */
#include "ADC.h"

uint32_t AD_last;                         /* Last converted value             */
uint8_t  AD_done = 0;                     /* AD conversion done flag          */

/*----------------------------------------------------------------------------
  Function that initializes ADC
    5 MHz operation, 20µs startup time, 600ns track and hold time
 *----------------------------------------------------------------------------*/
void ADC_Init (void) {

  PMC->PMC_WPMR = 0x504D4300;             /* Disable write protect            */

  /* Setup and initialize ADC pins                                            */
  PMC->PMC_PCER0 = (1 << ID_PIOB);        /* enable clock for PB (ADC input)  */

  PIOB->PIO_IDR  =
  PIOB->PIO_ODR  =
  PIOB->PIO_PER  = (PIO_PB1);             /* Setup Pin PB1 for ADC channel 5  */

  /* Setup and initialize ADC converter                                       */
  PMC->PMC_PCER0 =  ( 1 << ID_AFEC0);     /* enable clock for AnalogFrontEnd0 */

  AFEC0->AFEC_CR  =  ( 1 <<  0);          /* reset AFE                        */
  AFEC0->AFEC_MR  =  ( 7 <<  8) |         /* ADC clck =   6 MHz               */
                     ( 6 << 16) |         /* Start Up Time                    */
                     ( 0 << 20) |         /* Analog Settling Time             */
                     ( 1 << 23) |         /* setting for each channel         */
                     ( 3 << 24) |         /* Tracking Time                    */
                     ( 1 << 28)  ;        /* Transfer Period                  */

  AFEC0->AFEC_EMR =  ( 0 << 16) |         /* 12 bit resolution                */
                     ( 0 << 24)  ;        /* set no TAG in LCDR register      */

  AFEC0->AFEC_CSELR = 5;
  AFEC0->AFEC_COCR  = 0x800;

  AFEC0->AFEC_CHER =  ( 1 <<  5);          /* enable channel 5                 */

  AFEC0->AFEC_CR   =  ( 1 <<  3);          /* start Autocal                    */
  while (!(AFEC0->AFEC_ISR & (1ul << 31)));

#ifndef __ADC_IRQ
  /* Peripheral DMA (PDC) configuration --------------------------------------*/
  AFEC0->AFEC_RPR  = (uint32_t)&(AD_last);
  AFEC0->AFEC_RCR  = 1;
  AFEC0->AFEC_PTCR = ( 1 <<  0);          /* enable RX transfer               */

  AFEC0->AFEC_IER  = ( 1 << 27);          /* enable End of Receive Buffer irq */
#else
  AFEC0->AFEC_IER  = ( 1 <<  5);          /* enable channel 5  EOC irq        */
#endif

  NVIC_EnableIRQ(AFEC0_IRQn);             /* enable ADC Interrupt             */

  PMC->PMC_WPMR = 0x504D4301;             /* Enable write protect             */

}


/*----------------------------------------------------------------------------
  start AD Conversion
 *----------------------------------------------------------------------------*/
void ADC_StartCnv (void) {

  AFEC0->AFEC_CR   |=  ( 1 <<  1);          /* Start A/D conversion           */
}


/*----------------------------------------------------------------------------
  stop AD Conversion
 *----------------------------------------------------------------------------*/
void ADC_StopCnv (void) {

  AFEC0->AFEC_CR   |=  ( 1 <<  0);          /* Stop A/D conversion            */
}


/*----------------------------------------------------------------------------
  A/D IRQ: Executed when A/D Conversion is done
 *----------------------------------------------------------------------------*/
void AFEC0_Handler(void) {
  uint32_t afec0_isr;

  afec0_isr = (AFEC0->AFEC_ISR & AFEC0->AFEC_IMR);

  if (afec0_isr & (1 << 5)) {             /* ADC chn5 EOC interrupt?          */
    AD_last = AFEC0->AFEC_CDR & ADC_VALUE_MAX;

    AD_done = 1;
  }

  if (afec0_isr & (1 << 27)) {            /* ADC End of RX Buffer interrupt?  */
    AFEC0->AFEC_RPR  = (uint32_t)&(AD_last);
    AFEC0->AFEC_RCR  = 1;

    AD_done = 1;
  }

}
