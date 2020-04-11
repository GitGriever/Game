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
 * Copyright (c) 2011-2013 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <sam3u.h>
#include "ADC.h"

uint16_t AD_last;                       /* Last converted value               */
uint8_t  AD_done = 0;                   /* AD conversion done flag            */

/*----------------------------------------------------------------------------
  Function that initializes ADC
    5 MHz operation, 20µs startup time, 600ns track and hold time
 *----------------------------------------------------------------------------*/
void ADC_Init (void) {

  PMC->PMC_WPMR = 0x504D4300;           /* Disable write protect              */

  /* Setup and initialize ADC pins                                            */
  PMC->PMC_PCER0 = (1 << ID_PIOB);      /* enable clock for PB (ADC input)    */

  PIOB->PIO_IDR  = 
  PIOB->PIO_ODR  = 
  PIOB->PIO_PER  = (PIO_PB4);            /* Setup Pins PB4 for ADC  CHN3      */

  /* Setup and initialize ADC converter                                       */
  PMC->PMC_PCER0 =  ( 1 << ID_ADC12B);  /* enable clock for12-bit ADC         */

  ADC12B->ADC12B_CR  =   ( 1 <<  0);    /* reset ADC12B                       */
  ADC12B->ADC12B_MR  =   ( 0 <<  0) |   /* Starting conversion by software    */
                         ( 0 <<  1) |   /* external trigger                   */
                         ( 0 <<  4) |   /* 12-bit resolution                  */
                         ( 0 <<  5) |   /* Normal Mode                        */
                         ( 7 <<  8) |   /* ADC clck =   5.25 MHz              */
                         (12 << 16) |   /* startup  =  19 us                  */
                         ( 2 << 24);    /* shtim    = 570 ns                  */

  ADC12B->ADC12B_CHER =  ( 1 <<  3);    /* enable channel 3                   */


#ifndef __ADC_IRQ
  /* Peripheral DMA (PDC) configuration --------------------------------------*/
  ADC12B->ADC12B_RPR  = (uint32_t)&(AD_last); 
  ADC12B->ADC12B_RCR  = 1; 
  ADC12B->ADC12B_PTCR =  ( 1 <<  0);    /* enable RX transfer                 */

  ADC12B->ADC12B_IER  =  ( 1 << 18);    /* enable End of Receive Buffer irq   */
#else
  ADC12B->ADC12B_IER  =  ( 1 <<  3);    /* enable channel 3  EOC irq          */
#endif

  NVIC_EnableIRQ(ADC12B_IRQn);          /* enable ADC Interrupt               */

  PMC->PMC_WPMR = 0x504D4301;           /* Enable write protect               */
}


/*----------------------------------------------------------------------------
  start AD Conversion
 *----------------------------------------------------------------------------*/
void ADC_StartCnv (void) {

  ADC12B->ADC12B_CR   |=  ( 1 <<  1);   /* Start A/D conversion               */
}


/*----------------------------------------------------------------------------
  stop AD Conversion
 *----------------------------------------------------------------------------*/
void ADC_StopCnv (void) {

  ADC12B->ADC12B_CR   &= ~( 1 <<  1);   /* Stop A/D conversion                */
}


/*----------------------------------------------------------------------------
  A/D IRQ: Executed when A/D Conversion is done
 *----------------------------------------------------------------------------*/
void ADC12B_IRQHandler(void) {
  uint32_t adc_sr;

  adc_sr = (ADC12B->ADC12B_SR & ADC12B->ADC12B_IMR);
  if (adc_sr & (1 << 3)) {                /* ADC chn3 EOC interrupt?          */
    AD_last = ADC12B->ADC12B_CDR[3];
    AD_done = 1;
  }

  if (adc_sr & (1 << 18)) {               /* ADC End of RX Buffer interrupt?  */
    ADC12B->ADC12B_RPR  = (uint32_t)&(AD_last); 
    ADC12B->ADC12B_RCR  = 1; 

    AD_done = 1;
  }

}
