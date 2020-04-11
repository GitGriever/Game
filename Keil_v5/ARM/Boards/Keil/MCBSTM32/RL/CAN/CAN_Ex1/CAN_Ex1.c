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
#include <STM32F10x.h>                /* STM32F10x Definitions               */
#include <RTX_CAN.h>                  /* CAN Generic functions & defines     */ 
#include "LCD.h"                      /* LCD functions prototypes            */

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
  RCC->APB2ENR |=  1 <<  9;             /* Enable ADC1 clock                 */
  GPIOA->CRL   &= 0xFFFFFF0F;           /* Configure PA1 as ADC.1 input      */
  ADC1->SQR1    = 0x00000000;           /* Regular channel 1 conversion      */
  ADC1->SQR2    = 0x00000000;           /* Clear register                    */
  ADC1->SQR3    =  1 <<  0;             /* SQ1 = channel 1                   */
  ADC1->SMPR1   = 0x00000000;           /* Clear register                    */
  ADC1->SMPR2   =  5 <<  3;             /* Channel 1 sample time is 55.5 cyc */
  ADC1->CR1     =  1 <<  8;             /* Scan mode on                      */
  ADC1->CR2     = (1 << 20) |           /* Enable external trigger           */
                  (7 << 17) |           /* EXTSEL = SWSTART                  */
                  (1 <<  1) |           /* Continuous conversion             */
                  (1 <<  0) ;           /* ADC enable                        */
  ADC1->CR2    |=  1 <<  3;             /* Initialize calibration registers  */
  while (ADC1->CR2 & (1 << 3));         /* Wait for initialization to finish */
  ADC1->CR2    |=  1 <<  2;             /* Start calibration                 */
  while (ADC1->CR2 & (1 << 2));         /* Wait for calibration to finish    */
}

unsigned char In_Get (void) {
  U16 val;

  ADC1->CR2 |= 1 << 22;                 /* Start new conversion              */ 
  while (!(ADC1->SR & (1 << 1)));       /* Wait til end of conversion        */
  val = (ADC1->DR & 0x0FFF);            /* Get conversion result             */
  val = (val >> 4) & 0xFF;              /* Scale analog value to 8 bits      */
	
  return (val);
}

/*----------------------------------------------------------------------------
 *  Functions for init and output of value on visual element
 *---------------------------------------------------------------------------*/

void Out_Init (void) {
  int n;

  /* Setup LCD                                                               */
  lcd_init();                           /* Initialize the LCD display        */
  lcd_clear();                          /* Clear the LCD display             */
  lcd_print ("MCBSTM32 CanDemo");       /* Display string on LCD display     */
  set_cursor (0, 1);                    /* Set cursor pos on LCD display     */
  lcd_print ("  www.keil.com  ");
  for (n = 0; n < 15000000; n ++);      /* Wait for initial display (~2s)    */

  lcd_clear ();
  lcd_print ("CAN at 500kbit/s");
}

void Out_Val (void) {
  U32 val1, val2;
  static unsigned char disp_buf[] = "Tx:    , Rx:    ";

  val1 = Tx_val;                        /* Read values to local variable     */
  val2 = Rx_val;    

  Hex_Str(val1, &disp_buf[ 3]);         /* Display Tx, Rx values to LCD disp.*/ 
  Hex_Str(val2, &disp_buf[12]);
  set_cursor (0, 1);
  lcd_print  ((char *)disp_buf);
}


/*----------------------------------------------------------------------------
 *  Task 0: Initializes and starts other tasks
 *---------------------------------------------------------------------------*/

__task void task_init (void)  {
  os_tsk_create (task_send_CAN, 3);     /* Start          transmit task      */
  os_tsk_create (task_rece_CAN, 4);     /* Start           receive task      */
  os_tsk_create (task_disp    , 2);     /* Start displaying to LCD task      */
  os_tsk_delete_self();                 /* End      initialization task      */
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

    CAN_send (1, &msg_send, 0x0F00);  /* Send msg_send on controller 1       */
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
                                      /* - Analog voltage                    */
  Out_Init ();                        /* Initialize visual outputs           */
                                      /* - LCD display                       */

  os_sys_init (task_init);            /* Initialize OS and start init task   */
}


/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/
