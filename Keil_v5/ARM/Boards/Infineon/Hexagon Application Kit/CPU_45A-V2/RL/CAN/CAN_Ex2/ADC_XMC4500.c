/*------------------------------------------------------------------------------
 * Name:    ADC_XMC4500.c
 * Purpose: Low level ADC functions for Infineon XMC4500 Device Series
 * Note(s): possible defines select the used ADC interface:
 *            __ADC_IRQ   - ADC works in Interrupt mode
 *                        - ADC works in polling mode (default)
 *------------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2004-2013 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include "XMC4500.h"                         /* XMC4500 definitions           */
#include "ADC.h"

uint16_t AD_last;                            /* Last converted value          */
uint8_t  AD_done = 0;                        /* AD conversion done flag       */

/*----------------------------------------------------------------------------
  Function that initializes ADC
 *----------------------------------------------------------------------------*/
void ADC_Init (void) {

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

#ifdef __ADC_IRQ
  VADC->BRSMR    |= (1UL <<  3);             /* enable source interrupt       */

  NVIC_EnableIRQ(VADC0_C0_0_IRQn);           /* enable ADC Interrupt          */
#endif

}


/*----------------------------------------------------------------------------
  start AD Conversion
 *----------------------------------------------------------------------------*/
void ADC_StartCnv (void) {

  VADC->BRSMR    |=  (1UL <<  9);            /* generate Load event           */
}


/*----------------------------------------------------------------------------
  stop AD Conversion
 *----------------------------------------------------------------------------*/
void ADC_StopCnv (void) {

  ;                                          /* stop conversion               */
}


/*----------------------------------------------------------------------------
  get converted AD value
 *----------------------------------------------------------------------------*/
uint16_t ADC_GetCnv (void) {

#ifndef __ADC_IRQ
  while (!(VADC_G0->RES[0] & ( 1UL << 31))); /* Wait for Conversion end       */
  AD_last = VADC_G0->RES[0] & ADC_VALUE_MAX; /* Store converted value         */

  AD_done = 1;
#endif

  return(AD_last);
}


/*----------------------------------------------------------------------------
  A/D IRQ: Executed when A/D Conversion is done
 *----------------------------------------------------------------------------*/
#ifdef __ADC_IRQ
void VADC0_C0_0_IRQHandler(void) {

  AD_last = VADC_G0->RES[0] & ADC_VALUE_MAX; /* Store converted value         */

  AD_done = 1;
}
#endif
