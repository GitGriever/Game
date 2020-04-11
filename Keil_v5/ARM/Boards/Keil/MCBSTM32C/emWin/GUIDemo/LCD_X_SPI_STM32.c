/*********************************************************************
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2017  SEGGER Microcontroller GmbH & Co. KG       *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

** emWin V5.46 - Graphical user interface for embedded applications **
All  Intellectual Property rights  in the Software belongs to  SEGGER.
emWin is protected by  international copyright laws.  Knowledge of the
source code may not be used to write a similar product.  This file may
only be used in accordance with the following terms:

The software has been licensed to  ARM LIMITED whose registered office
is situated at  110 Fulbourn Road,  Cambridge CB1 9NJ,  England solely
for  the  purposes  of  creating  libraries  for  ARM7, ARM9, Cortex-M
series,  and   Cortex-R4   processor-based  devices,  sublicensed  and
distributed as part of the  MDK-ARM  Professional  under the terms and
conditions  of  the   End  User  License  supplied  with  the  MDK-ARM
Professional. 
Full source code is available at: www.segger.com

We appreciate your understanding and fairness.
----------------------------------------------------------------------
Licensing information
Licensor:                 SEGGER Software GmbH
Licensed to:              ARM Ltd, 110 Fulbourn Road, CB1 9NJ Cambridge, UK
Licensed SEGGER software: emWin
License number:           GUI-00181
License model:            LES-SLA-20007, Agreement, effective since October 1st 2011 
Licensed product:         MDK-ARM Professional
Licensed platform:        ARM7/9, Cortex-M/R4
Licensed number of seats: -
----------------------------------------------------------------------
File        : LCD_X_SPI_STM32.c
Purpose     : Port routines for STM32 SPI
----------------------------------------------------------------------
*/

#include "GUI.h"

/*********************************************************************
*
*       Hardware configuration
*
**********************************************************************
*/

#include <stm32f10x_cl.h>

/* SPI Interface: SPI3
   
   PINS: 
   - CS  = PB10 (GPIO pin)
   - SCK = PC10 (SPI3_SCK)
   - SDO = PC11 (SPI3_MISO)
   - SDI = PC12 (SPI3_MOSI)
*/

/* TWI Interface: GPIO
   
   PINS: 
   - CS  = PB10 (GPIO pin)
   - SCL = PC10 (GPIO pin)
   - SDA = PC12 (GPIO pin)
*/

#define PIN_CS      (1 << 10)
#define PIN_SCL     (1 << 10)
#define PIN_SDA     (1 << 12)

// SPI_SR - bit definitions
#define RXNE        0x01
#define TXE         0x02
#define BSY         0x80

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/

static unsigned short DevID;    /* Device ID */

/*********************************************************************
*
*       Static code
*
*********************************************************************
*/

#define LCD_CS(x)  ((x) ? (GPIOB->BSRR = PIN_CS)  : (GPIOB->BRR = PIN_CS))
#define LCD_SCL(x) ((x) ? (GPIOC->BSRR = PIN_SCL) : (GPIOC->BRR = PIN_SCL))
#define LCD_SDO(x) ((x) ? (GPIOC->BSRR = PIN_SDA) : (GPIOC->BRR = PIN_SDA))

#define LCD_SDI()        ((GPIOC->IDR &  PIN_SDA) != 0)

#define LCD_DIR(x) ((x) ? (GPIOC->CRH = (GPIOC->CRH & ~(0xF<<16)) | (3<<16)) : \
                          (GPIOC->CRH = (GPIOC->CRH & ~(0xF<<16)) | (1<<18)))

#define LCD_DLY(x) ((x) ? __nop(), __nop(), __nop(), __nop(), __nop() : __nop())

/*********************************************************************
*
*       SPI_Transfer
*
* Purpose:
*   Transfer (Write/Read) 1 byte, MSB first
*/
static unsigned char SPI_Transfer(unsigned char byte) {
  SPI3->DR = byte;
  while (!(SPI3->SR & RXNE));           /* Wait for send to finish */
  return (SPI3->DR);
}

/*********************************************************************
*
*       TWI_Send
*
* Purpose:
*   Send 1 byte, MSB first
*/
static void TWI_Send(unsigned char byte) {
  unsigned int  mask;

  for (mask = 0x80; mask != 0; mask >>= 1) {
    LCD_SCL(0);
    LCD_SDO(byte & mask);
    LCD_DLY(0);
    LCD_SCL(1);
    LCD_DLY(0);
  }
}

/*********************************************************************
*
*       TWI_Recv
*
* Purpose:
*   Receive 1 byte, MSB first
*/
static unsigned char TWI_Recv(void) {
  unsigned char val;
  unsigned int  mask;

  val = 0;
  for (mask = 0x80; mask != 0; mask >>= 1) {
    LCD_SCL(0);
    LCD_DLY(1);
    if (LCD_SDI()) val |= mask;
    LCD_SCL(1);
    LCD_DLY(1);
  }
  return (val);
}

/*********************************************************************
*
*       TWI_Select
*
* Purpose:
*   Select/Unselect TWI
*/
static void TWI_Select(unsigned int select) {

  if (select) {
    // SCL is GPIO output set to high
    GPIOC->BSRR = PIN_SCL;
    // Set MOSI, MISO and SCK as GPIO pins, with pull-down/pull-up disabled
    GPIOC->CRH &= 0xFFF000FF;
    GPIOC->CRH |= 0x00033300;
  } else {
    // Connect MOSI, MISO, and SCK to SSP peripheral
    GPIOC->CRH &= 0xFFF000FF;
    GPIOC->CRH |= 0x000B8B00;
  }
}

