/*----------------------------------------------------------------------------
 * Name:    Serial.c
 * Purpose: Low Level Serial Routines
 * Note(s): possible defines select the used communication interface:
 *            __DBG_ITM   - ITM SWO interface
 *                        - USART1  (default)
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2012 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include "em_cmu.h"
#include "em_gpio.h"
#include "em_usart.h"
#include "Serial.h"

#ifdef __DBG_ITM
volatile int ITM_RxBuffer = ITM_RXBUFFER_EMPTY;  /*  CMSIS Debug Input        */
#endif


/*----------------------------------------------------------------------------
  Initialize UART pins, Baudrate
 *----------------------------------------------------------------------------*/
void SER_Init (void) {
#ifndef __DBG_ITM
  USART_InitAsync_TypeDef init = USART_INITASYNC_DEFAULT;

  CMU_ClockEnable(cmuClock_HFPER, true);
  CMU_ClockEnable(cmuClock_USART1, true);

  /* Configure GPIO pins */
  CMU_ClockEnable(cmuClock_GPIO, true);
  /* Use default location 0: TX - Pin C0, RX - Pin C1 */
  /* To avoid false start, configure output as high */
  GPIO_PinModeSet(gpioPortC, 0, gpioModePushPull, 1);
  /* Define input, no filtering */
  GPIO_PinModeSet(gpioPortC, 1, gpioModeInput, 0);

  /* Configure USART for basic async operation */
  init.enable = usartDisable;
  USART_InitAsync(USART1, &init);

  /* Enable pins at default location */
  USART1->ROUTE = USART_ROUTE_RXPEN | USART_ROUTE_TXPEN;

  /* Finally enable it */
  USART_Enable(USART1, usartEnable);
#endif
}


/*----------------------------------------------------------------------------
  Write character to Serial Port
 *----------------------------------------------------------------------------*/
int32_t SER_PutChar (int32_t ch) {

#ifdef __DBG_ITM
  ITM_SendChar(ch);
#else
  while (!(USART1->STATUS & USART_STATUS_TXBL));
  USART1->TXDATA = (uint32_t)ch;
#endif
  return (ch);
}


/*----------------------------------------------------------------------------
  Read character from Serial Port   (non-blocking)
 *----------------------------------------------------------------------------*/
int32_t SER_GetChar (void) {

#ifdef __DBG_ITM
  if (ITM_CheckChar()) { 
    return (ITM_ReceiveChar());
  }
#else
  if (USART1->STATUS & USART_STATUS_RXDATAV) {
    return (USART1->RXDATA);
  }
#endif
  return (-1);
}
