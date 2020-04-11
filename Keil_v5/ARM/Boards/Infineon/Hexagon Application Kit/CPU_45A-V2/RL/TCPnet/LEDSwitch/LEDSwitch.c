/*----------------------------------------------------------------------------
 *      RL-ARM - TCPnet
 *----------------------------------------------------------------------------
 *      Name:    LEDSWITCH.C
 *      Purpose: LED Control Server demo example
 *----------------------------------------------------------------------------
 *      This code is part of the RealView Run-Time Library.
 *      Copyright (c) 2004-2011 KEIL - An ARM Company. All rights reserved.
 *---------------------------------------------------------------------------*/

#include <RTL.h>
#include <stdio.h>
#include <string.h>
#include <XMC4500.h>                    /* XMC4500 definitions                */
#include "Serial.h"
#include "LED.h"
#include "KBD.h"


U8 socket_tcp;
U8 socket_udp;

#define PORT_NUM 1001
#define BLINKLED 0x01  // Command for blink the leds on board

/*--------------------------- init ------------------------------------------*/

static void init () {
  /* Add System initialisation code here */

  /* update SystemCoreClock value */
  SystemCoreClockUpdate();

  /* Configure LEDs */
  LED_Init();

  /* Configure buttons */
  KBD_Init();

  /* Configure serial output */
  SER_Init();

  /* Setup and enable the SysTick timer for 10ms. */
  SystemCoreClockUpdate();
  SysTick->LOAD = (SystemCoreClock / 100) - 1;
  SysTick->CTRL = 0x05;
}


/*--------------------------- timer_poll ------------------------------------*/

static void timer_poll () {
  /* System tick timer running in poll mode */
  static U32 time;

  if (SysTick->CTRL & 0x10000) {
    time++;
    if (time == 10) {
      /* Timer tick every 100 ms */
      time = 0;
      timer_tick ();
    }
  }
}


/*--------------------------- fputc -----------------------------------------*/

int fputc (int ch, FILE *f)  {
  /* Debug output to serial port. */
  if (ch == '\n')  {
    SER_PutChar (0x0D);
  }
  SER_PutChar (ch & 0xFF);
  return (ch);
}


/*--------------------------- LED_out ---------------------------------------*/

void LED_out (U32 val) {
  /* Turn LEDs ON or OFF */
  int i;

  for (i = 0; i < LED_Num(); i++) {
    if (val & (1<<i)) {
      LED_On (i);
    } else {
      LED_Off(i);
    }
  }
}


/*--------------------------- Process received data  ------------------------*/

void procrec (U8 *buf) {
  switch (buf[0]) {
    case BLINKLED:
      LED_out (buf[1]);
      break;
  }
}
/*--------------------------- UDP socket ------------------------------------*/

U16 udp_callback (U8 soc, U8 *rip, U16 rport, U8 *buf, U16 len) {
  rip  = rip;
  rport= rport;
  len  = len;

  if (soc != socket_udp) {
    /* Check if this is the socket we are connected to */
    return (0);
  }
  procrec(buf);
  return (0);
}

/*--------------------------- TCP socket ------------------------------------*/

U16 tcp_callback (U8 soc, U8 evt, U8 *ptr, U16 par) {
  /* This function is called by the TCP module on TCP event */
  /* Check the 'Net_Config.h' for possible events.          */
  par = par;

  if (soc != socket_tcp) {
    return (0);
  }

  switch (evt) {
    case TCP_EVT_DATA:
      /* TCP data frame has arrived, data is located at *par1, */
      /* data length is par2. Allocate buffer to send reply.   */
      procrec(ptr);
      break;

    case TCP_EVT_CONREQ:
      /* Remote peer requested connect, accept it */
      return (1);

    case TCP_EVT_CONNECT:
      /* The TCP socket is connected */
      return (1);
  }
  return (0);
}


/*--------------------------- main ------------------------------------------*/

int main (void) {
  /* Main Thread of the TcpNet */

  init ();
  init_TcpNet ();

  /* Initialize UDP Socket and start listening */
  socket_udp = udp_get_socket (0, UDP_OPT_SEND_CS | UDP_OPT_CHK_CS, udp_callback);
  if (socket_udp != 0) {
    udp_open (socket_udp, PORT_NUM);
  }

  /* Initialize TCP Socket and start listening */
  socket_tcp = tcp_get_socket (TCP_TYPE_SERVER, 0, 10, tcp_callback);
  if (socket_tcp != 0) {
    tcp_listen (socket_tcp, PORT_NUM);
  }

  while (1) {
    timer_poll ();
    main_TcpNet ();
  }
}


/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/
