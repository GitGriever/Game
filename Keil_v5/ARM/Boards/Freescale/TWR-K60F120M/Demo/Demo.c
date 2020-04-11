/*------------------------------------------------------------------------------
 * Name:    Demo.c
 * Purpose: Demo example demonstrating basic peripherals on the Freescale 
 *          Kinetis TWR-K60F120M evaluation board
 *------------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2012 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <MK60F12.H>
#include <stdio.h>
#include "ADC.h"
#include "KBD.h"
#include "LED.h"
#include "SER.h"

uint8_t  ADC_flag  =    0;              /* Flag to start AD conversion        */
uint8_t  KBD_flag  =    0;              /* Flag to sample keyboard            */
uint8_t  SER_flag  =    0;              /* Flag to output to serial           */
uint8_t  LED_blink =    0;              /* Periodical blinking LED state      */

uint32_t ADC_val   =    0;              /* Last AD converted value            */
uint32_t ADC_bits  =   10;              /* Resolution of AD converter         */
uint32_t KBD_val   =    0;              /* Sampled keyboard state             */
uint32_t LED_peri  = 1024;              /* Time counter fro LED blinking      */
uint32_t msTicks   =    0;              /* Millisecond time counter           */

/*------------------------------------------------------------------------------
 * SysTick_Handler (every 1 ms)
 *----------------------------------------------------------------------------*/

void SysTick_Handler(void) {
  msTicks++;
  if (msTicks >= 1000) {                /* Time base of 1 second              */
    msTicks  = 0;
  }

  if (!(msTicks & 0x3F))                /* Activate flag every 64 ms          */
    KBD_flag = 1;

  if (!(msTicks & 0x7F))                /* Activate flag every 128 ms         */
    ADC_flag = 1;

  if (!msTicks) {                       /* Activate flag every 1 second       */
    SER_flag = 1;
    KBD_flag = 1;
    ADC_flag = 1;
  }

                                        /* Handle LED blinking with speed 
                                           determined by AD value             */
  LED_peri++;
  if (LED_peri >= (50 + (256 - (ADC_val >> (ADC_bits - 8))))) {
    LED_peri   = 0;
    LED_blink ^= 1;
    if (LED_blink) 
      LED_On  (0);
    else 
      LED_Off (0);
  }
}


/*------------------------------------------------------------------------------
  Main function
 *----------------------------------------------------------------------------*/

int main (void) {
  
  SystemCoreClockUpdate();              /* Update Core Clock Frequency        */
  SysTick_Config(SystemCoreClock/1000); /* Generate interrupt each 1 ms       */

  ADC_Init();
  KBD_Init();
  LED_Init();
  SER_Init();

  ADC_bits = ADC_Bits();

  printf ("\n\nKEIL Demo example for Freescale Kinetis TWR-K60F120M board!\n");

  while(1) {
    if (ADC_flag) {
      ADC_flag = 0;
      ADC_StartCnv();
      ADC_val  = ADC_GetVal();
    }

    if (SER_flag) {
      SER_flag = 0;
      printf ("AD value = 0x%04X\n\r", ADC_val);
    }

    if (KBD_flag) {
      KBD_flag = 0;
      KBD_val  = KBD_GetKeys();
      if (KBD_val & 1) 
        LED_On  (1);
      else 
        LED_Off (1);
      if (KBD_val & 2) 
        LED_On  (2);
      else 
        LED_Off (2);
    }
  }
}
