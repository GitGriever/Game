/*----------------------------------------------------------------------------
 * Name:    Blinky.c
 * Purpose: LED Flasher for STM320518-EVAL
 * Note(s): possible defines set in "options for target - C/C++ - Define"
 *            __USE_LCD   - enable Output on GLCD
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

#include <stdio.h>
#include "stm32f0xx.h"                  /* STM32F0xx Definitions              */
#include "GLCD.h"
#include "Serial.h"
#include "LED.h"
#include "ADC.h"

#define __FI        1                   /* Font index 16x24                   */
#if (__FI == 1)                         /* Font index  6x8                    */
  #define __FONT_WIDTH  16
  #define __FONT_HEIGHT 24
#else                                   /* Font index 16x24                   */
  #define __FONT_WIDTH   6
  #define __FONT_HEIGHT  8
#endif

char text[10];

/* Import external variables from IRQ.c file                                  */
extern volatile uint8_t  clock_1s;


/*----------------------------------------------------------------------------
  Main Program
 *----------------------------------------------------------------------------*/
int main (void) {
  uint32_t ad_avg = 0;
  uint16_t ad_val = 0, ad_val_ = 0xFFFF;

  LED_Init();                                /* LED Initialization            */
  SER_Init();                                /* UART Initialization           */
  ADC_Init();                                /* ADC Initialization            */

#ifdef __USE_LCD
  GLCD_Init();                               /* Initialize graphical LCD      */

  GLCD_Clear(White);                         /* Clear graphical LCD display   */
  GLCD_SetBackColor(Blue);
  GLCD_SetTextColor(White);
  GLCD_DisplayString(0, 0, __FI, (unsigned char *)" STM320518-EVAL Demo");
  GLCD_DisplayString(1, 0, __FI, (unsigned char *)"       Blinky       ");
  GLCD_DisplayString(2, 0, __FI, (unsigned char *)"    www.keil.com    ");
  GLCD_SetBackColor(White);
  GLCD_SetTextColor(Blue);
  GLCD_DisplayString(5, 0, __FI, (unsigned char *)"AD value:           ");
#endif

  SysTick_Config(SystemCoreClock/100);       /* Generate interrupt each 10 ms */

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

      sprintf(text, "0x%04X", ad_val);       /* format text for print out     */
#ifdef __USE_LCD
      GLCD_SetTextColor(Red);
      GLCD_DisplayString(5,  9, __FI,  (unsigned char *)text);
      GLCD_Bargraph ( 9*__FONT_WIDTH, 6*__FONT_HEIGHT,
                     11*__FONT_WIDTH,   __FONT_HEIGHT, (ad_val >> 2)); /* max bargraph is 10 bit */
#endif
    }

    /* Print message with AD value every second                               */
    if (clock_1s) {
      clock_1s = 0;

      printf("AD value: %s\r\n", text);
    }
  }
}
