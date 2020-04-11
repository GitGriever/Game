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
 * Copyright (c) 2008-2011 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include "NUC1xx.H"                       /* NUC1xx definitions               */
#include "ADC.h"

uint16_t AD_last;                         /* Last converted value             */
uint8_t  AD_done = 0;                     /* AD conversion done flag          */

/*----------------------------------------------------------------------------
  Function that initializes ADC
 *----------------------------------------------------------------------------*/
void ADC_Init (void) {

  SYS->GPAMFP.ADC0 = 1;                   /* ADC0 function is selected to PA.0*/

  SYSCLK->APBCLK.ADC_EN = 1;              /* A/D Converter Clock Enable       */

  ADC->ADCR.ADMD = 0;                     /* A/D Converter Single conversion  */ 
  ADC->ADCR.ADIE = 1;                     /* A/D Interrupt Enable             */
  ADC->ADCR.ADEN = 1;                     /* A/D Converter Enable             */
  ADC->ADCHER.CHEN = 1;                   /* Analog Input Channel 0 Enable    */

#ifdef __ADC_IRQ
  ADC->ADCR.ADIE = 1;                     /* A/D Interrupt Enable             */

  NVIC_EnableIRQ(ADC_IRQn);               /* enable ADC Interrupt             */
#endif
}


/*----------------------------------------------------------------------------
  start AD Conversion
 *----------------------------------------------------------------------------*/
void ADC_StartCnv (void) {

  ADC->ADCR.ADST = 1;                     /* A/D Conversion Start             */
}


/*----------------------------------------------------------------------------
  stop AD Conversion
 *----------------------------------------------------------------------------*/
void ADC_StopCnv (void) {

  ADC->ADCR.ADST = 0;                     /* A/D Conversion Stop              */
}


/*----------------------------------------------------------------------------
  get converted AD value
 *----------------------------------------------------------------------------*/
uint16_t ADC_GetCnv (void) {

#ifndef __ADC_IRQ
  while (!(ADC->ADSR.ADF));               /* Wait for Conversion end          */
  AD_last = (uint32_t)(ADC->ADDR[0].RSLT);
  ADC->ADSR.ADF = 1;                      /* A/D Conversion End Flag reset    */

  AD_done = 1;
#endif

  return(AD_last);
}


/*----------------------------------------------------------------------------
  A/D IRQ: Executed when A/D Conversion is done
 *----------------------------------------------------------------------------*/
#ifdef __ADC_IRQ
void ADC_IRQHandler(void) {               /* called when A/D Conv. is done    */

  if (ADC->ADSR.ADF) {                    /* A/D Conversion End Flag set ?    */
    AD_last = (uint32_t)(ADC->ADDR[0].RSLT);
    ADC->ADSR.ADF = 1;                    /* A/D Conversion End Flag reset    */

    AD_done = 1;
 }
}
#endif
