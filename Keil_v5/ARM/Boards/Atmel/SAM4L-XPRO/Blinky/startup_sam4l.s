;/*****************************************************************************
; * @file     startup_SAM4L.s
; * @brief    CMSIS Cortex-M4 Core Device Startup File
; *           for the Atmel SAM4L Device Series
; * @version  V1.00
; * @date     18. October 2012
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
;/*
;//-------- <<< Use Configuration Wizard in Context Menu >>> ------------------
;*/


; <h> Stack Configuration
;   <o> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Stack_Size      EQU     0x00000200

                AREA    STACK, NOINIT, READWRITE, ALIGN=3
Stack_Mem       SPACE   Stack_Size
__initial_sp


; <h> Heap Configuration
;   <o>  Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Heap_Size       EQU     0x00000200

                AREA    HEAP, NOINIT, READWRITE, ALIGN=3
__heap_base
Heap_Mem        SPACE   Heap_Size
__heap_limit


                PRESERVE8
                THUMB


; Vector Table Mapped to Address 0 at Reset

                AREA    RESET, DATA, READONLY
                EXPORT  __Vectors
                EXPORT  __Vectors_End
                EXPORT  __Vectors_Size

__Vectors       DCD     __initial_sp              ; Top of Stack
                DCD     Reset_Handler             ; Reset Handler
                DCD     NMI_Handler               ; NMI Handler
                DCD     HardFault_Handler         ; Hard Fault Handler
                DCD     MemManage_Handler         ; MPU Fault Handler
                DCD     BusFault_Handler          ; Bus Fault Handler
                DCD     UsageFault_Handler        ; Usage Fault Handler
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     SVC_Handler               ; SVCall Handler
                DCD     DebugMon_Handler          ; Debug Monitor Handler
                DCD     0                         ; Reserved
                DCD     PendSV_Handler            ; PendSV Handler
                DCD     SysTick_Handler           ; SysTick Handler

                ; Configurable interrupts
                DCD     HFLASHC_Handler           ; 16+ 0:
                DCD     PDCA_0_Handler            ; 16+ 1:
                DCD     PDCA_1_Handler            ; 16+ 2:
                DCD     PDCA_2_Handler            ; 16+ 3:
                DCD     PDCA_3_Handler            ; 16+ 4:
                DCD     PDCA_4_Handler            ; 16+ 5:
                DCD     PDCA_5_Handler            ; 16+ 6:
                DCD     PDCA_6_Handler            ; 16+ 7:
                DCD     PDCA_7_Handler            ; 16+ 8:
                DCD     PDCA_8_Handler            ; 16+ 9:
                DCD     PDCA_9_Handler            ; 16+10:
                DCD     PDCA_10_Handler           ; 16+11:
                DCD     PDCA_11_Handler           ; 16+12:
                DCD     PDCA_12_Handler           ; 16+13:
                DCD     PDCA_13_Handler           ; 16+14:
                DCD     PDCA_14_Handler           ; 16+15:
                DCD     PDCA_15_Handler           ; 16+16:
                DCD     CRCCU_Handler             ; 16+17:
                DCD     USBC_Handler              ; 16+18:
                DCD     PEVC_TR_Handler           ; 16+19:
                DCD     PEVC_OV_Handler           ; 16+20:
                DCD     AESA_Handler              ; 16+21:
                DCD     PM_Handler                ; 16+22:
                DCD     SCIF_Handler              ; 16+23:
                DCD     FREQM_Handler             ; 16+24:
                DCD     GPIO_0_Handler            ; 16+25:
                DCD     GPIO_1_Handler            ; 16+26:
                DCD     GPIO_2_Handler            ; 16+27:
                DCD     GPIO_3_Handler            ; 16+28:
                DCD     GPIO_4_Handler            ; 16+29:
                DCD     GPIO_5_Handler            ; 16+30:
                DCD     GPIO_6_Handler            ; 16+31:
                DCD     GPIO_7_Handler            ; 16+32:
                DCD     GPIO_8_Handler            ; 16+33:
                DCD     GPIO_9_Handler            ; 16+34:
                DCD     GPIO_10_Handler           ; 16+35:
                DCD     GPIO_11_Handler           ; 16+36:
                DCD     BPM_Handler               ; 16+37:
                DCD     BSCIF_Handler             ; 16+38:
                DCD     AST_ALARM_Handler         ; 16+39:
                DCD     AST_PER_Handler           ; 16+40:
                DCD     AST_OVF_Handler           ; 16+41:
                DCD     AST_READY_Handler         ; 16+42:
                DCD     AST_CLKREADY_Handler      ; 16+43:
                DCD     WDT_Handler               ; 16+44:
                DCD     EIC_1_Handler             ; 16+45:
                DCD     EIC_2_Handler             ; 16+46:
                DCD     EIC_3_Handler             ; 16+47:
                DCD     EIC_4_Handler             ; 16+48:
                DCD     EIC_5_Handler             ; 16+49:
                DCD     EIC_6_Handler             ; 16+50:
                DCD     EIC_7_Handler             ; 16+51:
                DCD     EIC_8_Handler             ; 16+52:
                DCD     IISC_Handler              ; 16+53:
                DCD     SPI_Handler               ; 16+54:
                DCD     TC00_Handler              ; 16+55:
                DCD     TC01_Handler              ; 16+56:
                DCD     TC02_Handler              ; 16+57:
                DCD     TC10_Handler              ; 16+58:
                DCD     TC11_Handler              ; 16+59:
                DCD     TC12_Handler              ; 16+60:
                DCD     TWIM0_Handler             ; 16+61:
                DCD     TWIS0_Handler             ; 16+62:
                DCD     TWIM1_Handler             ; 16+63:
                DCD     TWIS1_Handler             ; 16+64:
                DCD     USART0_Handler            ; 16+65:
                DCD     USART1_Handler            ; 16+66:
                DCD     USART2_Handler            ; 16+67:
                DCD     USART3_Handler            ; 16+68:
                DCD     ADCIFE_Handler            ; 16+69:
                DCD     DACC_Handler              ; 16+70:
                DCD     ACIFC_Handler             ; 16+71:
                DCD     ABDACB_Handler            ; 16+72:
                DCD     TRNG_Handler              ; 16+73:
                DCD     PARC_Handler              ; 16+74:
                DCD     CATB_Handler              ; 16+75:
                DCD     0                         ; 16+76: one not used
                DCD     TWIM2_Handler             ; 16+77:
                DCD     TWIM3_Handler             ; 16+78:
                DCD     LCDCA_Handler             ; 16+79:
