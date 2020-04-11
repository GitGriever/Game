/*----------------------------------------------------------------------------
 * Name:    Serial.c
 * Purpose: Low Level Serial Routines
 * Note(s): possible defines select the used communication interface:
 *            __DBG_ITM   - ITM SWO interface
 *                        - UART2 interface  (default)
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2011 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include "MDR32F9x.h"                      /* MDR32F9x definitions            */
#include "Serial.h"

#ifdef __DBG_ITM
volatile int ITM_RxBuffer = ITM_RXBUFFER_EMPTY;  /*  CMSIS Debug Input        */
#endif


/*----------------------------------------------------------------------------
  Initialize UART pins, Baudrate
 *----------------------------------------------------------------------------*/
void SER_Init (void) {
#ifndef __DBG_ITM

  MDR_RST_CLK->PER_CLOCK |= (1UL << 29);     /* enable clock for PORT F       */

  /* configure PORT F0 as UART1_RX pin */
  MDR_PORTF->OE     = ((MDR_PORTF->OE     &  ~(0x00000001 << 0  ))                      );    /* port  = input   */
  MDR_PORTF->FUNC   = ((MDR_PORTF->FUNC   &  ~(0x00000003 << 0*2)) | (0x00000003 << 0*2));    /* func  = ALTER   */
  MDR_PORTF->ANALOG = ((MDR_PORTF->ANALOG &  ~(0x00000001 << 0  )) | (0x00000001 << 0  ));    /* mode  = digital */
  MDR_PORTF->PULL   = ((MDR_PORTF->PULL   &  ~(0x00010001 << 0  ))                      );
  MDR_PORTF->PD     = ((MDR_PORTF->PD     &  ~(0x00010001 << 0  ))                      );
  MDR_PORTF->PWR    = ((MDR_PORTF->PWR    &  ~(0x00000003 << 0*2)) | (0x00000003 << 0*2));    /* speed = MaxFast */
  MDR_PORTF->GFEN   = ((MDR_PORTF->GFEN   &  ~(0x00000001 << 0  ))                      );

  /* configure PORT F1 as UART1_TX pin */
  MDR_PORTF->OE     = ((MDR_PORTF->OE     &  ~(0x00000001 << 1  )) | (0x00000001 << 1  ));    /* port  = output  */
  MDR_PORTF->FUNC   = ((MDR_PORTF->FUNC   &  ~(0x00000003 << 1*2)) | (0x00000003 << 1*2));    /* func  = ALTER   */
  MDR_PORTF->ANALOG = ((MDR_PORTF->ANALOG &  ~(0x00000001 << 1  )) | (0x00000001 << 1  ));    /* mode  = digital */
  MDR_PORTF->PULL   = ((MDR_PORTF->PULL   &  ~(0x00010001 << 1  ))                      );
  MDR_PORTF->PD     = ((MDR_PORTF->PD     &  ~(0x00010001 << 1  ))                      );
  MDR_PORTF->PWR    = ((MDR_PORTF->PWR    &  ~(0x00000003 << 1*2)) | (0x00000003 << 1*2));    /* speed = MaxFast */
  MDR_PORTF->GFEN   = ((MDR_PORTF->GFEN   &  ~(0x00000001 << 1  ))                      );

  /* Configure USART0 for 115200 baud. */
  MDR_RST_CLK->PER_CLOCK  |= (1UL <<  7);     /* enable clock for UART 2       */
  MDR_RST_CLK->UART_CLOCK |= (1UL << 25);     /* enable UART2 clock            */

  MDR_UART2->IBRD =  4;                       /* 115200 @ 8MHz ?*/
  MDR_UART2->FBRD = 21;

  MDR_UART2->LCR_H  = ((3 << 5) |             /* 8 bit                         */
                       (0 << 3) |             /* 1 stop bit                    */
                       (0 << 1)  );           /* no Parity                     */

  MDR_UART2->CR     = ((1 << 8) |             /* TX enable                     */
                       (1 << 9)  );           /* RX enable                     */

  MDR_UART2->CR    |=  (1 << 0);              /* UART enable                   */

#endif
}


/*----------------------------------------------------------------------------
  Write character to Serial Port
 *----------------------------------------------------------------------------*/
int SER_PutChar (int c) {

#ifdef __DBG_ITM
  ITM_SendChar(c);
#else
  while (!(MDR_UART2->FR & (1UL << 7)));
  MDR_UART2->DR = (c & 0xFF);
#endif
  return (c);
}


/*----------------------------------------------------------------------------
  Read character from Serial Port   (blocking read)
 *----------------------------------------------------------------------------*/
int SER_GetChar (void) {

#ifdef __DBG_ITM
  while (ITM_CheckChar() != 1) __NOP();
  return (ITM_ReceiveChar());
#else
  while (!(MDR_UART2->FR & (1UL << 6)));
  return ((MDR_UART2->DR & 0xFF));
#endif
}
