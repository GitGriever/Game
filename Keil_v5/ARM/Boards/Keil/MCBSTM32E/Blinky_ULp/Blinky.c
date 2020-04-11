/*----------------------------------------------------------------------------
 * Name:    Blinky.c
 * Purpose: LED Flasher
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

#include <stdio.h>
#include "stm32f10x.h"                  /* STM32F10x.h definitions            */
#include "Serial.h"
#include "LED.h"
#include "ADC.h"

char text[10];

/* Import external variables from IRQ.c file                                  */
extern uint8_t  clock_1s;


/* variable to trace in LogicAnalyzer (should not read to often)              */
volatile uint16_t AD_dbg;          


/*----------------------------------------------------------------------------
  Function that initializes GLCD
 *----------------------------------------------------------------------------*/
void GLCD_Init(void) {

  RCC->APB2ENR |= (1<<2);               /* enable periperal clock for GPIOA   */
  /* PA.08(LCD Backlight */
  GPIOA->BRR |=  0x00000100;            /* Backlight off                      */
  GPIOA->CRH &= ~0x0000000F;            /* clear Bits                         */
  GPIOA->CRH |=  0x00000003;            /* output Push-pull 50MHz             */
}

/*----------------------------------------------------------------------------
  Main Program
 *----------------------------------------------------------------------------*/
int main (void) {
  uint32_t ad_avg = 0;
  uint16_t ad_val = 0, ad_val_ = 0xFFFF;

  LED_Init();                                /* LED Initialization            */
  SER_Init();                                /* UART Initialization           */
  ADC_Init();                                /* ADC Initialization            */
  GLCD_Init();                               /* GLCD Initialization           */

  SysTick_Config(SystemCoreClock / 100);     /* Generate interrupt each 10 ms */

  while (1) {                                /* Loop forever                  */

    /* AD converter input                                                     */
    if (AD_done) {                           /* If conversion has finished    */
      AD_done = 0;

      ad_avg += AD_last << 8;                /* Add AD value to averaging     */
      ad_avg ++;
      if ((ad_avg & 0xFF) == 0x10) {         /* average over 16 values        */
        ad_val = (ad_avg >> 8) >> 4;         /* average devided by 16         */
        ad_avg = 0;
      }
    }

    if (ad_val ^ ad_val_) {                  /* AD value changed              */
      ad_val_ = ad_val;

      AD_dbg = ad_val;
      sprintf(text, "0x%04X", ad_val);       /* format text for print out     */
    }

    /* Print message with AD value every second                               */
    if (clock_1s) {
      clock_1s = 0;

      printf("AD value: %s\r\n", text);
    }
  }
}
