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
#include "..\..\..\rit128x96x4.h"     /* LCD functions                       */
#include <stdio.h>


__task void task_init     (void);
__task void task_send_CAN (void);
__task void task_rece_CAN (void);
__task void task_disp     (void);

#define LED      GPIO_PIN_2           /* STATUS LED pin                      */

U32  Tx_val = 0, Rx_val = 0;          /* Transmit and receive values         */
U32  Tx_cnt = 0, Rx_cnt = 0;          /* Transmit and receive counters       */
U8   lcd_text[8][21+1];               /* Buffer for LCD text                 */

/*----------------------------------------------------------------------------
 *  Functions for init and getting input value
 *---------------------------------------------------------------------------*/

void In_Init (void) {
}

unsigned char In_Get (void) {
  return (0);
}

/*----------------------------------------------------------------------------
 *  Functions for init and output of value on visual element
 *---------------------------------------------------------------------------*/

void Out_Init (void) {
  /* Set GPIOG LED pin as output                                             */
  SysCtlPeripheralEnable (SYSCTL_PERIPH_GPIOG);
  GPIOPadConfigSet (GPIO_PORTG_BASE, LED, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD);
  GPIODirModeSet (GPIO_PORTG_BASE, LED, GPIO_DIR_MODE_OUT);

  /* Initialize LCD and display initial message                              */
  RIT128x96x4Init(1000000);
  RIT128x96x4Clear ();
  sprintf((char *)lcd_text[0],"      Keil Demo      ");
  sprintf((char *)lcd_text[1],"   EK-LM3S2965 RL    ");
  sprintf((char *)lcd_text[2],"   CAN_Ex2 example   ");
  sprintf((char *)lcd_text[3],"                     ");
  sprintf((char *)lcd_text[4],"                     ");
  sprintf((char *)lcd_text[5],"  Received    : 0x00 ");
  sprintf((char *)lcd_text[6],"  TXed rqsts  :    0 ");
  sprintf((char *)lcd_text[7],"  RXed msgs   :    0 ");
  RIT128x96x4StringDraw ((const char *)&lcd_text[0], 0,      6, 11);
  RIT128x96x4StringDraw ((const char *)&lcd_text[1], 0, 10*1+6, 11);
  RIT128x96x4StringDraw ((const char *)&lcd_text[2], 0, 10*2+6, 11);
  RIT128x96x4StringDraw ((const char *)&lcd_text[3], 0, 10*3+6, 11);
  RIT128x96x4StringDraw ((const char *)&lcd_text[4], 0, 10*4+6, 11);
  RIT128x96x4StringDraw ((const char *)&lcd_text[5], 0, 10*5+6, 11);
  RIT128x96x4StringDraw ((const char *)&lcd_text[6], 0, 10*6+6, 11);
  RIT128x96x4StringDraw ((const char *)&lcd_text[7], 0, 10*7+6, 11);
}

void Out_Val (void) {
  static char led_blink = 0;

  /* Blink STATUS LED for 100 ms every 1 second                              */
  if (++led_blink > 9) led_blink = 0;
  GPIOPinWrite (GPIO_PORTG_BASE, LED, (led_blink < 1) ? LED : 0);

  /* Update received values on LCD                                           */
  sprintf((char *)lcd_text[5],"%.2X", Rx_val);
  sprintf((char *)lcd_text[6],"% 4i", Tx_cnt);
  sprintf((char *)lcd_text[7],"% 4i", Rx_cnt);
  RIT128x96x4StringDraw ((const char *)&lcd_text[5], 18*6, 10*5+6, 11);
  RIT128x96x4StringDraw ((const char *)&lcd_text[6], 16*6, 10*6+6, 11);
  RIT128x96x4StringDraw ((const char *)&lcd_text[7], 16*6, 10*7+6, 11);
}


/*----------------------------------------------------------------------------
 *  Task 0: Initializes and starts other tasks
 *---------------------------------------------------------------------------*/

__task void task_init (void)  {
  os_tsk_create (task_send_CAN, 2);   /* Start          transmit task        */
  os_tsk_create (task_rece_CAN, 3);   /* Start           receive task        */
  os_tsk_create (task_disp    , 4);   /* Start        displaying task        */
  os_tsk_delete_self();               /* End      initialization task        */
}

/*----------------------------------------------------------------------------
 *  Task 1: Sends remote transmission request over CAN periodically
 *---------------------------------------------------------------------------*/

__task void task_send_CAN (void)  {
  /* Initialize message  = { ID, {data[0] .. data[7]}, LEN, CHANNEL, FORMAT, TYPE } */
  CAN_msg msg_send       = { 33, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   
                              1, 2, STANDARD_FORMAT, REMOTE_FRAME };

  CAN_init (1, 500000);               /* CAN controller 1 init, 500 kbit/s   */

  /* Transmit object does not have to be enabled but channel has to be 
     reserved for transmitting, in this example channel 2 is reserved for 
     transmission of remote transmission request                             */

  CAN_rx_object (1, 1,  33, DATA_TYPE | STANDARD_TYPE); /* Enable reception  */
                                      /* of message on controller 1, channel */
                                      /* 1, data frame with standard id 33   */

  CAN_start (1);                      /* Start CAN controller 1              */

  /* The activation of loopback mode in line below is used for enabling 
     self-testing mode when CAN controller receives the message it sends so 
     program functionality can be tested without another CAN device          */
  /* COMMENT THE LINE BELOW TO ENABLE DEVICE TO PARTICIPATE IN CAN NETWORK   */
//CAN_hw_loopback (1);                /* Activate Loopback mode              */

  for (;;)  {
    Tx_val           = In_Get ();     /* Tx_val = value to send              */
    msg_send.data[0] = Tx_val;        /* Data[0] field = incrementing value  */
    CAN_request(1, &msg_send, 0x0F00);/* Send remote request on controller 1 */
    Tx_cnt++;                         /* Increment transmit counter          */
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
      Rx_cnt++;                       /* Increment receive counter           */
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
  U32 volatile dbg;

  for (dbg = 0; dbg < 1000000; dbg++);/* Ensure debugger connection 0.3s     */

  /* Set the clocking to run from PLL at 50 MHz                              */
  SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_8MHZ);

  In_Init  ();                        /* Initialize input                    */
  Out_Init ();                        /* Initialize visual outputs           */

  os_sys_init (task_init);            /* Initialize OS and start init task   */
}


/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/
