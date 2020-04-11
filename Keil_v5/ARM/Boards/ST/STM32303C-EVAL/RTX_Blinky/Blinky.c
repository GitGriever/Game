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
#include "GLCD.h"
#include "LED.h"

#define __FI        1                   /* Font index 16x24                  */

OS_TID t_phaseA;                        /* assigned task id of task: phase_a */
OS_TID t_phaseB;                        /* assigned task id of task: phase_b */
OS_TID t_phaseC;                        /* assigned task id of task: phase_c */
OS_TID t_phaseD;                        /* assigned task id of task: phase_d */
OS_TID t_clock;                         /* assigned task id of task: clock   */
OS_TID t_lcd;                           /* assigned task id of task: lcd     */

OS_MUT mut_GLCD;                        /* Mutex to controll GLCD access     */

#define LED_A      0
#define LED_B      1
#define LED_C      2
#define LED_D      3
#define LED_CLK    7


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
  switch LED on
 *---------------------------------------------------------------------------*/
void LED_on  (unsigned char led) {
  if (led != LED_CLK) LED_On (led);

  os_mut_wait(&mut_GLCD, 0xffff);
  GLCD_SetBackColor(White);
  GLCD_SetTextColor(Green);
  GLCD_DisplayChar(4, 5+led, __FI, 0x80+1); /* Circle Full                   */
  os_mut_release(&mut_GLCD);
}

/*----------------------------------------------------------------------------
  switch LED off
 *---------------------------------------------------------------------------*/
void LED_off (unsigned char led) {
  LED_Off(led);
  os_mut_wait(&mut_GLCD, 0xffff);
  GLCD_SetBackColor(White);
  GLCD_SetTextColor(Green);
  GLCD_DisplayChar(4, 5+led, __FI, 0x80+0);  /* Circle Empty                 */
  os_mut_release(&mut_GLCD);
}


/*----------------------------------------------------------------------------
  Function 'signal_func' called from multiple tasks
 *---------------------------------------------------------------------------*/
void signal_func (OS_TID task)  {
  os_evt_set (0x0100, t_clock);          /* send event signal to clock task  */
  os_dly_wait (50);                      /* delay 50 clock ticks             */
  os_evt_set (0x0100, t_clock);          /* send event signal to clock task  */
  os_dly_wait (50);                      /* delay 50 clock ticks             */
  os_evt_set (0x0001, task);             /* send event to task 'task'        */
  os_dly_wait (50);                      /* delay 50 clock ticks             */
}

/*----------------------------------------------------------------------------
  Task 1 'phaseA': Phase A output
 *---------------------------------------------------------------------------*/
__task void phaseA (void) {
  for (;;) {
    os_evt_wait_and (0x0001, 0xffff);    /* wait for an event flag 0x0001    */
    LED_on (LED_A);
    signal_func (t_phaseB);              /* call common signal function      */
    LED_off(LED_A);
  }
}

/*----------------------------------------------------------------------------
  Task 2 'phaseB': Phase B output
 *---------------------------------------------------------------------------*/
__task void phaseB (void) {
  for (;;) {
    os_evt_wait_and (0x0001, 0xffff);    /* wait for an event flag 0x0001    */
    LED_on (LED_B);
    signal_func (t_phaseC);              /* call common signal function      */
    LED_off(LED_B);
  }
}

/*----------------------------------------------------------------------------
  Task 3 'phaseC': Phase C output
 *---------------------------------------------------------------------------*/
__task void phaseC (void) {
  for (;;) {
    os_evt_wait_and (0x0001, 0xffff);    /* wait for an event flag 0x0001    */
    LED_on (LED_C);
    signal_func (t_phaseD);              /* call common signal function      */
    LED_off(LED_C);
  }
}

/*----------------------------------------------------------------------------
  Task 4 'phaseD': Phase D output
 *---------------------------------------------------------------------------*/
__task void phaseD (void) {
  for (;;) {
    os_evt_wait_and (0x0001, 0xffff);    /* wait for an event flag 0x0001    */
    LED_on (LED_D);
    signal_func (t_phaseA);              /* call common signal function      */
    LED_off(LED_D);
  }
}

/*----------------------------------------------------------------------------
  Task 5 'clock': Signal Clock
 *---------------------------------------------------------------------------*/
__task void clock (void) {
  for (;;) {
    os_evt_wait_and (0x0100, 0xffff);    /* wait for an event flag 0x0100    */
    LED_on (LED_CLK);
    os_dly_wait (8);                     /* delay 8 clock ticks              */
    LED_off(LED_CLK);
  }
}

/*----------------------------------------------------------------------------
  Task 6 'lcd': LCD Control task
 *---------------------------------------------------------------------------*/
__task void lcd (void) {

  for (;;) {
    os_mut_wait(&mut_GLCD, 0xffff);
    GLCD_SetBackColor(Blue);
    GLCD_SetTextColor(White);
    GLCD_DisplayString(0, 0, __FI, "STM32303C-EVAL Demo ");
    GLCD_DisplayString(1, 0, __FI, "    RTX Blinky      ");
    GLCD_DisplayString(2, 0, __FI, "   www.keil.com     ");
    os_mut_release(&mut_GLCD);
    os_dly_wait (400);

    os_mut_wait(&mut_GLCD, 0xffff);
    GLCD_SetBackColor(Blue);
    GLCD_SetTextColor(Red);
    GLCD_DisplayString(0, 0, __FI, "STM32303C-EVAL Demo ");
    GLCD_DisplayString(1, 0, __FI, "    RTX Blinky      ");
    GLCD_DisplayString(2, 0, __FI, "   www.keil.com     ");
    os_mut_release(&mut_GLCD);
    os_dly_wait (400);
  }
}

/*----------------------------------------------------------------------------
  Task 7 'init': Initialize
 *---------------------------------------------------------------------------*/
__task void init (void) {

  os_mut_init(&mut_GLCD);

  t_phaseA = os_tsk_create (phaseA, 0);  /* start task phaseA                */
  t_phaseB = os_tsk_create (phaseB, 0);  /* start task phaseB                */
  t_phaseC = os_tsk_create (phaseC, 0);  /* start task phaseC                */
  t_phaseD = os_tsk_create (phaseD, 0);  /* start task phaseD                */
  t_clock  = os_tsk_create (clock, 0);   /* start task clock                 */
  t_lcd    = os_tsk_create (lcd, 0);     /* start task lcd                   */
  os_evt_set (0x0001, t_phaseA);         /* send signal event to task phaseA */
  os_tsk_delete_self ();
}

/*----------------------------------------------------------------------------
  Main: Initialize and start RTX Kernel
 *---------------------------------------------------------------------------*/
int main (void) {

  SetSysClock();

  LED_Init ();                              /* Initialize the LEDs           */
  GLCD_Init();                              /* Initialize the GLCD           */

  GLCD_Clear(White);                        /* Clear the GLCD                */

  os_sys_init(init);                        /* Initialize RTX and start init */
}
