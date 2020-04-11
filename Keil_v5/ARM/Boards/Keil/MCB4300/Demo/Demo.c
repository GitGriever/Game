/*-----------------------------------------------------------------------------
 * Name:    Demo.c
 * Purpose: Demo example for MCB4300
 * Note(s):
 *-----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2004-2012 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <LPC43xx.h>                    /* LPC43xx Definitions                */
#include <stdio.h>
#include "I2C.h"
#include "LED.h"
#include "KBD.h"
#include "JOY.h"
#include "ADC.h"
#include "TSC.h"
#include "GLCD.h"
#include "TH_LM75.h"
#include "EEPROM.h"

/* Bargraph position definitions */
#define BAR_ST      (8*6+3)
#define BAR_LEN     (217)
#define BAR_HGT     (20)
#define BAR_LN(ln)  ((ln)*24)

/* Joystick position definitions */
#define JOY_X       (13*16)
#define JOY_Y       ( 5*24 + 6)

/* Button position definitions */
#define BTN_X       (9)
#define BTN_Y       (190)

/* bulp position definition */
#define BLP_X       (209)
#define BLP_Y       (190)

/* Temperature graph definitions */
#define GRAPH_SAMPLES 240               /* Number of samples to draw          */
#define GRAPH_Y_ORIG  220               /* Coordinate system Y origin (pixel) */
#define GRAPH_X_ORIG   32               /* Coordinate system X origin (pixel) */

#define AXIS_Y_LEN    120               /* Length of the Y axis               */
#define AXIS_X_LEN    250               /* Length of the X axis               */

/* Keys bit masks */
#define KEY_P4_0    1
#define KEY_P4_3    2
#define KEY_P4_4    4
#define KEY_WAKEUP  8
 
/* Mode definitions */
enum {
  M_INIT = 0,
  M_SCR1,
  M_SCR2,
};

/* EEPROM Logger definitions */
typedef struct {
  uint16_t MemSz;
  uint16_t WrAddr;
  uint16_t WrCount;
} EE_LOG;

EE_LOG Log;

/* Mode switch timeout variable */
uint32_t KeyTick, Measure;

/* GLCD string buffer */
#define STRINGBUF_LEN   21

char StringBuf[STRINGBUF_LEN];

/* Extern variables */
extern unsigned char Arrows_16bpp_red[];
extern unsigned char Button_16bpp[];
extern unsigned char Bulb_16bpp[];

/* Static variables */
static uint16_t Sample[GRAPH_SAMPLES];
static uint32_t LEDOn, LEDOff;
static int32_t  LEDDir = 1;


/*-----------------------------------------------------------------------------
  SysTick IRQ Handler @ 10 ms
 *----------------------------------------------------------------------------*/
void SysTick_Handler (void) {
  static uint8_t TickLed, TickKbd, TickTh;
  
  switch (TickLed++) {
    case  1: LEDOn   = 1; break;
    case  6: LEDOff  = 1; break;
    case 10: TickLed = 0; break;
  }

  if (KeyTick == 0) {
    TickKbd++;
    if (TickKbd == 50) {
      KeyTick = 1;
      TickKbd = 0;
    }
  }
  else TickKbd = 0;
  
  TickTh++;
  if (TickTh > 200) {
    TickTh  = 0;
    Measure = 1;
  }
}


/*-----------------------------------------------------------------------------
  Init EEPROM Logger
 *----------------------------------------------------------------------------*/
uint32_t LOG_Init (void) {
  uint32_t adr, cc, nc;
  uint8_t  cnt[2];

  Log.MemSz   = EE_GetSize();
  Log.WrCount = 0;

  /* Find last entry */
  for (adr = 0; adr < Log.MemSz; adr += 8) {
    Log.WrAddr  = adr;

    EE_Read (adr+2, cnt, 2);
    cc = (cnt[1] << 8) | cnt[0];

    EE_Read (adr+6, cnt, 2);
    nc = (cnt[1] << 8) | cnt[0];
        
    if (cc == 0xFFFF) {                 /* CC is empty entry, W = CC          */
      break;
    }    
    if (nc == 0xFFFF) {                 /* NC is empty entry, W = NC          */
      Log.WrAddr  += 4;
      Log.WrCount += 1;
      break;
    }
    if (cc > nc) {                      /* CC is last valid entry, W = NC     */
      if (!(cc == 0xFFFE && nc == 0x0000)) {
        /* This is not counter overlap, W = NC */
        Log.WrAddr  += 4;
        Log.WrCount += 1;
        break;
      }
    }
    Log.WrCount += 2;
  }
  return (0);
}


