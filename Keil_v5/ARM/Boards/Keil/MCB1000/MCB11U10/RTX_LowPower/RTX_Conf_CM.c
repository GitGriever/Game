/*----------------------------------------------------------------------------
 *      RL-ARM - RTX
 *----------------------------------------------------------------------------
 *      Name:    RTX_CONFIG.C
 *      Purpose: Configuration of RTX Kernel for Cortex-M
 *      Rev.:    V4.20
 *----------------------------------------------------------------------------
 *      This code is part of the RealView Run-Time Library.
 *      Copyright (c) 2004-2011 KEIL - An ARM Company. All rights reserved.
 *---------------------------------------------------------------------------*/

#include <RTL.h>
#include "LPC11Uxx.h"                   /* LPC11Uxx definitions              */

/*----------------------------------------------------------------------------
 *      RTX User configuration part BEGIN
 *---------------------------------------------------------------------------*/

//-------- <<< Use Configuration Wizard in Context Menu >>> -----------------
//
// <h>Task Configuration
// =====================
//
//   <o>Number of concurrent running tasks <0-250>
//   <i> Define max. number of tasks that will run at the same time.
//   <i> Default: 6
#ifndef OS_TASKCNT
 #define OS_TASKCNT     6
#endif

//   <o>Number of tasks with user-provided stack <0-250>
//   <i> Define the number of tasks that will use a bigger stack.
//   <i> The memory space for the stack is provided by the user.
//   <i> Default: 0
#ifndef OS_PRIVCNT
 #define OS_PRIVCNT     0
#endif

//   <o>Task stack size [bytes] <20-4096:8><#/4>
//   <i> Set the stack size for tasks which is assigned by the system.
//   <i> Default: 200
#ifndef OS_STKSIZE
 #define OS_STKSIZE     50
#endif

// <q>Check for the stack overflow
// ===============================
// <i> Include the stack checking code for a stack overflow.
// <i> Note that additional code reduces the Kernel performance.
#ifndef OS_STKCHECK
 #define OS_STKCHECK    1
#endif

// <q>Run in privileged mode
// =========================
// <i> Run all Tasks in privileged mode.
// <i> Default: Unprivileged
#ifndef OS_RUNPRIV
 #define OS_RUNPRIV     0
#endif

// </h>
// <h>SysTick Timer Configuration
// =============================
//   <o>Timer clock value [Hz] <1-1000000000>
//   <i> Set the timer clock value for selected timer.
//   <i> Default: 6000000  (6MHz)
#ifndef OS_CLOCK
 #define OS_CLOCK       48000000
#endif

//   <o>Timer tick value [us] <1-1000000>
//   <i> Set the timer tick value for selected timer.
//   <i> Default: 10000  (10ms)
#ifndef OS_TICK
 #define OS_TICK        10000
#endif

// </h>

// <h>System Configuration
// =======================
// <e>Round-Robin Task switching
// =============================
// <i> Enable Round-Robin Task switching.
#ifndef OS_ROBIN
 #define OS_ROBIN       1
#endif

//   <o>Round-Robin Timeout [ticks] <1-1000>
//   <i> Define how long a task will execute before a task switch.
//   <i> Default: 5
#ifndef OS_ROBINTOUT
 #define OS_ROBINTOUT   5
#endif

// </e>

//   <o>Number of user timers <0-250>
//   <i> Define max. number of user timers that will run at the same time.
//   <i> Default: 0  (User timers disabled)
#ifndef OS_TIMERCNT
 #define OS_TIMERCNT    0
#endif

//   <o>ISR FIFO Queue size<4=>   4 entries  <8=>   8 entries
//                         <12=> 12 entries  <16=> 16 entries
//                         <24=> 24 entries  <32=> 32 entries
//                         <48=> 48 entries  <64=> 64 entries
//                         <96=> 96 entries
//   <i> ISR functions store requests to this buffer,
//   <i> when they are called from the iterrupt handler.
//   <i> Default: 16 entries
#ifndef OS_FIFOSZ
 #define OS_FIFOSZ      16
#endif

// </h>

//------------- <<< end of configuration section >>> -----------------------

// Standard library system mutexes
// ===============================
//  Define max. number system mutexes that are used to protect 
//  the arm standard runtime library. For microlib they are not used.
#ifndef OS_MUTEXCNT
 #define OS_MUTEXCNT    8
#endif

/*----------------------------------------------------------------------------
 *      RTX User configuration part END
 *---------------------------------------------------------------------------*/

#define OS_TRV          ((U32)(((double)OS_CLOCK*(double)OS_TICK)/1E6)-1)


volatile static BOOL Suspend;             /* OS Suspend state                */
/*----------------------------------------------------------------------------
 *      Global Functions
 *---------------------------------------------------------------------------*/