__Vectors_End

__Vectors_Size  EQU     __Vectors_End - __Vectors

                AREA    |.text|, CODE, READONLY


; Reset Handler

Reset_Handler   PROC
                EXPORT  Reset_Handler             [WEAK]
                IMPORT  SystemInit
                IMPORT  __main
                LDR     R0, =SystemInit
                BLX     R0
                LDR     R0, =__main
                BX      R0
                ENDP


; Dummy Exception Handlers (infinite loops which can be modified)

NMI_Handler     PROC
                EXPORT  NMI_Handler               [WEAK]
                B       .
                ENDP
HardFault_Handler\
                PROC
                EXPORT  HardFault_Handler         [WEAK]
                B       .
                ENDP
MemManage_Handler\
                PROC
                EXPORT  MemManage_Handler         [WEAK]
                B       .
                ENDP
BusFault_Handler\
                PROC
                EXPORT  BusFault_Handler          [WEAK]
                B       .
                ENDP
UsageFault_Handler\
                PROC
                EXPORT  UsageFault_Handler        [WEAK]
                B       .
                ENDP
SVC_Handler     PROC
                EXPORT  SVC_Handler               [WEAK]
                B       .
                ENDP
DebugMon_Handler\
                PROC
                EXPORT  DebugMon_Handler          [WEAK]
                B       .
                ENDP
