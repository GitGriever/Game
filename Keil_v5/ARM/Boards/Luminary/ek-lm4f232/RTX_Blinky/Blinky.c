/*----------------------------------------------------------------------------
 *      RL-ARM - RTX
 *----------------------------------------------------------------------------
 *      Name:    BLINKY.C
 *      Purpose: RTX example program
 *----------------------------------------------------------------------------
 *      This code is part of the RealView Run-Time Library.
 *      Copyright (c) 2012 KEIL - An ARM Company. All rights reserved.
 *---------------------------------------------------------------------------*/

#include <RTL.h>
#include <LM4Fxxxx.H>

#define LED             GPIO_PIN_2      /* PG2 */

OS_TID t_ledOn;                         /* assigned task id of task: ledOn   */
OS_TID t_ledOff;                        /* assigned task id of task: ledOff  */

/*--------------------------- LED_On ----------------------------------------*/
static void LED_On (U32 led) {
  GPIOPinWrite(GPIO_PORTG_BASE, led, led);
}

/*--------------------------- LED_Off ---------------------------------------*/
static void LED_Off (U32 led) {
  GPIOPinWrite(GPIO_PORTG_BASE, led, 0);
}

/*----------------------------------------------------------------------------
  Task 1 'ledOn': switches the LED on
 *---------------------------------------------------------------------------*/
__task void ledOn (void) {
  for (;;) {
    LED_On(LED);                        /* Turn LED On                       */
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
    LED_Off(LED);                       /* Turn LED Off                      */
  }
}

/*----------------------------------------------------------------------------
  Task 3 'init': Initialize
 *---------------------------------------------------------------------------*/
__task void init (void) {

  /* Set the clocking to run from XTAL */
  ROM_SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

  /* Enable peripherals */
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOG);

  /* Configure Status LED as output */
  GPIOPadConfigSet(GPIO_PORTG_BASE, LED, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD);
  GPIODirModeSet  (GPIO_PORTG_BASE, LED, GPIO_DIR_MODE_OUT);
  GPIOPinWrite    (GPIO_PORTG_BASE, LED, 0);

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
