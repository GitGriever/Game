/*----------------------------------------------------------------------------
 * Name:    Tamper.c
 * Purpose: Tamper usage for STM32
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


/*----------------------------------------------------------------------------
  TAMPER Interrupt Handler
 *----------------------------------------------------------------------------*/
void TAMPER_IRQHandler(void) {

  if (BKP->CSR & (BKP_CSR_TIF) ) {                /* Tamper interrupt flag    */
  PWR->CR   |= PWR_CR_DBP;                        /* enable access to RTC,BDC */
  BKP->CSR  |= (BKP_CSR_CTE |                     /* clear Tamper event       */
                BKP_CSR_CTI  );                   /* clear Tamper Interrupt   */
  PWR->CR   &= ~PWR_CR_DBP;                       /* disable access to RTC    */
	if ((BKP->DR1 == 0) &&
	    (BKP->DR2 == 0)   )
      GPIOB->BSRR = (1UL << 9);                   /* LED PB9 on               */
    else
      GPIOB->BSRR = (1UL << 8);                   /* LED PB8 on               */
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

  /* configure GPIOs for TamperButton */
  RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;             /* Enable GPIOC clock       */
  GPIOC->CRH   &= ~(0x00F00000);                  /* PC.13 is Tamper button   */
  GPIOC->CRH   |=  (0x00400000);
}


/*----------------------------------------------------------------------------
  initialize Tamper
 *----------------------------------------------------------------------------*/
void TAMP_Init (void) {

  RCC->APB1ENR |= (RCC_APB1ENR_BKPEN |            /* ena. clock for Backup IF */
                   RCC_APB1ENR_PWREN  );          /* ena. clock for Power IF  */

  PWR->CR   |= PWR_CR_DBP;                        /* enable access to RTC,BDC */
  BKP->CR    = (BKP_CR_TPE  |                     /* Tamper pin enable        */
                BKP_CR_TPAL  );                   /* Tamper active level      */
  BKP->CSR   =  BKP_CSR_TPIE;                     /* Tamper Pin IRQ enable    */
  BKP->CSR  |= (BKP_CSR_CTE |                     /* clear Tamper event       */
                BKP_CSR_CTI  );                   /* clear Tamper Interrupt   */
  PWR->CR   &= ~PWR_CR_DBP;                       /* disable access to RTC    */

  NVIC_EnableIRQ(TAMPER_IRQn);                    /* Tamper Interrupt enable  */
}


/*----------------------------------------------------------------------------
  MAIN function
 *----------------------------------------------------------------------------*/
int main (void) {

  GPIO_Init ();
  TAMP_Init ();

  PWR->CR      |= PWR_CR_DBP;                     /* enable access to RTC,BDC */
  BKP->DR1      = 0x55AA;                         /* fill BKP_DR1 register    */
  BKP->DR2      = 0x33CC;                         /* fill BKP_DR2 register    */
  PWR->CR   &= ~PWR_CR_DBP;                       /* disable access to RTC    */

  while (1) {
    ;
  }
}
