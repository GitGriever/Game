/*----------------------------------------------------------------------------
 *      RL-ARM - TCPnet
 *----------------------------------------------------------------------------
 *      Name:    FTP_DEMO.C 
 *      Purpose: FTP Server demo example
 *----------------------------------------------------------------------------
 *      This code is part of the RealView Run-Time Library.
 *      Copyright (c) 2004-2011 KEIL - An ARM Company. All rights reserved.
 *---------------------------------------------------------------------------*/

#include <stdio.h>
#include <RTL.h>
#include <stm32f10x_cl.h>
#include "GLCD.h"

/* Enter your valid FTP Server IP address here. */
U8 srv_ip[4] = { 192,168,0,253 };

BOOL tick;
BOOL tick2;
BOOL active;

/*--------------------------- init ------------------------------------------*/

static void init () {
  /* Add System initialisation code here */

  /* Set the clocks. */
  SystemInit();
  RCC->APB2ENR |= 0x00000261;

  /* Configure the GPIO for LEDs. */
  GPIOE->CRH  = 0x33333333;

  /* Configure UART2 for 115200 baud. */
  AFIO->MAPR |= 0x00000008;
  GPIOD->CRL &= 0xF00FFFFF;
  GPIOD->CRL |= 0x04B00000;

  RCC->APB1ENR |= 0x00020000;
  USART2->BRR = 0x0135;
  USART2->CR3 = 0x0000;
  USART2->CR2 = 0x0000;
  USART2->CR1 = 0x200C;

  /* Setup and enable the SysTick timer for 100ms. */
  SysTick->LOAD = (SystemFrequency / 10) - 1;
  SysTick->CTRL = 0x05;
}


/*--------------------------- init_display ----------------------------------*/

static void init_display () {
  /* LCD Module init */

  GLCD_Init ();
  GLCD_Clear (White);
  GLCD_SetTextColor (Blue);
  GLCD_DisplayString (2, 3, 1, "    RL-ARM");
  GLCD_DisplayString (3, 3, 1, "  FTPC example");
}


/*--------------------------- LED_out ---------------------------------------*/

void LED_out (U32 val) {
  val <<= 8;
  GPIOE->BSRR = val;
  GPIOE->BRR  = val ^ 0xFF00;
}


/*--------------------------- timer_poll ------------------------------------*/

static void timer_poll () {
  /* System tick timer running in poll mode */

  if (SysTick->CTRL & 0x10000) {
    /* Timer tick every 100 ms */
    timer_tick ();
    tick  = __TRUE;
    tick2 = __TRUE;
  }
}


/*--------------------------- sendchar --------------------------------------*/

int sendchar (int ch)  {
  /* Debug output to serial port. */

  if (ch == '\n')  {
    while (!(USART2->SR & 0x0080));
    USART2->DR = 0x0D;
  }
  while (!(USART2->SR & 0x0080));
  USART2->DR = (ch & 0xFF);
  return (ch);
}


/*--------------------------- getkey ----------------------------------------*/

int getkey (void) {
  /* A dummy function for 'retarget.c' */
  return (0);
}


/*--------------------------- blink_led -------------------------------------*/

static void blink_led () {
  /* Blink the LEDs on an eval board */
  const U8 led_val[16] = { 0x48,0x88,0x84,0x44,0x42,0x22,0x21,0x11,
                           0x12,0x0A,0x0C,0x14,0x18,0x28,0x30,0x50 };
  static U32 cnt;

  if (tick == __TRUE) {
    /* Every 100 ms */
    tick = __FALSE;
    LED_out (led_val[cnt]);
    if (++cnt >= sizeof(led_val)) {
      cnt = 0;
    }
  }
}


/*---------------------------------------------------------------------------*/

void ftpc_notify (U8 event) {
  /* Result notification function. */

  active = __FALSE;
  switch (event) {
    case FTPC_EVT_SUCCESS:
      printf ("Command successful\n");
      break;

    case FTPC_EVT_TIMEOUT:
      printf ("Failed, timeout expired\n");
      break;

    case FTPC_EVT_LOGINFAIL:
      printf ("Failed, username/password invalid\n");
      break;

    case FTPC_EVT_NOACCESS:
      printf ("Failed, operation not allowed\n");
      break;

    case FTPC_EVT_NOTFOUND:
      printf ("Failed, file or path not found\n");
      break;

    case FTPC_EVT_NOPATH:
      printf ("Failed, working directory not found\n");
      break;

    case FTPC_EVT_ERRLOCAL:
      printf ("Failed, local file open error\n");
      break;

    case FTPC_EVT_ERROR:
      printf ("Failed, unspecified protocol error\n");
      break;
  }
}


/*---------------------------------------------------------------------------*/


int main (void) {
  /* Main Thread of the TcpNet */
  int delay = 0;

  init ();
  init_display ();
  init_TcpNet ();
  finit (NULL);

  printf ("FTP Client demo\n");
  active = __FALSE;
  while (1) {
    timer_poll ();
    main_TcpNet ();
    blink_led ();
    if (tick2 == __FALSE) {
      continue;
    }
    tick2 = __FALSE;
    if (active == __TRUE) {
      continue;
    }
    delay++;
    if (delay == 50) {
      /* After 5 seconds connect to FTP server. */
      printf("\nSend a file to server...\n");
      ftpc_connect (srv_ip, 0, FTPC_CMD_PUT, ftpc_notify);
      active = __TRUE;
      continue;
    }
    if (delay == 52) {
      /* Make a directory on FTP server. */
      printf ("\nMake a directory on server...\n");
      ftpc_connect (srv_ip, 0, FTPC_CMD_MKDIR, ftpc_notify);
      active = __TRUE;
      continue;
    }

    if (delay == 54) {
      /* List directory content on FTP server. */
      printf ("\nListing directory...\n");
      ftpc_connect (srv_ip, 0, FTPC_CMD_LIST, ftpc_notify);
      active = __TRUE;
      continue;
    }
  }
}

/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/
