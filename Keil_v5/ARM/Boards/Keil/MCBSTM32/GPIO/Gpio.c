/*----------------------------------------------------------------------------
 * Name:    Gpio.c
 * Purpose: GPIO usage for STM32
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

#define S3             0x2000                     /* PC13: S3                 */
#define S2             0x0001                     /* PA0 : S2                 */
#define UNBOUNCE_CNT        5                     /* unbounce the keys        */

int ledPosCur = 3;                                /* current led pos. 0..7    */
int ledPosOld = 3;                                /* old     led pos. 0..7    */


/*----------------------------------------------------------------------------
  initialize GPIO Pins
 *----------------------------------------------------------------------------*/
void GPIO_Init (void) {

  /* configure GPIOs used for LEDs */
  RCC->APB2ENR |=  RCC_APB2ENR_IOPBEN;            /* Enable GPIOB clock       */
  GPIOB->ODR   &= ~(0x0000FF00);                  /* switch off LEDs          */
  GPIOB->CRH    =  (0x33333333);                  /* PB8..15 GPIO             */

  /* configure GPIOs for PushButtons */
  RCC->APB2ENR |= (RCC_APB2ENR_IOPAEN |           /* Enable GPIOA clock       */
                   RCC_APB2ENR_IOPCEN  );         /* Enable GPIOC clock       */
  GPIOA->CRL   &= ~0x0000000F;                    /* PA.0 clear               */
  GPIOA->CRL   |=  0x00000004;                    /* PA.0 is Floating Input   */

  GPIOC->CRH   &= ~0x00F00000;                    /* PC.13 clear              */
  GPIOC->CRH   |=  0x00400000;                    /* PC.13 is Floating Input  */
}


/*----------------------------------------------------------------------------
  S2Pressed
  check if S2 is pressed (unbounced).
 *----------------------------------------------------------------------------*/
int S2Pressed (void) {
  static int S2KeyCount = 0, S2KeyPressed = 0;

  if (S2KeyPressed) {
    if (!((GPIOA->IDR & S2) == 0 )) {             /* Check Button S2          */
      if (S2KeyCount < UNBOUNCE_CNT) S2KeyCount++;
      else {
        S2KeyPressed = 0;
        S2KeyCount = 0;    
      }
    }
  }
  else {
    if (((GPIOA->IDR & S2) == 0 ))  {             /* Check Button S2          */
      if (S2KeyCount < UNBOUNCE_CNT) S2KeyCount++;
      else {
        S2KeyPressed = 1;
        S2KeyCount = 0;
        return (1);
      }
    }
  }
  return (0);
}


/*----------------------------------------------------------------------------
  S3Pressed
  check if S3 is pressed (unbounced).
 *----------------------------------------------------------------------------*/
int S3Pressed (void) {
  static int S3KeyCount = 0, S3KeyPressed = 0;

  if (S3KeyPressed) {
    if (!((GPIOC->IDR & S3) == 0 )) {             /* Check Button S3          */
      if (S3KeyCount < UNBOUNCE_CNT) S3KeyCount++;
      else {
        S3KeyPressed = 0;
        S3KeyCount = 0;    
      }
    }
  }
  else {
    if (((GPIOC->IDR & S3) == 0 ))  {             /* Check Button S3          */
      if (S3KeyCount < UNBOUNCE_CNT) S3KeyCount++;
      else {
        S3KeyPressed = 1;
        S3KeyCount = 0;
        return (1);
      }
    }
  }
  return (0);
}


/*----------------------------------------------------------------------------
  MAIN function
 *----------------------------------------------------------------------------*/
int main (void) {

  GPIO_Init ();
    
  GPIOB->ODR |=  (1 << (ledPosCur+8));            /* initial LED position     */

  while (1) {

    if (S2Pressed()) {                            /*  S2 pressed              */
      if (ledPosCur > 0) 
        ledPosCur -= 1;                           /* led pos. moves to right  */
    }

    if (S3Pressed()) {                            /*  S3 pressed              */
      if (ledPosCur < 7) 
        ledPosCur += 1;                           /* led pos. moves to left   */
    }

    if (ledPosCur != ledPosOld) {                 /* LED position has changed */
      GPIOB->ODR &= ~(1 << (ledPosOld+8));        /* switch off old LED pos.  */
      GPIOB->ODR |=  (1 << (ledPosCur+8));        /* switch on  new LED pos.  */
      ledPosOld = ledPosCur;
    }
  }
}
