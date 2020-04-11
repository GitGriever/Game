/*----------------------------------------------------------------------------
 * Name:    RTC.c
 * Purpose: Real Time Clock usage for STM32
 * Note(s): 
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

#include "STM32F10x.h"


unsigned int ledPosSec   = 0;                     /* led position for second  */
unsigned int ledPosAlarm = 2;                     /* led position for Alarm   */
unsigned int ledRtcSec   = 0;


/*----------------------------------------------------------------------------
  RTC Interrupt Handler
 *----------------------------------------------------------------------------*/
void RTC_IRQHandler(void) {

  if (RTC->CRL & RTC_CRL_SECF) {                  /* check second flag        */
    RTC->CRL &= ~RTC_CRL_SECF;                    /* clear second flag        */
    if ((ledRtcSec ^= 1) == 0) 
      GPIOB->BSRR = 1UL << (ledPosSec+8);         /* Second LED on            */
    else
      GPIOB->BRR  = 1UL << (ledPosSec+8);         /* Second LED off           */
    }

  if (RTC->CRL & RTC_CRL_ALRF) {                  /* check alarm flag         */
    RTC->CRL &= ~RTC_CRL_ALRF;                    /* clear alarm flag         */
    GPIOB->BSRR = 1 << (ledPosAlarm+8);           /* Alarm LED on             */
    }

}


/*----------------------------------------------------------------------------
  initialize GPIO Pins
 *----------------------------------------------------------------------------*/
void GPIO_Init (void) {

  /* configure GPIOs used for LEDs */
  RCC->APB2ENR |=  RCC_APB2ENR_IOPBEN;            /* Enable GPIOB clock       */
  GPIOB->ODR   &= ~(0x0000FF00);                  /* switch off LEDs          */
  GPIOB->CRH    =  (0x33333333);                  /* PB8..15 GPIO             */
}


/*----------------------------------------------------------------------------
  initialize RTC
 *----------------------------------------------------------------------------*/
void RTC_Init (void) {
  uint32_t cnt = ((12 * 3600UL) +                 /* counter hour value       */
                  ( 0 *   60UL) +                 /* counter minute value     */
                  ( 0 *    1UL)  );               /* counter second value     */
  uint32_t alr = ((12 * 3600UL) +                 /* alarm   hour value       */
                  ( 0 *   60UL) +                 /* alarm   minute value     */
                  (20 *    1UL)  );               /* alarm second value       */
                                                                              
  RCC->APB1ENR |= RCC_APB1ENR_PWREN;              /* enable clock for Power IF*/
  PWR->CR      |= PWR_CR_DBP;                     /* enable access to RTC,BDC */

  RCC->BDCR |= RCC_BDCR_LSEON;                    /* enable LSE               */
  while ((RCC->BDCR & RCC_BDCR_LSERDY) == 0);     /* Wait for LSERDY = 1      */

  RCC->BDCR |= (RCC_BDCR_RTCSEL_LSE |             /* LSE as RTC clock source  */
                RCC_BDCR_RTCEN       );           /* RTC clock enable         */

  RTC->CRL  |=  RTC_CRL_CNF;                      /* set configuration mode   */

  RTC->PRLH  = ((32768 - 1) >> 16) & 0x00FF;
  RTC->PRLL  = ((32768 - 1)      ) & 0xFFFF;

  RTC->CNTH  = (cnt >> 16) & 0xFFFF;
  RTC->CNTL  = (cnt      ) & 0xFFFF;
  RTC->ALRH  = (alr >> 16) & 0xFFFF;
  RTC->ALRL  = (alr      ) & 0xFFFF;

  RTC->CRH   = (RTC_CRH_SECIE |                   /* Second Interrupt Enable  */
                RTC_CRH_ALRIE  );                 /* Alarm  Interrupt Enable  */
  NVIC_EnableIRQ(RTC_IRQn);                       /* RTC    Interrupt enable  */

  RTC->CRL  &= ~RTC_CRL_CNF;                      /* reset configuration mode */
  while ((RTC->CRL & RTC_CRL_RTOFF) == 0);

  PWR->CR   &= ~PWR_CR_DBP;                       /* disable access to RTC    */
}


/*----------------------------------------------------------------------------
  MAIN function
 *----------------------------------------------------------------------------*/
int main (void) {

  GPIO_Init ();
  RTC_Init ();

  while (1) {
    ;
  }
}
