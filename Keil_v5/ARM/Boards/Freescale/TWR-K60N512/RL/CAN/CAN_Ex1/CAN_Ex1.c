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
#include <MK60N512MD100.H>
#include "ADC.h"
#include "LED.h"

__task void task_init     (void);
__task void task_init_CAN (void);
__task void task_send_CAN (void);
__task void task_rece_CAN (void);
__task void task_disp     (void);

U32 Tx_val = 0, Rx_val = 0;             /* Global variables used for display  */

unsigned char hex_chars[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};


/*------------------------------------------------------------------------------
 *  Function for converting 1 byte to string in hexadecimal notation
 *----------------------------------------------------------------------------*/

void Hex_Str (unsigned char hex, unsigned char *str) {
  *str++ = '0';
  *str++ = 'x';
  *str++ = hex_chars[hex >>  4];
  *str++ = hex_chars[hex & 0xF];
}


/*------------------------------------------------------------------------------
 *  Functions for init and getting input value
 *----------------------------------------------------------------------------*/

void In_Init (void) {

  ADC_Init ();
}

unsigned char In_Get (void) {

  ADC_StartCnv();
  return (ADC_GetVal() >> 8);
}

/*------------------------------------------------------------------------------
 *  Functions for init and output of value on visual element
 *----------------------------------------------------------------------------*/

void Out_Init (void) {

  LED_Init ();
}

void Out_Val (void) {

  LED_Val (Rx_val >> 4);
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
  CAN_init (2, 500000);                 /* CAN controller 2 init, 500 kbit/s  */

  CAN_rx_object (2, 2,  33, DATA_TYPE  | STANDARD_TYPE); /* Enable reception  */
                                        /* of message on controller 2, channel*/
                                        /* 2, data frame with standard id 33  */

  CAN_start (2);                        /* Start controller 2                 */
  os_tsk_delete_self();                 /* End CAN initialization task        */
}

/*------------------------------------------------------------------------------
 *  Task: Sends message with input value over CAN periodically
 *----------------------------------------------------------------------------*/

__task void task_send_CAN (void)  {
  /* Initialize message  = { ID, {data[0]             ..                 data[7]}, LEN, CHANNEL,          FORMAT,       TYPE } */
  CAN_msg msg_send       = { 33, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   1,       1, STANDARD_FORMAT, DATA_FRAME };

  for (;;)  {
    msg_send.data[0] = In_Get ();       /* Data[0] field = analog value from  */
                                        /* potentiometer                      */

    CAN_send (2, &msg_send, 0x0F00);    /* Send msg_send on controller 2      */
    Tx_val = msg_send.data[0];
    os_dly_wait (100);                  /* Wait 1 second                      */
  }
}

/*------------------------------------------------------------------------------
 *  Task: Received CAN message
 *----------------------------------------------------------------------------*/

__task void task_rece_CAN (void)  {
  CAN_msg msg_rece;

  for (;;)  {
    /* When message arrives store received value to global variable Rx_val    */
    if (CAN_receive (2, &msg_rece, 0x00FF) == CAN_OK)  {
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
