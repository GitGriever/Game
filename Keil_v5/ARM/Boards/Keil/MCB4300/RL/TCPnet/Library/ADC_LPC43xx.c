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

#include <LPC43xx.h>                    /* LPC43xx Definitions                */
#include "ADC.h"

#define ADC_BITS      10                /* Number of A/D converter bits       */
#define ADC_TOUT 2000000                /* Approx. A/D conversion timeout     */

static volatile uint16_t AD_last;       /* Last converted value               */
static volatile uint8_t  AD_done;       /* AD conversion done flag            */

/* Clock Control Unit register bits */
#define CCU_CLK_CFG_RUN   (1 << 0)
#define CCU_CLK_CFG_AUTO  (1 << 1)
#define CCU_CLK_STAT_RUN  (1 << 0)

/*-----------------------------------------------------------------------------
 *      ADC_Init: Initialize Analog to Digital Converter
 *
 * Parameters:  (none)
 * Return:      (none)
 *----------------------------------------------------------------------------*/
void ADC_Init (void) {
  /* Setup potentiometer pin ADC0_1 */
  LPC_CCU1->CLK_APB3_ADC0_CFG = CCU_CLK_CFG_AUTO | CCU_CLK_CFG_RUN;
  while (!(LPC_CCU1->CLK_APB3_ADC0_STAT & CCU_CLK_STAT_RUN));

  /* Configure ADC0_1 */
  LPC_ADC0->CR = (1UL <<  1) |          /* Select ADCx_1 pin for conversion   */
                 (2UL <<  8) |          /* 12MHz / (2+1) = 4MHz               */
                 (1UL << 21) ;          /* ADC is operational                 */

  /* Enable Channel 1 interrupt */
  LPC_ADC0->INTEN |= (1UL << 1);
  NVIC_EnableIRQ (ADC0_IRQn);
}


/*-----------------------------------------------------------------------------
 *      ADC_UnInit: Uninitialize Analog to Digital Converter
 *                  (set hardware to reset state)
 *
 * Parameters:  (none)
 * Return:      (none)
 *----------------------------------------------------------------------------*/
void ADC_UnInit (void) {
  LPC_CCU1->CLK_APB3_ADC0_CFG = 0;          /* Disable ADC0 branch clock      */
}


/*-----------------------------------------------------------------------------
  ADC Interrupt Handler
 *----------------------------------------------------------------------------*/
void ADC0_IRQHandler (void) {

  LPC_ADC0->GDR;                            /* Clear IRQ request flag       */
  AD_last = (LPC_ADC0->DR[1] >> 6) & 0x3FF; /* Read value and clear IRQ     */
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
  LPC_ADC0->CR |= (1UL << 24);          /* Start conversion                   */
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
