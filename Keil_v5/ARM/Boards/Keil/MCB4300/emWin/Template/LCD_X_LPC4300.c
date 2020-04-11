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
File        : LCD_X_LPC4300.c
Purpose     : Port routines for LPC4300 SPI & RGB
----------------------------------------------------------------------
*/

#include "GUI.h"

/*********************************************************************
*
*       Hardware configuration
*
**********************************************************************
*/

#include <LPC43xx.h>

#define SDRAM_BASE_ADDR 0x28000000

/* SPI Interface: SSP0
 
   PINS: 
   - CS  = PF_1 (GPIO7[16])
   - SCK = PF_0 (SCK0)
   - SDO = PF_2 (MISO0)
   - SDI = PF_3 (MOSI0)
*/

#define PIN_CS      (1 << 16)

/* SPI_SR - bit definitions */
#define TFE         0x01
#define RNE         0x04
#define BSY         0x10

/* SCU pin configuration definitions */
#define SPI_PIN_SET ((1 << 7) | (1 << 6) | (1 << 5))
#define LCD_PIN_SET ((1 << 7) | (1 << 6) | (1 << 5))
#define LCD_NPR_SET ((1 << 5) | (1 << 4))

/* Clock Control Unit register bits */
#define CCU_CLK_CFG_RUN   (1 << 0)
#define CCU_CLK_CFG_AUTO  (1 << 1)
#define CCU_CLK_STAT_RUN  (1 << 0)

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/

/*********************************************************************
*
*       Static code
*
*********************************************************************
*/

#define LCD_CS(x) ((x) ? (LPC_GPIO_PORT->SET[7] = PIN_CS) : (LPC_GPIO_PORT->CLR[7] = PIN_CS))

