/*------------------------------------------------------------------------------
 * Name:    ADC.c
 * Purpose: Low level ADC functions for Freescale K20D5
 * Note(s): 
 *------------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2004-2013 KEIL - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <MK20D5.H>
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
 *      ADC_Calib: Perform calibration of the ADC. For best calibration results,
 *                 maximum avg. samples should be set (32) and ADC clock should
 *                 be less or equal to 4MHz. After calibration, ADC can be
 *                 reconfigured as desired.
 *
 * Parameters:  (none)
 * Return:      (none)
 *----------------------------------------------------------------------------*/
static uint32_t ADC_Calib (void) {
  uint16_t val;

  ADC0->SC2 &= ~ADC_SC2_ADTRG_MASK;
  ADC0->SC3 &= ~ADC_SC3_ADCO_MASK;
  ADC0->SC3 |=  ADC_SC3_AVGE_MASK | ADC_SC3_AVGS_MASK;

  ADC0->SC3 |=  ADC_SC3_CAL_MASK;       /* Start calibration (set CAL bit)    */

  while (!(ADC0->SC1[0] & ADC_SC1_COCO_MASK));  /* Wait until conversion done */
  if (ADC0->SC3 & ADC_SC3_CALF_MASK) {
    return (1);                         /* Calibration failed                 */
  }

  val  = ADC0->CLPD + ADC0->CLPS;
  val += ADC0->CLP0 + ADC0->CLP1 + ADC0->CLP2 + ADC0->CLP3 + ADC0->CLP4;
  val /= 2;                             /* Variable must be divided by two    */
  ADC0->PG = val | 0x8000;              /* MBS must be set                    */

  val  = ADC0->CLMD + ADC0->CLMS;
  val += ADC0->CLM0 + ADC0->CLM1 + ADC0->CLM2 + ADC0->CLM3 + ADC0->CLM4;
  val /= 2;                             /* Variable must be divided by two    */
  ADC0->MG = val | 0x8000;              /* MBS must be set                    */

  ADC0->SC3 &= ADC_SC3_CAL_MASK;        /* Clear CAL bit                      */
  return (0);                           /* ADC Calibration Done               */

}

/*------------------------------------------------------------------------------
 *      ADC_Init: Initialize Analog to Digital Converter
 *                Channel: DAD1
 *
 * Parameters:  (none)
 * Return:      (none)
 *----------------------------------------------------------------------------*/
void ADC_Init (void) {
  /* Setup potentiometer pin ADC0_DM3 and A/D converter ADC0 */

  SIM->SCGC6 |= SIM_SCGC6_ADC0_MASK;    /* Enable ADC 0 gate clocking         */

  ADC0->CFG1 =   (3 << 5) |             /* ADC clock is (Input clock)/8       */
                 (1 << 4) |             /* Select Long sample time            */
                 (3 << 2) |             /* Select 16-bit conversion           */
                 (1 << 0) ;             /* Input clock is (Bus clock)/2       */

  ADC0->CFG2 =   (0 << 4) |             /* ADxxa channels are selected        */
                 (0 << 3) |             /* Async clock output disabled        */
                 (0 << 2) |             /* Normal conversion sequence         */
                 (0 << 0) ;             /* 20 extra ADCK cycles               */

  ADC0->SC1[0] = (1 << 6) |             /* Conversion complete interrupt enabled */
                 (1 << 5) |             /* DIFF conversions and input ch are selected */
                 (3 << 0) ;             /* Select DAD3 as a input             */

  ADC0->SC2  =   (0 << 6) |             /* Software trigger selected          */
                 (0 << 5) |             /* Compare function disabled          */
                 (0 << 4) |             /* Compare configures less than threshold */
                 (0 << 3) |             /* Range function disabled            */
                 (0 << 2) |             /* DMA is disabled                    */
                 (0 << 0) ;             /* Default voltage reference          */
  
  ADC0->SC3 |=   (1 << 2) |             /* Hardware average function enabled  */
                 (3 << 0) ;                

  ADC_Calib ();                         /* Perform calibration of the ADC     */
#if __ADC_IRQ
  NVIC_EnableIRQ(ADC0_IRQn);            /* Enable ADC Interrupt               */
#endif
}


/*------------------------------------------------------------------------------
 *      ADC_UnInit: Uninitialize Analog to Digital Converter
 *                  (set hardware to reset state)
 *
 * Parameters:  (none)
 * Return:      (none)
 *----------------------------------------------------------------------------*/
void ADC_UnInit (void) {
}


/*------------------------------------------------------------------------------
 *      ADC_StartCnv:  Start A/D conversion 
 *
 * Parameters:  (none)
 * Return:      (none)
 *----------------------------------------------------------------------------*/
void ADC_StartCnv (void) {
#if __ADC_IRQ
  done  = 0;
#endif
  ADC0->SC1[0] |= 3;                    /* Trigger software conversion        */  
}


/*------------------------------------------------------------------------------
 *      ADC_GetVal:  Get converted value from the ADC
 *
 * Parameters:  (none)
 * Return:      converted value or -1 if conversion in progress/failed
 *----------------------------------------------------------------------------*/
int32_t ADC_GetVal (void) {
  int32_t val;
#if __ADC_IRQ
  if (done) {
    done = 0;
    return (last);                      /* Return converted value             */
  }
#else
  int32_t i;

  for (i = ADC_TOUT; i; i--) {
    if (ADC0->SC1[0] & ADC_SC1_COCO_MASK) { /* Wait until conversion done     */
      val = ADC0->R[0];
      if (val & 0x8000)
        val = (~val + 1) & 0x7FFF;
      return (val);                     /* Return converted value             */
    }
  }
#endif
  return -1;                            /* Conversion timeout expired         */
}


/*------------------------------------------------------------------------------
 *      ADC_Bits: Get number of ADC bits
 *
 * Parameters:  (none)
 * Return:      number of ADC bits
 *----------------------------------------------------------------------------*/
int32_t ADC_Bits (void) {
  return (ADC_BITS);
}


/*------------------------------------------------------------------------------
 *      ADC_IRQHandler: A/D Converter Interrupt Handler
 *
 * Parameters:  (none)
 * Return:      (none)
 *----------------------------------------------------------------------------*/
#if __ADC_IRQ
void ADC0_IRQHandler (void) {
  if (ADC0->SC1[0] & ADC_SC1_COCO_MASK) {
    last = ADC0->R[0];
    done = 1;
  }
}
#endif
/*------------------------------------------------------------------------------
 * End of file
 *----------------------------------------------------------------------------*/
