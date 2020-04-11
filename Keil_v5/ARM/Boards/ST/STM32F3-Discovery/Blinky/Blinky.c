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
 * Copyright (c) 2014 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <stdio.h>
#include <stm32f3xx.h>
#include "LED.h"

/* Select the PLL clock source */
//#define PLL_SOURCE_HSI        // HSI (~8MHz) used to clock the PLL, and the PLL is used as system clock source
//#define PLL_SOURCE_HSE        // HSE (8MHz) used to clock the PLL, and the PLL is used as system clock source
#define PLL_SOURCE_HSE_BYPASS   // HSE bypassed with an external clock (8MHz, coming from ST-Link) used to clock
                                // the PLL, and the PLL is used as system clock source


volatile uint32_t msTicks;                      /* counts 1ms timeTicks       */
/*----------------------------------------------------------------------------
  SysTick_Handler
 *----------------------------------------------------------------------------*/
void SysTick_Handler(void) {
  msTicks++;
}

/*----------------------------------------------------------------------------
  delays number of tick Systicks (happens every 1 ms)
 *----------------------------------------------------------------------------*/
void Delay (uint32_t dlyTicks) {
  uint32_t curTicks;

  curTicks = msTicks;
  while ((msTicks - curTicks) < dlyTicks);
}


/*----------------------------------------------------------------------------
  Function that initializes Button pins
 *----------------------------------------------------------------------------*/
void BTN_Init(void) {

  RCC->AHBENR   |= ((1UL << 17) );              /* Enable GPIOA clock         */

  GPIOA->MODER    &= ~((3UL << 2*0)  );         /* PA.0 is input              */
  GPIOA->OSPEEDR  |=  ((3UL << 2*0)  );         /* PA.0 is High Speed         */
  GPIOA->PUPDR    &= ~((3UL << 2*0)  );         /* PA.0 is no Pull up         */
}

/*----------------------------------------------------------------------------
  Function that read Button pins
 *----------------------------------------------------------------------------*/
uint32_t BTN_Get(void) {

 return (GPIOA->IDR & (1UL << 0));
}


/*----------------------------------------------------------------------------
  Configures the System clock frequency, AHB/APBx prescalers and Flash settings.
  This function should be called only once the RCC clock configuration
  is reset to the default reset state (done in SystemInit() function).
 *----------------------------------------------------------------------------*/
static void SetSysClock(void)
{
  __IO uint32_t StartUpCounter = 0, HSEStatus = 0;

  /* SYSCLK, HCLK, PCLK2 and PCLK1 configuration ---------------------------*/
#if defined (PLL_SOURCE_HSI)
  /* At this stage the HSI is already enabled */

  /* Enable Prefetch Buffer and set Flash Latency */
  FLASH->ACR = FLASH_ACR_PRFTBE | FLASH_ACR_LATENCY_1;

  /* HCLK = SYSCLK /1  */
  RCC->CFGR |= (uint32_t)RCC_CFGR_HPRE_DIV1;

  /* PCLK = HCLK */
  RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE1_DIV2 | (uint32_t)RCC_CFGR_PPRE2_DIV1;

  /* PLL configuration */
  RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLXTPRE | RCC_CFGR_PLLMUL));
  RCC->CFGR |= (uint32_t)(RCC_CFGR_PLLSRC_HSI_DIV2 | RCC_CFGR_PLLMUL16);

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
#else
 #if defined (PLL_SOURCE_HSE)
  /* Enable HSE */
  RCC->CR |= ((uint32_t)RCC_CR_HSEON);
 #elif defined (PLL_SOURCE_HSE_BYPASS)
  /* HSE oscillator bypassed with external clock */
  RCC->CR |= (uint32_t)(RCC_CR_HSEON | RCC_CR_HSEBYP);
 #endif /* PLL_SOURCE_HSE */

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
#endif /* PLL_SOURCE_HSI */
}


/*----------------------------------------------------------------------------
  MAIN function
 *----------------------------------------------------------------------------*/
int main (void) {
  int32_t num    =  0;
  int32_t ledOn  =  1;
 uint32_t ledVal =  0;
 uint32_t btns   =  0;

  SetSysClock();

  SystemCoreClockUpdate();                      /* Get Core Clock Frequency   */
  if (SysTick_Config(SystemCoreClock / 1000)) { /* SysTick 1 msec interrupts  */
    while (1);                                  /* Capture error              */
  }

  LED_Init();
  BTN_Init();

  while(1) {                                    /* Loop forever               */
    btns = BTN_Get();                           /* Read button states         */

    if (btns != (1UL << 0)) {

      if (ledOn == 1)
        ledVal |=  (1UL << num);
      else
        ledVal &= ~(1UL << num);

      if (++num == LED_NUM) {
        num    = 0;
        ledOn ^= 1;
      }

      LED_Out(ledVal);
      Delay(100);                               /* Delay 100ms                */
    }
    else {
      LED_Out (0xFF);
      Delay(10);                                /* Delay 10ms                 */
    }

  }

}