PendSV_Handler  PROC
                EXPORT  PendSV_Handler            [WEAK]
                B       .
                ENDP
SysTick_Handler PROC
                EXPORT  SysTick_Handler           [WEAK]
                B       .
                ENDP

Default_Handler PROC

                EXPORT  HFLASHC_Handler           [WEAK]
                EXPORT  PDCA_0_Handler            [WEAK]
                EXPORT  PDCA_1_Handler            [WEAK]
                EXPORT  PDCA_2_Handler            [WEAK]
                EXPORT  PDCA_3_Handler            [WEAK]
                EXPORT  PDCA_4_Handler            [WEAK]
                EXPORT  PDCA_5_Handler            [WEAK]
                EXPORT  PDCA_6_Handler            [WEAK]
                EXPORT  PDCA_7_Handler            [WEAK]
                EXPORT  PDCA_8_Handler            [WEAK]
                EXPORT  PDCA_9_Handler            [WEAK]
                EXPORT  PDCA_10_Handler           [WEAK]
                EXPORT  PDCA_11_Handler           [WEAK]
                EXPORT  PDCA_12_Handler           [WEAK]
                EXPORT  PDCA_13_Handler           [WEAK]
                EXPORT  PDCA_14_Handler           [WEAK]
                EXPORT  PDCA_15_Handler           [WEAK]
                EXPORT  CRCCU_Handler             [WEAK]
                EXPORT  USBC_Handler              [WEAK]
                EXPORT  PEVC_TR_Handler           [WEAK]
                EXPORT  PEVC_OV_Handler           [WEAK]
                EXPORT  AESA_Handler              [WEAK]
                EXPORT  PM_Handler                [WEAK]
                EXPORT  SCIF_Handler              [WEAK]
                EXPORT  FREQM_Handler             [WEAK]
                EXPORT  GPIO_0_Handler            [WEAK]
                EXPORT  GPIO_1_Handler            [WEAK]
                EXPORT  GPIO_2_Handler            [WEAK]
                EXPORT  GPIO_3_Handler            [WEAK]
                EXPORT  GPIO_4_Handler            [WEAK]
                EXPORT  GPIO_5_Handler            [WEAK]
                EXPORT  GPIO_6_Handler            [WEAK]
                EXPORT  GPIO_7_Handler            [WEAK]
                EXPORT  GPIO_8_Handler            [WEAK]
                EXPORT  GPIO_9_Handler            [WEAK]
                EXPORT  GPIO_10_Handler           [WEAK]
                EXPORT  GPIO_11_Handler           [WEAK]
                EXPORT  BPM_Handler               [WEAK]
                EXPORT  BSCIF_Handler             [WEAK]
                EXPORT  AST_ALARM_Handler         [WEAK]
                EXPORT  AST_PER_Handler           [WEAK]
                EXPORT  AST_OVF_Handler           [WEAK]
                EXPORT  AST_READY_Handler         [WEAK]
                EXPORT  AST_CLKREADY_Handler      [WEAK]
                EXPORT  WDT_Handler               [WEAK]
                EXPORT  EIC_1_Handler             [WEAK]
                EXPORT  EIC_2_Handler             [WEAK]
                EXPORT  EIC_3_Handler             [WEAK]
                EXPORT  EIC_4_Handler             [WEAK]
                EXPORT  EIC_5_Handler             [WEAK]
                EXPORT  EIC_6_Handler             [WEAK]
                EXPORT  EIC_7_Handler             [WEAK]
                EXPORT  EIC_8_Handler             [WEAK]
                EXPORT  IISC_Handler              [WEAK]
                EXPORT  SPI_Handler               [WEAK]
                EXPORT  TC00_Handler              [WEAK]
                EXPORT  TC01_Handler              [WEAK]
                EXPORT  TC02_Handler              [WEAK]
                EXPORT  TC10_Handler              [WEAK]
                EXPORT  TC11_Handler              [WEAK]
                EXPORT  TC12_Handler              [WEAK]
                EXPORT  TWIM0_Handler             [WEAK]
                EXPORT  TWIS0_Handler             [WEAK]
                EXPORT  TWIM1_Handler             [WEAK]
                EXPORT  TWIS1_Handler             [WEAK]
                EXPORT  USART0_Handler            [WEAK]
                EXPORT  USART1_Handler            [WEAK]
                EXPORT  USART2_Handler            [WEAK]
                EXPORT  USART3_Handler            [WEAK]
                EXPORT  ADCIFE_Handler            [WEAK]
                EXPORT  DACC_Handler              [WEAK]
                EXPORT  ACIFC_Handler             [WEAK]
                EXPORT  ABDACB_Handler            [WEAK]
                EXPORT  TRNG_Handler              [WEAK]
                EXPORT  PARC_Handler              [WEAK]
                EXPORT  CATB_Handler              [WEAK]
                EXPORT  Dummy_Handler             [WEAK]
                EXPORT  TWIM2_Handler             [WEAK]
                EXPORT  TWIM3_Handler             [WEAK]
                EXPORT  LCDCA_Handler             [WEAK]

