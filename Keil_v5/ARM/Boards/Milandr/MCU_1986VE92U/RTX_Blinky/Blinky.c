/*----------------------------------------------------------------------------
 *      RL-ARM - RTX
 *----------------------------------------------------------------------------
 *      Name:    BLINKY.C
 *      Purpose: RTX example program
 *----------------------------------------------------------------------------
 *      This code is part of the RealView Run-Time Library.
 *      Copyright (c) 2004-2011 KEIL - An ARM Company. All rights reserved.
 *---------------------------------------------------------------------------*/

#include <RTL.h>
#include "MDR32F9x.h"                   /* MDR32F9x definitions              */
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

  /* use HSE 8MHz as core clock */
  MDR_RST_CLK->HS_CONTROL = 0x1;                     /* Enable HSE osc.      */
  while (MDR_RST_CLK->CLOCK_STATUS == 0x00) __NOP(); /* wait for HSE startup */
  MDR_RST_CLK->CPU_CLOCK = 0x102;                    /* switch to HSE (8 MHz)*/

  LED_Init();                            /* Initialize LEDs                  */

  t_ledOn  = os_tsk_create (ledOn, 0);   /* start task 'ledOn'               */
  t_ledOff = os_tsk_create (ledOff, 0);  /* start task 'ledOff'              */
  os_tsk_delete_self ();
}

/*----------------------------------------------------------------------------
  Main: Initialize and start RTX Kernel
 *---------------------------------------------------------------------------*/
int main (void) {
  os_sys_init (init);                   /* Initialize RTX and start init     */
}
