/*----------------------------------------------------------------------------
 * Name:    CAN.h
 * Purpose: CAN interface header file
 * Note(s): 
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2010-2013 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#ifndef _can_h_
#define _can_h_

#define FORMAT_TYPE     (1 << 0)
#define FRAME_TYPE      (1 << 1)
#define STANDARD_TYPE   (0 << 0)
#define EXTENDED_TYPE   (1 << 0)
#define DATA_TYPE       (0 << 1)
#define REMOTE_TYPE     (1 << 1)

#define STANDARD_FORMAT  0
#define EXTENDED_FORMAT  1
#define DATA_FRAME       0
#define REMOTE_FRAME     1

typedef struct  {
  unsigned int   id;                    /* 29 bit identifier                  */
  unsigned char  data[8];               /* Data field                         */
  unsigned char  len;                   /* Length of data field in bytes      */
  unsigned char  ch;                    /* Object channel                     */
  unsigned char  format;                /* 0 - STANDARD,   1 - EXTENDED ID    */
  unsigned char  type;                  /* 0 - DATA FRAME, 1 - REMOTE FRAME   */
} CAN_msg;

/* Functions defined in module CAN.c */
int can_testmode        (int ctrl, int testmode);
int can_setup           (int ctrl);
int can_init            (int ctrl, int baudrate);
int can_start           (int ctrl);
int can_receive         (int ctrl, int ch, CAN_msg *msg);
int can_send            (int ctrl,         CAN_msg *msg);
int can_rx_object       (int ctrl, int ch, int id, int object_para);
int can_tx_object       (int ctrl, int ch,         int object_para);

#endif // _can_h_


