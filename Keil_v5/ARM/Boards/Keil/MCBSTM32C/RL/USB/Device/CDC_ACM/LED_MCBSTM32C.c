/*------------------------------------------------------------------------------
 * Name:    LED_MCBSTM32C.c
 * Purpose: LED functions for the Keil MCBSTM32C evaluation board
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

#include <stm32f10x_cl.h>
#include "LED.h"

/* LEDS are on port PE bits 8. to 15.                                         */

#define LED_NUM   8                     /* Number of LEDs                     */
#define LED_MASK  0xFF00                /* Mask for LEDs                      */


/*------------------------------------------------------------------------------
 * LED_Initialize:  Initialize LEDs
 *----------------------------------------------------------------------------*/

void LED_Initialize (void) {

  RCC->APB2ENR |=  1 <<  6;             /* Enable GPIOE clock                 */
  RCC->APB2ENR |=  1 <<  5;             /* Enable GPIOD clock                 */
  GPIOE->CRH    =  0x33333333;          /* Configure the GPIO for LEDs        */
  LED_Value (0);                        /* Turn all LEDs off                  */
}


/*------------------------------------------------------------------------------
 * LED_Uninitialize: Ininitialize LEDs; pins are set to reset state
 *----------------------------------------------------------------------------*/

void LED_Uninitialize (void) {

  LED_Value (0);                        /* Turn all LEDs off                  */
}


/*------------------------------------------------------------------------------
 * LED_On:          Turns on requested LED
 *----------------------------------------------------------------------------*/

void LED_On (uint32_t idx) {

  GPIOE->BSRR = ((1 << (idx+8)) & LED_MASK);
}


/*------------------------------------------------------------------------------
 * LED_Off:         Turns off requested LED
 *----------------------------------------------------------------------------*/

void LED_Off (uint32_t idx) {

  GPIOE->BSRR = ((1 << (idx+8)) & LED_MASK) << 16;
}


/*------------------------------------------------------------------------------
 * LED_Value:       Write value to LEDs
 *----------------------------------------------------------------------------*/

void LED_Value (uint32_t val) {

  GPIOE->BSRR = (( val << 8) & LED_MASK);
  GPIOE->BSRR = ((~val << 8) & LED_MASK) << 16;
}


/*------------------------------------------------------------------------------
 * LED_GetNumber:   Get number of available LEDs
 *----------------------------------------------------------------------------*/

uint32_t LED_GetNumber (void) {

  return (LED_NUM);
}


/*------------------------------------------------------------------------------
 * End of file
 *----------------------------------------------------------------------------*/
