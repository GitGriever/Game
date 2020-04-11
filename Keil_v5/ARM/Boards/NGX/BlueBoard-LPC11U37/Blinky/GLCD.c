/*----------------------------------------------------------------------------
 * Name:    GLCD.c
 * Purpose: Low level Graphic LCD functions
 * Note(s):
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


#include <LPC11Uxx.h>
#include "GLCD.h"
#include "Font_7x8_h.h"

/*********************** Hardware specific configuration **********************/

/* 
  PINS: 
   - CS        =       P1.13
   - EN        =       P0.12
   - RW        =       P0.13
   - RS        =       P0.14
   - nRST      =       P0.0
   - D0..D3    =       P1.19..22 (GPIO pin)
   - D4..D6    =       P1.26..28 (GPIO pin)
   - D7        =       P1.31     (GPIO pin)
   - Backlight =       P1.14
*/

#define PIN_EN     (1UL << 12)
#define PIN_RW     (1UL << 13)
#define PIN_RS     (1UL << 14)

#define PIN_D0     (1UL << 19)
#define PIN_D1     (1UL << 20)
#define PIN_D2     (1UL << 21)
#define PIN_D3     (1UL << 22)
#define PIN_D4     (1UL << 26)
#define PIN_D5     (1UL << 27)
#define PIN_D6     (1UL << 28)
#define PIN_D7     (1UL << 31)
#define PIN_DMSK   (PIN_D0 | PIN_D1 | PIN_D2 | PIN_D3 | PIN_D4 | PIN_D5 | PIN_D6 | PIN_D7)

#define PIN_NRST   (1UL <<  0)
#define PIN_CS     (1UL << 13)
#define PIN_BL     (1UL << 14)

#define PAGE0	     0xB0                 /* page 0 = Line 0                    */

/*------------------------- Speed dependant settings -------------------------*/

/* If processor works on high frequency delay has to be increased, it can be 
   increased by factor 2^N by this constant                                   */
#define DELAY_2N    4

/*---------------------- Graphic LCD size definitions ------------------------*/

#define GLCD_WIDTH       128            /* Screen Width (in pixels)           */
#define GLCD_HEIGHT       64            /* Screen Hight (in pixels)           */

/*--------------- Graphic LCD interface hardware definitions -----------------*/

/* Pin xx setting to 0 or 1                                                   */
#define LCD_CS(x)   ((x) ? (LPC_GPIO->SET[1] = PIN_CS) : (LPC_GPIO->CLR[1] = PIN_CS));
#define LCD_RW(x)   ((x) ? (LPC_GPIO->SET[0] = PIN_RW) : (LPC_GPIO->CLR[0] = PIN_RW));
#define LCD_RS(x)   ((x) ? (LPC_GPIO->SET[0] = PIN_RS) : (LPC_GPIO->CLR[0] = PIN_RS));


/************************ Local auxiliary functions ***************************/

/*----------------------------------------------------------------------------
  Delay in while loop cycles
    Parameter:    cnt:    number of while cycles to delay
    Return:
 *----------------------------------------------------------------------------*/
static void delay (int cnt) {

  cnt <<= DELAY_2N;
  while (cnt--);
}


/*----------------------------------------------------------------------------
  Write a command the LCD controller 
    Parameter:    cmd:    command to be written 
   Return: 
 *----------------------------------------------------------------------------*/
static __inline void wr_cmd (unsigned char cmd) {

  LCD_RS(0);
  LCD_CS(0);
  LCD_RW(0);

  LPC_GPIO->CLR[1] = PIN_DMSK;
  LPC_GPIO->SET[1] = ((cmd & 0x80) << 24) | ((cmd & 0x70) << 22) | ((cmd & 0x0F) << 19);
  delay(10);

  LCD_RW(1);
  LCD_CS(1);
  LCD_RS(1);
}


/*----------------------------------------------------------------------------
  Write data to the LCD controller 
    Parameter:    dat:    data to be written 
   Return: 
 *----------------------------------------------------------------------------*/
static __inline void wr_dat (unsigned char dat) {

  LCD_RS(1);
  LCD_CS(0);
  LCD_RW(0);

  LPC_GPIO->CLR[1] = PIN_DMSK;
  LPC_GPIO->SET[1] = ((dat & 0x80) << 24) | ((dat & 0x70) << 22) | ((dat & 0x0F) << 19);
  delay(10);

  LCD_RW(1);
  LCD_CS(1);
  LCD_RS(0);
}


/************************ Exported functions **********************************/

/*----------------------------------------------------------------------------
  Initialize the Graphic LCD controller
    Parameter:
    Return:
 *----------------------------------------------------------------------------*/
