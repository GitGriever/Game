/*-----------------------------------------------------------------------------
 * Name:    I2C_STM32.c
 * Purpose: I2C low level routines for the STM32F10x
 * Note(s):
 *-----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2011 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <stm32f10x.h>                  /* STM32F2xx Definitions              */
#include "I2C.h"

#define I2C_TOUT  600000                /* Approx. delay for CPU @ 72MHz      */

#define A_WR      0                     /* Master will write to the I2C       */
#define A_RD      1                     /* Master will read from the I2C      */

/* Recovery procedure definitions */
#define IO_IN     0                     /* IO pin is input                    */
#define IO_OUT    1                     /* IO pin is output                   */
#define IO_LOW    0                     /* IO pin is low                      */
#define IO_HI     1                     /* IO pin is high                     */

#define IO_SDA    1                     /* Select SDA line                    */
#define IO_SCL    2                     /* Select SCL line                    */


/*-----------------------------------------------------------------------------
 *      Wait:    Approximate delay used for manual I2C clock generation
 *----------------------------------------------------------------------------*/
static void Wait (void) {
  uint32_t cyc = 1200;
  while (cyc--);
}

/*-----------------------------------------------------------------------------
 *      io_dir: Change direction of the 'io' line
 * Parameters:  io  - IO line definition
 *              dir - IO pin direction definition
 *
 * Return:     (none)
 *----------------------------------------------------------------------------*/
void io_dir (uint32_t io, uint32_t dir) {
  if (io & IO_SDA) {
    GPIOB->CRH &= ~0x00000030;         /* SDA input */
    if (dir == IO_OUT) {
      GPIOB->CRH |= 0x00000030;        /* SDA output */
    }    
  }
  if (io & IO_SCL) {
    GPIOB->CRH &= ~0x00000003;         /* SCL input */
    if (dir == IO_OUT) {
      GPIOB->CRH |= 0x00000003;        /* SCL output */
    }    
  }
}

/*-----------------------------------------------------------------------------
 *      io_pin: Change logical state of the 'io' line
 * Parameters:  io  - IO line definition
 *              st  - IO pin logical state definition
 *
 * Return:     (none)
 *----------------------------------------------------------------------------*/
void io_pin (uint32_t io, uint32_t st) {
  if (io & IO_SDA) {
    if (st == IO_LOW) GPIOB->BRR  = (1 << 9);
    else              GPIOB->BSRR = (1 << 9);
  }
  if (io & IO_SCL) {
    if (st == IO_LOW) GPIOB->BRR  = (1 << 8);
    else              GPIOB->BSRR = (1 << 8);
  }
}

/*-----------------------------------------------------------------------------
 *      io_state: Reads and return logical state of the 'io' line
 * Parameters:  io  - IO line definition
 *
 * Return:     IO_HI - if io line is at logical 1
 *             IO_LO - if io line is at logical 0
 *----------------------------------------------------------------------------*/
uint32_t io_state (uint32_t io) {
  if (io & IO_SDA) {
    return (GPIOB->IDR & (1 << 9)) ? (IO_HI) : (IO_LOW);
  }
  return   (GPIOB->IDR & (1 << 8)) ? (IO_HI) : (IO_LOW);
}


/*-----------------------------------------------------------------------------
 *      I2C_sr:  I2C communication status
 *
 * Parameters: (none)
 *
 * Return:     I2C peripheral status registers value
 *----------------------------------------------------------------------------*/
static __inline uint32_t I2C_sr (void) {
  uint32_t sr;

  sr  = I2C1->SR1;
  sr |= I2C1->SR2 << 16;
  return (sr);
}

/*-----------------------------------------------------------------------------
 *      WaitFlagSet:  Wait until flag is set or timeout occured
 *
 * Parameters:  fl  - flag to check
 *
 * Return:      0   - flag is set
 *              1   - flag not set, timeout occured
 *----------------------------------------------------------------------------*/
static uint32_t WaitFlagSet (uint32_t fl) {
  uint32_t i;
  for (i = I2C_TOUT; i; i--) {
    if (I2C_sr() & fl) {
      return (0);
    }
  }
  return (1);
}

/*-----------------------------------------------------------------------------
 *      WaitFlagSet:  Wait until flag is cleared or timeout occured
 *
 * Parameters:  fl  - flag to check
 *
 * Return:      0   - flag is cleared
 *              1   - flag not cleared, timeout occured
 *----------------------------------------------------------------------------*/
static uint32_t WaitFlagClr (uint32_t fl) {
  uint32_t i;
  for (i = I2C_TOUT; i; i--) {
    if (!(I2C_sr() & fl)) {
      return (0);
    }
  }
  return (1);
}


/*-----------------------------------------------------------------------------
 *      I2C_Init:  Initialize I2C interface in master mode
 *
 * Return:     0 on success, nonzero on error
 *----------------------------------------------------------------------------*/
