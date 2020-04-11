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
#include "LPC11Uxx.h"                   /* LPC11Uxx definitions              */
#include "LED.h"

OS_TID t_clock;                         /* assigned task id of task: clock   */
OS_TID t_key;                           /* assigned task id of task: key     */

#define LED_CLK    7


/*----------------------------------------------------------------------------
  Task 1 'clock': Signal Clock
 *---------------------------------------------------------------------------*/
__task void clock (void) {
  for (;;) {
    LED_On (LED_CLK);
    os_dly_wait (5);                     /* delay 5 clock ticks              */
    LED_Off(LED_CLK);
    os_evt_wait_and (0x0100, 295);       /* wait for event or timeout        */
  }
}

/*----------------------------------------------------------------------------
  Task 2 'key': Keyboard
 *---------------------------------------------------------------------------*/
__task void key (void) {
  for (;;) {
    if ((LPC_GPIO->PIN[0] & (1<<1)) == 0) { /* Check ISP Key (P0.1) */
      os_evt_set (0x0100, t_clock);      /* send event signal to clock task  */
    }
    os_dly_wait (5);                     /* delay 5 clock ticks              */
  }
}

/*----------------------------------------------------------------------------
  Task 0 'init': Initialize
 *---------------------------------------------------------------------------*/
__task void init (void) {

  t_clock = os_tsk_create (clock, 0);    /* start task clock                 */
  t_key   = os_tsk_create (key,   0);    /* start task key                   */
  os_tsk_delete_self ();
}

/*----------------------------------------------------------------------------
  Main: Initialize and start RTX Kernel
 *---------------------------------------------------------------------------*/
int main (void) {

  LED_Init ();                              /* Initialize the LEDs           */

  os_sys_init(init);                        /* Initialize RTX and start init */
}
