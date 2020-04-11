/*----------------------------------------------------------------------------
 * Name:    Measure.c
 * Purpose: Remote Measurement Recorder
 * Note(s): possible defines set in "options for target - C/C++ - Define"
 *            __USE_LCD   - enable Output on LCD
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2008-2011 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <stdio.h>                           /* standard I/O .h-file          */
#include <ctype.h>                           /* character functions           */
#include "stm32f10x.h"                       /* STM32F10x.h definitions       */
#include "GLCD.h"
#include "Serial.h"
#include "LED.h"
#include "ADC.h"
#include "Measure.h"

const char menu[] =
   "\r\n"
   "+***************** REMOTE MEASUREMENT RECORDER *****************+\r\n"
   "| This program is a simple Measurement Recorder. It is based on |\r\n"
   "| the STM32F103ZE and records the state of the Joystick         |\r\n"
   "| and the voltage on the analog input ADC1 channel14.           |\r\n"
   "+ command -+ syntax -----+ function ----------------------------+\r\n"
   "| Read     | R [n]       | read <n> recorded measurements       |\r\n"
   "| Display  | D           | display current measurement values   |\r\n"
   "| Time     | T hh:mm:ss  | set time                             |\r\n"
   "| Interval | I mm:ss.ttt | set interval time                    |\r\n"
   "| Clear    | C           | clear measurement records            |\r\n"
   "| Quit     | Q           | quit measurement recording           |\r\n"
   "| Start    | S           | start measurement recording          |\r\n"
   "+----------+-------------+--------------------------------------+\r\n";

struct interval setinterval;                 /* interval setting values       */
struct interval interval;                    /* interval counter              */

volatile int measurement_interval = 0;       /* measurement interval over     */
volatile int mdisplay = 0;                   /* measurement display requested */
volatile int startflag = 0;                  /* start measurement recording   */

struct mrec current;                         /* current measurements          */

#define SCNT 20                              /* number of records             */

struct mrec save_record[SCNT];               /* buffer for measurements       */
int sindex;                                  /* save index                    */
int savefirst;                               /* save first index              */

char ERROR_STR [] = "\r\n*** ERROR: %s\r\n"; /* ERROR message string in code  */

#define WRONGINDEX 0xffff                    /* error signal for wrong index  */


/*----------------------------------------------------------------------------
  Configure GPIOs used for Joystick
 *----------------------------------------------------------------------------*/
void JOY_Init(void) {

  RCC->APB2ENR |= (RCC_APB2ENR_IOPDEN |      /* Enable GPIOD clock            */
                   RCC_APB2ENR_IOPGEN  );    /* Enable GPIOG clock            */

  GPIOG->CRL   &= ~0xF0000000;          /* Configure the GPIO for Joystick    */
  GPIOG->CRL   |=  0x40000000;
  GPIOG->CRH   &= ~0xFFF00000;
  GPIOG->CRH   |=  0x44400000;

  GPIOD->CRL   &= ~0x0000F000;          /* Configure the GPIO for Joystick    */
  GPIOD->CRL   |=  0x00004000;
}


/*----------------------------------------------------------------------------
  Configure Timer/Counter 3
  Generate interrupt every 1ms @ 72 MHZ Timer clock
 *----------------------------------------------------------------------------*/
void TIM_Init (void) {

  RCC->APB1ENR |=  (1UL << 1);               /* enable TIM3 clock             */

  TIM3->PSC = 1;                             /* set prescaler to 1            */
  TIM3->ARR = 36000 - 1;

  TIM3->CR1 = 0;                             /* reset command register 1      */
  TIM3->CR2 = 0;                             /* reset command register 2      */

  TIM3->DIER = (1UL <<  0);                  /* enable update interrupt       */

  NVIC_EnableIRQ(TIM3_IRQn);                 /* enable TIM3 Interrupt         */

  TIM3->CR1 |= (1UL <<  0);                  /* enable counter                */
}


/*----------------------------------------------------------------------------
  Save current measurements in save_record
 *----------------------------------------------------------------------------*/
static void save_current_measurements (void) {
  save_record[sindex++] = current;           /* copy current measurements     */
  if (sindex == SCNT) sindex = 0;            /* check bounds of sindex        */
  if (sindex == savefirst)  {                /* check circular buffer limits  */
    if (++savefirst == SCNT)  savefirst = 0; /* check bounds of savefirst     */
  }
}


/*----------------------------------------------------------------------------
  Timer Counter 3 interrupt service function
 *----------------------------------------------------------------------------*/
