/*----------------------------------------------------------------------------
 *      RL-ARM - RTX
 *----------------------------------------------------------------------------
 *      Name:    TRAFFIC.C
 *      Purpose: Traffic Light Controller example program
 *----------------------------------------------------------------------------
 *      This code is part of the RealView Run-Time Library.
 *      Copyright (c) 2004-2011 KEIL - An ARM Company. All rights reserved.
 *---------------------------------------------------------------------------*/

#include <stdio.h>                    /* standard I/O .h-file                */
#include <ctype.h>                    /* character functions                 */
#include <string.h>                   /* string and memory functions         */
#include <RTL.h>                      /* RTX kernel functions & defines      */
#include "STM32F10x.H"                /* STM32F10x definitions               */
#include "GLCD.h"
#include "Serial.h"
#include "LED.h"

#define __FI        1                   /* Font index 16x24                  */

#define RED     0                     /* I/O Pin:  RED    lamp output        */
#define YELLOW  1                     /* I/O Pin:  YELLOW lamp output        */
#define GREEN   2                     /* I/O Pin:  GREEN  lamp output        */
#define STOP    4                     /* I/O Pin:  STOP   lamp output        */
#define WALK    5                     /* I/O Pin:  WALK   lamp output        */

const char menu[] = 
   "\r\n"
   "+**** TRAFFIC LIGHT CONTROLLER using RVCT and RTX kernel *****+\r\n"
   "| This program is a simple Traffic Light Controller. Between  |\r\n"
   "| start time and end time the system controls a traffic light |\r\n"
   "| with pedestrian self-service.  Outside of this time range   |\r\n"
   "| the YELLOW caution lamp is blinking.                        |\r\n"
   "+ command -+ syntax -----+ function --------------------------+\r\n"
   "| Display  | D           | display times                      |\r\n"
   "| Time     | T hh:mm:ss  | set clock time                     |\r\n"
   "| Start    | S hh:mm:ss  | set start time                     |\r\n"
   "| End      | E hh:mm:ss  | set end time                       |\r\n"
   "+----------+-------------+------------------------------------+\r\n";


extern void getline (char *, int);    /* external function:  input line      */

OS_TID t_command;                     /* assigned task id of task: command   */
OS_TID t_clock;                       /* assigned task id of task: clock     */
OS_TID t_lights;                      /* assigned task id of task: lights    */
OS_TID t_keyread;                     /* assigned task id of task: keyread   */
OS_TID t_getesc;                      /* assigned task id of task: get_esc   */
OS_TID t_lcd;                         /* assigned task id of task: lcd       */
  
OS_MUT mut_GLCD;                      /* Mutex to controll GLCD access       */

struct time  {                        /* structure of the time record        */
   U8 hour;                           /* hour                                */
   U8 min;                            /* minute                              */
   U8 sec;                            /* second                              */
};

struct time ctime = { 12,  0,  0 };   /* storage for clock time values       */
struct time start = {  7, 30,  0 };   /* storage for start time values       */
struct time end   = { 18, 30,  0 };   /* storage for end   time values       */

char cmdline[16];                     /* storage for command input line      */
struct time rtime;                    /* temporary storage for entry time    */
BIT display_time = __FALSE;           /* flag signal cmd state display_time  */
BIT escape;                           /* flag: mark ESCAPE character enteRED */

#define ESC  0x1B                     /* ESCAPE character code               */

static U64 cmd_stack[800/8];          /* A bigger stack for command task     */

__task void init (void);              /* Function prototypes                 */
__task void clock (void) ;
__task void get_escape (void);
__task void command (void);
__task void blinking (void);
__task void lights (void);
__task void keyread (void);
__task void lcd (void);


/*---------------------------------------------------------------------------
  Delay function
  delays number of tick Systicks (happens every 10 ms)
 *---------------------------------------------------------------------------*/
void Delay (U32 tick) {
   os_dly_wait (tick);                                 /* wait for timeout   */
}

/*----------------------------------------------------------------------------
  Function that initializes User Buttons
 *----------------------------------------------------------------------------*/
void BUT_Init(void) {
  RCC->APB2ENR |= (1UL << 8);           /* Enable GPIOG clock                 */

  GPIOG->CRH   &= ~0x0000000F;          /* Configure the GPIO for Button      */
  GPIOG->CRH   |=  0x00000004;
}

/*----------------------------------------------------------------------------
  Function that initializes User Buttons
 *----------------------------------------------------------------------------*/
