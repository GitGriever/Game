/*----------------------------------------------------------------------------
 *      RL-ARM - CAN
 *----------------------------------------------------------------------------
 *      Name:    CAN_Ex1.c
 *      Purpose: RTX CAN Driver usage example
 *      Rev.:    V4.70
 *----------------------------------------------------------------------------
 *      This code is part of the RealView Run-Time Library.
 *      Copyright (c) 2004-2013 KEIL - An ARM Company. All rights reserved.
 *---------------------------------------------------------------------------*/

#include <RTL.h>                      /* RTX kernel functions & defines      */
#include <stm32f2xx.h>                /* STM32F20x register definitions      */
#include <RTX_CAN.h>                  /* CAN Generic functions & defines     */ 
#include "GLCD.h"                     /* GLCD functions prototypes           */
#include "ADC.h"

__task void task_init     (void);
__task void task_send_CAN (void);
__task void task_rece_CAN (void);
__task void task_disp     (void);

U32 Tx_val = 0, Rx_val = 0;           /* Global variables used for display   */

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


/*----------------------------------------------------------------------------
 *  Functions for init and getting input value
 *---------------------------------------------------------------------------*/

void In_Init (void) {

  /* Setup and initialize ADC converter                                      */
  ADC_Init();  
}

unsigned char In_Get (void) {
  int32_t val;
  ADC_StartCnv();
  val = ADC_GetVal ();
  return (val >> 4);           /* Scale analog value to 8 bits      */
}

/*----------------------------------------------------------------------------
 *  Functions for init and output of value on visual element
 *---------------------------------------------------------------------------*/

void Out_Init (void) {

  /* Setup LCD                                                               */
  GLCD_Init();                          /* Initialize the GLCD               */
  GLCD_Clear(White);                    /* Clear the GLCD                    */

  GLCD_SetBackColor(Blue);              /* Set the Back Color                */
  GLCD_SetTextColor(White);             /* Set the Text Color                */
  GLCD_DisplayString(0, 0, 1, "  MCBSTM32F200 Demo ");
  GLCD_DisplayString(1, 0, 1, "      CAN_Ex1       ");
  GLCD_DisplayString(2, 0, 1, "    www.keil.com    ");
  GLCD_SetBackColor(White);             /* Set the Back Color                */
  GLCD_SetTextColor(Blue);              /* Set the Text Color                */

  GLCD_DisplayString( 4, 0, 1, " CAN at 500kbit/s   ");
  GLCD_DisplayString( 8, 0, 1, " AD value:          ");

}

void Out_Val (void) {
  U32 val1, val2;
  static unsigned char disp_buf[] = " Tx:    , Rx:    ";

  val1 = Tx_val;                        /* Read values to local variable     */
  val2 = Rx_val;    

  Hex_Str(val1, &disp_buf[ 4]);         /* Display Tx and Rx values to LCD   */ 
  Hex_Str(val2, &disp_buf[13]);
  GLCD_DisplayString(7, 0, 1, disp_buf);/* Print string to LCD               */

  GLCD_Bargraph (160, 8*24, 140, 20, Tx_val<<2); /* Disp tx val on bargraph */
}


/*----------------------------------------------------------------------------
 *  Task 0: Initializes and starts other tasks
 *---------------------------------------------------------------------------*/

__task void task_init (void)  {
  os_tsk_create (task_send_CAN, 3);   /* Start          transmit task        */
  os_tsk_create (task_rece_CAN, 4);   /* Start           receive task        */
  os_tsk_create (task_disp    , 2);   /* Start displaying to LCD task        */
  os_tsk_delete_self();               /* End      initialization task        */
}

/*----------------------------------------------------------------------------
 *  Task 1: Sends message with input value in data[0] over CAN periodically
 *---------------------------------------------------------------------------*/

__task void task_send_CAN (void)  {
  /* Initialize message  = { ID, {data[0] .. data[7]}, LEN, CHANNEL, FORMAT, TYPE } */
  CAN_msg msg_send       = { 33, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   
                              1, 2, STANDARD_FORMAT, DATA_FRAME };

  CAN_init (1, 500000);               /* CAN controller 1 init, 500 kbit/s   */

  CAN_rx_object (1, 2,  33, DATA_TYPE | STANDARD_TYPE); /* Enable reception  */
                                      /* of message on controller 1, channel */
                                      /* is not used for STM32 (can be set to*/
                                      /* whatever value), data frame with    */
                                      /* standard id 33                      */

  /* The activation of test mode in line below is used for enabling 
     self-testing mode when CAN controller receives the message it sends so 
     program functionality can be tested without another CAN device          */
  /* COMMENT THE LINE BELOW TO ENABLE DEVICE TO PARTICIPATE IN CAN NETWORK   */
  CAN_hw_testmode(1, CAN_BTR_SILM | CAN_BTR_LBKM); /* Loopback and           */
                                                   /* Silent Mode (self-test)*/

  CAN_start (1);                      /* Start controller 1                  */

  for (;;)  {
    msg_send.data[0] = In_Get ();     /* Data[0] field = analog value from   */
                                      /* potentiometer                       */

    CAN_send (1, &msg_send, 0x0F00);  /* Send msg_send on controller 2       */
    Tx_val = msg_send.data[0];
    os_dly_wait (100);                /* Wait 1 second                       */
  }
}

/*----------------------------------------------------------------------------
 *  Task 2: Received CAN message
 *---------------------------------------------------------------------------*/

__task void task_rece_CAN (void)  {
  CAN_msg msg_rece;

  for (;;)  {
    /* When message arrives store received value to global variable Rx_val   */
    if (CAN_receive (1, &msg_rece, 0x00FF) == CAN_OK)  {
      Rx_val = msg_rece.data[0];
    }
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
  Out_Init ();                        /* Initialize visual outputs           */

  os_sys_init (task_init);            /* Initialize OS and start init task   */
}


/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/
