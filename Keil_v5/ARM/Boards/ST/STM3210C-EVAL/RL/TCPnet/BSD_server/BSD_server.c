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
#include <stm32f10x.h>
#include "GLCD.h"

#define PORT_NUM 1001
#define BLINKLED 0x01                   /* Command for blink the leds        */

U64 stack[3][600/8];                    /* Stack for networking tasks        */


/*--------------------------- init ------------------------------------------*/

static void init () {
  /* Add System initialisation code here */

  /* Set the clocks. */
  SystemCoreClockUpdate();
  RCC->APB2ENR |= 0x00000261;

  /* Configure the GPIO for LEDs. */
  GPIOD->CRL &= 0xFFF00FFF;
  GPIOD->CRL |= 0x00033000;
  GPIOD->CRH &= 0xFF0FFFFF;
  GPIOD->CRH |= 0x00300000;
  GPIOE->CRH &= 0xF0FFFFFF;
  GPIOE->CRH |= 0x03000000;

  /* Configure UART2 for 115200 baud. */
  AFIO->MAPR |= 0x00000008;
  GPIOD->CRL &= 0xF00FFFFF;
  GPIOD->CRL |= 0x04B00000;

  RCC->APB1ENR |= 0x00020000;
  USART2->BRR = 0x0135;
  USART2->CR3 = 0x0000;
  USART2->CR2 = 0x0000;
  USART2->CR1 = 0x200C;
}


/*--------------------------- init_display ----------------------------------*/

static void init_display () {
  /* LCD Module init */

  GLCD_init();
  GLCD_clear(White);
  GLCD_setTextColor(Blue);
  GLCD_displayStringLn(Line2, "       RL-ARM");
  GLCD_displayStringLn(Line3, " BSD Server example");
}


/*--------------------------- fputc -----------------------------------------*/

int fputc (int ch, FILE *f)  {
  /* Debug output to serial port. */

  if (ch == '\n')  {
    while (!(USART2->SR & 0x0080));
    USART2->DR = 0x0D;
  }
  while (!(USART2->SR & 0x0080));
  USART2->DR = (ch & 0xFF);
  return (ch);
}


/*--------------------------- LED_out ---------------------------------------*/

void LED_out (U32 val) {
  U32 rv;

  rv = 0;
  if (val & 0x01) rv |= 0x00004000;
  GPIOE->BSRR = rv;
  GPIOE->BRR  = rv ^ 0x00004000;

  rv = 0;
  if (val & 0x02) rv |= 0x00002000;
  if (val & 0x04) rv |= 0x00000008;
  if (val & 0x08) rv |= 0x00000010;
  GPIOD->BSRR = rv;
  GPIOD->BRR  = rv ^ 0x0002018;
}


/*--------------------------- Process received data  ------------------------*/

void procrec (U8 *buf) {
  switch (buf[0]) {
    case BLINKLED:
      LED_out (buf[1]);
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
