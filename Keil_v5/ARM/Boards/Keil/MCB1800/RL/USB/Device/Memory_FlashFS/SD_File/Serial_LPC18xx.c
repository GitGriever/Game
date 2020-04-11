/*-----------------------------------------------------------------------------
 * Name:    Serial.c
 * Purpose: Serial Input Output for NXP LPC18xx
 * Note(s): Possible defines to select the used communication interface:
 *          __DBG_ITM   - ITM SWO interface (USART is used by default)
 *-----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2005-2013 KEIL - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <LPC18xx.h>                    /* LPC18xx Definitions                */
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
  /* Enable GPIO register interface clock                                     */
  LPC_CCU1->CLK_M3_GPIO_CFG     |= 1;
  while (!(LPC_CCU1->CLK_M3_GPIO_STAT   & 1));

  /* Enable USART3 peripheral clock                                           */
  LPC_CCU2->CLK_APB2_USART3_CFG |= 1;
  while (!(LPC_CCU2->CLK_APB2_USART3_STAT & 1));

  /* Enable USART3 register interface clock                                   */
  LPC_CCU1->CLK_M3_USART3_CFG   |= 1;
  while (!(LPC_CCU1->CLK_M3_USART3_STAT & 1));

  /* Init GPIO pins                                                           */
  LPC_SCU->SFSP2_3 =  (1 << 6) |        /* Input buffer enabled               */
                      (1 << 4) |        /* Pull-up disabled                   */
                      (2 << 0) ;        /* Pin P2_3 used as U3_TXD            */

  LPC_SCU->SFSP2_4 =  (1 << 6) |        /* Input buffer enabled               */
                      (1 << 4) |        /* Pull-up disabled                   */
                      (2 << 0) ;        /* Pin P2_3 used as U3_RXD            */

  /* Init USART3                                                              */
  LPC_USART3->LCR    = 0x83;            /* 8 bits, no Parity, 1 Stop bit      */
  LPC_USART3->DLL    = 0x06;            /* 115200 Baudrate @ 12 MHz IRC       */
  LPC_USART3->DLM    = 0x00;
  LPC_USART3->FDR    = 0xC1;
  LPC_USART3->LCR    = 0x03;            /* DLAB = 0                           */
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
  while (!(LPC_USART3->LSR & 0x20));
  LPC_USART3->THR = ch;
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
  if (LPC_USART3->LSR & 0x01) {
    return (LPC_USART3->RBR);
  }
#endif
  return (-1);
}

/*-----------------------------------------------------------------------------
 * End of file
 *----------------------------------------------------------------------------*/