HFLASHC_Handler
PDCA_0_Handler
PDCA_1_Handler
PDCA_2_Handler
PDCA_3_Handler
PDCA_4_Handler
PDCA_5_Handler
PDCA_6_Handler
PDCA_7_Handler
PDCA_8_Handler
PDCA_9_Handler
PDCA_10_Handler
PDCA_11_Handler
PDCA_12_Handler
PDCA_13_Handler
PDCA_14_Handler
PDCA_15_Handler
CRCCU_Handler
USBC_Handler
PEVC_TR_Handler
PEVC_OV_Handler
AESA_Handler
PM_Handler
SCIF_Handler
FREQM_Handler
GPIO_0_Handler
GPIO_1_Handler
GPIO_2_Handler
GPIO_3_Handler
GPIO_4_Handler
GPIO_5_Handler
GPIO_6_Handler
GPIO_7_Handler
GPIO_8_Handler
GPIO_9_Handler
GPIO_10_Handler
GPIO_11_Handler
BPM_Handler
BSCIF_Handler
AST_ALARM_Handler
AST_PER_Handler
AST_OVF_Handler
AST_READY_Handler
AST_CLKREADY_Handler
WDT_Handler
EIC_1_Handler
EIC_2_Handler
EIC_3_Handler
EIC_4_Handler
EIC_5_Handler
EIC_6_Handler
EIC_7_Handler
EIC_8_Handler
IISC_Handler
SPI_Handler
TC00_Handler
TC01_Handler
TC02_Handler
TC10_Handler
TC11_Handler
TC12_Handler
TWIM0_Handler
TWIS0_Handler
TWIM1_Handler
TWIS1_Handler
USART0_Handler
USART1_Handler
USART2_Handler
USART3_Handler
ADCIFE_Handler
DACC_Handler
ACIFC_Handler
ABDACB_Handler
TRNG_Handler
PARC_Handler
CATB_Handler
Dummy_Handler
TWIM2_Handler
TWIM3_Handler
LCDCA_Handler
                B       .
                ENDP


                ALIGN


; User Initial Stack & Heap

                IF      :DEF:__MICROLIB

                EXPORT  __initial_sp
                EXPORT  __heap_base
                EXPORT  __heap_limit

                ELSE

                IMPORT  __use_two_region_memory
                EXPORT  __user_initial_stackheap
__user_initial_stackheap

                LDR     R0, =  Heap_Mem
                LDR     R1, =(Stack_Mem + Stack_Size)
                LDR     R2, = (Heap_Mem +  Heap_Size)
                LDR     R3, = Stack_Mem
                BX      LR

                ALIGN

                ENDIF


                END