/*-----------------------------------------------------------------------------
  Write temperature to the EEPROM log
 *----------------------------------------------------------------------------*/
uint32_t LOG_Write (uint16_t temp) {
  uint32_t err;
  uint8_t  dat[4];

  dat[0] = (uint8_t)(temp        &  0x80);
  dat[1] = (uint8_t)(temp        >>    8);
  dat[2] = (uint8_t)(Log.WrCount &  0xFF);  
  dat[3] = (uint8_t)(Log.WrCount >>    8);  

  err = EE_Write (Log.WrAddr, dat, 4);

  Log.WrAddr += 4;
  if (Log.WrAddr >= Log.MemSz) {
    Log.WrAddr = 0;
  }

  Log.WrCount++;
  if (Log.WrCount == 0xFFFF) {
    Log.WrCount = 0;                    /* Max count is 0xFFFE                */
  }
  return (err);
}


/*-----------------------------------------------------------------------------
  Read temperature from the EEPROM log
 *----------------------------------------------------------------------------*/
uint32_t LOG_Read (uint16_t addr, uint16_t *temp, uint16_t *count) {
  uint8_t  dat[4];

  if (EE_Read (addr, dat, 4) == 0) {
    if ((dat[0] & 0x7F) == 0) {
      *temp  = (dat[1] << 8) | dat[0];
      *count = (dat[3] << 8) | dat[2];
      return (0);
    }
  }
  return (1);
}


/*-----------------------------------------------------------------------------
  Convert temperature to pixels along Y coordinate
 *----------------------------------------------------------------------------*/
uint32_t TempToY (uint16_t temp) {
  uint32_t t, px;

  t = (temp >> 8);

  if (t >= 40) return (GRAPH_Y_ORIG - AXIS_Y_LEN);
  if (t <= 20) return (GRAPH_Y_ORIG);

  px = (6 * (t-20));
  if (temp & 0x80) px += 3;

  return (GRAPH_Y_ORIG - px);
}


/*-----------------------------------------------------------------------------
  Connect two consecutive samples with a straight line
 *----------------------------------------------------------------------------*/
void ConnectSample (uint32_t x1, uint32_t y1, uint32_t y2) {
  int32_t i, k, len;

  if (y1 == y2) { 
    GLCD_PutPixel (x1 + 1, y2);
  }
  else {
    k = (y2 < y1) ? (-1) : (1);
    len = (y2 - y1);
    if (len < 0) len *= -1;

    for (i = 0; i < len; i++) {
      GLCD_PutPixel (x1, y1 + (k * i));
      if (i == len / 2) x1 += 1;
    }
  }
}


/*-----------------------------------------------------------------------------
  Draw temperature graph (without scaling)
 *----------------------------------------------------------------------------*/
void GraphDraw (void) {
  uint32_t c, cnt, x, y1, y2;
  uint16_t a, addr, sz;
  uint16_t temp, count;

  /* Determine if enough samples stored in memory */
  if (Log.WrCount < GRAPH_SAMPLES) {
    cnt = Log.WrCount;
  }
  else cnt = GRAPH_SAMPLES;

  sz = Log.MemSz;

  addr = Log.WrAddr;
  if (addr < (4 * cnt)) {
    c = cnt - Log.WrAddr/4;
    addr = sz - (c * 4);
  }
  else {
    addr -= (4 * cnt);
  }

  /* Redraw graph */
  for (a = addr, c = 0; c < cnt; c++, a += 4) {
    if (a == sz) a = 0;

    x = GRAPH_X_ORIG + 1 + c;
   
    /* Clear previous sample */
    GLCD_SetTextColor (Navy);

    if ((c + 1) < GRAPH_SAMPLES) {
      if (Sample[c] && (Sample[c + 1])) {
        y1 = TempToY (Sample[c]);
        y2 = TempToY (Sample[c + 1]);
        ConnectSample (x, y1, y2);
      }
    }
    
    /* Read and draw current sample */
err:
    if (LOG_Read (a, &temp, &count) == 0) { 
      /* Draw current sample */
      GLCD_SetTextColor (Green);      
      Sample[c] = temp;

      if (c > 0) {
        /* Connect samples */
        y1 = TempToY (Sample[c-1]);
        y2 = TempToY (Sample[c  ]);
        ConnectSample (x-1, y1, y2);
      }
      else {
        GLCD_PutPixel (x, TempToY(Sample[c]));
      }
    }
    else goto err;
  }
}