void GLCD_Init (void) { 

  LPC_SYSCON->SYSAHBCLKCTRL |= ((1UL <<  6) |   /* enable clock for GPIO0     */
                                (1UL << 16) );  /* enable clock for IOCON     */

  LPC_IOCON->TMS_PIO0_12  &= ~(7UL <<  0);      /* clear FUNC bits            */
  LPC_IOCON->TMS_PIO0_12  |=  (1UL <<  0);      /* P0.12 is GPIO              */
  LPC_IOCON->TDO_PIO0_13  &= ~(7UL <<  0);      /* clear FUNC bits            */
  LPC_IOCON->TDO_PIO0_13  |=  (1UL <<  0);      /* P0.13 is GPIO              */
  LPC_IOCON->TRST_PIO0_14 &= ~(7UL <<  0);      /* clear FUNC bits            */
  LPC_IOCON->TRST_PIO0_14 |=  (1UL <<  0);      /* P0.14 is GPIO              */

  LPC_GPIO->DIR[1] |= PIN_DMSK;                /* LCD D-pins are output       */
  LPC_GPIO->DIR[0] |= (PIN_RW | PIN_RS);
  LPC_GPIO->DIR[1] |= PIN_CS;                  /* LCD CS pin is output        */
  LPC_GPIO->DIR[0] |= PIN_NRST;
  LPC_GPIO->DIR[1] |= PIN_BL;                  /* Backlight pin is output     */


  LPC_GPIO->CLR[0] = PIN_NRST;
  LPC_GPIO->CLR[1] = PIN_BL;                   /* backlight off               */
  LPC_GPIO->SET[0] = PIN_NRST;

  wr_cmd(0xE2);                                /*  S/W RESWT                  */
  wr_cmd(0xA0);                                /*  ADC select                 */
  wr_cmd(0xC8);                                /*  SHL Normal                 */
  wr_cmd(0xA3);                                /*  LCD bias                   */
  wr_cmd(0x2F);                                /*  Power control              */
  wr_cmd(0x22);                                /*  reg resistor select        */
  wr_cmd(0x40);                                /*  Initial display line 40    */
  wr_cmd(0xA4);                                /*  Normal display             */
  wr_cmd(0xA6);                                /*  Reverce display a7         */
  wr_cmd(0x81);                                /*  Ref vg select mode         */
  wr_cmd(0x3f);                                /*  Ref vg reg select          */
  wr_cmd(0xB0);                                /*  Set page address           */
  wr_cmd(0x10);                                /*  Set coloumn addr  MSB      */
  wr_cmd(0x00);                                /*  Set coloumn addr LSB       */
  wr_cmd(0xAF);                                /*  Display ON                 */

  LPC_GPIO->SET[1] = PIN_BL;                   /* backlight on                */
}


/*----------------------------------------------------------------------------
  set cursor to position defined with page and col
    Parameter:      ln:       line number
                    y:        vertical position
    Return:
 *----------------------------------------------------------------------------*/
void GLCD_SetPos(unsigned int ln, unsigned int y)
{
	wr_cmd(ln);
	wr_cmd(((y & 0xF0) >> 4) | 0x10);
	wr_cmd(  y & 0x0F              );
}


/*----------------------------------------------------------------------------
  Clear display
    Parameter:
    Return:
 *----------------------------------------------------------------------------*/
void GLCD_Clear (void) {
  unsigned int i, j;

  for (i = 0; i < 8; i++) {
    GLCD_SetPos(PAGE0 + i , 0);
    for(j = 0; j < GLCD_WIDTH; j++) {
      wr_dat(0x00);
    }
  }

  GLCD_SetPos(PAGE0, 0);
}


/*----------------------------------------------------------------------------
  Draw character on given position
    Parameter:      ln:       line number
                    pos:      character position
                    c:        pointer to character bitmap
    Return:
 *----------------------------------------------------------------------------*/
void GLCD_DrawChar (unsigned int x, unsigned int y, unsigned char *c) {
  int i;

  GLCD_SetPos(PAGE0 + x , y * 7);

  for (i = 0; i < 7; i++) {
    wr_dat(*c++);
  }

}


/*----------------------------------------------------------------------------
  Disply character on given line
    Parameter:      ln:       line number
                    pos:      character position
                    c:        ascii character
    Return:
 *----------------------------------------------------------------------------*/
void GLCD_DisplayChar (unsigned int ln, unsigned int pos, unsigned char c) {

  c -= 32;
  GLCD_DrawChar (ln, pos, (unsigned char  *)&Font_7x8_h[c * 7]);
}


/*----------------------------------------------------------------------------
  Disply string on given line
    Parameter:      ln:       line number
                    pos:      character position
                    s:        pointer to string
    Return:
 *----------------------------------------------------------------------------*/
void GLCD_DisplayString (unsigned int ln, unsigned int pos, unsigned char *s) {

  while (*s) {
    GLCD_DisplayChar(ln, pos++, *s++);
  }
}


/*----------------------------------------------------------------------------
  Clear given line
    Parameter:      ln:       line number
    Return:
 *----------------------------------------------------------------------------*/
void GLCD_ClearLn (unsigned int ln) {
  unsigned char i;
  unsigned char buf[60];

  for (i = 0; i < (GLCD_WIDTH+6)/7; i++)
    buf[i] = ' ';
  buf[i+1] = 0;
  GLCD_DisplayString (ln, 0, buf);
}

/*----------------------------------------------------------------------------
  Draw bargraph
    Parameter:      ln:       line number
                    y:        vertical position
                    w:        maximum width of bargraph (in pixels)
                    val:      value of active bargraph (in 1/1024)
    Return:
 *----------------------------------------------------------------------------*/
void GLCD_Bargraph (unsigned int ln, unsigned int y, unsigned int w, unsigned int val) {
  int i;

  val = (val * w) >> 10;                /* Scale value                        */

  GLCD_SetPos(PAGE0 + ln , y);

  for (i = y; i <= GLCD_WIDTH; i++) {
    if (i >= (val + y)) {
      wr_dat(0x00);
    } else {
      wr_dat(0xFF);
    }
  }
}