void WDT_Feed (void) {
  /* Disable IRQs */
  __disable_irq();

  /* WDT feed sequence */
  LPC_WWDT->FEED = 0xAA;
  LPC_WWDT->FEED = 0x55;

  /* Enable IRQs */
  __enable_irq();
}

void AbortSleep (void) {
  if (Suspend) {
    Suspend = __FALSE;                    /* Prevent WFI execution            */

    LPC_WWDT->TC = 1010;                  /* WDT interrupt after 400us @25kHz */
    WDT_Feed ();
  }
}

void FLEX_INT0_IRQHandler (void) {
  /* IRQ processing here */
  /*        ..           */
  
  /* Abort sleep */
  AbortSleep ();
}

void WDT_IRQHandler (void) {
  LPC_WWDT->MOD |= (1UL << 3);                 /* Clear WDINT flag           */
}

/*--------------------------- os_idle_demon ---------------------------------*/

__task void os_idle_demon (void) {
  /* The idle demon is a system task, running when no other task is ready */
  /* to run. The 'os_xxx' function calls are not allowed from this task.  */
  U32 sleep;
  U32 tc;

  LPC_SYSCON->SYSAHBCLKCTRL |=  (1UL << 15) |  /* Enable clock for WWDT      */
                                (1UL << 19);   /* Enable clock for Pin IRQ   */

  LPC_SYSCON->PINTSEL[0]     =   1;            /* P0.1 selected as INT0 IRQ  */
  LPC_SYSCON->STARTERP0     |=  (1UL <<  0);   /* Enable INT0 wake-up        */
  LPC_SYSCON->STARTERP1     |=  (1UL << 12);   /* Enable WWDT wake-up        */

  LPC_SYSCON->WDTOSCCTRL     =  (3   <<  0) |  /* WDOSC DIVSEL=3             */
                                (2   <<  5);   /* WDOSC FREQ=0.8MHz          */
  LPC_SYSCON->PDRUNCFG      &= ~(1UL <<  6);   /* Power-up WDT Oscillator    */
  LPC_SYSCON->PDSLEEPCFG    &= ~(1UL <<  6);   /* Power WDT Oscillator in PD */

  LPC_WWDT->CLKSEL           =  (1UL <<  0) |  /* Select WDOSC as Clock      */
                                (1UL << 31);   /* Lock selection             */
  LPC_WWDT->WARNINT          =   1000;         /* WDT Warning IRQ value      */
  LPC_WWDT->MOD              =  (1UL <<  0);   /* Enable WDT                 */

  NVIC_EnableIRQ(FLEX_INT0_IRQn);              /* Enable INT0 IRQ (wake-up)  */
  NVIC_EnableIRQ(WDT_IRQn);                    /* Enable WWDT IRQ (wake-up)  */

  for (;;) {
    /* HERE: include optional user code to be executed when no task runs.    */
    Suspend = __TRUE;
    sleep = os_suspend();                      /* OS Suspend                 */

    if (sleep) {                               /* How long can we sleep?     */
      /* "sleep" is in OS ticks which is 10ms (current configuration)        */

      /* Configure for Power-down mode */
      LPC_SYSCON->PDAWAKECFG = LPC_SYSCON->PDRUNCFG;  /* Power after wake-up */
      LPC_PMU->PCON = 0x02;                    /* Select Power-down mode     */
      SCB->SCR = SCB_SCR_SLEEPDEEP_Msk;        /* Configure for deep sleep   */

      /* Setup WDT wake-up: WDT ticks @25kHz (FREQ/2/(DIVSEL+1)/4) */
      tc = (sleep * 250) + 1000;
      LPC_WWDT->TC   = tc;
      WDT_Feed ();

      if (Suspend) {
        __WFI();                               /* Enter Power-down mode      */
      }

      /* After Wake-up */
      sleep = (tc - LPC_WWDT->TV) / 250;
    }

    Suspend = __FALSE;
    os_resume(sleep);                          /* OS Resume                  */
  }
}


/*--------------------------- os_tmr_call -----------------------------------*/

void os_tmr_call (U16 info) {
  /* This function is called when the user timer has expired. Parameter   */
  /* 'info' holds the value, defined when the timer was created.          */

  /* HERE: include optional user code to be executed on timeout. */
}


/*--------------------------- os_error --------------------------------------*/

void os_error (U32 err_code) {
  /* This function is called when a runtime error is detected. Parameter */
  /* 'err_code' holds the runtime error code (defined in RTL.H).         */

  /* HERE: include optional code to be executed on runtime error. */
  for (;;);
}


/*----------------------------------------------------------------------------
 *      RTX Configuration Functions
 *---------------------------------------------------------------------------*/

#include <RTX_lib.c>

/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/
