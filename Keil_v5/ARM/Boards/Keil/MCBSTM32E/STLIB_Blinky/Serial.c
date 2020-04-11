/*----------------------------------------------------------------------------
 * Name:    Serial.c
 * Purpose: Low Level Serial Routines
 * Note(s): possible defines select the used communication interface:
 *            __DBG_ITM   - ITM SWO interface
 *                        - USART2 interface  (default)
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2008-2011 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include "STM32F10x.h"
#include "Serial.h"

#ifdef __DBG_ITM
volatile int ITM_RxBuffer = ITM_RXBUFFER_EMPTY;  /*  CMSIS Debug Input        */
#endif


/*----------------------------------------------------------------------------
  Initialize UART pins, Baudrate
 *----------------------------------------------------------------------------*/
void SER_Init (void) {
#ifndef __DBG_ITM
  GPIO_InitTypeDef  GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;

  /* Enable GPIOA clock                                                       */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

  /* Configure USART2 RTS (PA1), USART2 Tx (PA2) alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* Configure USART2 CTS (PA0) and USART2 Rx (PA3) as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* USART2 configured as follow:
        - BaudRate = 115200 baud  
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control disabled
        - Receive and transmit enabled
  */

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART2, &USART_InitStructure);

  USART_Cmd(USART2, ENABLE);
#endif
}


/*----------------------------------------------------------------------------
  Write character to Serial Port
 *----------------------------------------------------------------------------*/
int SER_PutChar (int c) {

#ifdef __DBG_ITM
  ITM_SendChar(c);
#else
  USART_SendData(USART2, (unsigned char) c);
  while (!(USART2->SR & USART_FLAG_TXE));
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
  while (!(USART2->SR & USART_FLAG_RXNE));
  return (USART_ReceiveData(USART2));
#endif
}
