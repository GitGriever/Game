/*----------------------------------------------------------------------------
 * Name:    Blinky.c
 * Purpose: LED Flasher
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
 * Copyright (c) 2008-2011 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include "stdio.h"
#include "stm32l1xx.h"                  /* STM32L1xx.h definitions            */
#include "LCD.h"
#include "GLCD.h"
#include "Serial.h"
#include "LED.h"
#include "ADC.h"

#define __FI        1                       /* Font index 16x24               */
#if (__FI == 1)                         /* Font index  6x8                    */
  #define __FONT_WIDTH  16
  #define __FONT_HEIGHT 24
#else                                   /* Font index 16x24                   */
  #define __FONT_WIDTH   6
  #define __FONT_HEIGHT  8
#endif
                                                                         
char text[10];

/* Import external variables from IRQ.c file                                  */
extern uint8_t  clock_1s;
extern uint8_t  clock_50ms;

char Demo_text[] = "* KEIL *";
char LCD_text[]  = "        ";


/*----------------------------------------------------------------------------
  Function that initializes RCC clock (used for LCD)
 *----------------------------------------------------------------------------*/
void RCC_Init(void) {

  RCC->APB1ENR  |= (1UL << 28) ;        /* enable PWR clock                   */

  PWR->CR  |=  (1UL <<  8);             /* enable access to RCC_CSR           */

  RCC->CSR |=  (1UL << 23);             /* start reset RTC                    */
  RCC->CSR &= ~(1UL << 23);             /* end   reset RTC                    */

  RCC->CSR |= (1UL << 8);               /* enable LSE                         */
  while (!(RCC->CSR & (1UL << 9)));     /* wait until LSE ready               */ 

  RCC->CSR |=  (1UL << 16);             /* LSE clock used as RTC clock        */
}


/*----------------------------------------------------------------------------
  Main Program
 *----------------------------------------------------------------------------*/
int main (void) {
  uint16_t LCD_cnt, LCD_pos = 0;
  uint32_t ad_avg = 0;
  uint16_t ad_val = 0, ad_val_ = 0xFFFF;

  RCC_Init();
  LED_Init();                                /* LED Initialization            */
  SER_Init();                                /* UART Initialization           */
  ADC_Init();                                /* ADC Initialization            */
  LCD_Init();                                /* LCD Initialization            */

#ifdef __USE_LCD
  GLCD_Init();                               /* Initialize graphical LCD      */

  GLCD_Clear(White);                         /* Clear graphical LCD display   */
  GLCD_SetBackColor(Blue);
  GLCD_SetTextColor(White);
  GLCD_DisplayString(0, 0, __FI, "STM32L152-EVAL Demo ");
  GLCD_DisplayString(1, 0, __FI, "       Blinky       ");
  GLCD_DisplayString(2, 0, __FI, "    www.keil.com    ");
  GLCD_SetBackColor(White);
  GLCD_SetTextColor(Blue);
  GLCD_DisplayString(5, 0, __FI, "AD value:            ");
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

    /* update LCD every 50 milli seconds */
    if (clock_50ms) {
      clock_50ms = 0;

      for (LCD_cnt = 0; LCD_cnt < 8; LCD_cnt++)
        LCD_text[((LCD_pos + LCD_cnt) & 0x07)] =  Demo_text[LCD_cnt];
      LCD_DisplayString(0, (uint8_t *)LCD_text);

      LCD_pos++;
      if (LCD_pos > 7) LCD_pos = 0;
    }

    /* Print message with AD value every second                               */
    if (clock_1s) {
      clock_1s = 0;

      printf("AD value: %s\r\n", text);
    }
  }
}
