/*------------------------------------------------------------------------------
 * Name:    UART_STM32F4xx.c
 * Purpose: UART I/O functions for the Keil MCBSTM32F400 evaluation board 
 *          with the ST STM32F407 microcontroller
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
#include <stm32f4xx.h>

#ifdef __USART1
  #define  USART            USART1
  #define  USART_IRQn       USART1_IRQn
  #define  USART_IRQHandler USART1_IRQHandler
  #define  UART_CLK        (84000000)
#else
  #define UART_CLK         (42000000)
  
#ifdef __USART3
  #define  USART            USART3
  #define  USART_IRQn       USART3_IRQn
  #define  USART_IRQHandler USART3_IRQHandler
#endif

#ifdef __USART4
  #define  USART            UART4
  #define  USART_IRQn       UART4_IRQn
  #define  USART_IRQHandler UART4_IRQHandler
#endif
#endif 

#define  BUFFER_SIZE          (128)     /* Size of Receive and Transmit buffers
                                           MUST BE 2^n                        */
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

#if defined __USART3  || defined __USART4
#if defined __USART3
  RCC->APB1ENR  |= (1 << 18);           /* Enable USART3 clock                */
#else
  RCC->APB1ENR  |= (1 << 19);           /* Enable USART4 clock                */
#endif
  RCC->AHB1ENR  |= (1 <<  2);           /* Enable GPIOC clock                 */

  USART->SR &= ~((1 << 5) | (1 << 7));

  /* Connect PC10 and PC11 to Alternate Function */
  GPIOC->MODER  &= ~((3 << 2*10) | (3 << 2*11));
  GPIOC->MODER  |= (2 << 2*10) | (2 << 2*11);

  GPIOC->AFR[1] &= ~0x0000FF00;
#if defined __USART3
  GPIOC->AFR[1] |=  0x00007700;         /* USART3: Alternate Function 7 (AF7) */
#else
  GPIOC->AFR[1] |=  0x00008800;         /* USART4: Alternate Function 8 (AF8) */
  #endif
#else
  RCC->APB2ENR  |= (1UL << 4);          /* Enable USART1 clock                */
  RCC->AHB1ENR  |= (1UL << 1);          /* Enable GPIOB clock                 */

  USART->SR &= ~((1 << 5) | (1 << 7));

  /* Connect PB6 and PB7 to Alternate Function 7 (AF7) */
  GPIOB->MODER  &= ~((3 << 2*6) | (3 << 2*7));
  GPIOB->MODER  |= (2 << 2*6) | (2 << 2*7);
  GPIOB->AFR[0] &= ~0xFF000000;
  GPIOB->AFR[0] |=  0x77000000;
#endif

  UART_Reset ();

  NVIC_EnableIRQ(USART_IRQn);           /* Enable USART interrupt             */

  return (1);
}


/*------------------------------------------------------------------------------
 * UART_Uninitialize: Uninitialize the Serial interface
 *----------------------------------------------------------------------------*/

int32_t UART_Uninitialize (void) {

  USART->CR1  &= ~((1 << 6) |           /* TXE interrupt disable              */
                   (1 <<  5));          /* RXNE interrupt disable             */
  USART->CR3  &= ~(1 << 10);            /* STC interrupt disable              */
  NVIC_DisableIRQ(USART_IRQn);          /* Enable USART interrupt             */
  
#if defined __USART3  || defined __USART4
#if defined __USART3
  RCC->APB1ENR  &= ~(1UL <<18);         /* Disable USART3 clock               */
#else                                   
  RCC->APB1ENR  &= ~(1UL <<19);         /* Disable USART4 clock               */
#endif

  /* Disconnect PC10 and PC11 from Alternate Function */
  GPIOC->MODER  &= ~((3 << 2*10) | (3 << 2*11));
  GPIOC->AFR[1] &= ~0x0000FF00;
#else
  RCC->APB2ENR  &= ~(1 << 4);           /* Disable USART1 clock               */
  
  /* Diconnect PB6 and PB7 from Alternate Function 7 (AF7) */
  GPIOB->MODER  &= ~((3 << 2*6) | (3 << 2*7));
  GPIOB->AFR[0] &= ~0xFF000000;
#endif
  return (1);
}


/*------------------------------------------------------------------------------
 * UART_Reset:  Reset the Serial module variables
 *----------------------------------------------------------------------------*/

