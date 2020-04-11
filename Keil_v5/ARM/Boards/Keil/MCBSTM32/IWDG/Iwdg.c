/*----------------------------------------------------------------------------
 * Name:    Iwdg.c
 * Purpose: IWDG usage for STM32
 * Note(s): 
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2008-2011 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include "STM32F10x.h"


unsigned int ledPosIwdg  = 4;                     /* led pos. for IWDG reset  */
unsigned int ledPosEnd   = 7;                     /* led pos. loop end        */

/*----------------------------------------------------------------------------
  delay
  insert a delay time.
 *----------------------------------------------------------------------------*/
void delay(unsigned int nCount) {

  for(; nCount != 0; nCount--) __NOP();
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


#define IWDG_PERIOD             2000000UL
#define IWDG_PR                 4
#define IWGDCLK                (40000UL / (0x04 << IWDG_PR))
#define IWDG_RLR               (IWDG_PERIOD * IWGDCLK / 1000000UL -  1)

/*----------------------------------------------------------------------------
  initialize IWDG
 *----------------------------------------------------------------------------*/
void IWDG_Init (void) {

  IWDG->KR  = 0x5555;                             /* enable write to PR, RLR  */
  IWDG->PR  = IWDG_PR;                            /* Init prescaler           */
  IWDG->RLR = IWDG_RLR;                           /* Init RLR                 */
  IWDG->KR  = 0xAAAA;                             /* Reload the watchdog      */
  IWDG->KR  = 0xCCCC;                             /* Start the watchdog       */
}


/*----------------------------------------------------------------------------
  MAIN function
 *----------------------------------------------------------------------------*/
int main (void) {
  int i;

  GPIO_Init ();
  IWDG_Init ();

  if (RCC->CSR & (1UL << 29)) {                   /* IWDG Reset Flag set      */
    RCC->CSR |= (1UL << 24);                      /* Clear Reset Flags        */
    GPIOB->BSRR = 1UL << (ledPosIwdg+8);          /* LED on                   */
  }
  else {
    GPIOB->BRR  = 1UL << (ledPosIwdg+8);          /* LED off                  */
  }

  for (i = 0; i < 10; i++) {
    delay(1000000);                               /* delay < watchdog timeout */
    IWDG->KR  = 0xAAAA;                           /* reload the watchdog      */
  }
  GPIOB->BSRR = 1UL << (ledPosEnd+8);             /* LED on                   */

  while (1) {
    ;
  }
}
