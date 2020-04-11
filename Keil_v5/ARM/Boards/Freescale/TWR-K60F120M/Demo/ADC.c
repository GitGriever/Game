/*------------------------------------------------------------------------------
 * Name:    ADC.c
 * Purpose: ADC functions for the 
 *          Freescale Kinetis TWR-K60F120M board
 * Note(s): 
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

#include <MK60F12.H>
#include "ADC.h"

#define __ADC_IRQ 0                     /* 0=pooling mode, 1=interrupt mode   */

#define ADC_BITS      16                /* Number of A/D converter bits       */
#define ADC_TOUT  200000                /* Approx. A/D conversion timeout     */

#define ADC_MAX_VAL ((1<<ADC_BITS)-1)   /* Maximum value from the ADC         */

#if __ADC_IRQ
static uint16_t last;                   /* Last converted value               */
static uint8_t  done;                   /* AD conversion done flag            */
#endif


/*------------------------------------------------------------------------------
 * ADC_Calib:       Perform calibration of the ADC. For best calibration 
 *                  results, maximum avg. samples should be set (32) and ADC 
 *                  clock should be less or equal to 4MHz. After calibration, 
 *                  ADC can be reconfigured as desired.
 *----------------------------------------------------------------------------*/

static uint32_t ADC_Calib (void) {
  uint16_t val;

  ADC1->SC2 &= ~ADC_SC2_ADTRG_MASK;
  ADC1->SC3 &= ~ADC_SC3_ADCO_MASK;
  ADC1->SC3 |=  ADC_SC3_AVGE_MASK | ADC_SC3_AVGS_MASK;

  ADC1->SC3 |=  ADC_SC3_CAL_MASK;       /* Start calibration (set CAL bit)    */

  while (!(ADC1->SC1[0] & ADC_SC1_COCO_MASK));  /* Wait until conversion done */
  if (ADC1->SC3 & ADC_SC3_CALF_MASK) {
    return (1);                         /* Calibration failed                 */
  }

  val  = ADC1->CLPD + ADC1->CLPS;
  val += ADC1->CLP0 + ADC1->CLP1 + ADC1->CLP2 + ADC1->CLP3 + ADC1->CLP4;
  val /= 2;                             /* Variable must be divided by two    */
  ADC1->PG = val | 0x8000;              /* MBS must be set                    */

  val  = ADC1->CLMD + ADC1->CLMS;
  val += ADC1->CLM0 + ADC1->CLM1 + ADC1->CLM2 + ADC1->CLM3 + ADC1->CLM4;
  val /= 2;                             /* Variable must be divided by two    */
  ADC1->MG = val | 0x8000;              /* MBS must be set                    */

  ADC1->SC3 &= ADC_SC3_CAL_MASK;        /* Clear CAL bit                      */
  return (0);                           /* ADC Calibration Done               */
}


/*------------------------------------------------------------------------------
 * ADC_Init:     Initialize Analog to Digital Converter
 *----------------------------------------------------------------------------*/

void ADC_Init (void) {

  /* Setup potentiometer pin ADC1_DM1 and A/D converter ADC1 */
  SIM->SCGC3  |=  SIM_SCGC3_ADC1_MASK;  /* Enable ADC 1 gate clocking         */

  ADC1->CFG1   = ( 3 << 5) |            /* ADC clock is (Input clock)/8       */
                 ( 1 << 4) |            /* Select Long sample time            */
                 ( 3 << 2) |            /* Select 16-bit conversion           */
                 ( 1 << 0) ;            /* Input clock is (Bus clock)/2       */

  ADC1->CFG2   = ( 0 << 4) |            /* ADxxa channels are selected        */
                 ( 0 << 3) |            /* Async clock output disabled        */
                 ( 0 << 2) |            /* Normal conversion sequence         */
                 ( 0 << 0) ;            /* 20 extra ADCK cycles               */

  ADC1->SC1[0] = ( 1 << 6) |            /* Conversion complete interrupt enabled */
                 (20 << 0) ;            /* Select AD20 as a input             */

  ADC1->SC2    = ( 0 << 6) |            /* Software trigger selected          */
                 ( 0 << 5) |            /* Compare function disabled          */
                 ( 0 << 4) |            /* Compare configures less than threshold */
                 ( 0 << 3) |            /* Range function disabled            */
                 ( 0 << 2) |            /* DMA is disabled                    */
                 ( 0 << 0) ;            /* Default voltage reference          */
  
  ADC1->SC3   |= ( 1 << 2) |            /* Hardware average function enabled  */
                 ( 3 << 0) ;                

  ADC_Calib ();                         /* Perform calibration of the ADC     */
#if __ADC_IRQ
  NVIC_EnableIRQ(ADC1_IRQn);            /* Enable ADC Interrupt               */
#endif
}


/*------------------------------------------------------------------------------
 * ADC_Uninit:      Uninitialize Analog to Digital Converter
 *----------------------------------------------------------------------------*/

void ADC_Uninit (void) {

  ADC1->SC3    =  0;                
  ADC1->SC2    =  0;                
  ADC1->SC1[0] =  1;                
  ADC1->CFG2   =  0;                
  ADC1->CFG1   =  0;                

  SIM->SCGC3  &= ~SIM_SCGC3_ADC1_MASK;
}


/*------------------------------------------------------------------------------
 * ADC_StartCnv:    Start A/D conversion 
 *----------------------------------------------------------------------------*/

void ADC_StartCnv (void) {

  ADC1->SC1[0] |= 20;                   /* Trigger software conversion        */  
}


/*------------------------------------------------------------------------------
 * ADC_GetVal:      Get converted value from the ADC
 *----------------------------------------------------------------------------*/

int32_t ADC_GetVal (void) {
#if __ADC_IRQ
  if (done) {
    done = 0;
    return (last);                      /* Return converted value             */
  }
#else
  int32_t val;
  int32_t i;

  for (i = ADC_TOUT; i; i--) {
    if (ADC1->SC1[0] & ADC_SC1_COCO_MASK) { /* Wait until conversion done     */
      val = ADC1->R[0];
      return (val);                     /* Return converted value             */
    }
  }
#endif
  return -1;                            /* Conversion timeout expired         */
}


/*------------------------------------------------------------------------------
 * ADC_Bits:        Get number of ADC bits
 *----------------------------------------------------------------------------*/

int32_t ADC_Bits (void) {

  return (ADC_BITS);
}


/*------------------------------------------------------------------------------
 * ADC_IRQHandler:  A/D Converter Interrupt Handler
 *----------------------------------------------------------------------------*/

#if __ADC_IRQ
void ADC1_IRQHandler (void) {

  if (ADC1->SC1[0] & ADC_SC1_COCO_MASK) {
    last = ADC1->R[0];
    done = 1;
  }
}
#endif


/*------------------------------------------------------------------------------
 * End of file
 *----------------------------------------------------------------------------*/
