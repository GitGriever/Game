/*----------------------------------------------------------------------------
 * Name:    Blinky.c
 * Purpose: LED Flasher for STM32373C-EVAL
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
 * Copyright (c) 2014 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <stdio.h>
#include <stm32f3xx.h>
#include "Serial.h"
#include "LED.h"
#include "SDADC.h"
#include "GLCD.h"

#define __FI        1                        /* Font index 16x24              */
#if (__FI == 1)                              /* Font index 16x24              */
  #define __FONT_WIDTH  16
  #define __FONT_HEIGHT 24
#else                                        /* Font index  6x8               */
  #define __FONT_WIDTH   6
  #define __FONT_HEIGHT  8
#endif

char text[10];


/* Import external variables from IRQ.c file                                  */
extern volatile unsigned char  clock_1s;

/*----------------------------------------------------------------------------
  Configures the System clock frequency, AHB/APBx prescalers and Flash settings.
  This function should be called only once the RCC clock configuration
  is reset to the default reset state (done in SystemInit() function).
 *----------------------------------------------------------------------------*/
static void SetSysClock(void)
{
  __IO uint32_t StartUpCounter = 0, HSEStatus = 0;

  /* SYSCLK, HCLK, PCLK2 and PCLK1 configuration ---------------------------*/
  /* Enable HSE */
  RCC->CR |= ((uint32_t)RCC_CR_HSEON);

  /* Wait till HSE is ready and if Time out is reached exit */
  do
  {
    HSEStatus = RCC->CR & RCC_CR_HSERDY;
    StartUpCounter++;
  } while((HSEStatus == 0) && (StartUpCounter != ((uint16_t)0x0500))); /* Time out for HSE start up */

  if ((RCC->CR & RCC_CR_HSERDY) != RESET)
  {
    HSEStatus = (uint32_t)0x01;
  }
  else
  {
    HSEStatus = (uint32_t)0x00;
  }

  if (HSEStatus == (uint32_t)0x01)
  {
    /* Enable Prefetch Buffer and set Flash Latency */
    FLASH->ACR = FLASH_ACR_PRFTBE | FLASH_ACR_LATENCY_1;

    /* HCLK = SYSCLK / 1 */
    RCC->CFGR |= (uint32_t)RCC_CFGR_HPRE_DIV1;

    /* PCLK2 = HCLK / 1 */
    RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE2_DIV1;

    /* PCLK1 = HCLK / 2 */
    RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE1_DIV2;


    /*  PLL configuration: PLLCLK = HSE * 9 = 72 MHz */
    RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLXTPRE | RCC_CFGR_PLLMUL));
    RCC->CFGR |= (uint32_t)(RCC_CFGR_PLLSRC_HSE_PREDIV | RCC_CFGR_PLLMUL9);

    /* Enable PLL */
    RCC->CR |= RCC_CR_PLLON;

    /* Wait till PLL is ready */
    while((RCC->CR & RCC_CR_PLLRDY) == 0)
    {
    }

    /* Select PLL as system clock source */
    RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
    RCC->CFGR |= (uint32_t)RCC_CFGR_SW_PLL;

    /* Wait till PLL is used as system clock source */
    while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != (uint32_t)RCC_CFGR_SWS_PLL)
    {
    }
  }
  else
  { /* If HSE fails to start-up, the application will have wrong clock
       configuration. User can add here some code to deal with this error */
	while(1);
  }
}


/*----------------------------------------------------------------------------
  MAIN function
 *----------------------------------------------------------------------------*/
int main (void) {
  uint32_t ad_avg = 0;
  uint16_t ad_val = 0, ad_val_ = 0xFFFF;

  SetSysClock();

  LED_Init();                                /* LED Initialization            */
  SER_Init();                                /* UART Initialization           */
  ADC_Init();                                /* ADC Initialization            */

#ifdef __USE_LCD
  GLCD_Init();                               /* GLCD Initialization           */

  GLCD_Clear(White);
  GLCD_SetBackColor(Blue);
  GLCD_SetTextColor(White);
  GLCD_DisplayString(0, 0, __FI, (unsigned char *)" STM32373C-EVAL Demo");
  GLCD_DisplayString(1, 0, __FI, (unsigned char *)"       Blinky       ");
  GLCD_DisplayString(2, 0, __FI, (unsigned char *)"    www.keil.com    ");
  GLCD_SetBackColor(White);
  GLCD_SetTextColor(Blue);
  GLCD_DisplayString(5, 0, __FI, (unsigned char *)"AD value:            ");
#endif

  SystemCoreClockUpdate();
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
                     11*__FONT_WIDTH,   __FONT_HEIGHT, (ad_val >> 6)); /* max bargraph is 10 bit */
#endif
    }

    /* Print message with AD value every second                               */
    if (clock_1s) {
      clock_1s = 0;

      printf("AD value: %s\r\n", text);
    }
  }
}
