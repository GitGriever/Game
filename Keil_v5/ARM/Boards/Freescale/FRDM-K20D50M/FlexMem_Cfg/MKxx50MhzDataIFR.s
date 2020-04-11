;/**************************************************************************//**
; * @file     MKXX50MHZDataIFR.s
; * @brief    FlexNVM Configuration (Data Flash IFR)
; *           Freescale 'MKXX' 50 Mhz Device Series 
; * @version  V1.01
; * @date     26. Jan 2012
; *------- <<< Use Configuration Wizard in Context Menu >>> ------------------
; *
; * @note
; * Copyright (C) 2012 ARM Limited. All rights reserved.
; *
; * @par
; * ARM Limited (ARM) is supplying this software for use with Cortex-M 
; * processor based microcontrollers.  This file can be freely distributed 
; * within development tools that are supporting such ARM based processors. 
; *
; * @par
; * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
; * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
; * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
; * ARM SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
; * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
; *
; ******************************************************************************/

;  <h>  FlexNVM Configuration (Data Flash IFR)
;    <i>  Nonvolatile memory information memory in data flash
;    <i>  The following only applies to devices with FlexNVM
;    <o0.0..3>  EEPROM Size (EEESIZE)
;      <i>  Encoding of the total available FlexRAM for EEPROM use.
;          <3=> 2048 Bytes 
;          <4=> 1024 Bytes 
;          <5=>  512 Bytes  
;          <6=>  256 Bytes  
;          <7=>  128 Bytes  
;          <8=>   64 Bytes  
;          <9=>   32 Bytes  
;          <15=>   0 Bytes
;    <o0.4..5>  EEPROM Split Factor (EEESPLIT)
;      <i>  Determines the relative sizes of the two EEPROM subsystems
;          <0=>  Subsys A: EEESIZE*(1/8), Subsys B: EEESIZE*(7/8)
;          <1=>  Subsys A: EEESIZE*(1/4), Subsys B: EEESIZE*(3/4)
;          <2=>  Subsys A: EEESIZE*(1/2), Subsys B: EEESIZE*(1/2)
;          <3=>  Subsys A: EEESIZE*(1/2), Subsys B: EEESIZE*(1/2)
;    <o1.0..3>  FlexNVM Partition Code (DEPART)
;      <i>  Encoding of the data flash / EEPROM backup split within the FlexNVM memory block.
;      <i>  Determines the relative sizes of Data Flash and EEPROM Backup within FlexNVM area
;          <0=>  Data Flash: 32 KB, EEPROM Backup:  0 KB
;          <1=>  Data Flash: 24 KB, EEPROM Backup:  8 KB
;          <2=>  Data Flash: 16 KB, EEPROM Backup: 16 KB
;          <3=>  Data Flash:  0 KB, EEPROM Backup: 32 KB
;          <8=>  Data Flash:  0 KB, EEPROM Backup: 32 KB
;          <9=>  Data Flash:  8 KB, EEPROM Backup: 24 KB
;          <10=> Data Flash: 16 KB, EEPROM Backup: 16 KB
;          <11=> Data Flash: 32 KB, EEPROM Backup:  0 KB
nDATA0          EQU     0x34
nDATA1          EQU     0x02
DATA0           EQU     nDATA0:AND:0x3F
DATA1           EQU     nDATA1:AND:0x0F
; </h>

                AREA    |.ARM.__AT_0x13FFFFFC|, CODE, READONLY
                DCB     0xFF,  0xFF, DATA0, DATA1

                END
