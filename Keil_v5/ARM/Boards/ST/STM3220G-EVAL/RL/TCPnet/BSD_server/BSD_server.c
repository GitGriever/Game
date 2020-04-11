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
#include <stm32f2xx.h>
#include "GLCD.h"

#define PORT_NUM 1001
#define BLINKLED 0x01                   /* Command for blink the leds        */

U64 stack[3][600/8];                    /* Stack for networking tasks        */


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

  /* Configure Button pins (PG15, PC13, PA0) as floating inputs */
  GPIOG->MODER  &= ~(3UL << 2 * 15);
  GPIOC->MODER  &= ~(3UL << 2 * 13);
  GPIOA->MODER  &= ~(3UL << 2 *  0);

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
}


/*--------------------------- init_display ----------------------------------*/

static void init_display () {
  /* LCD Module.2x16 init*/

  GLCD_Init ();
  GLCD_Clear (Blue);
  GLCD_SetBackColor (Blue);
  GLCD_SetTextColor (White);
  GLCD_DisplayString (2, 1, 1, "     RL-ARM");
  GLCD_DisplayString (3, 1, 1, "BSD Server example");
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
