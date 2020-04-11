/*------------------------------------------------------------------------------
 *      RL-ARM - CAN
 *------------------------------------------------------------------------------
 *      Name:    CAN_Ex1.c
 *      Purpose: RTX CAN Driver usage example
 *------------------------------------------------------------------------------
 *      This code is part of the RealView Run-Time Library.
 *      Copyright (c) 2004-2013 KEIL - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <RTL.h>                        /* RTX kernel functions & defines     */
#include <RTX_CAN.h>                    /* CAN Generic functions & defines    */
#include <XMC4500.h>
#include "LED.h"

__task void task_init     (void);
__task void task_init_CAN (void);
__task void task_send_CAN (void);
__task void task_rece_CAN (void);
__task void task_disp     (void);

U32 Tx_val = 0, Rx_val = 0;           /* Global variables used for display   */
U8  send_val;                         /* Value that will be sent periodically*/


/*------------------------------------------------------------------------------
 *  Functions for init and getting input value
 *----------------------------------------------------------------------------*/

void In_Init (void) {
  send_val = 255;
}

unsigned char In_Get (void) {
  send_val ++;
  send_val &= 0xFF;

  return (send_val);
}

/*------------------------------------------------------------------------------
 *  Functions for init and output of value on visual element
 *----------------------------------------------------------------------------*/

void Out_Init (void) {

  LED_Init ();
}

void Out_Val (void) {

  LED_Val (Rx_val & 1);
}


/*------------------------------------------------------------------------------
 *  Task: Initializes and starts other tasks
 *----------------------------------------------------------------------------*/

__task void task_init (void)  {
  os_tsk_create (task_init_CAN, 5);     /* Start CAN initialization task      */
  os_tsk_create (task_rece_CAN, 4);     /* Start            receive task      */
  os_tsk_create (task_send_CAN, 3);     /* Start           transmit task      */
  os_tsk_create (task_disp    , 2);     /* Start  displaying to LCD task      */
  os_tsk_delete_self();                 /* End       initialization task      */
}

/*------------------------------------------------------------------------------
 *  Task: Initialize CAN
 *----------------------------------------------------------------------------*/

__task void task_init_CAN (void)  {
  CAN_init (3, 500000);                 /* CAN controller 3 init, 500 kbit/s  */

  CAN_rx_object (3, 2,  33, DATA_TYPE  | STANDARD_TYPE); /* Enable reception  */
                                        /* of message on controller 3, channel*/
                                        /* 2, data frame with standard id 33  */

  CAN_tx_object (3, 1,      DATA_TYPE  | STANDARD_TYPE); /* Enable transmision*/
                                        /* of message on controller 3, channel*/
                                        /* 1, data frame with standard id 33  */

  CAN_start (3);                        /* Start controller 3                 */
  os_tsk_delete_self();                 /* End CAN initialization task        */
}

/*------------------------------------------------------------------------------
 *  Task: Send - Sends message with input value over CAN periodically
 *----------------------------------------------------------------------------*/

__task void task_send_CAN (void)  {
  /* Initialize message  = { ID, {data[0]             ..                 data[7]}, LEN, CHANNEL,          FORMAT,       TYPE } */
  CAN_msg msg_send       = { 33, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   1,       1, STANDARD_FORMAT, DATA_FRAME };

  for (;;)  {
    msg_send.data[0] = In_Get ();       /* Data[0] field                      */

    CAN_send (3, &msg_send, 0x0F00);    /* Send msg_send on controller 3      */
    Tx_val = msg_send.data[0];
    os_dly_wait (100);                  /* Wait 1 second                      */
  }
}

/*------------------------------------------------------------------------------
 *  Task: Receive - Receive CAN message
 *----------------------------------------------------------------------------*/

__task void task_rece_CAN (void)  {
  CAN_msg msg_rece;

  for (;;)  {
    /* When message arrives store received value to global variable Rx_val    */
    if (CAN_receive (3, &msg_rece, 0x00FF) == CAN_OK)  {
      Rx_val = msg_rece.data[0];
    }
  }
}

/*------------------------------------------------------------------------------
 *  Task: Activate visual outputs
 *----------------------------------------------------------------------------*/

__task void task_disp (void)  {
  for (;;)  {
    Out_Val ();                         /* Output info on visual outputs      */
    os_dly_wait (10);                   /* Wait 100 ms and display again      */
  }
}

/*------------------------------------------------------------------------------
 *        Main: Initialize and start RTX Kernel
 *----------------------------------------------------------------------------*/

int main (void)  {                      /* Program execution starts here      */
                                                                              
  In_Init  ();                          /* Initialize input                   */
                                        /* - Analog voltage                   */
  Out_Init ();                          /* Initialize visual outputs          */
                                        /* - LCD display                      */

  os_sys_init (task_init);              /* Initialize OS and start init task  */
}


/*------------------------------------------------------------------------------
 * end of file
 *----------------------------------------------------------------------------*/
