/*------------------------------------------------------------------------------
 *      RL-ARM - FlashFS
 *------------------------------------------------------------------------------
 *      Name:    MSD_File.c
 *      Purpose: File manipulation example program
 *------------------------------------------------------------------------------
 *      This code is part of the RealView Run-Time Library.
 *      Copyright (c) 2004-2013 KEIL - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <RTL.h>                      /* RTL kernel functions & defines       */
#include <rl_usb.h>                   /* RL-USB function prototypes           */
#include <stdio.h>                    /* standard I/O .h-file                 */
#include <ctype.h>                    /* character functions                  */
#include <string.h>                   /* string and memory functions          */
#include "File_Config.h"
#include "Terminal.h"
#include "Serial.h"


/* Command definitions structure. */
typedef struct scmd {
  char val[8];
  void (*func)(char *par);
} SCMD;

/* Command Functions */
static void cmd_capture (char *par);
static void cmd_type    (char *par);
static void cmd_rename  (char *par);
static void cmd_copy    (char *par);
static void cmd_delete  (char *par);
static void cmd_dir     (char *par);
static void cmd_format  (char *par);
static void cmd_help    (char *par);
static void cmd_fill    (char *par);

/* Local constants */
static const char intro[] =
  "\n\n\n\n\n\n\n\n"
  "+-----------------------------------------------------------------------+\n"
  "|          Mass Storage Device (MSD) File Manipulation example          |\n";
static const char help[] = 
  "+ command ------------------+ function ---------------------------------+\n"
  "| CAP \"fname\" [/A]          | captures serial data to a file            |\n"
  "|                           |  [/A option appends data to a file]       |\n"
  "| FILL \"fname\" [nnnn]       | create a file filled with text            |\n"
  "|                           |  [nnnn - number of lines, default=1000]   |\n"
  "| TYPE \"fname\"              | displays the content of a text file       |\n"
  "| REN \"fname1\" \"fname2\"     | renames a file 'fname1' to 'fname2'       |\n"
  "| COPY \"fin\" [\"fin2\"] \"fout\"| copies a file 'fin' to 'fout' file        |\n"
  "|                           |  ['fin2' option merges 'fin' and 'fin2']  |\n"
  "| DEL \"fname\"               | deletes a file                            |\n"
  "| DIR [\"mask\"]              | displays a list of files in the directory |\n"
  "| FORMAT [label [/FAT32]]   | formats the device                        |\n"
  "|                           | [/FAT32 option selects FAT32 file system] |\n"
  "| HELP  or  ?               | displays this help                        |\n"
  "+---------------------------+-------------------------------------------+\n";

static const SCMD cmd[] = {
  "CAP",    cmd_capture,
  "TYPE",   cmd_type,
  "REN",    cmd_rename,
  "COPY",   cmd_copy,
  "DEL",    cmd_delete,
  "DIR",    cmd_dir,
  "FORMAT", cmd_format,
  "HELP",   cmd_help,
  "FILL",   cmd_fill,
  "?",      cmd_help };

#define CMD_COUNT   (sizeof (cmd) / sizeof (cmd[0]))

/* Local variables */
static char in_line[160];

/* Local Function Prototypes */
static void dot_format (U64 val, char *sp);
static char *get_entry (char *cp, char **pNext);

/*------------------------------------------------------------------------------
 *        Initialize On Board LCD Module
 *----------------------------------------------------------------------------*/
static void init_display (void) {
  /* LCD Module init */
}

/*------------------------------------------------------------------------------
 *        Process input string for long or short name entry
 *----------------------------------------------------------------------------*/
