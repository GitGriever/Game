/*------------------------------------------------------------------------------
 *      RL-ARM
 *------------------------------------------------------------------------------
 *      Name:    Retarget.c
 *      Purpose: Retarget input and output to the Serial Communication Interface
 *------------------------------------------------------------------------------
 *      This code is part of the RealView Run-Time Library.
 *      Copyright (c) 2004-2012 KEIL - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <RTL.h>
#include <stdio.h>
#include <string.h>
#include <rt_sys.h>
#include "SER.h"

struct __FILE { int handle; /* Add whatever you need here */ };
FILE __stdout;

#ifdef STDIO

/*------------------------------------------------------------------------------
 * Write character to the Serial Port
 *----------------------------------------------------------------------------*/
int sendchar (int ch) {

  if (ch == '\n') 
    SER_PutChar ('\r');

  SER_PutChar (ch);

  return (ch);
}


/*------------------------------------------------------------------------------
 * Read character from the Serial Port
 *----------------------------------------------------------------------------*/
int getkey (void) {
  int ch;

  ch = SER_GetChar();

  if (ch < 0) 
    return 0;

  return ch;
}
#endif

/*--------------------------- fputc ------------------------------------------*/

int fputc (int ch, FILE *f) {
#ifdef STDIO
  return (sendchar (ch));
#endif
}

/*--------------------------- fgetc ------------------------------------------*/

int fgetc (FILE *f) {
#ifdef STDIO
  return (getkey ());
#endif
}

/*--------------------------- ferror -----------------------------------------*/

int ferror (FILE *f) {
  /* Your implementation of ferror */
  return EOF;
}

/*--------------------------- _ttywrch ---------------------------------------*/

void _ttywrch (int ch) {
#ifdef STDIO
  sendchar (ch);
#endif
}

/*--------------------------- _sys_exit --------------------------------------*/

void _sys_exit (int return_code) {
  /* Endless loop. */
  while (1);
}

/*------------------------------------------------------------------------------
 * end of file
 *----------------------------------------------------------------------------*/
