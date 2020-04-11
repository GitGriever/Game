/*-----------------------------------------------------------------------------
 *      RL-ARM - USB
 *-----------------------------------------------------------------------------
 *      Name:    Memory.c
 *      Purpose: USB Memory Storage Demo
 *      Rev.:    V4.70
 *-----------------------------------------------------------------------------
 *      This code is part of the RealView Run-Time Library.
 *      Copyright (c) 2004-2013 KEIL - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <RTL.h>
#include <File_Config.h>
#include <rl_usb.h>

FAT_VI *mc0;                            /* Media Control Block                */
Media_INFO info;


__task void init (void) {

  os_tsk_prio_self(100);
  mc0 = ioc_getcb (NULL);
  if (ioc_init (mc0) == 0) {
    ioc_read_info (&info, mc0);
    usbd_init();                        /* USB Device Initialization          */
    usbd_connect(__TRUE);               /* USB Device Connect                 */
  }
  os_tsk_delete_self();                 /* Terminate Task                     */
}

int main (void) {
  os_sys_init(init);                    /* Init RTX and start 'init'          */
}