/*-----------------------------------------------------------------------------
  Switch between modes
 *----------------------------------------------------------------------------*/
uint32_t SwitchMode (uint32_t mode) {  
  uint32_t k, n, x_px, y_px;
  uint8_t axis_val[8];

  GLCD_Clear(White);

  switch (mode) {
    case M_INIT:
      /* Prepare display for ADC, Keys, Joystick */                                        
      GLCD_SetBackColor  (Blue);
      GLCD_SetTextColor  (White);
      GLCD_DisplayString (0, 0, 1, "       MCB4300      ");
      GLCD_DisplayString (1, 0, 1, "    Demo Example    ");
      GLCD_SetBackColor  (White);
      GLCD_SetTextColor  (Blue);
      GLCD_DisplayString (3, 0, 1, "AD value:           ");
      GLCD_DisplayString (4, 0, 1, "Buttons :           ");
      GLCD_DisplayString (6, 0, 1, "Joystick:           ");
#ifdef _EVAL_VERSION_     // use a text button instead of bitmap with MDK-Lite in order to meet the code size limit
      GLCD_SetTextColor(Green);
      GLCD_DisplayString (7, 0, 1, "\x92\x91\x91\x91\x91\x91\x91\x91\x93"); // characters 0x90-0x95 for button frame
      GLCD_DisplayString (8, 0, 1, "\x90 Touch \x90");
      GLCD_DisplayString (9, 0, 1, "\x95\x91\x91\x91\x91\x91\x91\x91\x94");
#else
      GLCD_Bitmap(BTN_X, BTN_Y, 120, 48, Button_16bpp);
#endif
      GLCD_Bitmap(BLP_X, BLP_Y,  26, 40, Bulb_16bpp);
      mode = M_SCR1;
      break;

    case M_SCR1:
      /* Prepare display for thermometer and EEPROM */
      GLCD_Clear(Navy);
      GLCD_SetBackColor  (Navy);
      GLCD_SetTextColor  (White);
      GLCD_DisplayString (0, 0, 1, "       MCB4300      ");
      GLCD_DisplayString (1, 0, 1, " Temperature Logger ");
      GLCD_DisplayString (3, 0, 1, "PCB @               ");
      
      GLCD_DrawLine (GRAPH_X_ORIG, GRAPH_Y_ORIG, AXIS_X_LEN, 1);
      GLCD_DrawLine (GRAPH_X_ORIG, GRAPH_Y_ORIG-AXIS_Y_LEN, AXIS_Y_LEN, 0);

      y_px = GRAPH_Y_ORIG;                          /* 20 Degrees of Celsious */
      GLCD_DrawString (GRAPH_X_ORIG-18, y_px-4, 0, "20"); 

      y_px = GRAPH_Y_ORIG - AXIS_Y_LEN/4;           /* 25 Degrees of Celsious */
      GLCD_DrawLine   (GRAPH_X_ORIG-3,  y_px,   7,    1);
      GLCD_DrawString (GRAPH_X_ORIG-18, y_px-4, 0, "25");

      y_px = GRAPH_Y_ORIG - AXIS_Y_LEN/2;           /* 30 Degrees of Celsious */
      GLCD_DrawLine   (GRAPH_X_ORIG-3,  y_px,   7,    1);
      GLCD_DrawString (GRAPH_X_ORIG-18, y_px-4, 0, "30");

      y_px = GRAPH_Y_ORIG - AXIS_Y_LEN*3/4;         /* 35 Degrees of Celsious */
      GLCD_DrawLine   (GRAPH_X_ORIG-3,  y_px,   7,    1);
      GLCD_DrawString (GRAPH_X_ORIG-18, y_px-4, 0, "35");

      y_px = GRAPH_Y_ORIG - AXIS_Y_LEN;             /* 40 Degrees of Celsious */
      GLCD_DrawLine   (GRAPH_X_ORIG-3,  y_px,   7,    1);
      GLCD_DrawString (GRAPH_X_ORIG-18, y_px-4, 0, "40");
      
      k = 1;
      n = AXIS_X_LEN / 5;
      for (x_px = GRAPH_X_ORIG+n; x_px < GRAPH_X_ORIG+AXIS_X_LEN; x_px += n) {
        GLCD_DrawLine (x_px, GRAPH_Y_ORIG-3,    7,    0);
        sprintf ((char *)axis_val, "x+%d", n * k++);
        GLCD_DrawString (x_px-12, GRAPH_Y_ORIG+6, 0, axis_val);
      }
      GLCD_DrawString (x_px-24,           GRAPH_Y_ORIG+6, 0, "Samples");
      GLCD_DrawString (GRAPH_X_ORIG-3, GRAPH_Y_ORIG+6, 0, "x");

      mode = M_SCR2;
      break;

    case M_SCR2:
      mode = M_INIT;
      break;
    default:
      mode = M_INIT;
      break;
  }
  return (mode);
}


