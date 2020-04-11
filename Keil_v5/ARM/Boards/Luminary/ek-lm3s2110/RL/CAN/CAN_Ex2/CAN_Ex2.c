/*----------------------------------------------------------------------------
 *      RL-ARM - CAN
 *----------------------------------------------------------------------------
 *      Name:    CAN_Ex2.c
 *      Purpose: RTX CAN Driver usage example
 *      Rev.:    V4.70
 *----------------------------------------------------------------------------
 *      This code is part of the RealView Run-Time Library.
 *      Copyright (c) 2004-2013 KEIL - An ARM Company. All rights reserved.
 *---------------------------------------------------------------------------*/

#include <RTL.h>                      /* RTX kernel functions & defines      */
#include <LM3Sxxxx.H>                 /* LM3Sxxxx library definitions        */
#include <RTX_CAN.h>                  /* CAN Generic functions & defines     */ 


__task void task_init     (void);
__task void task_set_CAN  (void);
__task void task_disp     (void);

#define LED      GPIO_PIN_2           /* STATUS LED pin                      */

U32  Tx_val = 0, Rx_val = 0;          /* Global variables used for display   */
U8   set_val;                         /* Value that will be set periodically */

/*----------------------------------------------------------------------------
 *  Functions for init and getting input value
 *---------------------------------------------------------------------------*/

void In_Init (void) {
  set_val = 255;
}

unsigned char In_Get (void) {
  set_val ++;
  set_val &= 0xFF;

  return (set_val);
}

/*----------------------------------------------------------------------------
 *  Functions for init and output of value on visual element
 *---------------------------------------------------------------------------*/

void Out_Init (void) {
  /* Set GPIOF LED pin as output                                             */
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
  GPIOPadConfigSet(GPIO_PORTF_BASE, LED, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD);
  GPIODirModeSet(GPIO_PORTF_BASE, LED, GPIO_DIR_MODE_OUT);
}

void Out_Val (void) {
  GPIOPinWrite (GPIO_PORTF_BASE, LED, (Tx_val & 0x01) ? LED : 0);
}


/*----------------------------------------------------------------------------
 *  Task 0: Initializes and starts other tasks
 *---------------------------------------------------------------------------*/

__task void task_init (void)  {
  os_tsk_create (task_set_CAN , 2);   /* Start               set task        */
  os_tsk_create (task_disp    , 3);   /* Start        displaying task        */
  os_tsk_delete_self();               /* End      initialization task        */
}

/*----------------------------------------------------------------------------
 *  Task 1: Sets message with incrementing value in data[0] periodically (every 1s)
 *---------------------------------------------------------------------------*/

__task void task_set_CAN (void)  {
  /* Initialize message  = { ID, {data[0] .. data[7]}, LEN, CHANNEL, FORMAT, TYPE } */
  CAN_msg msg_set        = { 33, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   
                              1, 1, STANDARD_FORMAT, DATA_FRAME };

  CAN_init (1, 500000);               /* CAN controller 1 init, 500 kbit/s   */

  CAN_start (1);                      /* Start CAN controller 1              */

  for (;;)  {
    Tx_val           = In_Get ();     /* Tx_val = value to send              */
    msg_set.data[0]  = Tx_val;        /* Data[0] field = incrementing value  */
    CAN_set (1, &msg_set , 0x0F00);   /* Set answer to remote frame          */

    os_dly_wait (100);                /* Wait 1 second                       */
  }
}

/*----------------------------------------------------------------------------
 *  Task 2: Activate visual outputs
 *---------------------------------------------------------------------------*/

__task void task_disp (void)  {
  for (;;)  {
    Out_Val ();                       /* Output info on visual outputs       */
    os_dly_wait (10);                 /* Wait 100 ms and display again       */
  }
}


/*----------------------------------------------------------------------------
 *        Main: Initialize and start RTX Kernel
 *---------------------------------------------------------------------------*/

int main (void)  {                    /* Program execution starts here       */
  U32 volatile dbg;

  for (dbg = 0; dbg < 1000000; dbg++);/* Ensure debugger connection 0.3s     */

  /* Set the clocking to run from XTAL at 8 MHz                              */
  SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_8MHZ);

  In_Init  ();                        /* Initialize input                    */
  Out_Init ();                        /* Initialize visual outputs           */

  os_sys_init (task_init);            /* Initialize OS and start init task   */
}


/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/
