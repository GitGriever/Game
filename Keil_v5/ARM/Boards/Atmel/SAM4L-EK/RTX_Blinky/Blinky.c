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
#include <SAM4L.h>
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


/*----------------------------------------------------------------------------
  Main: Initialize and start RTX Kernel
 *---------------------------------------------------------------------------*/
int main (void) {

  /* set Flash Waite State to maximum */
  HFLASHC->FLASHCALW_FCR = FLASHCALW_FCR_FWS;

  /* set system clock to 40MHz (80 MHz RC Oscillator / 2) */
  SCIF->SCIF_UNLOCK  = SCIF_UNLOCK_KEY(0xAAu) |
                       SCIF_UNLOCK_ADDR((uint32_t)&SCIF->SCIF_RC80MCR - (uint32_t)SCIF);
  SCIF->SCIF_RC80MCR = SCIF_RC80MCR_EN;
  while((SCIF->SCIF_RC80MCR & SCIF_RC80MCR_EN) != SCIF_RC80MCR_EN);

  PM->PM_UNLOCK = PM_UNLOCK_KEY(0xAAu) |   /* Disable write protect           */
                  PM_UNLOCK_ADDR((uint32_t)&PM->PM_CPUSEL - (uint32_t)PM);
  PM->PM_CPUSEL = PM_CPUSEL_CPUDIV;             /* 80 MHz / 2                 */
  while((PM->PM_SR & PM_SR_CKRDY) != PM_SR_CKRDY);

  PM->PM_UNLOCK = PM_UNLOCK_KEY(0xAAu) |        /* Disable write protect      */
                  PM_UNLOCK_ADDR((uint32_t)&PM->PM_MCCTRL - (uint32_t)PM);
  PM->PM_MCCTRL = PM_MCCTRL_MCSEL(4);
  while((PM->PM_SR & PM_SR_CKRDY) != PM_SR_CKRDY);

  LED_Init ();                              /* Initialize the LEDs           */

  os_sys_init(init);                        /* Initialize RTX and start init */
}
