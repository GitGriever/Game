/*-----------------------------------------------------------------------------
 * Name:    I2C_LPC18xx.c
 * Purpose: I2C low level routines for the LPC18xx
 * Note(s):
 *-----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2005-2012 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <LPC18xx.h>                    /* LPC18xx Definitions                */
#include "I2C.h"

#define I2C_PERIPH_CLK  180000000
#define I2C_TOUT           100000       /* Approx. delay in CPU cycles        */

#define A_WR      0                     /* Master will write to the I2C       */
#define A_RD      1                     /* Master will read from the I2C      */

/* Clock Control Unit register bits */
#define CCU_CLK_CFG_RUN   (1 << 0)
#define CCU_CLK_CFG_AUTO  (1 << 1)
#define CCU_CLK_STAT_RUN  (1 << 0)

/* LPC18xx/LPC43xx Register Bits Definitions */
#define SFSI2C0_SCL_EFP   (1 <<  0)
#define SFSI2C0_SCL_EHD   (1 <<  2)
#define SFSI2C0_SCL_EZI   (1 <<  3)
#define SFSI2C0_SCL_ZIF   (1 <<  7)
#define SFSI2C0_SDA_EFP   (1 <<  8)
#define SFSI2C0_SDA_EHD   (1 << 10)
#define SFSI2C0_SDA_EZI   (1 << 11)
#define SFSI2C0_SDA_ZIF   (1 << 15)

#define I2C_CONCLR_AAC    (1 <<  2)     /* CONCLR:Assert Acknowledge Clear    */
#define I2C_CONCLR_SIC    (1 <<  3)     /* CONCLR:I2C Interrupt Clear Bit     */
#define I2C_CONCLR_STAC   (1 <<  5)     /* CONCLR:Start Flag Clear            */
#define I2C_CONCLR_I2ENC  (1 <<  6)     /* CONCLR:I2C Interface Disable       */

#define I2C_CONSET_AA     (1 <<  2)     /* CONSET:Assert acknowledge flag     */
#define I2C_CONSET_SI     (1 <<  3)     /* CONSET:I2C interrupt flag          */
#define I2C_CONSET_STO    (1 <<  4)     /* CONSET:STOP flag                   */
#define I2C_CONSET_STA    (1 <<  5)     /* CONSET:START flag                  */
#define I2C_CONSET_I2EN   (1 <<  6)     /* CONSET:I2C interface enable        */

#define I2C_STAT_ST       0x08          /* STAT:START has been transmitted    */
#define I2C_STAT_RPTST    0x10          /* STAT:Repeated START transmitted    */
#define I2C_STAT_SLAWA    0x18          /* STAT:SLA+W transmitted and ACKed   */
#define I2C_STAT_SLAWNA   0x20          /* STAT:SLA+W transmitted, no ACK     */
#define I2C_STAT_DTA      0x28          /* STAT:Data transmitted and ACKed    */
#define I2C_STAT_DTNA     0x30          /* STAT:Data transmitted, no ACK      */
#define I2C_STAT_ALOST    0x38          /* STAT:Arbitration lost              */

#define I2C_STAT_SLARA    0x40          /* STAT:SLA+R transmitted and ACKed   */
#define I2C_STAT_SLARNA   0x48          /* STAT:SLA+R transmitted, no ACK     */
#define I2C_STAT_DRA      0x50          /* STAT:Data received, ack returned   */
#define I2C_STAT_DRNA     0x58          /* STAT:Data received, no ack returned*/


/*-----------------------------------------------------------------------------
 *      WaitStart:  Wait until START condition transmitted
 *
 * Parameters:  (none)
 *
 * Return:      0   - START condition has been transmitted
 *              1   - No START condition, timeout occured
 *----------------------------------------------------------------------------*/
static uint32_t WaitStart (void) {
  uint32_t i, stat;
  for (i = I2C_TOUT; i; i--) {
    stat = LPC_I2C0->STAT;
    if (stat == I2C_STAT_ST || stat == I2C_STAT_RPTST) {
      return (0);
    }
  }
  return (1);
}


/*-----------------------------------------------------------------------------
 *      WaitStatus:  Wait for specified status code
 *
 * Parameters:  code - status code to wait for
 *
 * Return:      0   - code is set
 *              1   - code not set, timeout occured
 *----------------------------------------------------------------------------*/