static char *get_entry (char *cp, char **pNext) {
  char *sp, lfn = 0, sep_ch = ' ';

  if (cp == NULL) {                           /* skip NULL pointers           */
    *pNext = cp;
    return (cp);
  }

  for ( ; *cp == ' ' || *cp == '\"'; cp++) {  /* skip blanks and starting  "  */
    if (*cp == '\"') { sep_ch = '\"'; lfn = 1; }
    *cp = 0;
  }
 
  for (sp = cp; *sp != CR && *sp != LF && *sp != 0; sp++) {
    if ( lfn && *sp == '\"') break;
    if (!lfn && *sp == ' ' ) break;
  }

  for ( ; *sp == sep_ch || *sp == CR || *sp == LF; sp++) {
    *sp = 0;
    if ( lfn && *sp == sep_ch) { sp ++; break; }
  }

  *pNext = (*sp) ? sp : NULL;                 /* next entry                   */
  return (cp);
}

/*------------------------------------------------------------------------------
 *        Print size in dotted fomat
 *----------------------------------------------------------------------------*/
static void dot_format (U64 val, char *sp) {

  if (val >= (U64)1e12) {
    sp += sprintf (sp,"%d.",(U32)(val/(U64)1e12));
    val %= (U64)1e12;
    sp += sprintf (sp,"%03d.",(U32)(val/(U64)1e9));
    val %= (U64)1e9;
    sp += sprintf (sp,"%03d.",(U32)(val/(U64)1e6));
    val %= (U64)1e6;
    sprintf (sp,"%03d.%03d",(U32)(val/1000),(U32)(val%1000));
    return;
  }
  if (val >= (U64)1e9) {
    sp += sprintf (sp,"%d.",(U32)(val/(U64)1e9));
    val %= (U64)1e9;
    sp += sprintf (sp,"%03d.",(U32)(val/(U64)1e6));
    val %= (U64)1e6;
    sprintf (sp,"%03d.%03d",(U32)(val/1000),(U32)(val%1000));
    return;
  }
  if (val >= (U64)1e6) {
    sp += sprintf (sp,"%d.",(U32)(val/(U64)1e6));
    val %= (U64)1e6;
    sprintf (sp,"%03d.%03d",(U32)(val/1000),(U32)(val%1000));
    return;
  }
  if (val >= 1000) {
    sprintf (sp,"%d.%03d",(U32)(val/1000),(U32)(val%1000));
    return;
  }
  sprintf (sp,"%d",(U32)(val));
}

/*------------------------------------------------------------------------------
 *        Capture serial data to file
 *----------------------------------------------------------------------------*/
static void cmd_capture (char *par) {
  char *fname,*next;
  BOOL append,esc;
  U32  cnt;
  FILE *f;

  fname = get_entry (par, &next);
  if (fname == NULL) {
    printf ("\nFilename missing.\n");
    return;
  }
  append = __FALSE;
  if (next) {
    par = get_entry (next, &next);
    if ((strcmp (par, "/A") == 0) ||(strcmp (par, "/a") == 0)) {
      append = __TRUE;
    }
    else {
      printf ("\nCommand error.\n");
      return;
    }
  }
  printf ((append) ? "\nAppend data to file %s" :
                     "\nCapture data to file %s", fname);
  printf("\nPress ESC to stop.\n");
  f = fopen (fname,append ? "a" : "w"); /* open a file for writing            */
  if (f == NULL) {
    printf ("\nCan not open file!\n");  /* error when trying to open file     */
    return;
  }
  esc = __FALSE;
  do {
    cnt = getline (in_line, sizeof (in_line));
    if (cnt) {
      if (in_line[cnt-1] == ESC) {
        in_line[cnt-1] = 0;
        esc = __TRUE;
      }
      fputs (in_line, f);
    }
  } while (esc == __FALSE);
  fclose (f);                         /* close the output file                */
  printf ("\nFile closed.\n");
}

/*------------------------------------------------------------------------------
 *        Create a file and fill it with some text
 *----------------------------------------------------------------------------*/
static void cmd_fill (char *par) {
  char *fname, *next;
  FILE *f;
  int i,cnt = 1000;

  fname = get_entry (par, &next);
  if (fname == NULL) {
    printf ("\nFilename missing.\n");
    return;
  }
  if (next) {
    par = get_entry (next, &next);
    if (sscanf (par,"%d", &cnt) == 0) {
      printf ("\nCommand error.\n");
      return;
    }
  }

  f = fopen (fname, "w");               /* open a file for writing            */
  if (f == NULL) {
    printf ("\nCan not open file!\n");  /* error when trying to open file     */
    return;
  } 
  for (i = 0; i < cnt; i++)  {
    fprintf (f, "This is line # %d in file %s\n", i, fname);
    if (!(i & 0x3FF)) printf("."); fflush (stdout);
  }
  fclose (f);                           /* close the output file              */
  printf ("\nFile closed.\n");
}

