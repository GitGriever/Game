/*----------------------------------------------------------------------------
 *      RL-ARM - USB
 *----------------------------------------------------------------------------
 *      Name:    usbd_user_msc.c
 *      Purpose: Mass Storage Device Class User module
 *      Rev.:    V4.70
 *----------------------------------------------------------------------------
 *      This code is part of the RealView Run-Time Library.
 *      Copyright (c) 2004-2013 KEIL - An ARM Company. All rights reserved.
 *---------------------------------------------------------------------------*/

#include <RTL.h>
#include <rl_usb.h>
#include <File_Config.h>
#include <string.h>

extern FAT_VI *nand0; 

void usbd_msc_init () {
}

void usbd_msc_read_sect (U32 block, U8 *buf, U32 num_of_blocks) {
  ioc_read_sect(block, buf, num_of_blocks, nand0);
}

void usbd_msc_write_sect (U32 block, U8 *buf, U32 num_of_blocks) {
  ioc_write_sect(block, buf, num_of_blocks, nand0);
}