void TIM3_IRQHandler (void) {

  if (TIM3->SR & (1<<0)) {                   /* UIF interrupt ?               */

  if (AD_done == 1) {
    AD_done = 0;
    ADC_StartCnv ();
  }

    if (measurement_interval)  {             /* measurement done ?            */
      save_current_measurements ();          /* yes -> save measurements      */
      measurement_interval = 0;              /* Save measurements done        */
    }
                                             /* check if interval is over     */
    if (interval.min  == 0 &&
        interval.sec  == 0 &&
        interval.msec == 0     )  {
      interval = setinterval;                /* set interval time again       */
      measurement_interval = startflag;      /* determine measurement flag    */
    }
    else  {                                  /* update interval time          */
      if (interval.msec-- == 0)  {           /* calculate millisecond         */
        interval.msec = 999;
        if (interval.sec-- == 0)  {          /* calculate second              */
          interval.sec = 59;
          interval.min--;                    /* calculate minute              */
        }
      }
    }
                                             /* update current time           */
    if (++current.time.msec == 1000)  {      /* update millisecond cnt        */
      current.time.msec = 0;

      if (++current.time.sec == 60)  {       /* update second counter         */
        current.time.sec = 0;

        if (++current.time.min == 60)  {     /* update minute counter         */
          current.time.min = 0;

          if (++current.time.hour == 24)  {  /* update hour counter           */
            current.time.hour = 0;
          }
        }
      }
    }	/* end of if( ++current.time.msec... */

    if (current.time.msec == 0) {
      LED_On (0);                            /* Turn LED#0 On                 */
    }
    if (current.time.msec == 100) {
      LED_Off(0);                            /* Turn LED#0 Off                */
    }
  
    if (measurement_interval || mdisplay)  { /* process measurement           */
      current.gpiod = GPIOD->IDR & 0x0008;   /* Joystick status PD.8          */
      current.gpiog = GPIOG->IDR & 0xE080;   /* Joystick status PG.13..15,PG.7*/

      /* converted values a transmitted via DMA */
      current.analog[0] = AD_last;           /* AD value (12 bit)             */

      mdisplay = 0;                          /* mdisplay = 0 for ready sig.   */
    }

    TIM3->SR &= ~(1UL <<  0);                /* clear UIF flag                */
  }

}


/*----------------------------------------------------------------------------
  Calculate first Read Index
 *----------------------------------------------------------------------------*/
static int read_index (char *buffer) {
  int index = 0;
  int args;

  if (setinterval.min == 0     &&            /* check if setinterval is       */
      setinterval.sec == 0     &&            /* below 1 second and            */
      setinterval.msec < 999   &&            /* measurements are collected    */
      startflag                   )  {
    printf (ERROR_STR, "QUIT MEASUREMENTS BEFORE READ");
    return (WRONGINDEX);                     /* no display on the fly if      */
  }                                          /* interval time < 1 second      */
  args = sscanf (buffer, "%d", &index);      /* scan input for read count     */
  if (args == 0  ||  index == 0  || args == EOF)  index = SCNT-1;
  index = sindex - index;                    /* calculate first read index    */
  if (index < 0) index += SCNT;              /* from read count               */
  return (index);
}


/*----------------------------------------------------------------------------
  Clear Measurement Records
 *----------------------------------------------------------------------------*/
static void clear_records (void) {
  int idx;                                   /* index for loop                */

  startflag = 0;                             /* stop measurement collecting   */
  sindex = savefirst = 0;                    /* reset circular buffer index   */
  for (idx = 0; idx != SCNT; idx++)  {       /* mark all records unused       */
    save_record[idx].time.hour = 0xff;       /* unused flag: hour = 0xff      */
  }
}

/*----------------------------------------------------------------------------
  Update LCD Display
 *----------------------------------------------------------------------------*/
void updLCD (unsigned char state) {
#ifdef __USE_LCD
  char sDisplay[20];                         /* buffer used for sprintf()     */

  switch (state) {
    case 0:
      GLCD_SetTextColor(Blue);
      GLCD_DisplayString(5, 0, 1, "GPIOD: ");
      GLCD_DisplayString(6, 0, 1, "GPIOG: ");
      GLCD_DisplayString(7, 0, 1, "ADC1.14: ");
      GLCD_DisplayString(8, 0, 1, "Time: ");
      break;

    case 1:
      GLCD_SetTextColor(Red);
      sprintf(sDisplay, "%02X", (int)current.gpiod);
      GLCD_DisplayString(5, 8, 1, (unsigned char *)sDisplay);
      sprintf(sDisplay, "%02X", (int)current.gpiog);
      GLCD_DisplayString(6, 8, 1, (unsigned char *)sDisplay);
      sprintf(sDisplay, "%4.2fV", (float)current.analog[0] * 3.3 / 0xFFF);
      GLCD_DisplayString(7, 9, 1, (unsigned char *)sDisplay);
      sprintf(sDisplay, "%02d:%02d:%02d.%03d", current.time.hour, current.time.min, current.time.sec, current.time.msec);
      GLCD_DisplayString(8, 6, 1, (unsigned char *)sDisplay);

      GLCD_SetTextColor(Green);
    
      if (current.gpiog & (1 << 14))         /* evaluate  "Left" Key          */
        GLCD_DisplayChar(5, 15, 1, 0x80+8);  /* key released                  */
      else
        GLCD_DisplayChar(5, 15, 1, 0x80+9);  /* key pressed                   */

      if (current.gpiog & (1 << 13))         /* evaluate "Right" Key          */
        GLCD_DisplayChar(5, 17, 1, 0x80+10);
      else
        GLCD_DisplayChar(5, 17, 1, 0x80+11);

      if (current.gpiog & (1 << 15))         /* evaluate    "Up" Key          */
        GLCD_DisplayChar(4, 16, 1, 0x80+4);
      else
        GLCD_DisplayChar(4, 16, 1, 0x80+5);

      if (current.gpiod & (1 <<  3))         /* evaluate  "Down" Key          */
        GLCD_DisplayChar(6, 16, 1, 0x80+6);
      else
        GLCD_DisplayChar(6, 16, 1, 0x80+7);

      if (current.gpiog & (1 <<  7))         /* evaluate   "Sel" Key          */
        GLCD_DisplayChar(5, 16, 1, 0x80+0);
      else
        GLCD_DisplayChar(5, 16, 1, 0x80+1);
      
      break;

    case 2:
      GLCD_SetTextColor(White);
      GLCD_ClearLn(4, 1);
      GLCD_ClearLn(5, 1);
      GLCD_ClearLn(6, 1);
      GLCD_ClearLn(7, 1);
      break;
  }
#endif

}

