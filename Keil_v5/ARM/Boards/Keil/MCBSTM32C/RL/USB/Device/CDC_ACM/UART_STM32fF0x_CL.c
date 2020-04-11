/*------------------------------------------------------------------------------
 * Name:    UART_STM32F10x_CL.c
 * Purpose: UART I/O functions for the Keil MCBSTM32C evaluation board 
 *          with the ST STM32F107 microcontroller
 * Note(s):
 *------------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2004-2013 KEIL - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include "UART.h"
#include "LED.h"
#include <stm32f10x_cl.h>

#define  UART_CLK        (36000000)     /* Serial Peripheral Clock is 36 MHz  */

#define  USE_RTS_CTS_PINS        1

#define  BUFFER_SIZE          (128)     /* Size of Receive and Transmit buffers
                                           MUST BE 2^n                        */
#define  BUFFER_RX_THRESHOLD   (32)     /* Threshold when flow control        
                                           activates RTS line before end of   
                                           buffer, allows to receive that many
                                           bytes after RTS is deactivated     */

/*------------------------------------------------------------------------------
 * GLOBAL VARIABLES
 *----------------------------------------------------------------------------*/

static struct {
           uint8_t  data[BUFFER_SIZE];
  volatile uint16_t idx_in;
  volatile uint16_t idx_out;
  volatile  int16_t cnt_in;
  volatile  int16_t cnt_out;
} WrBuffer, RdBuffer;

static uint32_t Baudrate;
static uint32_t SendBreakFlag;
static uint32_t BreakFlag;
static uint32_t StatusRegister;
static uint32_t ControlLineState;

UART_FlowControl FlowControl = UART_FLOW_CONTROL_NONE;


/*------------------------------------------------------------------------------
 * Define  Baudrate setting for USART 
 *----------------------------------------------------------------------------*/

#define __DIV(__PCLK, __BAUD)       ((__PCLK*25)/(4*__BAUD))
#define __DIVMANT(__PCLK, __BAUD)   (__DIV(__PCLK, __BAUD)/100)
#define __DIVFRAQ(__PCLK, __BAUD)   (((__DIV(__PCLK, __BAUD) - (__DIVMANT(__PCLK, __BAUD) * 100)) * 16 + 50) / 100)
#define __USART_BRR(__PCLK, __BAUD) ((__DIVMANT(__PCLK, __BAUD) << 4)|(__DIVFRAQ(__PCLK, __BAUD) & 0x0F))


/*------------------------------------------------------------------------------
 * UART_Initialize:  Initialize the Serial interface
 *----------------------------------------------------------------------------*/

int32_t UART_Initialize (void) {

  RCC->APB2ENR |=  (1 <<  0);           /* Enable AFIO clock                  */
  RCC->APB2ENR |=  (1 <<  5);           /* Enable GPIOD clock                 */

  RCC->APB1ENR |=   0x00020000;         /* Enable USART2 clock                */
  USART2->SR   &= ~((1 << 5) | (1 << 7));

  AFIO->MAPR   |=   0x00000008;         /* Configure used Pins                */
  GPIOD->CRL   &=   0xF00FFFFF;
  GPIOD->CRL   |=   0x04B00000;         /* PD5 and PD6 USART2 TX and RX       */

  UART_Reset ();

  NVIC_EnableIRQ (USART2_IRQn);         /* Enable USART2 interrupt            */

  return (1);
}


/*------------------------------------------------------------------------------
 * UART_Uninitialize: Uninitialize the Serial interface
 *----------------------------------------------------------------------------*/

int32_t UART_Uninitialize (void) {

  USART2->CR1  &= ~((1 << 6) |          /* TXE interrupt disable              */
                    (1 << 5));          /* RXNE interrupt disable             */
  USART2->CR3  &= ~(1 << 10);           /* STC interrupt disable              */
  NVIC_DisableIRQ (USART2_IRQn);        /* Enable USART2 interrupt            */
  RCC->APB1ENR &= ~(1 << 17);           /* disable USART2 clock               */

  /* reset pin configuration                                                  */
  AFIO->MAPR   &=  ~0x00000008;
  GPIOD->CRL   &=   0xF00FFFFF;
  GPIOD->CRL   |=   0x04400000;
  
#if USE_RTS_CTS_PINS
  GPIOD->CRL   &=   0xFFF00FFF;
  GPIOD->CRL   |=   0x00044000;
#endif

  return (1);
}


