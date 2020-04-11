/******************************************************************************/
/* GLCD.h: Graphic LCD function prototypes and defines                        */
/******************************************************************************/
/* This file is part of the uVision/ARM development tools.                    */
/* Copyright (c) 2005-2012 Keil - An ARM Company. All rights reserved.        */
/* This software may only be used under the terms of a valid, current,        */
/* end user licence from KEIL for a compatible version of KEIL software       */
/* development tools. Nothing else gives you the right to use this software.  */
/******************************************************************************/

#ifndef _GLCD_H
#define _GLCD_H

/*------------------------------------------------------------------------------
  Color coding
  GLCD is coded:   15..11 red, 10..5 green, 4..0 blue  (unsigned short)  GLCD_R5, GLCD_G6, GLCD_B5   
  original coding: 17..12 red, 11..6 green, 5..0 blue                    ORG_R6,  ORG_G6,  ORG_B6

  ORG_R1..5 = GLCD_R0..4,  ORG_R0 = GLCD_R4
  ORG_G0..5 = GLCD_G0..5,
  ORG_B1..5 = GLCD_B0..4,  ORG_B0 = GLCD_B4
 *----------------------------------------------------------------------------*/
                            
/* GLCD RGB color definitions                                                 */
#define Black           0x0000      /*   0,   0,   0 */
#define Navy            0x000F      /*   0,   0, 128 */
#define DarkGreen       0x03E0      /*   0, 128,   0 */
#define DarkCyan        0x03EF      /*   0, 128, 128 */
#define Maroon          0x7800      /* 128,   0,   0 */
#define Purple          0x780F      /* 128,   0, 128 */
#define Olive           0x7BE0      /* 128, 128,   0 */
#define LightGrey       0xC618      /* 192, 192, 192 */
#define DarkGrey        0x7BEF      /* 128, 128, 128 */
#define Blue            0x001F      /*   0,   0, 255 */
#define Green           0x07E0      /*   0, 255,   0 */
#define Cyan            0x07FF      /*   0, 255, 255 */
#define Red             0xF800      /* 255,   0,   0 */
#define Magenta         0xF81F      /* 255,   0, 255 */
#define Yellow          0xFFE0      /* 255, 255, 0   */
#define White           0xFFFF      /* 255, 255, 255 */

extern void GLCD_Init           (void);
extern void GLCD_WindowMax      (void);
extern void GLCD_PutPixel       (uint32_t x, uint32_t y);
extern void GLCD_SetTextColor   (uint16_t color);
extern void GLCD_SetBackColor   (uint16_t color);
extern void GLCD_Clear          (uint16_t color);
extern void GLCD_DrawChar       (uint32_t x,  uint32_t y, uint32_t cw, uint32_t ch, uint8_t *c);
extern void GLCD_DrawLine       (uint32_t x,  uint32_t y, uint32_t len, uint32_t dir);
extern void GLCD_DrawString     (unsigned int x, unsigned int y, unsigned char fi, unsigned char *s);
extern void GLCD_DisplayChar    (uint32_t ln, uint32_t col, uint8_t fi, uint8_t  c);
extern void GLCD_DisplayString  (uint32_t ln, uint32_t col, uint8_t fi, uint8_t *s);
extern void GLCD_ClearLn        (uint32_t ln, uint8_t fi);
extern void GLCD_Bargraph       (uint32_t x,  uint32_t y, uint32_t w, uint32_t h, uint32_t val);
extern void GLCD_Bitmap         (uint32_t x,  uint32_t y, uint32_t w, uint32_t h, uint8_t *bitmap);
extern void GLCD_ScrollVertical (uint32_t dy);

extern void GLCD_WrCmd          (uint8_t cmd);
extern void GLCD_WrReg          (uint8_t reg, uint16_t val); 

#endif /* _GLCD_H */
