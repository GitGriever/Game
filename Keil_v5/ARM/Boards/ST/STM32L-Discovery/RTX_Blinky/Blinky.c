/*----------------------------------------------------------------------------
 *      RL-ARM - RTX
 *----------------------------------------------------------------------------
 *      Name:    BLINKY.C
 *      Purpose: RTX example program
 *----------------------------------------------------------------------------
 *      This code is part of the RealView Run-Time Library.
 *      Copyright (c) 2011 KEIL - An ARM Company. All rights reserved.
 *---------------------------------------------------------------------------*/

#include <RTL.h>
#include "STM32L1xx.h"
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
  set HSI as SystemCoreClock (HSE is not populated on STM32L-Discovery board)
 *---------------------------------------------------------------------------*/
void SystemCoreClockSetHSI(void) {
  
  RCC->CR |= ((uint32_t)RCC_CR_HSION);                      /* Enable HSI                        */ 
  while ((RCC->CR & RCC_CR_HSIRDY) == 0);                   /* Wait for HSI Ready                */

  FLASH->ACR |= FLASH_ACR_ACC64;                            /* Enable 64-bit access              */
  FLASH->ACR |= FLASH_ACR_PRFTEN;                           /* Enable Prefetch Buffer            */
  FLASH->ACR |= FLASH_ACR_LATENCY;                          /* Flash 1 wait state                */

  RCC->APB1ENR |= RCC_APB1ENR_PWREN;                        /* Enable the PWR APB1 Clock         */
  PWR->CR = PWR_CR_VOS_0;                                   /* Select the Voltage Range 1 (1.8V) */
  while((PWR->CSR & PWR_CSR_VOSF) != 0);                    /* Wait for Voltage Regulator Ready  */

  RCC->CFGR |= (uint32_t)RCC_CFGR_HPRE_DIV1;                /* HCLK = SYSCLK                     */
  RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE2_DIV1;               /* PCLK2 = HCLK                      */
  RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE1_DIV1;               /* PCLK1 = HCLK                      */
    
  RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_SW) | RCC_CFGR_SW_HSI; /* HSI is system clock               */
  while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSI);   /* Wait for HSI used as system clock */
}


/*----------------------------------------------------------------------------
  Main: Initialize and start RTX Kernel
 *---------------------------------------------------------------------------*/
int main (void) {

  SystemCoreClockSetHSI();              /* configure HSI as System Clock     */         

  os_sys_init (init);                   /* Initialize RTX and start init     */
}