uint32_t I2C_Init (void) {
  unsigned int tout;

  /* Enable clock for I2C1, GPIOB and AFIO                                    */
  RCC->APB2ENR |= RCC_APB2ENR_IOPBEN | RCC_APB2ENR_AFIOEN;
  RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;

  /* I2C1 pins remapped, use PB8, PB9                                         */
  AFIO->MAPR   |= AFIO_MAPR_I2C1_REMAP;
  GPIOB->CRH   |= 0x000000FF;

  I2C1->CR1     = I2C_CR1_SWRST;        /* Reset I2C peripheral               */
  for (tout = 100; tout; tout--);
  I2C1->CR1     = 0x0000;

  /* Configure I2C peripheral                                                 */
  I2C1->CR1     = I2C_CR1_PE;          /* Enable peripheral                   */
  I2C1->CR2     = 0x0024;              /* Clock: 36MHz, interrupts disabled   */
  I2C1->CR1     = 0x0000;              /* Disable peripheral                  */
  I2C1->TRISE   = 0x0025;
  I2C1->CCR     = 0x005A;
  I2C1->CR1    |= 0x0401;
  I2C1->OAR1    = 0x40A0;
  return (0);
}


/*-----------------------------------------------------------------------------
 *      I2C_Recovery:   Recovery procedure for I2C lines when irregular 
 *                      communication interrupt occures.
 *
 * Return:     0 on success, nonzero on error
 *----------------------------------------------------------------------------*/
uint32_t I2C_Recovery (uint32_t err) {
  uint32_t i;
#if 1
  GPIOB->CRH &= ~0x000000BB;            /* Set SDA and SCL as GPIO            */
  I2C1->CR1 |= I2C_CR1_SWRST;           /* Put I2C peripheral in reset mode   */

  io_dir (IO_SDA | IO_SCL, IO_OUT);     /* SDA and SCL pins are outputs       */
  io_pin (IO_SDA | IO_SCL, IO_HI);      /* Pull both lines high               */
  Wait();  
  io_pin (IO_SCL, IO_LOW);              /* Generate start ccondition          */
  io_pin (IO_SDA, IO_HI);               /* Pull SDA high                      */
  for (i = 0; i < 9; i++) {
    if (io_state (IO_SDA) == IO_HI) break;  /* Break if SDA released by slave */
    io_pin (IO_SCL, IO_HI);             /* Clock high                         */
    Wait();
    io_pin (IO_SCL, IO_LOW);            /* Clock low                          */
    Wait();    
  }
  /* Send stop */
  io_pin (IO_SDA, IO_LOW);              /* Pull SDA low                       */
  io_pin (IO_SCL, IO_HI);               /* Pull SCL high                      */
  Wait();
  io_pin (IO_SDA, IO_HI);               /* Pull SDA high == stop condition    */

  I2C1->CR1 &= ~I2C_CR1_SWRST;          /* Dereset I2C peripheral             */
  return (I2C_Init());                  /* Reinit I2C peripheral              */
#else
  return (1);
#endif
}

/*-----------------------------------------------------------------------------
 *      I2C_Start:  Generate start condition on I2C bus
 *
 * Return:     0 on success, nonzero on error
 *----------------------------------------------------------------------------*/
uint32_t I2C_Start (void) {
  I2C1->CR1 |= I2C_CR1_START;
  return (WaitFlagSet (I2C_SR1_SB));
}


/*-----------------------------------------------------------------------------
 *      I2C_Stop:  Generate stop condition on I2C bus
 *
 * Return:     0 on success, nonzero on error
 *----------------------------------------------------------------------------*/
uint32_t I2C_Stop (void) {
  I2C1->CR1 |= I2C_CR1_STOP;
  return (WaitFlagClr (I2C_SR2_BUSY << 16));
}


/*-----------------------------------------------------------------------------
 *      I2C_Addr:  Write address on I2C interface
 *
 * Return:     0 on success, nonzero on error
 *----------------------------------------------------------------------------*/
uint32_t I2C_Addr (uint8_t adr, uint8_t dir) {
  I2C1->DR = (adr << 1) | dir;
  return (WaitFlagSet (I2C_SR1_ADDR));
}


/*-----------------------------------------------------------------------------
 *      I2C_Write:  Write a byte to I2C interface
 *
 * Return:     0 on success, nonzero on error
 *----------------------------------------------------------------------------*/
uint32_t I2C_Write (uint8_t byte) {
  I2C1->DR = byte;
  return (WaitFlagSet (I2C_SR1_TXE |I2C_SR1_BTF));   /* Byte transfer finished bit is set */
}


/*-----------------------------------------------------------------------------
 *      I2C_Read:  Read a byte from I2C interface
 *----------------------------------------------------------------------------*/
uint32_t I2C_Read (uint32_t ack, uint8_t *byte) {
  /* Enable/disable Master acknowledge                                        */
  if (ack) I2C1->CR1 |=  I2C_CR1_ACK;
  else     I2C1->CR1 &= ~I2C_CR1_ACK;

  if (WaitFlagSet (I2C_SR1_RXNE) == 0) {
      /* RxNE bit is set */
      *byte = I2C1->DR;
      return (0);
  }
  return (1);
}


