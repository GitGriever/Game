/*-----------------------------------------------------------------------------
 * Name:    ADC_LPC177x_8x.c
 * Purpose: Low level ADC functions
 * Note(s): ADC works in Interrupt mode
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

#include "LPC177x_8x.h"                 /* LPC177x_8x definitions             */
#include "ADC.h"

#define ADC_BITS      12                /* Number of A/D converter bits       */
#define ADC_TOUT 2000000                /* Approx. A/D conversion timeout     */

#define ADC_VALUE_MAX (0xFFF)           /* Maximum ADC value                  */

static volatile uint16_t AD_last;       /* Last converted value               */
static volatile uint8_t  AD_done;       /* AD conversion done flag            */


/*-----------------------------------------------------------------------------
 *      ADC_Init: Initialize Analog to Digital Converter
 *
 * Parameters:  (none)
 * Return:      (none)
 *----------------------------------------------------------------------------*/
void ADC_Init (void) {

  LPC_SC->PCONP    |= (( 1UL << 12) |        /* enable power to ADC           */
                       ( 1UL << 15)  );      /* enable power to IOCON         */

  LPC_IOCON->P0_25  =  ( 1UL <<  0);         /* P0.25 is AD0.2                */

  LPC_ADC->CR       = (( 1UL <<  2) |        /* select AD0.2 pin              */
                       ( 4UL <<  8) |        /* ADC clock is 60MHz / 5        */
                       ( 1UL << 21)  );      /* enable ADC                    */

  LPC_ADC->INTEN    =  ( 1UL <<  8);         /* global enable interrupt       */

  NVIC_EnableIRQ(ADC_IRQn);                  /* enable ADC Interrupt          */
}


/*-----------------------------------------------------------------------------
 *      ADC_UnInit: Uninitialize Analog to Digital Converter
 *                  (set hardware to reset state)
 *
 * Parameters:  (none)
 * Return:      (none)
 *----------------------------------------------------------------------------*/
void ADC_UnInit (void) {
  LPC_SC->PCONP &= (1UL << 12);              /* disable power to ADC          */
}


/*----------------------------------------------------------------------------
  A/D IRQ: Executed when A/D Conversion is done
 *----------------------------------------------------------------------------*/
void ADC_IRQHandler(void) {
  volatile uint32_t adstat;

  adstat = LPC_ADC->STAT;                    /* Read ADC clears interrupt     */

  AD_last = (LPC_ADC->GDR >> 4) & ADC_VALUE_MAX;   /* Store converted value   */

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
  LPC_ADC->CR &= ~( 7UL << 24);              /* stop conversion               */
  LPC_ADC->CR |=  ( 1UL << 24);              /* start conversion              */
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