/*------------------------------------------------------------------------------
 * UART_Reset:  Reset the Serial module variables
 *----------------------------------------------------------------------------*/

int32_t UART_Reset (void) {
  uint8_t *ptr;
  int32_t  i;

  NVIC_DisableIRQ (USART2_IRQn);        /* Enable USART2 interrupt            */

  Baudrate         = 0;
  SendBreakFlag    = 0;
  BreakFlag        = 0;
  StatusRegister   = 0;
  ControlLineState = 3;                 /* Invalidate control line status     */
  ptr = (uint8_t *)&WrBuffer;
  for (i = 0; i < sizeof(WrBuffer); i++) *ptr++ = 0;
  ptr = (uint8_t *)&RdBuffer;
  for (i = 0; i < sizeof(RdBuffer); i++) *ptr++ = 0;

  NVIC_EnableIRQ (USART2_IRQn);         /* Enable USART2 interrupt            */

  return (1);
}


/*------------------------------------------------------------------------------
 * UART_SetConfiguration:   Set configuration parameters of the Serial interface
 *----------------------------------------------------------------------------*/

int32_t UART_SetConfiguration (UART_Configuration *config) {
  uint32_t i, cr1, cr2;

  /* USART supports:
      8bit data; even, odd and none parity; 1, 1.5 and 2 stop bits */
  
  /* Data bits */
  if (config->DataBits != UART_DATA_BITS_8) return(0);

  /* Parity */
  switch (config->Parity) {
    case UART_PARITY_NONE: cr1 = 0;                                 break;
    case UART_PARITY_EVEN: cr1 = (1 << 12) | (1 << 10);             break;
    case UART_PARITY_ODD:  cr1 = (1 << 12) | (1 << 10) | (1 <<  9); break;
    default: return (0);
  }

  /* Stop bits */
  switch (config->StopBits) {
    case UART_STOP_BITS_1:    cr2 = 0;         break;
    case UART_STOP_BITS_2:    cr2 = (2 << 12); break;
    case UART_STOP_BITS_1_5:  cr2 = (3 << 12); break;
    default: return (0);
  }
  
  /* Baudrate */
  Baudrate = config->Baudrate;
  USART2->BRR = __USART_BRR(UART_CLK, Baudrate);
  
  /* Flow control */
  FlowControl = config->FlowControl;

#if USE_RTS_CTS_PINS
  /* RTS and CTS configured as GPIO pins */
  GPIOD->BSRR  = (1 << 3) | (1 << (4 + 16));
  GPIOD->CRL  &= 0xFFF00FFF;
  GPIOD->CRL  |= 0x00014000;
#endif
  
  switch (FlowControl) {
    case UART_FLOW_CONTROL_NONE:    USART2->CR3 = 0; break;
    case UART_FLOW_CONTROL_RTS_CTS:
      USART2->CR3  = (1 << 9);          /* CTS enable                         */
    break;
    default: return (0);
  }

  USART2->CR2 = cr2;                    /* stop bits settings                 */
  for (i = 0; i < 0x1000; i++) __NOP(); /* avoid unwanted output              */
  USART2->CR1 = (1 << 13) |             /* USART enable                       */
                (1 <<  3) |             /* transmiter enable                  */
                (1 <<  2) |             /* receiver enable                    */
                (1 <<  5) |             /* enable RXNE interrupt              */
                cr1;                    /* parity and data bit settings       */

  return (1);
}


/*------------------------------------------------------------------------------
 * UART_GetConfiguration:   Get configuration parameters of the serial interface
 *----------------------------------------------------------------------------*/

int32_t UART_GetConfiguration (UART_Configuration *config) {
  uint32_t br;

  /* Baudrate */
  br = ((UART_CLK << 4) / (USART2->BRR & 0xFFFF)) >> 4;
  /* If inside +/- 2% tolerance */
  if (((br * 100) <= (Baudrate * 102)) && ((br * 100) >= (Baudrate * 98)))
    config->Baudrate = Baudrate;
  else
    config->Baudrate = br;

  /* Get data bits */
  if ((USART2->CR1 & (1 << 12)) && ((USART2->CR1 & (1 << 10)) == 0)) return (0);
  else config->DataBits = UART_DATA_BITS_8;

  /* Get parity */
  if ((USART2->CR1 & (1 << 10)) == 0) config->Parity = UART_PARITY_NONE;
  else {
    if (USART2->CR1 & (1 << 9)) config->Parity = UART_PARITY_ODD;
    else config->Parity = UART_PARITY_EVEN;
  }

  switch ((USART2->CR2 >> 12) & 3)  {   /* Get stop bits                      */
    case 0: config->StopBits = UART_STOP_BITS_1;   break;
    case 2: config->StopBits = UART_STOP_BITS_2;   break;
    case 3: config->StopBits = UART_STOP_BITS_1_5; break;
    default: return (0);
  }

  config->FlowControl = FlowControl;    /* Get flow control                   */

  return (1);
}