/*----------------------------------------------------------------------------
  Main Program
 *----------------------------------------------------------------------------*/
int main (void)  {                           /* main entry for program        */
  char cmdbuf [15];                          /* command input buffer          */
  int i;                                     /* index for command buffer      */
  int idx;                                   /* index for circular buffer     */

  /*--------------------------------------------------------------------------*/
  LED_Init();                                /* LED Initialization            */
  JOY_Init();                                /* Joystick Initialization       */
  SER_Init();                                /* UART Initialization           */
  TIM_Init();                                /* TIM3 Initialisation           */
  ADC_Init();                                /* ADC Initialization            */
  ADC_StartCnv ();                           /* start initial AD conversion   */


#ifdef __USE_LCD
  GLCD_Init();                                 /* Initialize the GLCD         */
  GLCD_Clear(White);                           /* Clear the GLCD              */

  GLCD_SetBackColor(Blue);                     /* Set the Back Color          */
  GLCD_SetTextColor(White);                    /* Set the Text Color          */
  GLCD_DisplayString(0, 0, 1, "   MCBSTM32E Demo   ");
  GLCD_DisplayString(1, 0, 1, "      Measure       ");
  GLCD_DisplayString(2, 0, 1, "    www.keil.com    ");
  GLCD_SetBackColor(White);                    /* Set the Back Color          */
  GLCD_SetTextColor(Blue);                     /* Set the Text Color          */
#endif

  clear_records ();                          /* initialize circular buffer    */
  printf ( menu );                           /* display command menu          */

  while (1)  {                               /* loop forever                  */
    printf ("\r\nCommand: ");
    getline (&cmdbuf[0], sizeof (cmdbuf));   /* input command line            */

    for (i = 0; cmdbuf[i] != 0; i++)  {      /* convert to upper characters   */
      cmdbuf[i] = toupper(cmdbuf[i]);
    }

    for (i = 0; cmdbuf[i] == ' '; i++);      /* skip blanks                   */

    switch (cmdbuf[i])  {                    /* proceed to command function   */

      case 'R':                              /* Read circular Buffer          */
        if ((idx = read_index (&cmdbuf[i+1])) == WRONGINDEX)  break;
        while (idx != sindex)  {             /* check end of table            */
          if (SER_CheckChar())  {            /* check serial interface        */
            if (SER_GetChar() == 0x1B) break;/* for escape character          */
          }
          if (save_record[idx].time.hour != 0xff)  {
            measure_display (save_record[idx]);  /* display record            */
            printf ("\r\n");
          }
          if (++idx == SCNT) idx = 0;        /* next circular buffer entry    */
        }
        break;

      case 'T':                              /* Enter Current Time            */
        set_time (&cmdbuf[i+1]);
        break;

      case 'I':                              /* Enter Interval Time           */
        set_interval (&cmdbuf[i+1]);
        break;

      case 'D':                              /* Display Command               */
        printf ("\r\nDisplay current Measurements: (ESC to abort)\r\n");
        updLCD (0);                          /* display initial Text          */
        do  {
          while (!(SER_CheckChar()))  {      /* check serial interface        */
            mdisplay = 1;                    /* request measurement           */
            while (mdisplay);                /* wait for measurement          */
            measure_display (current);       /* display values                */
            updLCD (1);                      /* update display                */
          }
        } while (SER_GetChar () != 0x1B);    /* escape terminates command     */
        printf ("\r\n\r\n");
        updLCD (2);                          /* clear used display lines      */
        break;

      case 'S':                              /* Start Command                 */
        printf ("\r\nStart Measurement Recording\r\n");
        startflag = 1;
        break;

      case 'Q':                              /* Quit Command                  */
        printf ("\r\nQuit Measurement Recording\r\n");
        startflag = 0;
        break;

      case 'C':                              /* Clear Command                 */
        printf ("\r\nClear Measurement Records\r\n");
        clear_records ();
        break;

      default:                               /* Error Handling                */
        printf (ERROR_STR, "UNKNOWN COMMAND");
        printf (menu);                       /* display command menu          */
        break;
    }
  }
}
