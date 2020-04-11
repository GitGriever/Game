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
#include <RTX_CAN.h>                  /* CAN Generic functions & defines     */ 
#include "BTN.h"                      /* Button functions prototypes         */
#include "LED.h"                      /* LED functions prototypes            */
#include "ADC.h"                      /* ADC functions prototypes            */

__task void task_init     (void);
__task void task_set_CAN  (void);
__task void task_RTR_CAN  (void);
__task void task_disp     (void);

U32 Tx_val = 0, Rx_val = 0;           /* Global variables used for display   */


/*----------------------------------------------------------------------------
 *  Functions for init and getting input value
 *---------------------------------------------------------------------------*/

void In_Init (void) {
  BTN_Init();
  ADC_Init();
}

unsigned char In_Get (void) {
  return (ADC_Get() >> 2);
}

/*----------------------------------------------------------------------------
 *  Functions for init and output of value on visual element
 *---------------------------------------------------------------------------*/

void Out_Init (void) {
  LED_Init();
}

void Out_Val (void) {
  LED_Out(Rx_val >> 4);
}


/*----------------------------------------------------------------------------
 *  Task 0: Initializes and starts other tasks
 *---------------------------------------------------------------------------*/

__task void task_init (void)  {
  CAN_init (1, 500000);               /* CAN controller 1 init, 500 kbit/s   */

  CAN_rx_object (1, 1,  33, REMOTE_TYPE | STANDARD_TYPE); /* Enable reception*/
                                      /* of message on controller 1, channel */
                                      /* 1, remote frame with standard id 33 */
  CAN_rx_object (1, 2,  33, DATA_TYPE   | STANDARD_TYPE); /* Enable reception*/
                                      /* of message on controller 1, channel */
                                      /* 2, data frame with standard id 33   */

  /* The activation of test mode in line below is used for enabling 
     self-testing mode when CAN controller receives the message it sends so 
     program functionality can be tested without another CAN device          */
  if (BTN_Get() & (1 << 0)) {
    CAN_hw_testmode(1, __TRUE);       /* Activate loopback                   */
  }

  CAN_start (1);                      /* Start controller 1                  */

  os_tsk_create (task_set_CAN, 4);    /* Start                set task       */
  os_tsk_create (task_RTR_CAN, 3);    /* Start                RTR task       */
  os_tsk_create (task_disp,    2);    /* Start            display task       */
  os_tsk_delete_self();               /* End      initialization  task       */
}

/*----------------------------------------------------------------------------
 *  Task 1: Sends REMOTE FRAME REQUEST, and receives data response
 *---------------------------------------------------------------------------*/

__task void task_RTR_CAN (void)  {
  /* Initialize message  = { ID, {data[0] .. data[7]}, LEN, CHANNEL, FORMAT, TYPE } */
  CAN_msg msg_send       = { 33, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   
                              1, 3, STANDARD_FORMAT, REMOTE_FRAME };
  CAN_msg msg_rece;

  os_dly_wait (10);                   /* Wait 100 ms before first RTR        */

  for (;;)  {
    CAN_request (1, &msg_send, 0x00FF);  /* Send RTR                         */

    /* When message arrives store received value to global variable Rx_val   */
    if (CAN_receive (1, &msg_rece, 0x000F) == CAN_OK)  {
      Rx_val = msg_rece.data[0];
    }

    os_dly_wait (100);                /* Wait 1 second                       */
  }
}

/*----------------------------------------------------------------------------
 *  Task 2: Set data that will be sent automatically upon REMOTE FRAME REQUEST
 *---------------------------------------------------------------------------*/

__task void task_set_CAN (void)  {
  /* Initialize message  = { ID, {data[0] .. data[7]}, LEN, CHANNEL, FORMAT, TYPE } */
  CAN_msg msg_set        = { 33, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   
                              1, 1, STANDARD_FORMAT, DATA_TYPE };

  for (;;)  {
    msg_set.data[0] = In_Get ();      /* Data[0] field = analog value from 
                                         potentiometer                       */

    CAN_set (1, &msg_set, 0x0F00);    /* Set data                            */
    os_dly_wait (100);                /* Wait 1 second                       */
  }
}

/*----------------------------------------------------------------------------
 *  Task 3: Activate visual outputs
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

  In_Init  ();                        /* Initialize input                    */
                                      /* - Analog voltage                    */
  Out_Init ();                        /* Initialize visual outputs           */
                                      /* - LEDs                              */

  os_sys_init (task_init);            /* Initialize OS and start init task   */
}


/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/
