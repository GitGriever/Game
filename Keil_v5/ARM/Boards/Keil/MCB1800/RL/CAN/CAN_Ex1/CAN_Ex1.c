/*----------------------------------------------------------------------------
 *      RL-ARM - CAN
 *----------------------------------------------------------------------------
 *      Name:    CAN_Ex1.c
 *      Purpose: RTX CAN Driver usage example
 *----------------------------------------------------------------------------
 *      This code is part of the RealView Run-Time Library.
 *      Copyright (c) 2004-2013 KEIL - An ARM Company. All rights reserved.
 *---------------------------------------------------------------------------*/

#include <RTL.h>                      /* RTX kernel functions & defines      */
#include <RTX_CAN.h>                  /* CAN Generic functions & defines     */ 
#include <LPC18xx.H>
#include "LED.h"
#include "ADC.h"
#include "GLCD.h"

__task void task_init     (void);
__task void task_init_CAN (void);
__task void task_send_CAN (void);
__task void task_rece_CAN (void);
__task void task_disp     (void);

unsigned char hex_chars[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};


/*----------------------------------------------------------------------------
 *  Function for converting 1 byte to string in hexadecimal notation
 *---------------------------------------------------------------------------*/

void Hex_Str (unsigned char hex, unsigned char *str) {
  *str++ = '0';
  *str++ = 'x';
  *str++ = hex_chars[hex >>  4];
  *str++ = hex_chars[hex & 0xF];
}

U32  Tx_val = 0, Rx_val = 0;          /* Transmit and receive values         */
U32  Tx_cnt = 0, Rx_cnt = 0;          /* Transmit and receive counters       */

/*----------------------------------------------------------------------------
 *  Functions for init and getting input value
 *---------------------------------------------------------------------------*/

void In_Init (void) {

  ADC_Init();
}

unsigned char In_Get (void) {
  int val;

  ADC_StartCnv();
  do {
    val = ADC_GetVal();
  } while (val == -1);

  return (val >> 2);
}

/*----------------------------------------------------------------------------
 *  Functions for init and output of value on visual element
 *---------------------------------------------------------------------------*/

void Out_Init (void) {

  LED_Init();                                      /* Initialize the LEDs    */

  /* Setup LCD                                                               */
  GLCD_Init();                          /* Initialize the GLCD               */
  GLCD_Clear(White);                    /* Clear the GLCD                    */

  GLCD_SetBackColor(Blue);              /* Set the Back Color                */
  GLCD_SetTextColor(White);             /* Set the Text Color                */
  GLCD_DisplayString(0, 0, 1, "    MCB1800 Demo    ");
  GLCD_DisplayString(1, 0, 1, "      CAN_Ex1       ");
  GLCD_DisplayString(2, 0, 1, "    www.keil.com    ");
  GLCD_SetBackColor(White);             /* Set the Back Color                */
  GLCD_SetTextColor(Blue);              /* Set the Text Color                */

  GLCD_DisplayString(4, 0, 1, " CAN1 at 500kbit/s  ");
  GLCD_DisplayString(9, 0, 1, " AD value:          ");
}

void Out_Val (void) {
  U32 tx_val, rx_val, tx_cnt, rx_cnt;
  static unsigned char disp_buf_tx[] = "Tx:   , TxCnt:     ";
  static unsigned char disp_buf_rx[] = "Rx:   , RxCnt:     ";

  tx_val = Tx_val;                    /* Read values to local variable     */
  rx_val = Rx_val;    
  tx_cnt = Tx_cnt;
  rx_cnt = Rx_cnt;    

  Hex_Str(tx_val, &disp_buf_tx[ 3]);  /* Display Tx and TxCnt to disp buf  */ 
  Hex_Str(tx_cnt, &disp_buf_tx[14]);
  Hex_Str(rx_val, &disp_buf_rx[ 3]);  /* Display Rx and RxCnt to disp buf  */ 
  Hex_Str(rx_cnt, &disp_buf_rx[14]);
  GLCD_DisplayString(7, 0, 1, disp_buf_tx);
  GLCD_DisplayString(8, 0, 1, disp_buf_rx);

  GLCD_Bargraph (160, 9*24, 140, 20, Tx_val<<2); /* Disp tx val on bargraph*/
}


/*----------------------------------------------------------------------------
 *  Task 0: Initializes and starts other tasks
 *---------------------------------------------------------------------------*/

__task void task_init (void)  {
  os_tsk_create (task_init_CAN, 5);     /* Start CAN initialization task     */
  os_tsk_create (task_rece_CAN, 4);     /* Start            receive task     */
  os_tsk_create (task_send_CAN, 3);     /* Start           transmit task     */
  os_tsk_create (task_disp    , 2);     /* Start  displaying to LCD task     */
  os_tsk_delete_self();                 /* End       initialization task     */
}

/*----------------------------------------------------------------------------
 *  Task 1: Initialize CAN
 *---------------------------------------------------------------------------*/

__task void task_init_CAN (void)  {
  CAN_init (1, 500000);               /* CAN controller 1 init, 500 kbit/s   */

  CAN_rx_object (1, 2,  33, DATA_TYPE | STANDARD_TYPE); /* Enable reception  */
                                      /* of message on controller 1, channel */
                                      /* 2, data frame with standard id 33   */

  CAN_start (1);                      /* Start controller 1                  */
  os_tsk_delete_self();               /* End CAN initialization task         */
}

/*----------------------------------------------------------------------------
 *  Task 2: Sends message with input value in data[0] over CAN periodically
 *---------------------------------------------------------------------------*/

__task void task_send_CAN (void)  {
  /* Initialize message  = { ID, {data[0] .. data[7]}, LEN, CHANNEL, FORMAT, TYPE } */
  CAN_msg msg_send       = { 33, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   
                              1, 1, STANDARD_FORMAT, DATA_FRAME };

  for (;;)  {
    msg_send.data[0] = In_Get ();     /* Data[0] field = analog value from   */
                                      /* potentiometer                       */

    CAN_send (1, &msg_send, 0x0F00);  /* Send msg_send on controller 1       */
    Tx_val = msg_send.data[0];
    Tx_cnt++;                         /* Increment transmit counter          */
    os_dly_wait (100);                /* Wait 1 second                       */
  }
}

/*----------------------------------------------------------------------------
 *  Task 3: Receives CAN message
 *---------------------------------------------------------------------------*/

__task void task_rece_CAN (void)  {
  CAN_msg msg_rece;

  for (;;)  {
    /* When message arrives store received value to global variable Rx_val   */
    if (CAN_receive (1, &msg_rece, 0x00FF) == CAN_OK)  {
      Rx_val = msg_rece.data[0];
      Rx_cnt++;                       /* Increment receive counter           */
    }
  }
}

/*----------------------------------------------------------------------------
 *  Task 4: Activate visual outputs
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
  SystemCoreClockUpdate();            /* Update System Core Clock            */

  In_Init  ();                        /* Initialize input                    */
                                      /* - Analog voltage                    */
  Out_Init ();                        /* Initialize visual outputs           */
                                      /* - LCD display                       */

  os_sys_init (task_init);            /* Initialize OS and start init task   */
}


/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/