/*------------------------------------------------------------------------------
 *        Read file and dump it to serial window
 *----------------------------------------------------------------------------*/
static void cmd_type (char *par) {
  char *fname,*next;
  FILE *f;
  int ch;

  fname = get_entry (par, &next);
  if (fname == NULL) {
    printf ("\nFilename missing.\n");
    return;
  }
  printf("\nRead data from file %s\n",fname);
  f = fopen (fname,"r");              /* open the file for reading            */
  if (f == NULL) {
    printf ("\nFile not found!\n");
    return;
  }
 
  while ((ch = fgetc (f)) != EOF) {   /* read the characters from the file    */
    putchar (ch);                     /* and write them on the screen         */
  }
  fclose (f);                         /* close the input file when done       */
  printf ("\nFile closed.\n");
}

/*------------------------------------------------------------------------------
 *        Rename a File
 *----------------------------------------------------------------------------*/
static void cmd_rename (char *par) {
  char *fname,*fnew,*next,dir;

  fname = get_entry (par, &next);
  if (fname == NULL) {
    printf ("\nFilename missing.\n");
    return;
  }
  fnew = get_entry (next, &next);
  if (fnew == NULL) {
    printf ("\nNew Filename missing.\n");
    return;
  }
  if (strcmp (fname,fnew) == 0) {
    printf ("\nNew name is the same.\n");
    return;
  }

  dir = 0;
  if (*(fname + strlen(fname) - 1) == '\\') {
    dir = 1;
  }

  if (frename (fname, fnew) == 0) {
    if (dir) {
      printf ("\nDirectory %s renamed to %s\n",fname,fnew);
    }
    else {
      printf ("\nFile %s renamed to %s\n",fname,fnew);
    }
  }
  else {
    if (dir) {
      printf ("\nDirectory rename error.\n");
    }
    else {
      printf ("\nFile rename error.\n");
    }
  }
}

/*------------------------------------------------------------------------------
 *        Copy a File
 *----------------------------------------------------------------------------*/
static void cmd_copy (char *par) {
  char *fname,*fnew,*fmer,*next;
  FILE *fin,*fout;
  U32 cnt,total;
  char buf[512];
  BOOL merge;

  fname = get_entry (par, &next);
  if (fname == NULL) {
    printf ("\nFilename missing.\n");
    return;
  }
  fmer = get_entry (next, &next);
  if (fmer == NULL) {
    printf ("\nNew Filename missing.\n");
    return;
  }
  fnew = get_entry (next, &next);
  if (fnew != NULL) {
    merge = __TRUE;
  }
  else {
    merge = __FALSE;
    fnew = fmer;
  }
  if ((strcmp (fname,fnew) == 0)        ||
      (merge && strcmp (fmer,fnew) == 0)) {
    printf ("\nNew name is the same.\n");
    return;
  }

  fin = fopen (fname,"r");            /* open the file for reading            */
  if (fin == NULL) {
    printf ("\nFile %s not found!\n",fname);
    return;
  }

  if (merge == __FALSE) {
    printf ("\nCopy file %s to %s\n",fname,fnew);
  }
  else {
    printf ("\nCopy file %s, %s to %s\n",fname,fmer,fnew);
  }
  fout = fopen (fnew,"w");            /* open the file for writing            */
  if (fout == NULL) {
    printf ("\nFailed to open %s for writing!\n",fnew);
    fclose (fin);
    return;
  }

  total = 0;
  while ((cnt = fread (&buf, 1, 512, fin)) != 0) {
    fwrite (&buf, 1, cnt, fout);
    total += cnt;
  }
  fclose (fin);                       /* close input file when done           */

  if (merge == __TRUE) {
    fin = fopen (fmer,"r");           /* open the file for reading            */
    if (fin == NULL) {
      printf ("\nFile %s not found!\n",fmer);
    }
    else {
      while ((cnt = fread (&buf, 1, 512, fin)) != 0) {
        fwrite (&buf, 1, cnt, fout);
        total += cnt;
      }
      fclose (fin);
    }
  }
  fclose (fout);
  dot_format (total, &buf[0]);
  printf ("\n%s bytes copied.\n", &buf[0]);
}

