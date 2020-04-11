;/**************************************************************************//**
; * @file     startup_TSX1001.s
; * @brief    CMSIS Core Device Startup File for
; *           Triad TSX1001 Device Series
; * @version  V1.00
; * @date     27. July 2012
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

Heap_Size       EQU     0x00000000

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
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     SVC_Handler               ; SVCall Handler
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     PendSV_Handler            ; PendSV Handler
                DCD     SysTick_Handler           ; SysTick Handler

                ; External Interrupts
                DCD     GPIO0_IRQHandler          ;  0:  External Interrupt from GPIO1
                DCD     GPIO1_IRQHandler          ;  1:  External Interrupt from GPIO1
                DCD     ADC_IRQHandler            ;  2:
                DCD     DAC_IRQHandler            ;  3:
                DCD     PWM_IRQHandler            ;  4:
                DCD     TMR_IRQHandler            ;  5:
                DCD     UART_IRQHandler           ;  6:
                DCD     SPI_IRQHandler            ;  7:
                DCD     INT8n_IRQHandler          ;  8:
                DCD     INT9n_IRQHandler          ;  9:
                DCD     INT8_IRQHandler           ; 10:
                DCD     INT9_IRQHandler           ; 11:
                DCD     ExtGPIO_IRQHandler        ; 12:   External Interrupt from External GPIO
                DCD     Default_IRQHandler        ; 13:
                DCD     Default_IRQHandler        ; 14:
                DCD     Default_IRQHandler        ; 15:
                DCD     Default_IRQHandler        ; 16:
                DCD     Default_IRQHandler        ; 17:
                DCD     Default_IRQHandler        ; 18:
                DCD     Default_IRQHandler        ; 19:
                DCD     Default_IRQHandler        ; 20:
                DCD     Default_IRQHandler        ; 21:
                DCD     Default_IRQHandler        ; 22:
                DCD     Default_IRQHandler        ; 23:
                DCD     Default_IRQHandler        ; 24:
                DCD     Default_IRQHandler        ; 25:
                DCD     Default_IRQHandler        ; 26:
                DCD     Default_IRQHandler        ; 27:
                DCD     Default_IRQHandler        ; 28:
                DCD     Default_IRQHandler        ; 29:
                DCD     Default_IRQHandler        ; 30:
                DCD     Default_IRQHandler        ; 31:
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
SVC_Handler     PROC
                EXPORT  SVC_Handler               [WEAK]
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

                EXPORT  GPIO0_IRQHandler          [WEAK]
                EXPORT  GPIO1_IRQHandler          [WEAK]
                EXPORT  ADC_IRQHandler            [WEAK]
                EXPORT  DAC_IRQHandler            [WEAK]
                EXPORT  PWM_IRQHandler            [WEAK]
                EXPORT  TMR_IRQHandler            [WEAK]
                EXPORT  UART_IRQHandler           [WEAK]
                EXPORT  SPI_IRQHandler            [WEAK]
                EXPORT  INT8n_IRQHandler          [WEAK]
                EXPORT  INT9n_IRQHandler          [WEAK]
                EXPORT  INT8_IRQHandler           [WEAK]
                EXPORT  INT9_IRQHandler           [WEAK]
                EXPORT  ExtGPIO_IRQHandler        [WEAK]
                EXPORT  Default_IRQHandler        [WEAK]

GPIO0_IRQHandler
GPIO1_IRQHandler
ADC_IRQHandler
DAC_IRQHandler
PWM_IRQHandler
TMR_IRQHandler
UART_IRQHandler
SPI_IRQHandler
INT8n_IRQHandler
INT9n_IRQHandler
INT8_IRQHandler
INT9_IRQHandler
ExtGPIO_IRQHandler
Default_IRQHandler
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

__user_initial_stackheap PROC
                LDR     R0, =  Heap_Mem
                LDR     R1, =(Stack_Mem + Stack_Size)
                LDR     R2, = (Heap_Mem +  Heap_Size)
                LDR     R3, = Stack_Mem
                BX      LR
                ENDP

                ALIGN

                ENDIF


                END
