/*----------------------------------------------------------------------------
 *      RL-ARM - TCPnet
 *----------------------------------------------------------------------------
 *      Name:    CLIENT.C
 *      Purpose: LED Control Client demo example
 *----------------------------------------------------------------------------
 *      This code is part of the RealView Run-Time Library.
 *      Copyright (c) 2004-2011 KEIL - An ARM Company. All rights reserved.
 *---------------------------------------------------------------------------*/

#include <RTL.h>
#include <stdio.h>
#include <stm32f2xx.h>
#include "GLCD.h"
#include <string.h>

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

//   <o>Communication Protocol <0=> TCP <1=> UDP
//   <i> Selecet a protocol for sending data.
#define PROTOCOL       1

//   <o>LED Blinking speed <1-100>
//   <i> Blinking speed = SPEED * 100ms
//   <i> Default: 2
#define SPEED          2


//------------- <<< end of configuration section >>> -----------------------


#define BLINKLED 0x01  /* Command for blink the leds on board */
#define SENDLEN  2     /* Number of bytes to send */
#define TCP      0
#define UDP      1

BOOL tick;
U8 socket_tcp;
U8 socket_udp;
U8 Rem_IP[4] = {IP1,IP2,IP3,IP4};


/*--------------------------- init ------------------------------------------*/

static void init () {
  /* Add System initialisation code here */

  /* Set the clocks. */
  RCC->APB1ENR |= (1UL << 19) ;    /* Enable UART 4 clock */
  RCC->AHB1ENR |= (1UL <<  0) |    /* Enable GPIOA clock  */
                  (1UL <<  2) |    /* Enable GPIOC clock  */
                  (1UL <<  6) |    /* Enable GPIOG clock  */
                  (1UL <<  8) ;    /* Enable GPIOI clock  */  

  /* Configure LED (PG6, PG8, PI9, PC7) pins as push-pull outputs */
  GPIOG->MODER  &= ~((3UL << 2 * 6) | (3UL << 2 * 8));
  GPIOG->MODER  |=   (1UL << 2 * 6) | (1UL << 2 * 8);  
  GPIOG->OTYPER &= ~((1UL <<     6) | (1UL <<     8));

  GPIOI->MODER  &= ~(3UL << 2 * 9);
  GPIOI->MODER  |=  (1UL << 2 * 9);
  GPIOI->OTYPER &= ~(1UL <<     9);

  GPIOC->MODER  &= ~(3UL << 2 * 7);
  GPIOC->MODER  |=  (1UL << 2 * 7);
  GPIOC->OTYPER &= ~(1UL <<     7);

  /* Configure UART4 (PC10, PC11) for 115200 baud @ 30MHz */  
  UART4->BRR = (16 << 4) | 4;
  UART4->CR3 = 0x0000;    
  UART4->CR2 = 0x0000;
  UART4->CR1 = 0x200C;
  
  GPIOC->MODER  &= ~((3UL << 2 * 11) | (3UL << 2 * 10));
  GPIOC->MODER  |=   (2UL << 2 * 11) | (2UL << 2 * 10);  
  GPIOC->OTYPER &= ~((1UL <<     11) | (1UL <<     10));

  GPIOC->AFR[1] &= ~0x0000FF00;
  GPIOC->AFR[1] |=  0x00008800;

  /* Setup and enable the SysTick timer for 100ms. */
  SysTick->LOAD = (SystemCoreClock / 10) - 1;
  SysTick->CTRL = 0x05;
}


/*--------------------------- init_display ----------------------------------*/

static void init_display () {
  /* LCD Module init */

  GLCD_Init ();
  GLCD_Clear (Blue);
  GLCD_SetBackColor (Blue);
  GLCD_SetTextColor (White);
  GLCD_DisplayString (3, 3, 1, "    RL-ARM");
  GLCD_DisplayString (4, 3, 1, "Client example");
}


/*--------------------------- LED_out ---------------------------------------*/

