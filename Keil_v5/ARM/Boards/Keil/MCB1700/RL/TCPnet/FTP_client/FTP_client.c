/*----------------------------------------------------------------------------
 *      RL-ARM - TCPnet
 *----------------------------------------------------------------------------
 *      Name:    FTP_CLIENT.C 
 *      Purpose: FTP Client demo example
 *----------------------------------------------------------------------------
 *      This code is part of the RealView Run-Time Library.
 *      Copyright (c) 2004-2012 KEIL - An ARM Company. All rights reserved.
 *---------------------------------------------------------------------------*/

#include <stdio.h>
#include <RTL.h>
#include <LPC17xx.h>                    /* LPC17xx definitions               */
#include "GLCD.h"

/* Enter your valid FTP Server IP address here. */
U8 srv_ip[4] = { 192,168,0,253 };

BOOL tick;
BOOL tick2;
BOOL active;

/*--------------------------- init ------------------------------------------*/

static void init () {
  /* Add System initialisation code here */

  /* Configure the GPIO for LEDs. */
  LPC_GPIO1->FIODIR   |= 0xB0000000;
  LPC_GPIO2->FIODIR   |= 0x0000007C;

  /* Configure UART1 for 115200 baud. */
  LPC_PINCON->PINSEL4 &= 0xFFFFFFF0;
  LPC_PINCON->PINSEL4 |= 0x0000000A;
  LPC_UART1->LCR = 0x83;
  LPC_UART1->DLL = 9;                 /* 115200 Baud Rate @ 25.0 MHZ PCLK    */
  LPC_UART1->FDR = 0x21;              /* FR 1,507, DIVADDVAL = 1, MULVAL = 2 */
  LPC_UART1->DLM = 0;
  LPC_UART1->LCR = 0x03;

  /* Setup and enable the SysTick timer for 100ms. */
  SysTick->LOAD = (SystemCoreClock / 10) - 1;
  SysTick->CTRL = 0x05;
}


/*--------------------------- LED_out ---------------------------------------*/

void LED_out (U32 val) {
  const U8 led_pos[8] = { 28, 29, 31, 2, 3, 4, 5, 6 };
  U32 i,mask;

  for (i = 0; i < 8; i++) {
    mask = 1 << led_pos[i];
    if (val & (1<<i)) {
      if (i < 3) LPC_GPIO1->FIOSET = mask;
      else       LPC_GPIO2->FIOSET = mask;
    }
    else {
      if (i < 3) LPC_GPIO1->FIOCLR = mask;
      else       LPC_GPIO2->FIOCLR = mask;
    }
  }

}


/*--------------------------- init_display ----------------------------------*/

static void init_display () {
  /* LCD Module init*/

  GLCD_Init ();
  GLCD_Clear (White);
  GLCD_SetTextColor (Blue);
  GLCD_DisplayString (1, 4, 1, "   RL-ARM");
  GLCD_DisplayString (2, 4, 1, "FTPC example");
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
    while (!(LPC_UART1->LSR & 0x20));
    LPC_UART1->THR = 0x0D;
  }
  while (!(LPC_UART1->LSR & 0x20));
  LPC_UART1->THR = (ch & 0xFF);
  return (ch);
}


/*--------------------------- getkey ----------------------------------------*/

int getkey (void) {
  /* A dummy function for 'retarget.c' */
  return (0);
}


/*--------------------------- blink_led -------------------------------------*/

static void blink_led () {
  /* Blink the LEDs on MCB2300 board */
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