int32_t UART_Reset (void) {
  uint8_t *ptr;
  int32_t  i;

  NVIC_DisableIRQ(USART_IRQn);          /* Enable USART interrupt             */

  Baudrate         = 0;
  SendBreakFlag    = 0;
  BreakFlag        = 0;
  StatusRegister   = 0;
  ptr = (uint8_t *)&WrBuffer;
  for (i = 0; i < sizeof(WrBuffer); i++) *ptr++ = 0;
  ptr = (uint8_t *)&RdBuffer;
  for (i = 0; i < sizeof(RdBuffer); i++) *ptr++ = 0;

  NVIC_EnableIRQ (USART_IRQn);          /* Enable USART interrupt             */

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
  USART->BRR = __USART_BRR(UART_CLK, Baudrate);
  
  /* Flow control */
  FlowControl = config->FlowControl;

  switch (config->FlowControl) {        /* Prepare flow control value for MR  */
    case UART_FLOW_CONTROL_NONE:
      FlowControl = UART_FLOW_CONTROL_NONE;
      break;
    case UART_FLOW_CONTROL_RTS_CTS:
    default:
      return (0);
  }

  USART->CR2 = cr2;                     /* stop bits settings                 */
  for (i = 0; i < 0x1000; i++) __NOP(); /* avoid unwanted output              */
  USART->CR1 = (1 << 13) |              /* USART enable                       */
               (1 <<  3) |              /* transmiter enable                  */
               (1 <<  2) |              /* receiver enable                    */
               (1 <<  5) |              /* enable RXNE interrupt              */
                cr1;                    /* parity and data bit settings       */

  return (1);
}


/*------------------------------------------------------------------------------
 * UART_GetConfiguration:   Get configuration parameters of the serial interface
 *----------------------------------------------------------------------------*/

int32_t UART_GetConfiguration (UART_Configuration *config) {
  uint32_t br;

  /* Baudrate */
  br = ((UART_CLK << 4) / (USART->BRR & 0xFFFF)) >> 4;
  /* If inside +/- 2% tolerance */
  if (((br * 100) < (Baudrate * 102)) && ((br * 100) > (Baudrate * 98)))
    config->Baudrate = Baudrate;
  else
    config->Baudrate = br;

  /* Get data bits */
  if ((USART->CR1 & (1 << 12)) && ((USART->CR1 & (1 << 10)) == 0)) return (0);
  else config->DataBits = UART_DATA_BITS_8;

  /* Get parity */
  if ((USART->CR1 & (1 << 10)) == 0) config->Parity = UART_PARITY_NONE;
  else {
    if (USART->CR1 & (1 << 9)) config->Parity = UART_PARITY_ODD;
    else config->Parity = UART_PARITY_EVEN;
  }

  switch ((USART->CR2 >> 12) & 3)  {    /* Get stop bits                      */
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

  USART->CR1 |= (1 << 7);

  return (cnt);
}


/*------------------------------------------------------------------------------
 * UART_ReadData:     Read data to buffer
 *----------------------------------------------------------------------------*/

int32_t UART_ReadData (uint8_t *data, uint16_t size) {
  uint32_t cnt;
  
  if (size == 0) return (0);
  
  cnt = 0;
  while (size--) {
    if (RdBuffer.cnt_in != RdBuffer.cnt_out) {
      *data++ = RdBuffer.data[RdBuffer.idx_out++];
      RdBuffer.idx_out &= (BUFFER_SIZE - 1);
      RdBuffer.cnt_out++;
      cnt++;
    }
  }

  USART->CR1 |= (1 << 5);

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

  return (0);
}


/*------------------------------------------------------------------------------
 * UART_GetControlLineState:  Get state of control lines
 *----------------------------------------------------------------------------*/

uint16_t UART_GetControlLineState (uint16_t msk) {

  return (0);
}


/*------------------------------------------------------------------------------
 * UART_GetStatusLineState:   Get state of status lines
 *----------------------------------------------------------------------------*/

int32_t UART_GetStatusLineState (void) {

  return (0);
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
  USART->CR1    |=  1;                  /* send 1 break character             */

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

void USART_IRQHandler (void) {
  uint8_t  ch;
  int16_t  len_in_buf;

  StatusRegister = USART->SR;           /* Read status register               */

  /* Transmit data register empty interrupt */
  if (StatusRegister & (1 << 7)) {
    if (SendBreakFlag == 1) {
      USART->CR1 |=  1;                 /* send 1 break character             */
    } else if (WrBuffer.cnt_in != WrBuffer.cnt_out) {
      USART->DR = WrBuffer.data[WrBuffer.idx_out++];
      WrBuffer.idx_out &= (BUFFER_SIZE - 1);
      WrBuffer.cnt_out++;
    } else {
      USART->CR1 &= ~(1 << 7);
    }
  }
  /* Read data register not empty interrupt */
  if (StatusRegister & (1 << 5)) {
    len_in_buf = RdBuffer.cnt_in - RdBuffer.cnt_out;
    if (len_in_buf < BUFFER_SIZE) {
      ch = (uint8_t)USART->DR;
      RdBuffer.data[RdBuffer.idx_in++] = ch;
      if ((USART->SR & (1 << 1))                &&   /* framing error         */
          (ch == 0)) {                               /* break character       */
            BreakFlag = 1;
      }
      else BreakFlag = 0;
      RdBuffer.idx_in &= (BUFFER_SIZE - 1);
      RdBuffer.cnt_in++;
    } else {
      USART->CR1 &= ~(1 << 5);
    }
  }
}


/*------------------------------------------------------------------------------
 * End of file
 *----------------------------------------------------------------------------*/

