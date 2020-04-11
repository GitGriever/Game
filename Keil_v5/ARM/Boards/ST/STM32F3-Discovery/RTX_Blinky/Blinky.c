/*----------------------------------------------------------------------------
 *      RL-ARM - RTX
 *----------------------------------------------------------------------------
 *      Name:    BLINKY.C
 *      Purpose: RTX example program
 *----------------------------------------------------------------------------
 *      This code is part of the RealView Run-Time Library.
 *      Copyright (c) 2014 KEIL - An ARM Company. All rights reserved.
 *---------------------------------------------------------------------------*/

#include <RTL.h>
#include <stm32f3xx.h>
#include "LED.h"

/* Select the PLL clock source */
//#define PLL_SOURCE_HSI        // HSI (~8MHz) used to clock the PLL, and the PLL is used as system clock source
//#define PLL_SOURCE_HSE        // HSE (8MHz) used to clock the PLL, and the PLL is used as system clock source
#define PLL_SOURCE_HSE_BYPASS   // HSE bypassed with an external clock (8MHz, coming from ST-Link) used to clock
                                // the PLL, and the PLL is used as system clock source


OS_TID t_ledOn;                         /* assigned task id of task: ledOn   */
OS_TID t_ledOff;                        /* assigned task id of task: ledOff  */


/*----------------------------------------------------------------------------
  Task 1 'ledOn': switches the LED on
 *---------------------------------------------------------------------------*/
__task void ledOn (void) {
  for (;;) {
    LED_On(0);                          /* Turn LED On                       */
    os_evt_set (0x0001, t_ledOff);      /* send event to task 'ledoff'       */
    os_dly_wait (50);                   /* delay 50 clock ticks              */
  }
}

/*----------------------------------------------------------------------------
  Task 2 'ledOff': switches the LED off
 *---------------------------------------------------------------------------*/
__task void ledOff (void) {
  for (;;) {
    os_evt_wait_and (0x0001, 0xffff);   /* wait for an event flag 0x0001     */
    os_dly_wait (8);                    /* delay 8 clock ticks               */
    LED_Off(0);                         /* Turn LED Off                      */
  }
}

/*----------------------------------------------------------------------------
  Task 3 'init': Initialize
 *---------------------------------------------------------------------------*/
__task void init (void) {

  LED_Init();                            /* Initialize LEDs                  */

  t_ledOn  = os_tsk_create (ledOn, 0);   /* start task 'ledOn'               */
  t_ledOff = os_tsk_create (ledOff, 0);  /* start task 'ledOff'              */
  os_tsk_delete_self ();
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
  Main: Initialize and start RTX Kernel
 *---------------------------------------------------------------------------*/
int main (void) {

  SetSysClock();

  os_sys_init (init);                   /* Initialize RTX and start init     */
}
