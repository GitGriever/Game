/*-----------------------------------------------------------------------------
 * Name:    ADC.c
 * Purpose: Low level ADC functions
 * Note(s): 
 *-----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2004-2012 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <XMC4500.h>                    /* XMC4500 definitions                */
#include "ADC.h"

#define ADC_BITS              12        /* Number of A/D converter bits       */
#define ADC_TOUT         2000000        /* Approx. A/D conversion timeout     */
#define ADC_VALUE_MAX      0xFFF        /* Maximum value from the ADC         */

static volatile uint16_t AD_last;       /* Last converted value               */
static volatile uint8_t  AD_done;       /* AD conversion done flag            */


/*-----------------------------------------------------------------------------
 *      ADC_Init: Initialize Analog to Digital Converter
 *
 * Parameters:  (none)
 * Return:      (none)
 *----------------------------------------------------------------------------*/
void ADC_Init (void) {
  /* Setup potentiometer pin */

  SCU_RESET->PRCLR0 |= ( 1 <<  0) ;          /* De-assert VADC Reset          */

  VADC->CLC       =   0;                     /* enable the VADC module clock  */
  VADC->GLOBCFG   = ((1UL << 15) |           /* enable DIVA write control     */
                     (6UL <<  0)  );         /* set DIVA to 6                 */
  /* Arbitration */
  VADC_G0->ARBCFG = ((3UL << 16) |           /* ANONS normal operation        */ 
                     (3UL <<  0)  );         /* ANONC normal operation        */
  VADC_G0->ARBPR  = ((1UL << 26) |           /* enable arbitration slot 02    */
                     (1UL <<  8)  );         /* priority set to 1             */

  /* BACKGROUND SOURCE */
  VADC->BRSSEL[0] =  (1UL << 1);             /* enable input chn 1 group 0    */
  VADC->BRSCTRL   = ((1UL << 23) |           /* enable write control GTMODE,. */
                     (1UL << 15)  );         /* enable write control XTMODE,. */

  VADC->BRSMR     = ((1UL <<  0)  );         /* ENGT = 01B                    */

  VADC->BRSMR    |= (1UL <<  3);             /* enable source interrupt       */
  NVIC_EnableIRQ(VADC0_C0_0_IRQn);           /* enable ADC Interrupt          */
}


/*-----------------------------------------------------------------------------
 *      ADC_UnInit: Uninitialize Analog to Digital Converter
 *                  (set hardware to reset state)
 *
 * Parameters:  (none)
 * Return:      (none)
 *----------------------------------------------------------------------------*/
void ADC_UnInit (void) {
  /* Reset A/D Converter */
  SCU_RESET->PRCLR0 |= ( 1 <<  0) ;          /* De-assert VADC Reset          */
}


/*-----------------------------------------------------------------------------
  ADC Interrupt Handler
 *----------------------------------------------------------------------------*/
void VADC0_C0_0_IRQHandler (void) {

  AD_last = VADC_G0->RES[0] & ADC_VALUE_MAX; /* Store converted value         */
  AD_done = 1;
}

/*-----------------------------------------------------------------------------
 *      ADC_StartCnv:  Start A/D conversion 
 *
 * Parameters:  (none)
 * Return:      (none)
 *----------------------------------------------------------------------------*/
void ADC_StartCnv (void) {
  AD_done = 0;
  VADC->BRSMR    |=  (1UL <<  9);            /* generate Load event           */
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