static uint32_t WaitStatus (uint32_t code) {
  uint32_t i;
  for (i = I2C_TOUT; i; i--) {
    if (LPC_I2C0->STAT == code) {
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
  /* Connect base clock */
  LPC_CGU->BASE_APB1_CLK = (1    << 11) |
                           (0x09 << 24) ; /* PLL1 is APB1 clock source        */

  /* Enable I2C0 peripheral clock */
  LPC_CCU1->CLK_APB1_I2C0_CFG = CCU_CLK_CFG_AUTO | CCU_CLK_CFG_RUN;
  while (!(LPC_CCU1->CLK_APB1_I2C0_STAT & CCU_CLK_STAT_RUN));

  /* Configure I2C Pins */
  LPC_SCU->SFSI2C0 = SFSI2C0_SDA_EZI | SFSI2C0_SCL_EZI;

  /* Clock rate 400kHz @ APB1 Clock */
  LPC_I2C0->SCLH = I2C_PERIPH_CLK / 400000;
  LPC_I2C0->SCLL = I2C_PERIPH_CLK / 400000;

  /* Set I2C Operation */
  LPC_I2C0->CONCLR = I2C_CONCLR_I2ENC | I2C_CONCLR_STAC | I2C_CONCLR_SIC | I2C_CONCLR_AAC;
  LPC_I2C0->CONSET = I2C_CONSET_I2EN; 
  return (0);
}


/*-----------------------------------------------------------------------------
 *      I2C_Recovery:   Recovery procedure for I2C lines when irregular 
 *                      communication interrupt occures.
 *
 * Return:     0 on success, nonzero on error
 *----------------------------------------------------------------------------*/
uint32_t I2C_Recovery (uint32_t err) {
  LPC_I2C0->CONSET = I2C_CONSET_STO;
  LPC_I2C0->CONCLR = I2C_CONCLR_STAC | I2C_CONSET_SI;  
  return (I2C_Init());
}


/*-----------------------------------------------------------------------------
 *      I2C_Start:  Generate start condition on I2C bus
 *
 * Return:     0 on success, nonzero on error
 *----------------------------------------------------------------------------*/
uint32_t I2C_Start (void) {
  LPC_I2C0->CONCLR = I2C_CONCLR_SIC;
  LPC_I2C0->CONSET = I2C_CONSET_STA;  
  return (WaitStart());
}


/*-----------------------------------------------------------------------------
 *      I2C_Stop:  Generate stop condition on I2C bus
 *
 * Return:     0 on success, nonzero on error
 *----------------------------------------------------------------------------*/
uint32_t I2C_Stop (void) {
  LPC_I2C0->CONCLR = I2C_CONCLR_STAC;
  LPC_I2C0->CONSET = I2C_CONSET_STO;
  return (0);
}


/*-----------------------------------------------------------------------------
 *      I2C_Addr:  Write address on I2C interface
 *
 * Return:     0 on success, nonzero on error
 *----------------------------------------------------------------------------*/
uint32_t I2C_Addr (uint8_t adr, uint8_t dir) {
  uint32_t stat;

  LPC_I2C0->DAT = (adr << 1) | dir;     /* Load address and direction bit     */
  LPC_I2C0->CONCLR = I2C_CONCLR_SIC;    /* Clear SI bit                       */
  stat = (dir) ?  (I2C_STAT_SLARA) : (I2C_STAT_SLAWA);
  return (WaitStatus (stat));           /* Wait until address sent or tout    */
}


/*-----------------------------------------------------------------------------
 *      I2C_Write:  Write a byte to I2C interface
 *
 * Return:     0 on success, nonzero on error
 *----------------------------------------------------------------------------*/
uint32_t I2C_Write (uint8_t byte) {
  LPC_I2C0->DAT = byte;
  LPC_I2C0->CONCLR = I2C_CONCLR_SIC | I2C_CONCLR_STAC;
  return (WaitStatus (I2C_STAT_DTA));   /* Wait for Data Tx, ACK Rx           */
}


/*-----------------------------------------------------------------------------
 *      I2C_Read:  Read a byte from I2C interface
 *----------------------------------------------------------------------------*/
uint32_t I2C_Read (uint32_t ack, uint8_t *byte) {
  uint32_t stat;
  if (ack) LPC_I2C0->CONSET = I2C_CONSET_AA;
  else     LPC_I2C0->CONCLR = I2C_CONCLR_AAC;

  LPC_I2C0->CONCLR = I2C_CONCLR_STAC | I2C_CONCLR_SIC;

  stat = (ack) ? (I2C_STAT_DRA) : (I2C_STAT_DRNA);
  if (WaitStatus (stat) == 0) {
    *byte = LPC_I2C0->DAT;
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
