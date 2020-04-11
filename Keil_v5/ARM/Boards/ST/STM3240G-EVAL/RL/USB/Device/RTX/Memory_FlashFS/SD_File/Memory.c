/*-----------------------------------------------------------------------------
 *      RL-ARM - USB
 *-----------------------------------------------------------------------------
 *      Name:    Memory.c
 *      Purpose: USB Memory Storage Demo
 *      Rev.:    V4.70
 *-----------------------------------------------------------------------------
 *      This code is part of the RealView Run-Time Library.
 *      Copyright (c) 2004-2013 KEIL - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <stm32f4xx.h>                  /* STM32F4xx definitions              */

#include <RTL.h>
#include <File_Config.h>
#include <rl_usb.h>

#include "Terminal.h"
#include "Memory.h"
#include "Serial.h"
#include "GLCD.h"
#include "LED.h"
#include "KBD.h"

#define WAKEUP    1

#define SEL_TERM  0
#define SEL_PC    1

OS_TID tid_sel;                         /* Interface selection task ID        */
OS_TID tid_term;                        /* Terminal task ID                   */

FAT_VI *mc0;                            /* FAT Media Control Block            */
Media_INFO info;                        /* Media information                  */ 

U64 term_stack[800/4];                  /* Terminal uses user provided stack  */

extern __task void file_man (void);     /* File manipulation main task        */

/*-----------------------------------------------------------------------------
  Check and debounce the wakeup key
 *----------------------------------------------------------------------------*/
__task void chk_key (void) {
  static BOOL WKUP_Key = __TRUE;
  BOOL key;

  os_itv_set (50);                      /* Set task interval to 50ms          */
  while (1) {
    key = ((KBD_GetKeys() & WAKEUP) == 0);
    if (key && !WKUP_Key) {
      /* Key was pressed, switch the interface */
      os_evt_set (EVT_SW, tid_sel);
    }
    WKUP_Key = key;
    os_itv_wait();
  }
}


/*-----------------------------------------------------------------------------
  Setup GLCD and print out example title
 *----------------------------------------------------------------------------*/
static void GLCD_Setup (void) {
  GLCD_Init ();
  GLCD_Clear (Blue);
  GLCD_SetBackColor (Blue);
  GLCD_SetTextColor (White);
  GLCD_DisplayString (0, 0, 1, "    STM3240G-EVAL   ");
  GLCD_DisplayString (1, 0, 1, "       RL-ARM       ");
  GLCD_DisplayString (2, 0, 1, "    MSD example     ");
  GLCD_DisplayString (4, 0, 1, " Current:           ");
}


/*-----------------------------------------------------------------------------
  Interface selection task
 *----------------------------------------------------------------------------*/
__task void sel_if (void) {
  U32 sel = SEL_TERM;

  while (1) {
    if (sel == SEL_PC) {
      /* Switch from the PC to Terminal Interface                             */
      sel = SEL_TERM;
      usbd_connect(__FALSE);            /* USB Device Disconnect              */
      LED_Val (LED_MSK_OFF);
      GLCD_DisplayString (5, 0, 1, "   Term Interface   ");

      os_evt_set (EVT_SW, tid_term);    /* Send terminal activation request   */
    }
    else {
      /* Switch from the Terminal to PC Interface                             */
      sel = SEL_PC;
      
      os_evt_set (EVT_SW, tid_term);    /* Send terminal deactivation request */
      os_evt_wait_and (EVT_SW, 0xFFFF); /* Wait until terminal is deactivated */

      mc0 = ioc_getcb (NULL);
      if (ioc_init (mc0) == 0) {
        ioc_read_info (&info, mc0);
        usbd_msc_init ();               /* Init USB MSC Global Variables      */
        usbd_connect(__TRUE);           /* USB Device Connect                 */
        GLCD_DisplayString (5, 0, 1, "   PC Interface     ");
      }
      else {
        LED_Val (LED_MSK_ALL);
        GLCD_DisplayString (5, 0, 1, "  Memory Failure!   ");
      }
    }
    while (os_evt_wait_and (EVT_SW, 0) == OS_R_TMO) {
      os_dly_wait (50);
  }
}
}


/*-----------------------------------------------------------------------------
  Init peripheral and create application tasks
 *----------------------------------------------------------------------------*/
__task void init (void) {
  SER_Init   ();                        /* Init serial interface              */
  LED_Init   ();                        /* Init LEDs                          */
  KBD_Init   ();                        /* Init Keyboard                      */
  usbd_init  ();                        /* Init USB Device                    */
  GLCD_Setup ();                        /* Init and setup GLCD                */

  os_tsk_prio_self (10);
  os_tsk_create (chk_key, 5);           /* Create Wakeup key checking task    */
  tid_sel = os_tsk_create (sel_if,  4); /* Create Interface selection task    */
  
  /* Create File manipulation task with user provided stack                   */
  tid_term = os_tsk_create_user (file_man, 4, &term_stack, sizeof (term_stack));
  os_tsk_delete_self ();  
}


/*-----------------------------------------------------------------------------
  Main Entry
 *----------------------------------------------------------------------------*/
int main (void) {
  os_sys_init (init);                   /* Init RTX and start application     */
}

/*-----------------------------------------------------------------------------
 * End of file
 *----------------------------------------------------------------------------*/