/*------------------------------------------------------------------------------
 *        Delete a File
 *----------------------------------------------------------------------------*/
static void cmd_delete (char *par) {
  char *fname,*next,dir;

  fname = get_entry (par, &next);
  if (fname == NULL) {
    printf ("\nFilename missing.\n");
    return;
  }

  dir = 0;
  if (*(fname + strlen(fname) - 1) == '\\') {
    dir = 1;
  }

  if (fdelete (fname) == 0) {
    if (dir) {
      printf ("\nDirectory %s deleted.\n",fname);
    }
    else {
      printf ("\nFile %s deleted.\n",fname);
    }
  }
  else {
    if (dir) {
      printf ("\nDirectory %s not found or not empty.\n",fname);
    }
    else {
      printf ("\nFile %s not found.\n",fname);
    }
  }
}

/*------------------------------------------------------------------------------
 *        Print a Directory
 *----------------------------------------------------------------------------*/
static void cmd_dir (char *par) {
  U64 fsize;
  U32 files,dirs,i;
  char temp[32],*mask,*next,ch;
  FINFO info;

  mask = get_entry (par, &next);
  if (mask == NULL) {
    mask = "*.*";
  } else if ((mask[1] == ':') && (mask[2] == 0)) {
    mask[2] = '*'; 
    mask[3] = '.'; 
    mask[4] = '*'; 
    mask[5] = 0; 
  } else if ((mask[2] == ':') && (mask[3] == 0)) {
    mask[3] = '*'; 
    mask[4] = '.'; 
    mask[5] = '*'; 
    mask[6] = 0; 
  }

  printf ("\nFile System Directory...");
  files = 0;
  dirs  = 0;
  fsize = 0;
  info.fileID  = 0;
  while (ffind (mask,&info) == 0) {
    if (info.attrib & ATTR_DIRECTORY) {
      i = 0;
      while (strlen((const char *)info.name+i) > 41) {
        ch = info.name[i+41];
        info.name[i+41] = 0;
        printf ("\n%-41s", &info.name[i]);
        info.name[i+41] = ch;
        i += 41;
      }
      printf ("\n%-41s    <DIR>       ", &info.name[i]);
      printf ("  %02d.%02d.%04d  %02d:%02d",
               info.time.day, info.time.mon, info.time.year,
               info.time.hr, info.time.min);
      dirs++;
    }
    else {
      dot_format (info.size, &temp[0]);
      i = 0;
      while (strlen((const char *)info.name+i) > 41) {
        ch = info.name[i+41];
        info.name[i+41] = 0;
        printf ("\n%-41s", &info.name[i]);
        info.name[i+41] = ch;
        i += 41;
      }
      printf ("\n%-41s %14s ", &info.name[i], temp);
      printf ("  %02d.%02d.%04d  %02d:%02d",
               info.time.day, info.time.mon, info.time.year,
               info.time.hr, info.time.min);
      fsize += info.size;
      files++;
    }
  }
  if (info.fileID == 0) {
    printf ("\nNo files...");
  }
  else {
    dot_format (fsize, &temp[0]);
    printf ("\n              %9d File(s)    %21s bytes", files, temp);
  }
  dot_format (ffree(mask), &temp[0]);
  if (dirs) {
    printf ("\n              %9d Dir(s)     %21s bytes free.\n", dirs, temp);
  }
  else {
    printf ("\n%56s bytes free.\n",temp);
  }
}

/*------------------------------------------------------------------------------
 *        Format Device
 *----------------------------------------------------------------------------*/
