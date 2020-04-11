/*----------------------------------------------------------------------------
 *      RL-ARM - TCPnet
 *----------------------------------------------------------------------------
 *      Name:    BSD_CLIENT.C
 *      Purpose: LED Client demo example using BSD socket interface
 *----------------------------------------------------------------------------
 *      This code is part of the RealView Run-Time Library.
 *      Copyright (c) 2004-2011 KEIL - An ARM Company. All rights reserved.
 *---------------------------------------------------------------------------*/

#include <RTL.h>
#include <stdio.h>
#include <stm32f10x.h>
#include "GLCD.h"

//-------- <<< Use Configuration Wizard in Context Menu >>> -----------------

//   <h>Remote IP Address
//   ====================
//
//     <o>IP1: Address byte 1 <0-255>
//     <i> Default: 192
#define IP1            192

//     <o>IP2: Address byte 2 <0-255>
//     <i> Default: 168
#define IP2            168

//     <o>IP3: Address byte 3 <0-255>
//     <i> Default: 0
#define IP3            0

//     <o>IP4: Address byte 4 <0-255>
//     <i> Default: 100
#define IP4            100

//   </h>

//   <o>Remote Port <1-65535>
//   <i> Do not set number of port too small,
//   <i> maybe it is already used.
//   <i> Default: 1001
#define PORT_NUM       1001

//   <o>Communication Protocol <0=> Stream (TCP) <1=> Datagram (UDP)
//   <i> Selecet a protocol for sending data.
#define PROTOCOL       1

//   <o>LED Blinking speed <1-100>
//   <i> Blinking speed = SPEED * 100ms
//   <i> Default: 2
#define SPEED          2

//------------- <<< end of configuration section >>> -----------------------

#define BLINKLED 0x01                   /* Command for blink the leds        */
#if (PROTOCOL == 0)
 #define SOCKTYPE   SOCK_STREAM
#else
 #define SOCKTYPE   SOCK_DGRAM
#endif

U64 stack[2][600/8];                    /* Stack for networking tasks        */


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
  GLCD_displayStringLn(Line3, "BSD Client example");
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


/*--------------------------- client ----------------------------------------*/

__task void client (void) {
  /* LED Control client task. */
  SOCKADDR_IN addr;
  int sock, res;
  char dbuf[4];
  U8 p2val,lshf;

  while (1) {
    sock = socket (AF_INET, SOCKTYPE, 0);

    addr.sin_port      = htons(PORT_NUM);
    addr.sin_family    = PF_INET;
    addr.sin_addr.s_b1 = IP1;
    addr.sin_addr.s_b2 = IP2;
    addr.sin_addr.s_b3 = IP3;
    addr.sin_addr.s_b4 = IP4;

    connect (sock, (SOCKADDR *)&addr, sizeof (addr));

    lshf  = 1;
    p2val = 0x01;
    while (1) {
      /* Shift the LEDs */
      LED_out (p2val);
      p2val = lshf ? (p2val << 1) : (p2val >> 1);
      if (p2val == 0x80) lshf = 0;
      if (p2val == 0x01) lshf = 1;
    
      /* Send the data to LED Server. */
      dbuf[0] = BLINKLED;
      dbuf[1] = p2val;
      res = send (sock, (char *)&dbuf, 2, 0);
      if (res < 0) {
        break;
      }
      os_dly_wait (10 * SPEED);
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
  os_tsk_create_user (client, 2, &stack[1], sizeof(stack[1]));

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
