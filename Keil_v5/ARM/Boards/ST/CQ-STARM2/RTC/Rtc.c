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


uint16_t secCnt   = 0;                            /* counts the seconds       */

/*----------------------------------------------------------------------------
  Display value (0..9) on right segment
 *----------------------------------------------------------------------------*/
void displayRight (unsigned int val) {

                               /* switch off PC11,PD12,PD11,PD8,PD7,PD2,PE14 */
  GPIOC->BRR = (1<<11);
  GPIOD->BRR = (1<<12)|(1<<11)|(1<<8)|(1<<7)|(1<<2);
  GPIOE->BRR = (1<<14);

  switch (val) {
    case 0:                     /* switch on  PC11,     PD11,PD8,PD7,PD2,PE14 */
      GPIOC->BSRR = (1<<11);
      GPIOD->BSRR = (1<<11)|(1<<8)|(1<<7)|(1<<2);
      GPIOE->BSRR = (1<<14);   
      break;
    case 1:                     /* switch on                     PD7,PD2      */
      GPIOD->BSRR = (1<<7)|(1<<2);
      break;
    case 2:                     /* switch on  PC11,PD12,     PD8,    PD2,PE14 */
      GPIOC->BSRR = (1<<11);
      GPIOD->BSRR = (1<<12)|(1<<8)|(1<<2); 
      GPIOE->BSRR = (1<<14);
      break;
    case 3:                     /* switch on  PC11,PD12,     PD8,PD7,PD2      */
      GPIOC->BSRR = (1<<11);
      GPIOD->BSRR = (1<<12)|(1<<8)|(1<<7)|(1<<2);
      break;
    case 4:                     /* switch on       PD12,PD11,    PD7,PD2      */
      GPIOD->BSRR = (1<<12)|(1<<11)|(1<<7)|(1<<2);
      break;
    case 5:                     /* switch on  PC11,PD12,PD11,PD8,PD7          */
      GPIOC->BSRR = (1<<11);
      GPIOD->BSRR = (1<<12)|(1<<11)|(1<<8)|(1<<7);
      break;
    case 6:                     /* switch on  PC11,PD12,PD11,PD8,PD7,    PE14 */
      GPIOC->BSRR = (1<<11);
      GPIOD->BSRR = (1<<12)|(1<<11)|(1<<8)|(1<<7);
      GPIOE->BSRR = (1<<14);
      break;
    case 7:                     /* switch on  PC11,              PD7,PD2      */
      GPIOC->BSRR = (1<<11);
      GPIOD->BSRR = (1<<7)|(1<<2);
      break;
    case 8:                     /* switch on  PC11,PD12,PD11,PD8,PD7,PD2,PE14 */
      GPIOC->BSRR = (1<<11);
      GPIOD->BSRR = (1<<12)|(1<<11)|(1<<8)|(1<<7)|(1<<2);
      GPIOE->BSRR = (1<<14);
      break;
    case 9:                     /* switch on  PC11,PD12,PD11,PD8,PD7,PD2      */
      GPIOC->BSRR = (1<<11);
      GPIOD->BSRR = (1<<12)|(1<<11)|(1<<8)|(1<<7)|(1<<2);
      break;
    default:                    /* switch on  PC11,PD12,PD11,PD8,PD7,    PE14 */
      GPIOC->BSRR = (1<<11);
      GPIOD->BSRR = (1<<12)|(1<<11)|(1<<8);
      GPIOE->BSRR = (1<<14);
      break;
  }

}

/*----------------------------------------------------------------------------
  Display value (0..9) on left segment
 *----------------------------------------------------------------------------*/
void displayLeft (unsigned int val) {

                                /* switch off PB15,PB12,PB7,PB6,PB5,PC8,PC7   */
  GPIOB->BRR = (1<<15)|(1<<12)|(1<<7)|(1<<6)|(1<<5);
  GPIOC->BRR = (1<<8)|(1<<7);

  switch (val) {
    case 0:	                    /* switch on  PB15,PB12,PB7,PB6,PB5,          */
      GPIOB->BSRR = (1<<15)|(1<<12)|(1<<7)|(1<<6)|(1<<5);
      GPIOC->BSRR = (1<<7);
      break;
    case 1:                     /* switch on            PB7,PB6,              */
      GPIOB->BSRR = (1<<7)|(1<<6);
      break;
    case 2:                     /* switch on  PB15,PB12,    PB6,PB5,PC8       */
      GPIOB->BSRR = (1<<15)|(1<<12)|(1<<6)|(1<<5);
      GPIOC->BSRR = (1<<8);
      break;
    case 3:                     /* switch on       PB12,PB7,PB6,PB5,PC8       */
      GPIOB->BSRR = (1<<12)|(1<<7)|(1<<6)|(1<<5);
      GPIOC->BSRR = (1<<8);
      break;
    case 4:                     /* switch on            PB7,PB6,    PC8,PC7   */
      GPIOB->BSRR = (1<<7)|(1<<6);
      GPIOC->BSRR = (1<<8)|(1<<7);
      break;
    case 5:                     /* switch on       PB12,PB7,    PB5,PC8,PC7   */
      GPIOB->BSRR = (1<<12)|(1<<7)|(1<<5);
      GPIOC->BSRR = (1<<8)|(1<<7);
      break;
    case 6:                     /* switch on  PB15,PB12,PB7,    PB5,PC8,PC7   */
      GPIOB->BSRR = (1<<15)|(1<<12)|(1<<7)|(1<<5);
      GPIOC->BSRR = (1<<8)|(1<<7);
      break;
    case 7:                     /* switch on            PB7,PB6,PB5           */
      GPIOB->BSRR = (1<<7)|(1<<6)|(1<<5);
      break;
    case 8:                     /* switch on  PB15,PB12,PB7,PB6,PB5,PC8,PC7   */
      GPIOB->BSRR = (1<<15)|(1<<12)|(1<<7)|(1<<6)|(1<<5);
      GPIOC->BSRR = (1<<8)|(1<<7);
      break;
    case 9:                     /* switch on       PB12,PB7,PB6,PB5,PC8,PC7   */
      GPIOB->BSRR = (1<<12)|(1<<7)|(1<<6)|(1<<5);
      GPIOC->BSRR = (1<<8)|(1<<7);
      break;
    default:                    /* switch on  PB15,PB12,        PB5,PC8,PC7   */
      GPIOB->BSRR = (1<<15)|(1<<12)|(1<<5);
      GPIOC->BSRR = (1<<8)|(1<<7);
      break;
  }

}


