/*----------------------------------------------------------------------------
 *      RL-ARM - TCPnet
 *----------------------------------------------------------------------------
 *      Name:    BSD_SERVER.C
 *      Purpose: LED Server demo example using BSD socket interface
 *----------------------------------------------------------------------------
 *      This code is part of the RealView Run-Time Library.
 *      Copyright (c) 2004-2011 KEIL - An ARM Company. All rights reserved.
 *---------------------------------------------------------------------------*/

#include <RTL.h>
#include <stdio.h>
#include <LM3Sxxxx.H>
#include "..\..\..\rit128x96x4.h"

#define PORT_NUM 1001
#define BLINKLED 0x01                   /* Command for blink the leds        */

U64 stack[3][600/8];                    /* Stack for networking tasks        */


/*--------------------------- init ------------------------------------------*/

static void init () {
  /* Add System initialisation code here */

  /* Set the clocking to run from the PLL at 50 MHz */
  SysCtlClockSet (SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL |
                  SYSCTL_OSC_MAIN | SYSCTL_XTAL_8MHZ);

  /* Configure the GPIO for the LED. */
  SysCtlPeripheralEnable (SYSCTL_PERIPH_GPIOF);
  GPIOPadConfigSet (GPIO_PORTF_BASE, GPIO_PIN_0, GPIO_STRENGTH_2MA,
                    GPIO_PIN_TYPE_STD);
  GPIODirModeSet (GPIO_PORTF_BASE, GPIO_PIN_0, GPIO_DIR_MODE_OUT);

  /* Configure UART0 for 115200 baud. */
  SysCtlPeripheralEnable (SYSCTL_PERIPH_GPIOA);
  SysCtlPeripheralEnable (SYSCTL_PERIPH_UART0);
  GPIOPinTypeUART (GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
  UARTConfigSet(UART0_BASE, 115200, (UART_CONFIG_WLEN_8 |
                                     UART_CONFIG_STOP_ONE |
                                     UART_CONFIG_PAR_NONE));
  UARTEnable(UART0_BASE);
}


/*--------------------------- init_display ----------------------------------*/

static void init_display () {
  /* OLED Module init */

  RIT128x96x4Init(1000000);
  RIT128x96x4Clear ();
  RIT128x96x4StringDraw ("    RL-ARM      ", 20, 20, 11);
  RIT128x96x4StringDraw ("BSD server exam.", 20, 35, 11);
}


/*--------------------------- fputc -----------------------------------------*/

int fputc (int ch, FILE *f)  {
  /* Debug output to serial port. */

  if (ch == '\n')  {
    UARTCharPut (UART0_BASE, '\r');          /* output CR                    */
  }
  UARTCharPut (UART0_BASE, ch);
  return (ch);
}


/*--------------------------- Process received data  ------------------------*/

void procrec (U8 *buf) {
  switch (buf[0]) {
    case BLINKLED:
      GPIOPinWrite (GPIO_PORTF_BASE, GPIO_PIN_0, buf[1] & 1);
      break;
  }
}


/*--------------------------- server ----------------------------------------*/

__task void server (void *argv) {
  /* Server task runs in 2 instances. */
  SOCKADDR_IN addr;
  int sock, sd, res;
  int type = (int)argv;
  char dbuf[4];

  while (1) {
    sock = socket (AF_INET, type, 0);

    addr.sin_port        = htons(PORT_NUM);
    addr.sin_family      = PF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;

    bind (sock, (SOCKADDR *)&addr, sizeof(addr));

    if (type == SOCK_STREAM) {
      listen (sock, 1);
      sd = accept (sock, NULL, NULL);
      closesocket (sock);
      sock = sd;
    }

    while (1) {
      res = recv (sock, dbuf, sizeof (dbuf), 0);
      if (res <= 0) {
        break;
      }
      procrec ((U8 *)dbuf);
    }
    closesocket (sock);
  }
}


/*--------------------------- timer_task ------------------------------------*/

__task void timer_task (void) {
  /* TCPnet timebase task. */

  os_itv_set (10);
  while (1) {
    /* 100 ms interval */
    timer_tick ();
    os_itv_wait ();
  }
}


/*--------------------------- net_task --------------------------------------*/

__task void net_task (void) {
  /* TCPnet engine task */

  /* Init peripherals and TCPnet stack. */
  init ();
  init_display ();
  init_TcpNet ();

  /* Create networking tasks. */
  os_tsk_create (timer_task, 10);

  /* Server task is created in 2 instances */
  os_tsk_create_user_ex (server, 2, &stack[1], sizeof(stack[1]), (void *)SOCK_STREAM);
  os_tsk_create_user_ex (server, 2, &stack[2], sizeof(stack[2]), (void *)SOCK_DGRAM);

  while (1) {
    main_TcpNet ();
    os_tsk_pass ();
  }
}

/*--------------------------- main ------------------------------------------*/

int main (void) {

  os_sys_init_user (net_task, 1, &stack[0], sizeof(stack[0]));
  for (;;);
}


/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/