/*********************************************************************
*
*       Exported code
*
*********************************************************************
*/
/*********************************************************************
*
*       LCD_X_Init
*
* Purpose:
*   This routine should be called from your application program
*   to set port pins to their initial values
*/
void LCD_X_Init(void) {

  // Enable clock for GPIOB,C AFIO and SPI3
  RCC->APB2ENR |= 0x00000019;
  RCC->APB1ENR |= 0x00008000;

  // Set SPI3 remap (use PC10..PC12)
  AFIO->MAPR   |= 0x10000000;

  // NCS is PB10, GPIO output set to high
  GPIOB->CRH   &= 0xFFFFF0FF;
  GPIOB->CRH   |= 0x00000300;
  GPIOB->CRL   &= 0xFFFFFFF0;
  GPIOB->CRL   |= 0x00000003;
  GPIOB->BSRR   = 0x00000401;

  // SPI3_SCK, SPI3_MISO, SPI3_MOSI are SPI pins
  GPIOC->CRH   &= 0xFFF000FF;
  GPIOC->CRH   |= 0x000B8B00;

  // Enable SPI in Master Mode, CPOL=1, CPHA=1
  // Max. 18 MBit used for Data Transfer @ 72MHz
  SPI3->CR1     = 0x0347;
  SPI3->CR2     = 0x0000;

  // Detect Device (TWI)
  LCD_CS(0);
  SPI_Transfer(0x70);           // Start + WR Register
  SPI_Transfer(0);
  LCD_CS(1);
  TWI_Select(1);
  LCD_CS(0);
  LCD_DIR(1);
  TWI_Send(0x73);               // Start + RD Data
  LCD_DIR(0);
  DevID = TWI_Recv();
  LCD_CS(1);
  TWI_Select(0);

  if (DevID != 0) return;

  // Detect Device (SPI)
  LCD_CS(0);
  SPI_Transfer(0x73);           // Start + RD Data
  SPI_Transfer(0);              // Dummy RD
  DevID  = SPI_Transfer(0) << 8;
  DevID |= SPI_Transfer(0);
  LCD_CS(1);
}

/*********************************************************************
*
*       LCD_X_Write00_16
*
* Purpose:
*   Write to controller, with A0 = 0
*/
void LCD_X_Write00_16(U16 c) {

  LCD_CS(0);
  SPI_Transfer(0x70);           // Start + WR Register
  if (DevID != 0x47) {
    SPI_Transfer((U8)(c >> 8));
  }
  SPI_Transfer((U8)(c));
  LCD_CS(1);
}

/*********************************************************************
*
*       LCD_X_Write01_16
*
* Purpose:
*   Write to controller, with A0 = 1
*/
void LCD_X_Write01_16(U16 c) {

  LCD_CS(0);
  SPI_Transfer(0x72);           // Start + WR Data
  SPI_Transfer((U8)(c >> 8));
  SPI_Transfer((U8)(c));
  LCD_CS(1);
}

/*********************************************************************
*
*       LCD_X_Read01_16
*
* Purpose:
*   Read from controller, with A0 = 1
*/
U16 LCD_X_Read01_16(void) {
  U16 c;

  if (DevID == 0x47) {          // Himax HX8347
    TWI_Select(1);
    LCD_CS(0);
    LCD_DIR(1);
    TWI_Send(0x73);             // Start + RD Data
    LCD_DIR(0);
    c = TWI_Recv();
    LCD_CS(1);
    TWI_Select(0);
  } else {
    LCD_CS(0);
    SPI_Transfer(0x73);         // Start + RD Data
    SPI_Transfer(0);            // Dummy RD
    c  = SPI_Transfer(0) << 8;
    c |= SPI_Transfer(0);
    LCD_CS(1);
  }

  return c;
}

/*********************************************************************
*
*       LCD_X_WriteM01_16
*
* Purpose:
*   Write multiple bytes to controller, with A0 = 1
*/
void LCD_X_WriteM01_16(U16 * pData, int NumWords) {

  LCD_CS(0);
  SPI_Transfer(0x72);           // Start + WR Data
  for (; NumWords; NumWords--) {
    SPI_Transfer((U8)(*pData >> 8));
    SPI_Transfer((U8)(*pData));
    pData++;
  }
  LCD_CS(1);
}

/*********************************************************************
*
*       LCD_X_ReadM01_16
*
* Purpose:
*   Read multiple bytes from controller, with A0 = 1
*/
void LCD_X_ReadM01_16(U16 * pData, int NumWords) {
  U32 v;

  if (DevID == 0x47) {          // Himax HX8347
    if (NumWords != 4) return;
    TWI_Select(1);
    LCD_CS(0);
    LCD_DIR(1);
    TWI_Send(0x73);             // Start + RD Data
    LCD_DIR(0);
    TWI_Recv();                 // Dummy RD
    v = (TWI_Recv() << 16) |
        (TWI_Recv() <<  8) |
        (TWI_Recv() <<  0);
     pData++;
    *pData++ = (v & 0x03F000) >> 10;
    *pData++ = (v & 0xFC0000) >> 16;
    *pData++ = (v & 0x000FC0) >>  4;    
    LCD_CS(1);
    TWI_Select(0);
  } else {
    LCD_CS(0);
    SPI_Transfer(0x73);         // Start + RD Data
    SPI_Transfer(0);            // Dummy RD
    SPI_Transfer(0);            // Dummy RD
    SPI_Transfer(0);            // Dummy RD
    SPI_Transfer(0);            // Dummy RD
    SPI_Transfer(0);            // Dummy RD
    for (; NumWords; NumWords--) {
      *pData  = SPI_Transfer(0) << 8;
      *pData |= SPI_Transfer(0);
      *pData++;
    }
    LCD_CS(1);
  }
}

/*************************** End of file ****************************/