static void cmd_format (char *par) {
  char *label,*next,*opt;
  char arg[20];
  int  retv;

  label = get_entry (par, &next);
  if (label == NULL) {
    label = "KEIL";
  }
  strcpy (arg, label);
  opt = get_entry (next, &next);
  if (opt != NULL) {
    if ((strcmp (opt, "/FAT32") == 0) ||(strcmp (opt, "/fat32") == 0)) {
      strcat (arg, "/FAT32");
    }
  }
  printf ("\nFormat the Memory Device? [Y/N]\n");
  while ((retv = getkey()) == 0);
  if (retv == 'y' || retv == 'Y') {
    /* Format the Device with Label "KEIL". "*/
    if (fformat (arg) == 0) {
      printf ("Memory Device Formatted with Label %s\n", label);
    }
    else {
      printf ("Formatting failed.\n");
    }
  }
}

/*------------------------------------------------------------------------------
 *        Display Command Syntax help
 *----------------------------------------------------------------------------*/
static void cmd_help (char *par) {
  printf (help);
}

/*------------------------------------------------------------------------------
 *        Initialize a Flash Mass Storage Device
 *----------------------------------------------------------------------------*/
static int init_msd (char *letter) {
  U32 retv;

  retv = finit (letter);
  if (retv > 1) {
    printf ("\nMass Storage Device %s is Unformatted", letter);
    strcpy (&in_line[0], "KEIL\r\n");
    cmd_format (&in_line[0]);
    retv = 0;
  }
  return (~(retv) & 1);
}

/*------------------------------------------------------------------------------
 *        Main: 
 *----------------------------------------------------------------------------*/
int main (void) {
  char *sp,*cp,*next;
  U8    con, con_ex;
  U32   i;

  init_display ();                            /* initialize the display       */
  SER_Init ();                                /* initialize serial interface  */

  printf (intro);                             /* display example info         */
  printf (help);

  con = (init_msd ("U1:") << 1) |             /* initialize MSDs              */
         init_msd ("U0:");
  con_ex = con | 0x80;                        /* force initial display        */
  while (1) {
    usbh_engine_all();
    con = ((usbh_msc_status(0, 1) << 1) | usbh_msc_status(0, 0)) & 3;
    if (con ^ con_ex) {                       /* if connection changed        */
      if ((con ^ con_ex) & ~con & 1)          /* if device 0 not connected    */
        printf ("\nDisconnected Drive U0:");
      if ((con ^ con_ex) & ~con & 2)          /* if device 1 not connected    */
        printf ("\nDisconnected Drive U1:");
      if ((con ^ con_ex) &  con & 1) 
        con |= init_msd ("U0:");
      if ((con ^ con_ex) &  con & 2) 
        con |= init_msd ("U1:") << 1;
      if ((con ^ con_ex) &  con & 1)          /* if device 0 connected        */
        printf ("\nConnected Drive U0:");
      if ((con ^ con_ex) &  con & 2)          /* if device 1 connected        */
        printf ("\nConnected Drive U1:");
      if (con)
        printf ("\nCmd> ");                   /* display prompt               */
      else 
        printf ("\nNo drives connected, please connect any drive ... ");
      fflush (stdout);
      con_ex = con;
    }

                                              /* get command line input       */
    if (getline (in_line, sizeof (in_line))) {
      sp = get_entry (&in_line[0], &next);
      if (*sp == 0) {
        continue;
      }
      for (cp = sp; *cp && *cp != ' '; cp++) {
        *cp = toupper (*cp);                  /* command to upper-case        */
      }
      for (i = 0; i < CMD_COUNT; i++) {
        if (strcmp (sp, (const char *)&cmd[i].val)) {
          continue;
        }
        cmd[i].func (next);                   /* execute command function     */
        break;
      }
      if (i == CMD_COUNT) {
        printf ("\nCommand error\n");
      }
      printf ("\nCmd> ");                     /* display prompt               */
      fflush (stdout);
    }
  }
}

/*------------------------------------------------------------------------------
 * end of file
 *----------------------------------------------------------------------------*/
