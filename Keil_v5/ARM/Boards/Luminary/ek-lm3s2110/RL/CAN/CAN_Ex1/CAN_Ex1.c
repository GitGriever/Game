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
#include <LM3Sxxxx.H>                 /* LM3Sxxxx library definitions        */
#include <RTX_CAN.h>                  /* CAN Generic functions & defines     */ 


__task void task_init     (void);
__task void task_send_CAN (void);
__task void task_rece_CAN (void);
__task void task_disp     (void);

#define LED      GPIO_PIN_2           /* STATUS LED pin                      */

U32  Tx_val = 0, Rx_val = 0;          /* Global variables used for display   */
U8   send_val;                        /* Value that will be sent periodically*/

/*----------------------------------------------------------------------------
 *  Functions for init and getting input value
 *---------------------------------------------------------------------------*/

void In_Init (void) {
  send_val = 255;
}

unsigned char In_Get (void) {
  send_val ++;
  send_val &= 0xFF;

  return (send_val);
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
  GPIOPinWrite (GPIO_PORTF_BASE, LED, (Rx_val & 0x01) ? LED : 0);
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
 *  Task 1: Sends message with input value in data[0] over CAN periodically
 *---------------------------------------------------------------------------*/

__task void task_send_CAN (void)  {
  /* Initialize message  = { ID, {data[0] .. data[7]}, LEN, CHANNEL, FORMAT, TYPE } */
  CAN_msg msg_send       = { 33, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   
                              1, 1, STANDARD_FORMAT, DATA_FRAME };

  CAN_init (1, 500000);               /* CAN controller 1 init, 500 kbit/s   */

  /* Transmit object does not have to be enabled but channel has to be 
     reserved for transmitting, in this example channel 1 is reserved for 
     transmission                                                            */

  CAN_rx_object (1, 2,  33, DATA_TYPE | STANDARD_TYPE); /* Enable reception  */
                                      /* of message on controller 1, channel */
                                      /* 2, data frame with standard id 33   */

  CAN_start (1);                      /* Start CAN controller 1              */

  /* The activation of loopback mode in line below is used for enabling 
     self-testing mode when CAN controller receives the message it sends so 
     program functionality can be tested without another CAN device          */
  /* COMMENT THE LINE BELOW TO ENABLE DEVICE TO PARTICIPATE IN CAN NETWORK   */
//CAN_hw_loopback (1);                /* Activate Loopback mode              */

  os_dly_wait (50);                   /* Wait 0.5 second                     */
  for (;;)  {
    Tx_val           = In_Get ();     /* Tx_val = value to send              */
    msg_send.data[0] = Tx_val;        /* Data[0] field = incrementing value  */
    CAN_send (1, &msg_send, 0x0F00);  /* Send msg_send on controller 1       */
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