/*------------------------------------------------------------------------------
 * UART_WriteData:     Write data from buffer
 *----------------------------------------------------------------------------*/

int32_t UART_WriteData (uint8_t *data, uint16_t size) {
  uint32_t cnt;
  int16_t  len_in_buf;

  if (size == 0) return (0);

  cnt = 0;
  while (size--) {
    len_in_buf = WrBuffer.cnt_in - WrBuffer.cnt_out;
    if (len_in_buf < BUFFER_SIZE) {
      WrBuffer.data[WrBuffer.idx_in++] = *data++;
      WrBuffer.idx_in &= (BUFFER_SIZE - 1);
      WrBuffer.cnt_in++;
      cnt++;
    }
  }

  USART2->CR1 |= (1 << 7);

  return (cnt);
}


/*------------------------------------------------------------------------------
 * UART_ReadData:     Read data to buffer
 *----------------------------------------------------------------------------*/

int32_t UART_ReadData (uint8_t *data, uint16_t size) {
  uint32_t cnt;
  int16_t  len_in_buf;
  
  if (size == 0) return (0);
  
  cnt = 0;
  while (size--) {
    if (RdBuffer.cnt_in != RdBuffer.cnt_out) {
      *data++ = RdBuffer.data[RdBuffer.idx_out++];
      RdBuffer.idx_out &= (BUFFER_SIZE - 1);
      RdBuffer.cnt_out++;
      cnt++;

#if USE_RTS_CTS_PINS
      len_in_buf = WrBuffer.cnt_in - WrBuffer.cnt_out;
      if (FlowControl == UART_FLOW_CONTROL_RTS_CTS) {
        if (len_in_buf < (BUFFER_SIZE-BUFFER_RX_THRESHOLD)) {
          if (ControlLineState & UART_CONTROL_LINE_RTS_Msk) {
            USART2->CR1 |= (1 << 5);
            UART_SetControlLineState(0, UART_CONTROL_LINE_RTS_Msk);
          }
        }
      }
#endif
    }
  }

  USART2->CR1 |= (1 << 5);

  return (cnt);
}


/*------------------------------------------------------------------------------
 * UART_GetChar:     Read a received character
 *----------------------------------------------------------------------------*/

int32_t UART_GetChar (void) {
  uint8_t ch;
  
  if ((UART_ReadData (&ch, 1)) == 1) return ((int32_t)ch);
  else return (-1);
}


/*------------------------------------------------------------------------------
 * UART_PutChar:     Write a character
 *----------------------------------------------------------------------------*/

int32_t UART_PutChar (uint8_t ch) {

  if ((UART_WriteData (&ch, 1)) == 1) return ((uint32_t) ch);
  else return (-1);
}


/*------------------------------------------------------------------------------
 * UART_DataAvailable: returns number of bytes available in RdBuffer
 *----------------------------------------------------------------------------*/

int32_t UART_DataAvailable (void) {

  return (RdBuffer.cnt_in - RdBuffer.cnt_out);
}


/*------------------------------------------------------------------------------
 * UART_SetControlLineState:  Set state of control lines
 *----------------------------------------------------------------------------*/

int32_t UART_SetControlLineState (uint16_t ls, uint16_t msk) {

#if USE_RTS_CTS_PINS
  if (msk & UART_CONTROL_LINE_RTS_Msk) {
    ((ls & 1) ? (LED_Off (2)) : (LED_On (2)));
    GPIOD->ODR = (GPIOD->ODR & ~(1 << 4)) | ((ls & UART_CONTROL_LINE_RTS_Msk) << 4);
    if (ls & UART_CONTROL_LINE_RTS_Msk) {
      ControlLineState |=  UART_CONTROL_LINE_RTS_Msk;
    } else {
      ControlLineState &= ~UART_CONTROL_LINE_RTS_Msk;
    }
  }
#endif

  return (1);
}


