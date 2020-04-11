/*------------------------------------------------------------------------------
 * Name:    UART_SAM3U.c
 * Purpose: UART I/O functions for the Atmel SAM3U-EK evaluation board 
 *          with the Atmel SAM3U microcontroller
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
#include <sam3u.h>

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
static uint32_t BreakFlag;
static uint32_t StatusRegister;
static uint32_t ControlLineState;

UART_FlowControl FlowControl = UART_FLOW_CONTROL_NONE;


/*------------------------------------------------------------------------------
 * Define  Baudrate setting for USART 
 *----------------------------------------------------------------------------*/

#define BAUD(b) ((SystemCoreClock + 8*b)/(16*b))


/*------------------------------------------------------------------------------
 * UART_Initialize:  Initialize the Serial interface
 *----------------------------------------------------------------------------*/

int32_t UART_Initialize (void) {

  PMC->PMC_WPMR = 0x504D4300;           /* Disable write protect              */
  PMC->PMC_PCER0   =  (1 << ID_PIOA)  | /* Enable PIOA clock                  */
                      (1 << ID_USART1); /* Enable USART1 clock                */
  PMC->PMC_WPMR = 0x504D4301;           /* Enable write protect               */

  /* Configure RX and TX pins                                                 */
  PIOA->PIO_IDR    =  (PIO_PA21A_RXD1 | PIO_PA20A_TXD1);  
  PIOA->PIO_PUDR   =  (PIO_PA21A_RXD1 | PIO_PA20A_TXD1);
  PIOA->PIO_PDR    =  (PIO_PA21A_RXD1 | PIO_PA20A_TXD1);
  PIOA->PIO_ABSR  &= ~(PIO_PA21A_RXD1 | PIO_PA20A_TXD1);

#ifdef USE_RTS_CTS_PINS
  /* Configure RTS and CTS pins                                               */
  PIOA->PIO_IDR    =  (PIO_PA23B_CTS1);  
  PIOA->PIO_PUDR   =  (PIO_PA23B_CTS1);
  PIOA->PIO_PDR    =  (PIO_PA23B_CTS1);
  PIOA->PIO_ABSR  |=  (PIO_PA23B_CTS1);

  PIOA->PIO_IDR    =  (PIO_PA22B_RTS1);
  PIOA->PIO_OER    =  (PIO_PA22B_RTS1);
  PIOA->PIO_PER    =  (PIO_PA22B_RTS1);
#endif

  USART1->US_IER = 1 << 8;              /* Enable break interrupt             */

  UART_Reset ();

  NVIC_EnableIRQ (USART1_IRQn);         /* Enable USART1 interrupt            */

  return (1);
}


/*------------------------------------------------------------------------------
 * UART_Uninitialize: Uninitialize the Serial interface
 *----------------------------------------------------------------------------*/

int32_t UART_Uninitialize (void) {

#ifdef USE_RTS_CTS_PINS
  /* Unconfigure RTS and CTS pins                                             */
  PIOA->PIO_ABSR  &= ~(PIO_PA22B_RTS1 | PIO_PA23B_CTS1);
  PIOA->PIO_PER    =  (PIO_PA22B_RTS1 | PIO_PA23B_CTS1);
#endif

  /* Unconfigure RX and TX pins                                               */
  PIOA->PIO_PER    =  (PIO_PA21A_RXD1 | PIO_PA20A_TXD1);

  PMC->PMC_WPMR = 0x504D4300;           /* Disable write protect              */
  PMC->PMC_PCDR0   =  (1 << ID_USART1); /* Disable USART1 clock               */
  PMC->PMC_WPMR = 0x504D4301;           /* Enable write protect               */

  NVIC_DisableIRQ (USART1_IRQn);        /* Disable USART1 interrupt           */

  return (1);
}


/*------------------------------------------------------------------------------
 * UART_Reset:  Reset the Serial module variables
 *----------------------------------------------------------------------------*/

int32_t UART_Reset (void) {
  uint8_t *ptr;
  int32_t  i;

  NVIC_DisableIRQ (USART1_IRQn);        /* Enable USART1 interrupt            */

  BreakFlag        = 0;
  StatusRegister   = 0;
  ControlLineState = 3;                 /* Invalidate control line status     */
  ptr = (uint8_t *)&WrBuffer;
  for (i = 0; i < sizeof(WrBuffer); i++) *ptr++ = 0;
  ptr = (uint8_t *)&RdBuffer;
  for (i = 0; i < sizeof(RdBuffer); i++) *ptr++ = 0;

  NVIC_EnableIRQ (USART1_IRQn);         /* Enable USART1 interrupt            */

  return (1);
}


/*------------------------------------------------------------------------------
 * UART_SetConfiguration:   Set configuration parameters of the Serial interface
 *----------------------------------------------------------------------------*/

