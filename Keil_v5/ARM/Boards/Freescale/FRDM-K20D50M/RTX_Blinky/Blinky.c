/*----------------------------------------------------------------------------
 *      RL-ARM - RTX
 *----------------------------------------------------------------------------
 *      Name:    BLINKY.C
 *      Purpose: RTX example program
 *----------------------------------------------------------------------------
 *      This code is part of the RealView Run-Time Library.
 *      Copyright (c) 2006-2013 KEIL - An ARM Company. All rights reserved.
 *---------------------------------------------------------------------------*/

#include <RTL.h>
#include <MK20D5.H>

OS_TID t_phaseA;                        /* assigned task id of task: phase_a */
OS_TID t_phaseB;                        /* assigned task id of task: phase_b */
OS_TID t_phaseC;                        /* assigned task id of task: phase_c */
OS_TID t_phaseD;                        /* assigned task id of task: phase_d */
OS_TID t_clock;                         /* assigned task id of task: clock   */

#define LED_NUM     3                   /* Number of user LEDs                */
const uint32_t led_mask[] = {1UL << 4, 1UL << 3, 1UL << 2};

#define LED_GREEN  0
#define LED_RED    1
#define LED_BLUE   2
#define LED_A      0
#define LED_B      1
#define LED_C      2
#define LED_D      3
#define LED_CLK    4

/*----------------------------------------------------------------------------
  Function that initializes LEDs
 *----------------------------------------------------------------------------*/
void LED_init(void) {
  SIM->SCGC5   |= (1UL << 9) | (1UL << 11) | (1UL << 12); /* Enable Clock to Port A,C,D */ 
  PORTD->PCR[4]  = (1UL <<  8);                           /* PTD.4 is GPIO */
  PORTC->PCR[3]  = (1UL <<  8);                           /* PTC.3 is GPIO */
  PORTA->PCR[2]  = (1UL <<  8);                           /* PTA.2 is GPIO */

  /* Switch LEDs off */
  PTD->PDOR = led_mask[0];
  PTC->PDOR = led_mask[1];
  PTA->PDOR = led_mask[2];
  /* Enable Output */
  PTD->PDDR = led_mask[0];
  PTC->PDDR = led_mask[1];
  PTA->PDDR = led_mask[2];
}

/*----------------------------------------------------------------------------
  Function that turns on Red LED
 *----------------------------------------------------------------------------*/
void LEDRed_On (void) {
  PTA->PSOR   = led_mask[LED_BLUE];   /* Blue LED Off*/
  PTD->PSOR   = led_mask[LED_GREEN];  /* Green LED Off*/
  PTC->PCOR   = led_mask[LED_RED];    /* Red LED On*/
}

/*----------------------------------------------------------------------------
  Function that turns on Blue LED
 *----------------------------------------------------------------------------*/
void LEDGreen_On (void) {
  PTC->PSOR   = led_mask[LED_RED];    /* Red LED Off*/
  PTA->PSOR   = led_mask[LED_BLUE];   /* Blue LED Off*/
  PTD->PCOR   = led_mask[LED_GREEN];  /* Green LED On*/
}

/*----------------------------------------------------------------------------
  Function that turns on Green LED
 *----------------------------------------------------------------------------*/
void LEDBlue_On (void) {
  PTD->PSOR   = led_mask[LED_GREEN];  /* Green LED Off*/
  PTC->PSOR   = led_mask[LED_RED];    /* Red LED Off*/
  PTA->PCOR   = led_mask[LED_BLUE];   /* Blue LED On*/
}


/*----------------------------------------------------------------------------
 *        Function 'signal_func' called from multiple tasks
 *---------------------------------------------------------------------------*/
void signal_func (OS_TID task)  {
  os_evt_set (0x0100, t_clock);          /* send event signal to clock task  */
  os_dly_wait (5);                       /* delay 5 clock ticks              */
  os_evt_set (0x0100, t_clock);          /* send event signal to clock task  */
  os_dly_wait (5);                       /* delay 5 clock ticks              */
  os_evt_set (0x0001, task);             /* send event to task 'task'        */
}

/*----------------------------------------------------------------------------
 *        Task 1 'phaseA': Phase A output
 *---------------------------------------------------------------------------*/
__task void phaseA (void) {
  for (;;) {
    os_evt_wait_and (0x0001, 0xffff);    /* wait for an event flag 0x0001    */
    LEDRed_On();
    os_dly_wait (100);                   /* delay 100 clock ticks            */
    signal_func (t_phaseB);              /* call common signal function      */
  }
}

/*----------------------------------------------------------------------------
 *        Task 2 'phaseB': Phase B output
 *---------------------------------------------------------------------------*/
__task void phaseB (void) {
  for (;;) {
    os_evt_wait_and (0x0001, 0xffff);    /* wait for an event flag 0x0001    */
    LEDBlue_On();
    os_dly_wait (100);                   /* delay 100 clock ticks            */
    signal_func (t_phaseC);              /* call common signal function      */
  }
}

/*----------------------------------------------------------------------------
 *        Task 3 'phaseC': Phase C output
 *---------------------------------------------------------------------------*/
__task void phaseC (void) {
  for (;;) {
    os_evt_wait_and (0x0001, 0xffff);    /* wait for an event flag 0x0001    */
    LEDRed_On();
    os_dly_wait (100);                   /* delay 100 clock ticks            */
    signal_func (t_phaseD);              /* call common signal function      */
  }
}

/*----------------------------------------------------------------------------
 *        Task 4 'phaseD': Phase D output
 *---------------------------------------------------------------------------*/
__task void phaseD (void) {
  for (;;) {
    os_evt_wait_and (0x0001, 0xffff);    /* wait for an event flag 0x0001    */
    LEDBlue_On();
    os_dly_wait (100);                   /* delay 100 clock ticks            */
    signal_func (t_phaseA);              /* call common signal function      */
  }
}

/*----------------------------------------------------------------------------
 *        Task 5 'clock': Signal Clock
 *---------------------------------------------------------------------------*/
__task void clock (void) {
  for (;;) {
    os_evt_wait_and (0x0100, 0xffff);    /* wait for an event flag 0x0100    */
    LEDGreen_On();
    os_dly_wait (20);                    /* delay 20 clock ticks             */
  }
}


/*----------------------------------------------------------------------------
 *        Task 6 'init': Initialize
 *---------------------------------------------------------------------------*/
__task void init (void) {
  t_phaseA = os_tsk_create (phaseA, 0);  /* start task phaseA                */
  t_phaseB = os_tsk_create (phaseB, 0);  /* start task phaseB                */
  t_phaseC = os_tsk_create (phaseC, 0);  /* start task phaseC                */
  t_phaseD = os_tsk_create (phaseD, 0);  /* start task phaseD                */
  t_clock  = os_tsk_create (clock, 0);   /* start task clock                 */
  os_evt_set (0x0001, t_phaseA);         /* send signal event to task phaseA */
  os_tsk_delete_self ();
}

/*----------------------------------------------------------------------------
 *        Main: Initialize and start RTX Kernel
 *---------------------------------------------------------------------------*/
int main (void) {

  LED_init ();                              /* Initialize the LEDs           */
 
  os_sys_init(init);                        /* Initialize RTX and start init */
}