uint32_t BUT_Get(void) {
  uint32_t but_val;

  but_val = (GPIOG->IDR >> 8) & 0x01;
  return (but_val);
}

/*---------------------------------------------------------------------------
  Set Lights function
  controls LEDs and LCD display
 *---------------------------------------------------------------------------*/
void SetLights (U32 light, U32 on) {

   os_mut_wait(&mut_GLCD, 0xffff);
   GLCD_SetBackColor(White);
   if (light == RED) {
      GLCD_SetTextColor(Red);
      if (on == 0) {
         GLCD_DisplayChar(4, 15, __FI, 0x80+0);
      }
      else {
         GLCD_DisplayChar(4, 15, __FI, 0x80+1);
      }
   }
   if (light == YELLOW) {
      GLCD_SetTextColor(Yellow);
      if (on == 0) {
         GLCD_DisplayChar(5, 15, __FI, 0x80+0);
      }
      else {
         GLCD_DisplayChar(5, 15, __FI, 0x80+1);
      }
   }
   if (light == GREEN) {
      GLCD_SetTextColor(Green);
      if (on == 0) {
         GLCD_DisplayChar(6, 15, __FI, 0x80+0);
      }
      else {
         GLCD_DisplayChar(6, 15, __FI, 0x80+1);
      }
   }  
   if (light == STOP) {
      GLCD_SetTextColor(Red);
      if (on == 0) {
         GLCD_DisplayChar(5, 13, __FI, 0x80+12);
      }
      else {
         GLCD_DisplayChar(5, 13, __FI, 0x80+13);
      }
   }
   if (light == WALK) {
      GLCD_SetTextColor(Green);
      if (on == 0) {
         GLCD_DisplayChar(6, 13, __FI, 0x80+14);
      }
      else {
         GLCD_DisplayChar(6, 13, __FI, 0x80+15);
      }
   }
   os_mut_release(&mut_GLCD);

   if (on == 0) {
      LED_Off (light);
   }
   else {
      LED_On (light);
   }
}


/*----------------------------------------------------------------------------
  Main: Initialize and start RTX Kernel
 *---------------------------------------------------------------------------*/
int main (void) {                         /* program execution starts here   */

  GLCD_Init();                            /* initialize GLCD                 */
  SER_Init ();                            /* initialize serial interface     */
  LED_Init ();                            /* initialize LEDs                 */
  BUT_Init ();                            /* initialize Push Button          */

  os_sys_init (init);                     /* init and start with task 'INIT' */
}

/*----------------------------------------------------------------------------
  Task 1 'init': Initialize
 *---------------------------------------------------------------------------*/
__task void init (void) {

  t_clock   = os_tsk_create (clock,  0); /* start clock task                 */
  t_lcd     = os_tsk_create (lcd, 0);    /* start task lcd                   */
  os_dly_wait (50);
                                         /* start command task               */
  t_command = os_tsk_create_user (command,0,&cmd_stack,sizeof(cmd_stack));
  t_lights  = os_tsk_create (lights, 0); /* start lights task                */
  t_keyread = os_tsk_create (keyread,0); /* start keyread task               */
  os_tsk_delete_self ();                 /* STOP init task (no longer needed)*/
}

/*----------------------------------------------------------------------------
  Task 3 'clock'
 *---------------------------------------------------------------------------*/
__task void clock (void) {
  os_itv_set (100);                   /* set wait interval:  1 second        */
  while (1) {                         /* clock is an endless loop            */
    if (++ctime.sec == 60) {          /* calculate the second                */
      ctime.sec = 0;
      if (++ctime.min == 60) {        /* calculate the minute                */
        ctime.min = 0;
        if (++ctime.hour == 24) {     /* calculate the hour                  */
          ctime.hour = 0;
        }
      }
    }
    if (display_time) {               /* if command_status == display_time   */
      os_evt_set (0x0001,t_command);  /* signal to task command time changed */
    }
    os_itv_wait ();                   /* wait interval (already set to 1s )  */
  }
}

/*----------------------------------------------------------------------------
  readtime: convert line input to time values & store in rtime
 *---------------------------------------------------------------------------*/
char readtime (char *buffer) {
  int args;                                    /* number of arguments        */
  int hour,min,sec;

  rtime.sec = 0;                               /* preset second              */
  args = sscanf (buffer, "%d:%d:%d",           /* scan input line for        */
                         &hour, &min, &sec);   /* hour, minute and second    */
  
  if (hour > 23  ||  min > 59  ||              /* check for valid inputs     */
      sec > 59   ||  args < 2        ) {
     printf ("\r\n*** ERROR: INVALID TIME FORMAT\r\n");
     return (0);
  }
  rtime.hour = hour;                           /* setting a new time: hour   */
  rtime.min  = min;                            /* setting a new time: min    */
  rtime.sec  = sec;                            /* setting a new time: sec    */
  return (1);
}

