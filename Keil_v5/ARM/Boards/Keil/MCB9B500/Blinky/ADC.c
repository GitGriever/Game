/*----------------------------------------------------------------------------
 * Name:    ADC.c
 * Purpose: low level ADC functions
 * Note(s):        - ADC works in Interrupt mode
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2011 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include "mcu.h"
#include "ADC.h"

uint16_t AD_last;                         /* Last converted value             */
uint8_t  AD_done = 0;                     /* AD conversion done flag          */

/*----------------------------------------------------------------------------
  Function that initializes ADC
 *----------------------------------------------------------------------------*/
void ADC_Init (void) {

  /* configure Pins used for ADC                                              */
  FM3_GPIO->PFR1    &= ~(0x0001);         /* P10 is GPIO                      */
  FM3_GPIO->DDR1    &= ~(0x0001);         /* P10 is input                     */
  FM3_GPIO->ADE     |=  (0x0001);         /* P10 is analog input              */

  /* Setup and initialize ADC converter                                       */
  FM3_ADC0->ADCEN   |=  (0x01);           /* operation enable state           */ 
  while (!(FM3_ADC0->ADCEN & (1 << 1)));  /* wait until in operation state    */ 

  FM3_ADC0->ADCR     = ((1 << 3) |        /* enable Scan conversion interrupt */
                          (0 << 0) );
  FM3_ADC0->SFNS     =  (0x00);           /* only use one FIFO stage          */
  FM3_ADC0->SCIS0    =  (0x01);           /* chn. AN0 selected for conversion */
  FM3_ADC0->ADSS0   &= ~(0x01);           /* use ADST0 sampling time register */
  FM3_ADC0->ADST0    =  (0x09);           /* 2,5us @ 40MHz (25ns * (9+1) * 1) */

  NVIC_EnableIRQ(ADC0_IRQn);              /* enable ADC Interrupt             */
}


/*----------------------------------------------------------------------------
  start AD Conversion
 *----------------------------------------------------------------------------*/
void ADC_StartCnv (void) {

  FM3_ADC0->SCCR  =  (0x01);              /* Start A/D Conversion             */
}

/*----------------------------------------------------------------------------
  A/D IRQ: Executed when A/D Conversion is done
 *----------------------------------------------------------------------------*/
void ADC0_IRQHandler(void) {

  FM3_ADC0->ADCR &= ~0x80;                /* clear SCIF bit                   */  
  AD_last = (FM3_ADC0->SCFD >> 20) & ADC_VALUE_MAX;
  AD_done = 1;

  FM3_ADC0->SCCR |= (0x10);               /* clear FIFO                       */
}