int32_t UART_SetConfiguration (UART_Configuration *config) {
  uint32_t mr = 0;

  switch (config->DataBits) {           /* Prepare data bits value for MR reg */
    case UART_DATA_BITS_5:
      break;
    case UART_DATA_BITS_6:
      mr |= 1 << 6;
      break;
    case UART_DATA_BITS_7:
      mr |= 2 << 6;
      break;
    case UART_DATA_BITS_8:
      mr |= 3 << 6;
      break;
    default:
      return (0);
  }

  switch (config->Parity) {             /* Prepare parity value for MR reg    */
    case UART_PARITY_NONE:
      mr |= 4 << 9;
      break;
    case UART_PARITY_ODD:
      mr |= 1 << 9;
      break;
    case UART_PARITY_EVEN:
      mr |= 0 << 9;
      break;
    case UART_PARITY_MARK:
      mr |= 3 << 9;
      break;
    case UART_PARITY_SPACE:
      mr |= 2 << 9;
      break;
    default:
      return (0);
  }

  switch (config->StopBits) {           /* Prepare stop bits value for MR reg */
    case UART_STOP_BITS_1:
      break;
    case UART_STOP_BITS_1_5:
      mr |= 1 << 12;
      break;
    case UART_STOP_BITS_2:
      mr |= 2 << 12;
      break;
    default:
      return (0);
  }

  switch (config->FlowControl) {        /* Prepare flow control value for MR  */
    case UART_FLOW_CONTROL_NONE:
      FlowControl = UART_FLOW_CONTROL_NONE;
      break;
    case UART_FLOW_CONTROL_RTS_CTS:
      mr |= 2 << 0;
      FlowControl = UART_FLOW_CONTROL_RTS_CTS;
      break;
    default:
      return (0);
  }

  Baudrate = config->Baudrate;

  USART1->US_IDR   =  0xFFFFFFFF;
  USART1->US_MR    =  mr         |
                     (0x0 <<  4) |      /* (USART) USCLKS: MCK Clock          */
                     (0x0 << 14) ;      /* (USART) CHMODE: Normal Mode        */
  USART1->US_BRGR  =  BAUD(config->Baudrate);
  USART1->US_CR    =  US_CR_RSTRX    | US_CR_RSTTX;
  USART1->US_PTCR  =  US_PTCR_RXTDIS | US_PTCR_TXTDIS;
  USART1->US_CR    =  US_CR_RXEN     | US_CR_TXEN;

  USART1->US_IER  = 1 << 0;             /* Enable RXRDY interrupt             */

  return (1);
}


/*------------------------------------------------------------------------------
 * UART_GetConfiguration:   Get configuration parameters of the serial interface
 *----------------------------------------------------------------------------*/

int32_t UART_GetConfiguration (UART_Configuration *config) {
  uint32_t br;
  uint32_t mr;

  mr = USART1->US_MR;

  br = SystemCoreClock / (16 * USART1->US_BRGR);  /* Baudrate                 */
  /* If inside +/- 2% tolerance */
  if (((br * 100) <= (Baudrate * 102)) && ((br * 100) >= (Baudrate * 98)))
    config->Baudrate = Baudrate;
  else
    config->Baudrate = br;

  switch ((mr >> 6) & 3) {                        /* Get data bits            */
    case 0:
      config->DataBits = UART_DATA_BITS_5;
      break;
    case 1:
      config->DataBits = UART_DATA_BITS_6;
      break;
    case 2:
      config->DataBits = UART_DATA_BITS_7;
      break;
    case 3:
      config->DataBits = UART_DATA_BITS_8;
      break;
    default:
      return (0);
  }

  switch ((mr >> 9) & 7) {                        /* Get parity               */
    case 0:
      config->Parity = UART_PARITY_EVEN;
      break;
    case 1:
      config->Parity = UART_PARITY_ODD;
      break;
    case 2:
      config->Parity = UART_PARITY_SPACE;
      break;
    case 3:
      config->Parity = UART_PARITY_MARK;
      break;
    case 4:
      config->Parity = UART_PARITY_NONE;
      break;
    default:
      return (0);
  }

  switch ((mr >> 12) & 3) {                       /* Get stop bits            */
    case 0:
      config->StopBits = UART_STOP_BITS_1;
      break;
    case 1:
      config->StopBits = UART_STOP_BITS_1_5;
      break;
    case 2:
      config->StopBits = UART_STOP_BITS_2;
      break;
    default:
      return (0);
  }

  switch (mr & 15) {                              /* Get flow control         */
    case 0:
      config->FlowControl = UART_FLOW_CONTROL_NONE;
      break;
    case 2:
      config->FlowControl = UART_FLOW_CONTROL_RTS_CTS;
      break;
    default:
      return (0);
  }

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

  USART1->US_IER = 1 << 1;              /* Enable TXRDY interrupt             */

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
            USART1->US_IER = 1 << 0;    /* Enable RXRDY interrupt             */
            UART_SetControlLineState(0, UART_CONTROL_LINE_RTS_Msk);
          }
        }
      }
