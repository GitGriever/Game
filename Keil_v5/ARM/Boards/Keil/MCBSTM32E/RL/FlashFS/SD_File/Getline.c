/*----------------------------------------------------------------------------
 *      RL-ARM - FlashFS
 *----------------------------------------------------------------------------
 *      Name:    GETLINE.C
 *      Purpose: Line Edited Character Input
 *----------------------------------------------------------------------------
 *      This code is part of the RealView Run-Time Library.
 *      Copyright (c) 2004-2011 KEIL - An ARM Company. All rights reserved.
 *---------------------------------------------------------------------------*/

#include <RTL.h>
#include <stdio.h>
#include "SD_File.h"

/*----------------------------------------------------------------------------
 *      Line Editor
 *---------------------------------------------------------------------------*/
BOOL getline (char *lp, U32 n) {
  U32 cnt = 0;
  char c;

  do {
    c = getkey ();
    switch (c) {
      case _CNTLQ:                         /* ignore Control S/Q             */
      case _CNTLS:
        break;
      case _BS:
      case _DEL:
        if (cnt == 0) {
          break;
        }
        cnt--;                             /* decrement count                */
        lp--;                              /* and line pointer               */
        putchar (0x08);                    /* echo backspace                 */
        putchar (' ');
        putchar (0x08);
        fflush (stdout);
        break;
      case _ESC:
        *lp = 0;                           /* ESC - stop editing line        */
        return (__FALSE);
      case _CR:                            /* CR - done, stop editing line   */
        *lp = c;
        lp++;                              /* increment line pointer         */
        cnt++;                             /* and count                      */
        c = _LF;
      default:
        putchar (*lp = c);                 /* echo and store character       */
        fflush (stdout);
        lp++;                              /* increment line pointer         */
        cnt++;                             /* and count                      */
        break;
    }
  } while (cnt < n - 2  &&  c != _LF);     /* check limit and CR             */
  *lp = 0;                                 /* mark end of string             */
  return (__TRUE);
}

/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/
