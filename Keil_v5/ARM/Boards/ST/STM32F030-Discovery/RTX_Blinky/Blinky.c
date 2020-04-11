/*----------------------------------------------------------------------------
 *      RL-ARM - RTX
 *----------------------------------------------------------------------------
 *      Name:    BLINKY.C
 *      Purpose: RTX example program
 *----------------------------------------------------------------------------
 *      This code is part of the RealView Run-Time Library.
 *      Copyright (c) 2013 KEIL - An ARM Company. All rights reserved.
 *---------------------------------------------------------------------------*/

#include <RTL.h>
#include <stm32f0xx.h>
#include "LED.h"

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

/*---------------------------------------------------------------------------
  set HSI as SystemCoreClock (HSE is not populated on STM32F030-Discovery board)
 *---------------------------------------------------------------------------*/
void SystemCoreClockSetHSI(void) {

  RCC->CR |= ((uint32_t)RCC_CR_HSION);                      /* Enable HSI                        */
  while ((RCC->CR & RCC_CR_HSIRDY) == 0);                   /* Wait for HSI Ready                */

  RCC->CFGR = RCC_CFGR_SW_HSI;                              /* HSI is system clock               */
  while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSI);   /* Wait for HSI used as system clock */

  FLASH->ACR  = FLASH_ACR_PRFTBE;                           /* Enable Prefetch Buffer            */
  FLASH->ACR |= FLASH_ACR_LATENCY;                          /* Flash 1 wait state                */

  RCC->CFGR |= RCC_CFGR_HPRE_DIV1;                          /* HCLK = SYSCLK                     */
  RCC->CFGR |= RCC_CFGR_PPRE_DIV1;                          /* PCLK = HCLK                       */

  RCC->CR &= ~RCC_CR_PLLON;                                 /* Disable PLL */

  /*  PLL configuration:  = HSI * 12 = 48 MHz */
  RCC->CFGR &= ~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLXTPRE | RCC_CFGR_PLLMULL);
  RCC->CFGR |=  (RCC_CFGR_PLLSRC_HSI_Div2 | RCC_CFGR_PLLMULL12);

  RCC->CR |= RCC_CR_PLLON;                                  /* Enable PLL                        */
  while((RCC->CR & RCC_CR_PLLRDY) == 0) __NOP();            /* Wait till PLL is ready            */

  RCC->CFGR &= ~RCC_CFGR_SW;                                /* Select PLL as system clock source */
  RCC->CFGR |=  RCC_CFGR_SW_PLL;

  while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);   /* Wait till PLL is system clock src */
}


/*----------------------------------------------------------------------------
  Main: Initialize and start RTX Kernel
 *---------------------------------------------------------------------------*/
int main (void) {

  SystemCoreClockSetHSI();              /* configure HSI as System Clock     */

  os_sys_init (init);                   /* Initialize RTX and start init     */
}
