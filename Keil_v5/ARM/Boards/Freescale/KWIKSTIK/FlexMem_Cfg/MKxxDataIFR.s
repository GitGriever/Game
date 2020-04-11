;/**************************************************************************//**
; * @file     MKXXDataIFR.s
; * @brief    FlexNVM Configuration (Data Flash IFR)
; *           Freescale 'MKXX' Device Series 
; * @version  V1.01
; * @date     04. Oct 2010
; *------- <<< Use Configuration Wizard in Context Menu >>> ------------------
; *
; * @note
; * Copyright (C) 2010 ARM Limited. All rights reserved.
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
;      <i>  Determines the relative size of EEPROM within FlexRAM area
;          <2=>  (FlexRAM Size)*(1/1)
;          <3=>  (FlexRAM Size)*(1/2)
;          <4=>  (FlexRAM Size)*(1/4)
;          <5=>  (FlexRAM Size)*(1/8)
;          <6=>  (FlexRAM Size)*(1/16)
;          <7=>  (FlexRAM Size)*(1/32)
;          <8=>  (FlexRAM Size)*(1/64)
;          <9=>  (FlexRAM Size)*(1/128)
;          <15=> (FlexRAM Size)*(0)
;    <o0.4..5>  EEPROM Split Factor (EEESPLIT)
;      <i>  Determines the relative sizes of the two EEPROM subsystems
;          <0=>  Subsys A: EEESIZE*(1/8), Subsys B: EEESIZE*(7/8)
;          <1=>  Subsys A: EEESIZE*(1/4), Subsys B: EEESIZE*(3/4)
;          <2=>  Subsys A: EEESIZE*(1/2), Subsys B: EEESIZE*(1/2)
;    <o1.0..3>  FlexNVM Partition Code (DEPART)
;      <i>  Encoding of the data flash / EEPROM backup split within the FlexNVM memory block.
;      <i>  Determines the relative sizes of Data Flash and EEPROM Backup within FlexNVM area
;          <0=>  Data Flash: (FlexNVM Size)*(16/16), EEPROM Backup: (FlexNVM Size)*( 0/16)
;          <2=>  Data Flash: (FlexNVM Size)*(15/16), EEPROM Backup: (FlexNVM Size)*( 1/16)
;          <3=>  Data Flash: (FlexNVM Size)*(14/16), EEPROM Backup: (FlexNVM Size)*( 2/16)
;          <4=>  Data Flash: (FlexNVM Size)*(12/16), EEPROM Backup: (FlexNVM Size)*( 4/16)
;          <5=>  Data Flash: (FlexNVM Size)*( 8/16), EEPROM Backup: (FlexNVM Size)*( 8/16)
;          <6=>  Data Flash: (FlexNVM Size)*( 0/16), EEPROM Backup: (FlexNVM Size)*(16/16)
;          <10=> Data Flash: (FlexNVM Size)*( 1/16), EEPROM Backup: (FlexNVM Size)*(15/16)
;          <11=> Data Flash: (FlexNVM Size)*( 2/16), EEPROM Backup: (FlexNVM Size)*(14/16)
;          <12=> Data Flash: (FlexNVM Size)*( 4/16), EEPROM Backup: (FlexNVM Size)*(12/16)
nDATA0          EQU     0x03
nDATA1          EQU     0x05
DATA0           EQU     nDATA0:AND:0x3F
DATA1           EQU     nDATA1:AND:0x0F
; </h>

                AREA    |.ARM.__AT_0x13FFFFFC|, CODE, READONLY
                DCB     0xFF,  0xFF, DATA0, DATA1

                END
