/*----------------------------------------------------------------------------
 *      RL-ARM - USB
 *----------------------------------------------------------------------------
 *      Name:    USBD_Demo.c
 *      Purpose: USB Device Demonstration
 *      Rev.:    V4.70
 *----------------------------------------------------------------------------
 *      This code is part of the RealView Run-Time Library.
 *      Copyright (c) 2004-2013 KEIL - An ARM Company. All rights reserved.
 *---------------------------------------------------------------------------*/

#include <RTL.h>
#include <rl_usb.h>
#include <MK20D7.h>

#include "KBD.h"

void clock_init (void) { 
  /* PEE mode, MCCGOUTCLK = 48 MHz                                            */

  /* SIM->CLKDIV1: OUTDIV1=0,OUTDIV2=0,OUTDIV3=1,OUTDIV4=1,                   */
  SIM->CLKDIV1 = (uint32_t)0x00110000u; /* Update system prescalers           */

  /* Switch to FBE Mode                                                       */
  /* OSC0->CR: ERCLKEN=0, EREFSTEN=0, SC2P=0, SC4P=0, SC8P=0, SC16P=0         */
  OSC->CR = (uint8_t)0x00u;
  /* MCG->C7: OSCSEL=0                                                        */
  MCG->C7 = (uint8_t)0x00u;
  /* MCG->C2: RANGE0=2,HGO=0,EREFS=1,LP=0,IRCS=0                              */
  MCG->C2 = (uint8_t)0x24u;
  /* MCG->C1: CLKS=2,FRDIV=3,IREFS=0,IRCLKEN=1,IREFSTEN=0                     */
  MCG->C1 = (uint8_t)0x9Au;
  /* MCG->C4: DMX32=0,DRST_DRS=0                                              */
  MCG->C4 &= (uint8_t)~(uint8_t)0xE0u;
  /* MCG->C5: ??=0,PLLCLKEN=0,PLLSTEN=0,PRDIV0=3                              */
  MCG->C5 = (uint8_t)0x03u;
  /* MCG->C6: LOLIE=0,PLLS=0,CME=0,VDIV0=0                                    */
  MCG->C6 = (uint8_t)0x00u;	
  /* Check that the oscillator is running                                     */
  while((MCG->S & MCG_S_OSCINIT0_MASK) == 0u);
  /* Wait until external reference clock is selected as MCG output            */
  while((MCG->S & 0x0Cu) != 0x08u);

  /* Switch to PBE Mode                                                       */
  /* MCG_C5: PLLCLKEN=0,PLLSTEN=0,PRDIV0=3                                    */
  MCG->C5 = (uint8_t)0x03u;
  /* MCG->C6: LOLIE=0,PLLS=1,CME=0,VDIV0=0                                    */
  MCG->C6 = (uint8_t)0x40u;
  /* Wait until the source of the PLLS clock has switched to the PLL          */
  while((MCG->S & MCG_S_PLLST_MASK) == 0u);
  /* Wait until locked                                                        */
  while((MCG->S & MCG_S_LOCK0_MASK) == 0u);
  /* Switch to PEE Mode                                                       */
  /* MCG->C1: CLKS=0,FRDIV=3,IREFS=0,IRCLKEN=1,IREFSTEN=0                     */
  MCG->C1 = (uint8_t)0x1Au;
  /* Wait until output of the PLL is selected                                 */
  while((MCG->S & 0x0Cu) != 0x0Cu);
  /* Wait until locked                                                        */
  while((MCG->S & MCG_S_LOCK0_MASK) == 0u);
}

__task void init (void) {
  static U8 but_ex;
         U8 but;
         U8 buf[1];

  clock_init ();

  os_tsk_prio_self(100);
  usbd_init();                          /* USB Device Initialization          */
  usbd_connect(__TRUE);                 /* USB Device Connect                 */
  os_tsk_prio_self(1);
  while (1) {                           /* Loop forever                       */
    but = KBD_GetKeys();
    if (but ^ but_ex) {
      buf[0] = but;
      usbd_hid_get_report_trigger(0, buf, 1);
      but_ex = but;
    }  
  }
}

int main (void) {

  os_sys_init(init);                    /* Init RTX and start 'init'          */
}