/*------------------------------------------------------------------------------
 * UART_GetControlLineState:  Get state of control lines
 *----------------------------------------------------------------------------*/

uint16_t UART_GetControlLineState (uint16_t msk) {

  return (ControlLineState);
}


/*------------------------------------------------------------------------------
 * UART_GetStatusLineState:   Get state of status lines
 *----------------------------------------------------------------------------*/

int32_t UART_GetStatusLineState (void) {
  int32_t ls;
  static int32_t ls_ex = 0xFF;

  ls = (GPIOD->IDR >> 3) & 1;           /* Get CTS line state                 */

  if (ls ^ ls_ex) {
    ls_ex = ls;
    ((ls & UART_STATUS_LINE_CTS_Msk) ? (LED_Off (3)) : (LED_On (3)));
  }

  return (ls);
}


/*------------------------------------------------------------------------------
 * UART_GetCommunicationErrorStatus:  Get error status
 *----------------------------------------------------------------------------*/

int32_t UART_GetCommunicationErrorStatus (void) {
  int32_t err;
  
  err = 0;
  if ((StatusRegister >> 0) & 1)   err |= UART_PARITY_ERROR_Msk;
  if ((StatusRegister >> 3) & 1)   err |= UART_OVERRUN_ERROR_Msk;
  if (BreakFlag == 0) {
    if ((StatusRegister >> 1) & 1) err |= UART_PARITY_ERROR_Msk;
  }
  
  return (err);
}


/*------------------------------------------------------------------------------
 * UART_SetBreak :  set break condition
 *----------------------------------------------------------------------------*/

int32_t UART_SetBreak (void) {

  SendBreakFlag  =  1;                  /* Set send break flag and            */
  USART2->CR1   |=  1;                  /* send 1 break character             */

  return (1);
}


/*------------------------------------------------------------------------------
 * UART_ClearBreak :  clear break condition
 *----------------------------------------------------------------------------*/

int32_t UART_ClearBreak (void) {

  SendBreakFlag &= ~1;                  /* Clear send break flag              */

  return (1);
}


/*------------------------------------------------------------------------------
 * UART_GetBreak:   if break character received, return 1
 *----------------------------------------------------------------------------*/

int32_t UART_GetBreak (void) {

  return (BreakFlag);
}


/*------------------------------------------------------------------------------
 * UART_IRQ:        Serial interrupt handler routine
 *----------------------------------------------------------------------------*/

void USART2_IRQHandler (void) {
  uint8_t  ch;
  int16_t  len_in_buf;

  StatusRegister = USART2->SR;          /* Read status register               */

  /* Transmit data register empty interrupt */
  if (StatusRegister & (1 << 7)) {
    if (SendBreakFlag == 1) {
      USART2->CR1 |=  1;                /* send 1 break character             */
    } else if (WrBuffer.cnt_in != WrBuffer.cnt_out) {
      USART2->DR = WrBuffer.data[WrBuffer.idx_out++];
      WrBuffer.idx_out &= (BUFFER_SIZE - 1);
      WrBuffer.cnt_out++;
    } else {
      USART2->CR1 &= ~(1 << 7);
    }
  }
  /* Read data register not empty interrupt */
  if (StatusRegister & (1 << 5)) {
    len_in_buf = RdBuffer.cnt_in - RdBuffer.cnt_out;
    if (len_in_buf < BUFFER_SIZE) {
      ch = (uint8_t)USART2->DR;
      RdBuffer.data[RdBuffer.idx_in++] = ch;
      if ((USART2->SR & (1 << 1))                &&  /* framing error         */
          (ch == 0)) {                               /* break character       */
            BreakFlag = 1;
      }
      else BreakFlag = 0;
      RdBuffer.idx_in &= (BUFFER_SIZE - 1);
      RdBuffer.cnt_in++;
    } else {
      USART2->CR1 &= ~(1 << 5);
    }
#if USE_RTS_CTS_PINS
    if (FlowControl == UART_FLOW_CONTROL_RTS_CTS) 
      if (len_in_buf >= (BUFFER_SIZE-BUFFER_RX_THRESHOLD)) {
        if (!(ControlLineState & UART_CONTROL_LINE_RTS_Msk))
          UART_SetControlLineState (1, UART_CONTROL_LINE_RTS_Msk);
    }
#endif
  }
}


/*------------------------------------------------------------------------------
 * End of file
 *----------------------------------------------------------------------------*/