/*----------------------------------------------------------------------------
  Task 7 'get_escape': check if ESC (escape character) was enteRED
 *---------------------------------------------------------------------------*/
__task void get_escape (void) {
  while (1)  {                                /* endless loop                */
    if (SER_GetChar () == ESC) {              /* If ESC entered, set flag    */ 
      escape = __TRUE;                        /* 'escape', set event flag of */
      os_evt_set (0x0002, t_command);         /* task 'command' and          */
    }
  }
}

/*----------------------------------------------------------------------------
  Task 2 'command': command processor
 *---------------------------------------------------------------------------*/
__task void command (void) {
  U32 i;

  printf (menu);                              /* display command menu        */
  while (1) {                                 /* endless loop                */
    printf ("\r\nCommand: ");                   /* display prompt              */
    getline (cmdline, sizeof (cmdline));      /* get command line input      */

    for (i = 0; cmdline[i] != 0; i++) {       /* convert to uppercase        */
      cmdline[i] = (char) toupper(cmdline[i]);
    }

    for (i = 0; cmdline[i] == ' '; i++);      /* skip blanks                 */

    switch (cmdline[i]) {                     /* proceed to command function */
      case 'D':                               /* Display Time Command        */
        printf ("Start Time: %02d:%02d:%02d    "
                "End Time: %02d:%02d:%02d\r\n",
                 start.hour, start.min, start.sec,
                 end.hour,   end.min,   end.sec);
        printf ("                        type ESC to abort\r");

        t_getesc = os_tsk_create (get_escape, 0);/* ESC check in display loop*/
        escape = __FALSE;                     /* clear escape flag           */
        display_time = __TRUE;                /* set display time flag       */
        os_evt_clr (0x0003, t_command);       /* clear pending signals       */

        while (!escape) {                     /* while no ESC enteRED        */
          printf ("Clock Time: %02d:%02d:%02d\r",   /* display time          */
                   ctime.hour, ctime.min, ctime.sec);
          os_evt_wait_or (0x0003, 0xffff);     /* wait for time change or ESC*/
        }

        os_tsk_delete (t_getesc);             /* terminate 'get_escape' task.*/
        display_time = __FALSE;               /* clear display time flag     */
        printf ("\r\n\r\n");
        break;

      case 'T':                               /* Set Time Command            */
        if (readtime (&cmdline[i+1])) {       /* read time input and         */
          ctime.hour = rtime.hour;            /* store in 'ctime'            */
          ctime.min  = rtime.min;
          ctime.sec  = rtime.sec;
        }
        break;

      case 'E':                               /* Set End Time Command        */
        if (readtime (&cmdline[i+1]))  {      /* read time input and         */
          end.hour = rtime.hour;              /* store in 'end'              */
          end.min  = rtime.min;
          end.sec  = rtime.sec;
        }
        break;

      case 'S':                               /* Set Start Time Command      */
        if (readtime (&cmdline[i+1]))  {      /* read time input and         */
          start.hour = rtime.hour;            /* store in 'start'            */
          start.min  = rtime.min;
          start.sec  = rtime.sec;
        }
        break;

      default:                                /* Error Handling              */
        printf (menu);                        /* display command menu        */
        break;
    }   
  }
}

/*----------------------------------------------------------------------------
  signalon: check if clock time is between start and end
 *---------------------------------------------------------------------------*/
char signalon (void) {
  if (memcmp (&start, &end, sizeof (struct time)) < 0)  {
    if (memcmp (&start, &ctime, sizeof (struct time)) < 0  &&
        memcmp (&ctime, &end,   sizeof (struct time)) < 0) {
      return (1);
    }
  }
  else { 
    if (memcmp (&end,   &ctime, sizeof (start)) > 0  &&
        memcmp (&ctime, &start, sizeof (start)) > 0) {
      return (1);
    }
  }
  return (0);                                 /* signal off, blinking on     */
}

/*----------------------------------------------------------------------------
  Task 4 'blinking': runs if current time is outside start & end time 
 *---------------------------------------------------------------------------*/