#endif
    }
  }

  USART1->US_IER = 1 << 0;              /* Enable RXRDY interrupt             */

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
    ((ls & 1) ? (LED_Off (0)) : (LED_On (0)));
    if (ls & UART_CONTROL_LINE_RTS_Msk) {
      PIOA->PIO_SODR = PIO_PA22B_RTS1;  /* Drive RTS line high                */
      ControlLineState |=  UART_CONTROL_LINE_RTS_Msk;
    } else {
      PIOA->PIO_CODR = PIO_PA22B_RTS1;  /* Drive RTS line low                 */
      ControlLineState &= ~UART_CONTROL_LINE_RTS_Msk;
    }
  }
  if (msk & UART_CONTROL_LINE_DTR_Msk) {
    if (ls & UART_CONTROL_LINE_DTR_Msk) {
      USART1->US_CR = 1 << 16;          /* Activate DTREN                     */
      ControlLineState |=  UART_CONTROL_LINE_DTR_Msk;
    } else {
      USART1->US_CR = 1 << 17;          /* Activate DTRDIS                    */
      ControlLineState &= ~UART_CONTROL_LINE_DTR_Msk;
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

  ls = 0;
  if ((StatusRegister >> 23) & 1) ls |= UART_STATUS_LINE_CTS_Msk;
  if ((StatusRegister >> 22) & 1) ls |= UART_STATUS_LINE_DCD_Msk;
  if ((StatusRegister >> 21) & 1) ls |= UART_STATUS_LINE_DSR_Msk;
  if ((StatusRegister >> 20) & 1) ls |= UART_STATUS_LINE_RI_Msk;

  if (ls ^ ls_ex) {
    ls_ex = ls;
    ((ls & UART_STATUS_LINE_CTS_Msk) ? (LED_Off (1)) : (LED_On (1)));
  }

  return (ls);
}


/*------------------------------------------------------------------------------
 * UART_GetCommunicationErrorStatus:  Get error status
 *----------------------------------------------------------------------------*/

int32_t UART_GetCommunicationErrorStatus (void) {
  int32_t err;
  
  err = 0;
  if ((StatusRegister >> 5) & 1) err |= UART_OVERRUN_ERROR_Msk;
  if ((StatusRegister >> 6) & 1) err |= UART_FRAMING_ERROR_Msk;
  if ((StatusRegister >> 7) & 1) err |= UART_PARITY_ERROR_Msk;
  
  return (err);
}


/*------------------------------------------------------------------------------
 * UART_SetBreak :  set break condition
 *----------------------------------------------------------------------------*/

int32_t UART_SetBreak (void) {

  USART1->US_CR |= 1 <<  9;             /* Start break                        */

  return (1);
}


/*------------------------------------------------------------------------------
 * UART_ClearBreak :  clear break condition
 *----------------------------------------------------------------------------*/

int32_t UART_ClearBreak (void) {

  USART1->US_CR |= 1 << 10;             /* Stop break                         */

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

void USART1_IRQHandler (void) {
  int16_t  len_in_buf;

  StatusRegister = USART1->US_CSR;      /* Read status register               */

  /* Handle character to transmit                                             */
  if (StatusRegister & (1 << 1)) {      /* If TXRDY bit in US_CSR reg is set  */
    if ((WrBuffer.cnt_in != WrBuffer.cnt_out)) {
      USART1->US_THR = WrBuffer.data[WrBuffer.idx_out++];
      WrBuffer.idx_out &= (BUFFER_SIZE - 1);
      WrBuffer.cnt_out++;
    } else {
      USART1->US_IDR = 1 << 1;
    }
  }
  /* Handle received character with use of flow control                       */
  if (StatusRegister & (1 << 0)) {      /* If RXRDY bit in US_CSR reg is set  */
    len_in_buf = RdBuffer.cnt_in - RdBuffer.cnt_out;
    if (len_in_buf < BUFFER_SIZE) {
      RdBuffer.data[RdBuffer.idx_in++] = USART1->US_RHR;
      RdBuffer.idx_in &= (BUFFER_SIZE - 1);
      RdBuffer.cnt_in++;
    } else {
      USART1->US_IDR = 1 << 0;          /* Disable RXRDY interrupt            */
    }
#if USE_RTS_CTS_PINS
    if (FlowControl == UART_FLOW_CONTROL_RTS_CTS) 
      if (len_in_buf >= (BUFFER_SIZE-BUFFER_RX_THRESHOLD)) {
        if (!(ControlLineState & UART_CONTROL_LINE_RTS_Msk))
          UART_SetControlLineState (1, UART_CONTROL_LINE_RTS_Msk);
    }
#endif
  }

  /* Handle break                                                             */
  if (StatusRegister & (1 << 2)) {      /* If RXBRK bit in US_CSR reg is set  */
    BreakFlag ^= 1;
    USART1->US_CR = 1 << 8;             /* Activate RSTSTA bit in US_CR reg   */
  }
}


/*------------------------------------------------------------------------------
 * End of file
 *----------------------------------------------------------------------------*/