/*----------------------------------------------------------------------------
  RTC Interrupt Handler
 *----------------------------------------------------------------------------*/
void RTC_IRQHandler(void) {

  if (RTC->CRL & RTC_CRL_SECF) {                  /* check second flag        */
    RTC->CRL &= ~RTC_CRL_SECF;                    /* clear second flag        */

    displayLeft  ((secCnt/10));
    displayRight ((secCnt%10));
    GPIOD->ODR ^= (1<<15);                        /* toggle right dot         */
	secCnt = (secCnt >= 59) ? 0 : secCnt+1;    
  }

}


/*----------------------------------------------------------------------------
  initialize GPIO Pins
 *----------------------------------------------------------------------------*/
void GPIO_Init (void) {

  /* configure GPIOs used for Segment displays */
  RCC->APB2ENR |= ((1UL << 3) |                   /* Enable GPIOB clock       */
                   (1UL << 4) |                   /* Enable GPIOC clock       */
                   (1UL << 5) |                   /* Enable GPIOD clock       */
                   (1UL << 6)  );                 /* Enable GPIOE clock       */

  GPIOB->ODR   &= ~0x000090A0;                    /* switch off LEDs          */
  GPIOB->CRL   &= ~0xFFF00000;                    /* PB.5..7 clear            */
  GPIOB->CRL   |=  0x33300000;                    /* PB.5..7 is output        */
  GPIOB->CRH   &= ~0xF00F0000;                    /* PB.12,15 clear           */
  GPIOB->CRH   |=  0x30030000;                    /* PB.12,15 is output       */

  GPIOC->ODR   &= ~0x00000D80;                    /* switch off LEDs          */
  GPIOC->CRL   &= ~0xF0000000;                    /* PC.7 clear               */
  GPIOC->CRL   |=  0x30000000;                    /* PC.7 is output           */
  GPIOC->CRH   &= ~0x0000FF0F;                    /* PC.8,10,11 clear         */
  GPIOC->CRH   |=  0x00003303;                    /* PC.8,10,11 is output     */

  GPIOD->ODR   &= ~0x00009984;                    /* switch off LEDs          */
  GPIOD->CRL   &= ~0xF0000F00;                    /* PD.2,7 clear             */
  GPIOD->CRL   |=  0x30000300;                    /* PD.2,7 is output         */
  GPIOD->CRH   &= ~0xF00FF00F;                    /* PD.8,11,12,15 clear      */
  GPIOD->CRH   |=  0x30033003;                    /* PD.8,11,12,15 is output  */

  GPIOE->ODR   &= ~0x00004000;                    /* switch off LEDs          */
  GPIOE->CRH   &= ~0x0F000000;                    /* PE.14 clear              */
  GPIOE->CRH   |=  0x03000000;                    /* PE.14 is output          */

}


/*----------------------------------------------------------------------------
  initialize RTC
 *----------------------------------------------------------------------------*/
void RTC_Init (void) {
  uint32_t cnt = ((12 * 3600UL) +                 /* counter hour value       */
                  ( 0 *   60UL) +                 /* counter minute value     */
                  ( 0 *    1UL)  );               /* counter second value     */
                                                                              
  RCC->APB1ENR |= RCC_APB1ENR_PWREN;              /* enable clock for Power IF*/
  PWR->CR      |= PWR_CR_DBP;                     /* enable access to RTC,BDC */

  RCC->CSR |= RCC_CSR_LSION;                      /* enable LSI               */
  while ((RCC->CSR & RCC_CSR_LSIRDY) == 0);       /* Wait for LSIRDY = 1      */

  RCC->BDCR |= (RCC_BDCR_RTCSEL_LSI |             /* LSI as RTC clock source  */
                RCC_BDCR_RTCEN       );           /* RTC clock enable         */

  RTC->CRL  |=  RTC_CRL_CNF;                      /* set configuration mode   */

  RTC->PRLH  = ((32768 - 1) >> 16) & 0x00FF;
  RTC->PRLL  = ((32768 - 1)      ) & 0xFFFF;

  RTC->CNTH  = (cnt >> 16) & 0xFFFF;
  RTC->CNTL  = (cnt      ) & 0xFFFF;

  RTC->CRH   = RTC_CRH_SECIE;                     /* Second Interrupt Enable  */

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
