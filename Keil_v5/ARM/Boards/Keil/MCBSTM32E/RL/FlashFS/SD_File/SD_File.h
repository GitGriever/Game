/*----------------------------------------------------------------------------
 *      RL-ARM - FlashFS
 *----------------------------------------------------------------------------
 *      Name:    SD_FILE.H
 *      Purpose: File manipulation example definitions
 *----------------------------------------------------------------------------
 *      This code is part of the RealView Run-Time Library.
 *      Copyright (c) 2004-2011 KEIL - An ARM Company. All rights reserved.
 *---------------------------------------------------------------------------*/

#define _CNTLQ       0x11     /* Control Q       */
#define _CNTLS       0x13     /* Control S       */
#define _DEL         0x7F     /* Delete          */
#define _BS          0x08     /* Backspace       */
#define _CR          0x0D     /* Carriage Return */
#define _LF          0x0A     /* Line Feed       */
#define _ESC         0x1B     /* Escape          */

/* Command definitions structure. */
typedef struct scmd {
  char val[8];
  void (*func)(char *par);
} SCMD;

/* External functions */
extern BOOL getline (char *, U32);
extern void init_serial (void);
extern int  getkey (void);

/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/