/*********************************************************************
*
*       SPI_Transfer
*
* Purpose:
*   Transfer (Write/Read) 1 byte, MSB first
*/
static unsigned char SPI_Transfer(unsigned char byte) {
  LPC_SSP0->DR = byte;
  while (!(LPC_SSP0->SR & RNE));        // Wait for send to finish
  return (LPC_SSP0->DR);
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
  int  i;

  /* Connect base clock */
  LPC_CGU->BASE_SSP0_CLK = (1    << 11) |
                           (0x09 << 24) ; /* PLL1 is SSP0 clock source        */

  LPC_CGU->BASE_LCD_CLK  = (1    << 11) |
                           (0x10 << 24) ; /* IDIVE is clock source            */

  /* Enable GPIO register interface clock */
  LPC_CCU1->CLK_M4_GPIO_CFG = CCU_CLK_CFG_AUTO | CCU_CLK_CFG_RUN;
  while (!(LPC_CCU1->CLK_M4_GPIO_STAT & CCU_CLK_STAT_RUN));

  /* Enable SSP0 register interface clock */
  LPC_CCU1->CLK_M4_SSP0_CFG = CCU_CLK_CFG_AUTO | CCU_CLK_CFG_RUN;
  while (!(LPC_CCU1->CLK_M4_SSP0_STAT & CCU_CLK_STAT_RUN));

  /* Enable LCD clock */
  LPC_CCU1->CLK_M4_LCD_CFG  = CCU_CLK_CFG_AUTO | CCU_CLK_CFG_RUN;
  while (!(LPC_CCU1->CLK_M4_LCD_STAT & CCU_CLK_STAT_RUN));


  /* Configure SSP0 peripheral -----------------------------------------------*/
  LPC_SSP0->CR0  = (1<<7)|(1<<6)|0x7;   /* 8-bit transfer CPHA = 1, CPOL = 1  */
  LPC_SSP0->CPSR = 0x08;                /* SPI Clock = PCLK / (8 + 1)         */
  LPC_SSP0->CR1  = (0<<2)|(1<<1);       /* Enable SSP controller, master mode */

  /* Configure SPI peripheral pins -------------------------------------------*/
  LPC_SCU->SFSPF_0  = SPI_PIN_SET | 0;  /* PF_0: SCL (SSP0 pin)               */
  LPC_SCU->SFSPF_1  = SPI_PIN_SET | 4;  /* PF_1: CS  (GPIO7[16] pin)          */
  LPC_SCU->SFSPF_2  = SPI_PIN_SET | 2;  /* PF_2: SDO (SSP0 pin)               */
  LPC_SCU->SFSPF_3  = SPI_PIN_SET | 2;  /* PF_3: SDI (SSP0 pin)               */

  LPC_GPIO_PORT->DIR[7] |= PIN_CS;      /* Chip select pin is GPIO output     */


  /* Configure LCD controller pins -------------------------------------------*/
  LPC_SCU->SFSP4_1  = LCD_PIN_SET | 5;  /* P4_1:  LCD_VD19                    */
  LPC_SCU->SFSP4_2  = LCD_PIN_SET | 2;  /* P4_2:  LCD_VD3                     */
  LPC_SCU->SFSP4_5  = LCD_PIN_SET | 2;  /* P4_5:  LCD_VSYNC                   */
  LPC_SCU->SFSP4_6  = LCD_PIN_SET | 2;  /* P4_6:  LCD_EN                      */
  LPC_SCU->SFSP4_7  = LCD_PIN_SET | 0;  /* P4_7:  LCD_DOTCLK                  */
  LPC_SCU->SFSP4_9  = LCD_PIN_SET | 2;  /* P4_9:  LCD_VD11                    */
  LPC_SCU->SFSP4_10 = LCD_PIN_SET | 2;  /* P4_10: LCD_VD10                    */

  LPC_SCU->SFSP7_0  = LCD_NPR_SET | 0;  /* P7_0:  LCD_BL_EN                   */
  LPC_SCU->SFSP7_6  = LCD_PIN_SET | 3;  /* P7_6:  LCD_HSYNC                   */

  LPC_SCU->SFSP8_3  = LCD_PIN_SET | 3;  /* P8_3: LCD_VD12                     */
  LPC_SCU->SFSP8_4  = LCD_PIN_SET | 3;  /* P8_4: LCD_VD7                      */
  LPC_SCU->SFSP8_5  = LCD_PIN_SET | 3;  /* P8_5: LCD_VD6                      */
  LPC_SCU->SFSP8_6  = LCD_PIN_SET | 3;  /* P8_6: LCD_VD5                      */
  LPC_SCU->SFSP8_7  = LCD_PIN_SET | 3;  /* P8_7: LCD_VD4                      */

  LPC_SCU->SFSPB_0  = LCD_PIN_SET | 2;  /* PB_0: LCD_VD23                     */
  LPC_SCU->SFSPB_1  = LCD_PIN_SET | 2;  /* PB_1: LCD_VD22                     */
  LPC_SCU->SFSPB_2  = LCD_PIN_SET | 2;  /* PB_2: LCD_VD21                     */
  LPC_SCU->SFSPB_3  = LCD_PIN_SET | 2;  /* PB_3: LCD_VD20                     */
  LPC_SCU->SFSPB_4  = LCD_PIN_SET | 2;  /* PB_4: LCD_VD15                     */
  LPC_SCU->SFSPB_5  = LCD_PIN_SET | 2;  /* PB_5: LCD_VD14                     */
  LPC_SCU->SFSPB_6  = LCD_PIN_SET | 2;  /* PB_6: LCD_VD13                     */	


  /* Configure LCD controller ------------------------------------------------*/
  LPC_RGU->RESET_CTRL0 = (1U << 16);
  while ((LPC_RGU->RESET_ACTIVE_STATUS0 & (1U << 16)) == 0);

  LPC_LCD->CTRL &= ~(1 << 0);           /* Disable LCD                        */ 
  LPC_LCD->INTMSK = 0;                  /* Disable all interrupts             */

  LPC_LCD->UPBASE = SDRAM_BASE_ADDR;

  LPC_LCD->TIMH  = (7    << 24) |       /* Horizontal back porch              */
                   (3    << 16) |       /* Horizontal front porch             */
                   (3    <<  8) |       /* Horizontal sync pulse width        */
                   (14   <<  2) ;       /* Pixels-per-line                    */
  LPC_LCD->TIMV  = (3    << 24) |       /* Vertical back porch                */
                   (2    << 16) |       /* Vertical front porch               */
                   (3    << 10) |       /* Vertical sync pulse width          */
                   (319  <<  0) ;       /* Lines per panel                    */
  LPC_LCD->POL   = (1    << 26) |       /* Bypass pixel clock divider         */
                   (239  << 16) |       /* Clocks per line: num of LCDCLKs    */
                   (1    << 13) |       /* Invert panel clock                 */
                   (1    << 12) |       /* Invert HSYNC                       */
                   (1    << 11) ;       /* Invert VSYNC                       */  
  LPC_LCD->LE    = (1    << 16) |       /* LCDLE Enabled: 1, Disabled: 0      */
                   (9    <<  0) ;       /* Line-end delay: LCDCLK clocks - 1  */
  LPC_LCD->CTRL  = (1    << 11) |       /* LCD Power Enable                   */
                   (1    <<  5) |       /* 0 = STN display, 1: TFT display    */
                   (6    <<  1) ;       /* Bits per pixel: 16bpp (5:6:5)      */
  
  for (i = 0; i < 256; i++) {
    LPC_LCD->PAL[i] = 0;                /* Clear palette                      */
  }
  LPC_LCD->CTRL |= (1 <<  0);           /* LCD enable                         */

  /* Turn on backlight -------------------------------------------------------*/
  LPC_GPIO_PORT->DIR[3] |= (1 << 8);
  LPC_GPIO_PORT->SET[3]  = (1 << 8);
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


/*************************** End of file ****************************/