/*-----------------------------------------------------------------------------
 *      I2C_WrBuf:
 *
 * Parameters: addr - 7-bit device address
 *             buf  - data buffer
 *             cnt  - number of bytes to write
 *
 * Return:     0 on success, nonzero on error
 *----------------------------------------------------------------------------*/
uint32_t I2C_WrBuf (uint8_t addr, uint8_t *buf, uint32_t cnt) {
  uint8_t  *dp  = buf;
  uint32_t  num = cnt;
  uint32_t  err = 0;
  uint32_t  st  = 0;
  
  do {
    switch (st++) {
      case 0: err  = I2C_Start ();            break;
      case 1: err |= I2C_Addr  (addr, A_WR);  break;
      case 2:
        while (!err && num--) {
          err |= I2C_Write (*dp++);
        }
        break;
      case 3: err |= I2C_Stop  ();            break;
    }
  
    if (err) {
      if (I2C_Recovery (err) == 0) {
        /* Recovery succedded, retry */
        dp  = buf;
        num = cnt;
        err = 0;
        st  = 0;
      }
    }
  } while (err == 0 && st < 4);
  return (err);
}


/*-----------------------------------------------------------------------------
 *      I2C_RdBuf:
 *
 * Parameters: addr - 7-bit device address
 *             buf  - data buffer
 *             cnt  - number of bytes to write
 *
 * Return:     0 on success, nonzero on error
 *----------------------------------------------------------------------------*/
uint32_t I2C_RdBuf (uint8_t addr, uint8_t *buf, uint32_t cnt) {
  uint8_t  *dp  = buf;
  uint32_t  num = cnt;
  uint32_t  err = 0;
  uint32_t  st  = 0;
  
  do {
    switch (st++) {
      case 0: err  = I2C_Start ();            break;
      case 1: err |= I2C_Addr  (addr, A_RD);  break;
      case 2:
        while (!err && num--) {
          err |= I2C_Read ((num != 0), dp++);
        }
        break;
      case 3: err |= I2C_Stop  ();            break;
    }
    
    if (err) {
      if (I2C_Recovery (err) == 0) {
        /* Recovery succedded, retry */
        dp  = buf;
        num = cnt;
        err = 0;
        st  = 0;
      }
    }
  } while (err == 0 && st < 4);
  return (err);
}


/*-----------------------------------------------------------------------------
 *      I2C_WrData:
 *
 * Parameters: addr    - 7-bit device address
 *             secByte - second byte to send
 *             buf     - data buffer
 *             cnt     - number of bytes to write
 *
 * Return:     0 on success, nonzero on error
 *----------------------------------------------------------------------------*/
uint32_t I2C_WrData (uint8_t addr, uint8_t secByte, uint8_t *buf, uint32_t cnt) {
  uint8_t  *dp  = buf;
  uint32_t  num = cnt;
  uint32_t  err = 0;
  uint32_t  st  = 0;
  
  do {
    switch (st++) {
      case 0: err  = I2C_Start ();            break;
      case 1: err |= I2C_Addr  (addr, A_WR);  break;
      case 2: err |= I2C_Write (secByte);     break;
      case 3:
        while (!err && num--) {
          err |= I2C_Write (*dp++);
        }
        break;
      case 4: err |= I2C_Stop  ();            break;
    }
  
    if (err) {
      if (I2C_Recovery (err) == 0) {
        /* Recovery succedded, retry */
        dp  = buf;
        num = cnt;
        err = 0;
        st  = 0;
      }
    }
  } while (err == 0 && st < 5);
  return (err);
}


/*-----------------------------------------------------------------------------
 *      I2C_RdData:
 *
 * Parameters: addr - 7-bit device address
 *             secByte - byte to send after address, before switching to read mode
 *             buf     - data buffer
 *             cnt     - number of bytes to read
 *
 * Return:     0 on success, nonzero on error
 *----------------------------------------------------------------------------*/
uint32_t I2C_RdData (uint8_t addr, uint8_t secByte, uint8_t *buf, uint32_t cnt) {
  uint8_t  *dp  = buf;
  uint32_t  num = cnt;
  uint32_t  err = 0;
  uint32_t  st  = 0;

  do {
    switch (st++) {
      case 0: err  = I2C_Start ();            break;
      case 1: err |= I2C_Addr  (addr, A_WR);  break;
      case 2: err |= I2C_Write (secByte);     break;
      case 3: err |= I2C_Start ();            break;
      case 4: err |= I2C_Addr  (addr, A_RD);  break;
      case 5:
        while (!err && num--) {
          err |= I2C_Read ((num != 0), dp++);
        }
        break;
      case 6: err |= I2C_Stop  ();            break;
    }
    
    if (err) {
      if (I2C_Recovery (err) == 0) {
        /* Recovery succedded, retry */
        dp  = buf;
        num = cnt;
        err = 0;
        st  = 0;
      }
    }
  } while (err == 0 && st < 7);
  return (err);
}


/*-----------------------------------------------------------------------------
 * End of file
 *----------------------------------------------------------------------------*/
