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
#include <a2fxxxm3.h>

#define PORT_NUM 1001
#define BLINKLED 0x01                   /* Command for blink the leds        */

U64 stack[3][600/8];                    /* Stack for networking tasks        */


/*--------------------------- init ------------------------------------------*/

static void init () {
  /* Add System initialisation code here */

  /* Disable the watchdog. */
  WATCHDOG->WDOGENABLE = 0x4C6E55FA;
  SYSREG->SOFT_RST_CR |= 0x00080000;

  /* Configure the GPIO for LEDs. */
  GPIO->GPIO_0_CFG = 5;
  GPIO->GPIO_1_CFG = 5;
  GPIO->GPIO_2_CFG = 5;
  GPIO->GPIO_3_CFG = 5;
  GPIO->GPIO_4_CFG = 5;
  GPIO->GPIO_5_CFG = 5;
  GPIO->GPIO_6_CFG = 5;
  GPIO->GPIO_7_CFG = 5;
  GPIO->GPIO_OUT  |= 0xFF;

  /* Configure UART0 for 115200 baud. */
  SYSREG->SOFT_RST_CR &= ~0x00000080;
  UART0->LCR = 0x83;                         /* 8 bits, no Parity, 1 Stop bit*/
  UART0->DLR = 54;                           /* for 100MHz PCLK0 clock       */
  UART0->DMR = 0;
  UART0->LCR = 0x03;                         /* DLAB = 0                     */
}


/*--------------------------- fputc -----------------------------------------*/

int fputc (int ch, FILE *f)  {
  /* Debug output to serial port. */

  if (ch == '\n')  {
    while (!(UART0->LSR & 0x20));
    UART0->THR = '\r';                      /* output CR                    */
  }
  while (!(UART0->LSR & 0x20));
  return (UART0->THR = ch);
}


/*--------------------------- Process received data  ------------------------*/

void procrec (U8 *buf) {
  switch (buf[0]) {
    case BLINKLED:
      GPIO->GPIO_OUT = (GPIO->GPIO_OUT & ~0xFF) | ~buf[1];
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
