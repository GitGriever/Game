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
 * Copyright (c) 2004-2013 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <stm32f4xx.h>                  /* STM32F4xx Definitions              */
#include "ADC.h"

#define ADC_BITS      12                /* Number of A/D converter bits       */
#define ADC_TOUT  200000                /* Approx. A/D conversion timeout     */

#define ADC_MAX_VAL ((1<<ADC_BITS)-1)   /* Maximum value from the ADC         */

static uint16_t last;                   /* Last converted value               */
static uint8_t  done;                   /* AD conversion done flag            */

/*-----------------------------------------------------------------------------
 *      ADC_Init: Initialize Analog to Digital Converter
 *
 * Parameters:  (none)
 * Return:      (none)
 *----------------------------------------------------------------------------*/
void ADC_Init (void) {
  /* Setup potentiometer pin PF9 and A/D converter ADC3 */

  RCC->APB2ENR |= (1UL <<  10);         /* Enable ADC3 clock                  */
  RCC->AHB1ENR |= (1UL <<   5);         /* Enable GPIOF clock                 */
  GPIOF->MODER |= (3UL << 2*9);         /* PF9 is in Analog mode              */

  ADC3->SQR1   =   0;
  ADC3->SQR2   =   0;
  ADC3->SQR3   =  (7UL << 0);           /* SQ1 = channel 7                   */
  ADC3->SMPR1  =  (7UL << 6);           /* Channel 7 sample time is 480 cyc. */
  ADC3->SMPR2  =   0;                   /* Clear register                     */
  ADC3->CR1    =  (1UL << 8);           /* Scan mode on                       */

  ADC3->CR2   |=  (1UL << 3);           /* Initialize calibration registers   */
  while (ADC3->CR2 & (1UL << 3));       /* Wait for initialization to finish  */
  ADC3->CR2   |=  (1UL << 2);           /* Start calibration                  */
  while (ADC3->CR2 & (1UL << 2));       /* Wait for calibration to finish     */

  ADC3->CR1   |=  (1UL << 5);           /* Enable EOC interrupt               */
  NVIC_EnableIRQ(ADC_IRQn);             /* Enable ADC Interrupt               */
  ADC3->CR2   |=  (1UL << 0);           /* ADC enable                         */
}


/*-----------------------------------------------------------------------------
 *      ADC_UnInit: Uninitialize Analog to Digital Converter
 *                  (set hardware to reset state)
 *
 * Parameters:  (none)
 * Return:      (none)
 *----------------------------------------------------------------------------*/
void ADC_UnInit (void) {
  RCC->APB2ENR &= ~(1UL <<  10);        /* Disable ADC3 clock                 */
  GPIOF->MODER &= ~(3UL << 2*9);        /* PF9 input (reset state)            */
}


/*-----------------------------------------------------------------------------
 *      ADC_StartCnv:  Start A/D conversion
 *
 * Parameters:  (none)
 * Return:      (none)
 *----------------------------------------------------------------------------*/
void ADC_StartCnv (void) {
  done  = 0;
  ADC3->CR2 |= (1 << 30);               /* Start conversion                   */
}


/*-----------------------------------------------------------------------------
 *      ADC_GetVal:  Get converted value from the ADC
 *
 * Parameters:  (none)
 * Return:      converted value or -1 if conversion in progress/failed
 *----------------------------------------------------------------------------*/
int32_t ADC_GetVal (void) {

  if (done) {
    done = 0;
    return (last);                      /* Return converted value             */
  }
  return -1;                            /* Conv. in progress or tout expired  */
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
 *      ADC_IRQHandler: A/D Converter Interrupt Handler
 *
 * Parameters:  (none)
 * Return:      (none)
 *----------------------------------------------------------------------------*/
void ADC_IRQHandler (void) {

  if (ADC3->SR & (1<<1)) {              /* ADC EOC interrupt?                 */
    last  = (ADC3->DR & ADC_MAX_VAL);
    done  = 1;
    ADC3->SR &= ~(1<<1);                /* Clear EOC interrupt                */
  }
}

/*-----------------------------------------------------------------------------
 * End of file
 *----------------------------------------------------------------------------*/
