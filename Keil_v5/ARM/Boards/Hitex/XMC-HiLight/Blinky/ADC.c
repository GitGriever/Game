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

#include <XMC4500.h>                    /* XMC4500 Definitions                */
#include "ADC.h"

#define ADC_BITS    12                  /* Number of A/D converter bits       */
#define ADC_TOUT    200000              /* Approx. A/D conversion timeout     */

#define ADC_MAX_VAL ((1<<ADC_BITS)-1)   /* Maximum value from the ADC         */


/*-----------------------------------------------------------------------------
 *      ADC_Init: Initialize Analog to Digital Converter
 *                Channel: VADC1
 *
 * Parameters:  (none)
 * Return:      (none)
 *----------------------------------------------------------------------------*/
void ADC_Init (void) {
  /* Setup potentiometer pin VADC_G0CH1 and A/D converter ADC1 */

  SCU_RESET->PRCLR0 = SCU_RESET_PRCLR0_VADCRS_Msk;

  VADC->CLC       = 0x00000000;         /* Enable Peripheral Clock            */
  VADC->GLOBCFG   = VADC_GLOBCFG_DIVWC_Msk |
                    0x00000006;         /* Write control and DIVA=6           */
  VADC_G0->ARBCFG = 0x00030003;         /* Normal operation                   */
  VADC_G0->ARBPR  = 0x07000001;         /* All slots acitve and prio          */

  VADC->BRSSEL[0] = 0x00000002;         /* Channels taking part CH0           */
  VADC->BRSCTRL   = 0x00808001;         /* RESREG = G0RES0                    */
  VADC->BRSMR     = 0x00000211;         /* Enable gating always               */
}


/*-----------------------------------------------------------------------------
 *      ADC_UnInit: Uninitialize Analog to Digital Converter
 *                  (set hardware to reset state)
 *
 * Parameters:  (none)
 * Return:      (none)
 *----------------------------------------------------------------------------*/
void ADC_UnInit (void) {
  SCU_RESET->PRSET0 = SCU_RESET_PRSTAT0_VADCRS_Msk ;
}


/*-----------------------------------------------------------------------------
 *      ADC_StartCnv:  Start A/D conversion 
 *
 * Parameters:  (none)
 * Return:      (none)
 *----------------------------------------------------------------------------*/
void ADC_StartCnv (void) {
  VADC->BRSMR |= 0x0000200;             /* generate Load event                */
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
    if (VADC_G0->RES[0] >> 31) {        /* Wait until conversion done         */
      return (VADC_G0->RES[0] & 0x0FFF);/* Return converted value             */
    }
  }
  return (-1);                          /* Conversion timeout expired         */
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
