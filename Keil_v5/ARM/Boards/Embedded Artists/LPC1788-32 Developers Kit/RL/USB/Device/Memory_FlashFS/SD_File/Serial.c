/*-----------------------------------------------------------------------------
 *      RL-ARM
 *-----------------------------------------------------------------------------
 *      Name:    Serial.c
 *      Purpose: Serial Input Output for NXP NXP LPC178x/7x
 *      Note(s): Possible defines to select the used communication interface:
 *               __DBG_ITM   - ITM SWO interface (USART is used by default)
 *-----------------------------------------------------------------------------
 *      This code is part of the RealView Run-Time Library.
 *      Copyright (c) 2004-2013 KEIL - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <LPC177x_8x.h>                 /* LPC177x/8x definitions             */
#include "Serial.h"

#ifdef __DBG_ITM
volatile int32_t ITM_RxBuffer;
#endif

/*-----------------------------------------------------------------------------
 *       SER_Init:  Initialize Serial Interface
 *----------------------------------------------------------------------------*/
void SER_Init (void) {
#ifdef __DBG_ITM
  ITM_RxBuffer = ITM_RXBUFFER_EMPTY;

#else
  /* Power Up the UART2 controller. */
  LPC_SC->PCONP |=  (1 << 24);
  
  /* Configure UART2 pins */
  LPC_IOCON->P0_10 = (1 << 9) | 1;
  LPC_IOCON->P0_11 = (1 << 9) | 1; 

  /* Init UART2                                                               */
  LPC_UART2->LCR   = 0x83;              /* 8 bits, no parity, 1 stop bit      */
  LPC_UART2->DLL   = 24;                /* 115200 Baudrate @ 60 MHz PCLK      */
  LPC_UART2->DLM   = 0;
  LPC_UART2->FDR   = 229;
  LPC_UART2->LCR   = 0x03;              /* DLAB = 0                           */
#endif
}


/*-----------------------------------------------------------------------------
 *       SER_PutChar:  Write a character to the Serial Port
 *----------------------------------------------------------------------------*/
int32_t SER_PutChar (int32_t ch) {
#ifdef __DBG_ITM
  int i;
  ITM_SendChar (ch & 0xFF);
  for (i = 10000; i; i--);
#else
  while (!(LPC_UART2->LSR & 0x20));
  LPC_UART2->THR = ch;
#endif  

  return (ch);
}


/*-----------------------------------------------------------------------------
 *       SER_GetChar:  Read a character from the Serial Port
 *----------------------------------------------------------------------------*/
int32_t SER_GetChar (void) {
#ifdef __DBG_ITM
  if (ITM_CheckChar())
    return ITM_ReceiveChar();
#else
  if (LPC_UART2->LSR & 0x01) {
    return (LPC_UART2->RBR);
  }
#endif
  return (-1);
}

/*-----------------------------------------------------------------------------
 * End of file
 *----------------------------------------------------------------------------*/