/*-----------------------------------------------------------------------------
  Blink LEDs
 *----------------------------------------------------------------------------*/
void BlinkLed (void) {
  int32_t max_num = LED_Num() - 1;
  static int32_t num;

  if (LEDOn) {
    LEDOn = 0;
    LED_On (num);                         /* Turn specified LED on            */
  }

  if (LEDOff) {
    LEDOff = 0;
    LED_Off (num);                        /* Turn specified LED off           */

    num += LEDDir;                        /* Change LED number                */
    
    if (LEDDir == 1 && num == max_num) {
      LEDDir = -1;                        /* Change direction to down         */
    }
    else if (num == 0) {
      LEDDir =  1;                        /* Change direction to up           */
    }
  }
}


/*-----------------------------------------------------------------------------
  Main function
 *----------------------------------------------------------------------------*/
int main (void) {
  TH_DATA    therm;
  TSC_DATA   coord;
  uint32_t mode, ofs;
  uint32_t keyMsk, joyMsk, adcVal;
  int32_t key  = -1;
  int32_t joy  = -1;
  int32_t adc  = -1;

  SystemCoreClockUpdate();              /* Update SystemCoreClock variable    */
  SysTick_Config(SystemCoreClock/100-1);/* Generate interrupt each 10 ms      */

  I2C_Init ();                          /* I2C interface Init                 */
  LED_Init ();                          /* LED Init                           */
  KBD_Init ();                          /* Keyboard Init                      */
  JOY_Init ();                          /* Joystick Init                      */
  ADC_Init ();                          /* A/D Converter Init                 */
  TSC_Init ();                          /* Touchscreen Controller Init        */
  GLCD_Init();                          /* Graphical Display Init             */
  TH_Init  ();                          /* Thermometer Init                   */
  EE_Init  ();                          /* EEPROM Init                        */
  LOG_Init ();                          /* Logger Init                        */

  GLCD_Clear (White);
  GLCD_SetBackColor  (Blue);
  GLCD_SetTextColor  (White);
  GLCD_DisplayString (0, 0, 1, "       MCB4300      ");
  GLCD_DisplayString (1, 0, 1, "    Demo Example    ");
  GLCD_SetBackColor  (White);
  GLCD_SetTextColor  (Blue);
  
  GLCD_DisplayString (3, 0, 1, " Use WAKEUP key to  ");
  GLCD_DisplayString (4, 0, 1, "   switch example   ");
  GLCD_DisplayString (5, 0, 1, "        mode        ");

  GLCD_DisplayString (8, 0, 1, "  Press any key to  ");
  GLCD_DisplayString (9, 0, 1, "      continue      ");

  /* Wait for user to press the key */
  while (!KBD_GetKeys() && !JOY_GetKeys());
  mode = SwitchMode (M_INIT);
  
  KeyTick = 0;
  while (1) {
    BlinkLed();

    if (KeyTick && (KBD_GetKeys() & KEY_WAKEUP)) {
      mode = SwitchMode (mode);
      KeyTick = 0;
      /* Force refresh */
      key = -1;
      joy = -1;
      adc = -1;
    }

    switch (mode) {

      /* Show potentiometer, keys, joystick */
      case M_SCR1:
        keyMsk = KBD_GetKeys();         /* Show buttons state                 */
        if (key ^ keyMsk) {          
          GLCD_SetTextColor(Black);
          if (keyMsk & KEY_P4_0)    { GLCD_DisplayString (4,  9, 1, "P4_0"); }
          if (keyMsk & KEY_P4_3)    { GLCD_DisplayString (4, 14, 1, "P4_3"); }
          if (keyMsk & KEY_P4_4)    { GLCD_DisplayString (5,  9, 1, "P4_4"); }
          
          GLCD_SetTextColor(LightGrey);
          if (!(keyMsk & KEY_P4_0)) { GLCD_DisplayString (4,  9, 1, "P4_0"); }
          if (!(keyMsk & KEY_P4_3)) { GLCD_DisplayString (4, 14, 1, "P4_3"); }
          if (!(keyMsk & KEY_P4_4)) { GLCD_DisplayString (5,  9, 1, "P4_4"); }
        }

        joyMsk = JOY_GetKeys();         /* Show joystick arrows               */
        if (joy ^ joyMsk) {
          joy = joyMsk;

          /* Left arrow white/red? */
          ofs = (joy & JOY_DOWN)   ? (2416) : (0);
          GLCD_Bitmap(JOY_X+ 0, JOY_Y+20, 19, 14, &Arrows_16bpp_red[ofs + 0*532]);

          /* Right arrow white/red? */
          ofs = (joy & JOY_UP)     ? (2416) : (0);
          GLCD_Bitmap(JOY_X+35, JOY_Y+20, 19, 14, &Arrows_16bpp_red[ofs + 1*532]);

          /* Center dot white/red */
          ofs = (joy & JOY_CENTER) ? (2416) : (0);
          GLCD_Bitmap(JOY_X+21, JOY_Y+21, 12, 12, &Arrows_16bpp_red[ofs + 4*532]);

          /* Up arrow white/red? */
          ofs = (joy & JOY_LEFT)   ? (2416) : (0);
          GLCD_Bitmap(JOY_X+20, JOY_Y+ 0, 14, 19, &Arrows_16bpp_red[ofs + 2*532]);

          /* Down arrow white/red? */
          ofs = (joy & JOY_RIGHT)  ? (2416) : (0);
          GLCD_Bitmap(JOY_X+20, JOY_Y+35, 14, 19, &Arrows_16bpp_red[ofs + 3*532]);
        }

        ADC_StartCnv();                 /* Show A/D conversion bargraph       */
        adcVal = ADC_GetVal();
        if (adc ^ adcVal) {
          GLCD_SetTextColor(Green);
          GLCD_Bargraph (144, 3*24, 160, 20, adcVal);
        }
        
        if (TSC_TouchDet()) {           /* Show touch screen activity         */
          TSC_GetData (&coord);
        }
        else {
          coord.x = 0;
          coord.y = 0;
          coord.z = 0;          
        }
        
        if (coord.x >= 200 && coord.x <= 1000 &&
            coord.y >= 260 && coord.y <= 1650) {
#ifdef _EVAL_VERSION_     // use a text button instead of bitmap with MDK-Lite in order to meet the code size limit
          GLCD_SetTextColor(DarkGreen);
          GLCD_DisplayString (7, 0, 1, "\x92\x91\x91\x91\x91\x91\x91\x91\x93"); // characters 0x90-0x95 for button frame
          GLCD_DisplayString (8, 0, 1, "\x90 Touch \x90");
          GLCD_DisplayString (9, 0, 1, "\x95\x91\x91\x91\x91\x91\x91\x91\x94");
#else
          GLCD_Bitmap(BTN_X, BTN_Y, 120, 48, &Button_16bpp[1*11520]);
#endif
          GLCD_Bitmap(BLP_X, BLP_Y,  26, 40, &Bulb_16bpp[1*2080]);
        }
        else {
#ifdef _EVAL_VERSION_     // use a text button instead of bitmap with MDK-Lite in order to meet the code size limit
          GLCD_SetTextColor(Green);
          GLCD_DisplayString (7, 0, 1, "\x92\x91\x91\x91\x91\x91\x91\x91\x93"); // characters 0x90-0x95 for button frame
          GLCD_DisplayString (8, 0, 1, "\x90 Touch \x90");
          GLCD_DisplayString (9, 0, 1, "\x95\x91\x91\x91\x91\x91\x91\x91\x94");
#else
          GLCD_Bitmap(BTN_X, BTN_Y, 120, 48, &Button_16bpp[0*11520]);
#endif
          GLCD_Bitmap(BLP_X, BLP_Y,  26, 40, &Bulb_16bpp[0*2080]);
        }
        break;
        
      case M_SCR2:
        /* Show temperature and EEPROM graph */
        GLCD_SetTextColor (Green);

        if (Measure) {
          Measure = 0;
          /* Get temperature */
          if (TH_GetTemp (&therm) == 0) {
            sprintf (StringBuf, "%.1f C", (float)therm.Temp / 256);
            GLCD_DisplayString (3, 6, 1, (unsigned char *)StringBuf);
            LOG_Write (therm.Temp);
          }
          /* Draw temperature graph */
          GraphDraw();
        }
        break;

      default:
        mode = SwitchMode (mode);
        break;
    }    
  }
}