__task void blinking (void) {                 /* blink YELLOW light          */
  SetLights (RED, 0);                         /* all lights off              */
  SetLights (YELLOW, 0);
  SetLights (GREEN, 0);
  SetLights (STOP, 0);
  SetLights (WALK, 0);
  while (1) {                                 /* endless loop                */
    SetLights (YELLOW, 1);                    /* YELLOW light on             */
    os_dly_wait (30);                         /* wait for timeout: 30 ticks  */
    SetLights (YELLOW, 0);                    /* YELLOW light off            */
    os_dly_wait (30);                         /* wait for timeout: 30 ticks  */
    if (signalon ()) {                        /* if blinking time over       */
      os_tsk_create (lights, 0);              /* start lights                */
      os_tsk_delete_self ();                  /* and STOP blinking           */
    }
  }
}

/*----------------------------------------------------------------------------
  Task 5 'lights': executes if current time is between start & end time
 *---------------------------------------------------------------------------*/
__task void lights (void) {                   /* traffic light operation     */
  SetLights (RED, 1);                         /* RED & STOP lights on        */
  SetLights (STOP, 1);
  SetLights (YELLOW, 0);
  SetLights (GREEN, 0);
  SetLights (WALK, 0);
  while (1) {                                 /* endless loop                */
    os_dly_wait (50);                         /* wait for timeout: 50 ticks  */
    if (!signalon ()) {                       /* if traffic signal time over */
      os_tsk_create (blinking, 0);            /* start blinking              */
      os_tsk_delete_self ();                  /* STOP lights                 */
    }
    SetLights (YELLOW, 1);
    os_dly_wait (50);                         /* wait for timeout: 50 ticks  */
    SetLights (RED, 0);                       /* GREEN light for cars        */
    SetLights (YELLOW, 0);
    SetLights (GREEN, 1);
    os_evt_clr (0x0010, t_lights);
    os_dly_wait (50);                         /* wait for timeout: 50 ticks  */
    os_evt_wait_and (0x0010, 750);            /* wait for event with timeout */
    SetLights (YELLOW, 1);                    /* timeout value: 750 ticks    */
    SetLights (GREEN, 0);
    os_dly_wait (50);                         /* wait for timeout: 50 ticks  */
    SetLights (RED, 1);                       /* RED light for cars          */
    SetLights (YELLOW, 0);
    os_dly_wait (50);                         /* wait for timeout: 50 ticks  */
    SetLights (STOP, 0);                      /* GREEN light for WALKers     */    
    SetLights (WALK, 1);
    os_dly_wait (250);                        /* wait for timeout: 250 ticks */
    SetLights (STOP, 1);                      /* RED light for WALKers       */        
    SetLights (WALK, 0);
  }
}

/*----------------------------------------------------------------------------
  Task 6 'keyread': process key stroke from pedestrian push button
 *---------------------------------------------------------------------------*/
__task void keyread (void) {
  while (1) {                                 /* endless loop                */
    if (BUT_Get() == 0) {                     /* if key pressed              */
      os_evt_set (0x0010, t_lights);          /* send signal to task lights  */
    }
    os_dly_wait (5);                          /* wait for timeout: 5 ticks   */
  }
}

/*----------------------------------------------------------------------------
  Task 8 'lcd': LCD Control task
 *---------------------------------------------------------------------------*/
__task void lcd (void) {

  GLCD_Clear(White);                                /* Clear the LCD         */
  for (;;) {
    os_mut_wait(&mut_GLCD, 0xffff);
    GLCD_SetBackColor(Blue);                        /* Set the Back Color    */
    GLCD_SetTextColor(White);                       /* Set the Text Color    */
    GLCD_DisplayString(0, 0, __FI, "   MCBSTM32E Demo   ");
    GLCD_DisplayString(1, 0, __FI, "    RTX Traffic     ");
    GLCD_DisplayString(2, 0, __FI, "    www.keil.com    ");
    os_mut_release(&mut_GLCD);
    os_dly_wait (400);

    os_mut_wait(&mut_GLCD, 0xffff);
    GLCD_SetBackColor(Blue);                        /* Set the Back Color    */
    GLCD_SetTextColor(Red);                         /* Set the Text Color    */
    GLCD_DisplayString(0, 0, __FI, "   MCBSTM32E Demo   ");
    GLCD_DisplayString(1, 0, __FI, "    RTX Traffic     ");
    GLCD_DisplayString(2, 0, __FI, "    www.keil.com    ");
    os_mut_release(&mut_GLCD);
    os_dly_wait (400);
  }
}

/*----------------------------------------------------------------------------
  end of file
 *---------------------------------------------------------------------------*/