void LED_out (U32 val) {
  volatile unsigned short *BSR;

  /* LD1 on/off */
   BSR  = (val & 1) ? &GPIOG->BSRRL : &GPIOG->BSRRH;
  *BSR |= (1 << 6);

  /* LD2 on/off */
   BSR  = (val & 2) ? &GPIOG->BSRRL : &GPIOG->BSRRH;
  *BSR |= (1 << 8);

  /* LD3 on/off */
   BSR  = (val & 4) ? &GPIOI->BSRRL : &GPIOI->BSRRH;
  *BSR |= (1 << 9);

  /* LD4 on/off */
   BSR  = (val & 8) ? &GPIOC->BSRRL : &GPIOC->BSRRH;
  *BSR |= (1 << 7);
}


/*--------------------------- timer_poll ------------------------------------*/

static void timer_poll () {
  /* System tick timer running in poll mode */

  if (SysTick->CTRL & 0x10000) {
    /* Timer tick every 100 ms */
    timer_tick ();
    tick = __TRUE;
  }
}


/*--------------------------- fputc -----------------------------------------*/

int fputc (int ch, FILE *f)  {
  /* Debug output to serial port. */

  if (ch == '\n')  {
    while (!(UART4->SR & 0x0080));
    UART4->DR = 0x0D;
  }
  while (!(UART4->SR & 0x0080));
  UART4->DR = (ch & 0xFF);
  return (ch);
}


/*--------------------------- UDP socket ------------------------------------*/

U16 udp_callback (U8 soc, U8 *rip, U16 rport, U8 *buf, U16 len) {
  /* This function is called by the UDP module when UDP packet is received. */

  /* Make a reference to suppress compiler warnings. */
  buf  = buf;
  rip  = rip;
  rport= rport;
  len  = len;
  soc  = soc;
  return (0);
}


/*--------------------------- TCP socket ------------------------------------*/

U16 tcp_callback (U8 soc, U8 evt, U8 *ptr, U16 par) {
  /* This function is called by the TCP module on TCP event */
  /* Check the Net_Config.h for possible events.            */

  /* Make a reference to suppress compiler warnings. */
  soc = soc;
  par = par;
  evt = evt;
  ptr = ptr;
  return (0);
}


/*--------------------------- TCP send --------------------------------------*/

void send_data (U8 p2val) {
  U8 *sendbuf;
  U8 p2;

  /* UDP */
  if (socket_udp != 0) {
    /* Start Connection */
    sendbuf = udp_get_buf (SENDLEN);
    sendbuf[0] = BLINKLED;
    sendbuf[1] = p2val;
    udp_send (socket_udp, Rem_IP, 1001, sendbuf, SENDLEN);
  }

  /* TCP */
  if (socket_tcp != 0) {
    /* Start Connection */
    p2 = p2val;
    switch (tcp_get_state(socket_tcp)) {
      case TCP_STATE_FREE:
      case TCP_STATE_CLOSED:
        tcp_connect (socket_tcp, Rem_IP, PORT_NUM, 0);
        break;
      case TCP_STATE_CONNECT:
        if (tcp_check_send (socket_tcp) == __TRUE) {
          sendbuf = tcp_get_buf(SENDLEN);
          sendbuf[0] = BLINKLED;
          sendbuf[1] = p2;
          tcp_send (socket_tcp, sendbuf, SENDLEN);
        }
        break;
    }
  }
}


/*--------------------------- main ------------------------------------------*/

int main (void) {
  /* Main Thread of the TcpNet */
  U8 p2val, cnt, lshf;
  U8 protocol;

  init ();
  init_display ();
  init_TcpNet ();

  protocol = PROTOCOL;
  switch (protocol) {
    case TCP:
      socket_tcp = tcp_get_socket (TCP_TYPE_CLIENT, 0, 10, tcp_callback);
      break;
    case UDP:
      socket_udp = udp_get_socket (0, UDP_OPT_SEND_CS | UDP_OPT_CHK_CS, udp_callback);
      if (socket_udp != 0) {
        udp_open (socket_udp, PORT_NUM);
      }
      break;
  }

  p2val = 1;
  cnt   = 0;
  lshf  = 1;  /* left shift */
  while (1) {
    timer_poll ();
    main_TcpNet ();
    if (tick == __TRUE) {
      if (++cnt == SPEED) {
        LED_out (p2val);
        send_data(p2val);
        p2val = lshf ? (p2val << 1) : (p2val >> 1);
        if (p2val == 0x80) lshf = 0;
        if (p2val == 0x01) lshf = 1;
        cnt = 0;
      }
      tick = __FALSE;
    }
  }
}


/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/
