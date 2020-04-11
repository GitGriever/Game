;/**************************************************************************//**
; * @file     startup_S3FM02G.s
; * @brief    CMSIS Cortex-M3 Device Peripheral Access Layer Source File
; *           for the Samsung S3FM02G Device Series
; * @version  V0.01
; * @date     02. February 2011
; *------- <<< Use Configuration Wizard in Context Menu >>> ------------------
; *
; * @note
; * Copyright (C) 2011 ARM Limited. All rights reserved.
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


; <h> Stack Configuration
;   <o> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Stack_Size      EQU     0x00000400

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

                ; External Interrupts

                DCD     WDTINT_Handler            ; Watch-dog Timer Interrupt 
                DCD     CMINT_Handler             ; Clock Manager Interrupt 
                DCD     PFCINT_Handler            ; Program Flash Controller Interrupt 
                DCD     DFCINT_Handler            ; Data Flash Controller Interrupt 
                DCD     DMAINT_Handler            ; DMA Controller Interrupt 
                DCD     FRTINT_Handler            ; Free-running Timer Interrupt 
                DCD     WSI0INT_Handler           ; Wakeup source 0 
                DCD     WSI1INT_Handler           ; Wakeup source 1 
                DCD     IMC0INT_Handler           ; Inverter Motor Controller 0 Interrupt 
                DCD     ENC0INT_Handler           ; Encoder Counter 0 Interrupt 
                DCD     IMC1INT_Handler           ; Inverter Motor Controller 1 Interrupt 
                DCD     ENC1INT_Handler           ; Encoder Counter 1 Interrupt 
                DCD     CAN0INT_Handler           ; CAN0 Interrupt 
                DCD     USART0INT_Handler         ; USART0 Interrupt 
                DCD     ADC0INT_Handler           ; ADC0 Interrupt 
                DCD     ADC1INT_Handler           ; ADC1 Interrupt 
                DCD     SSP0INT_Handler           ; SSP0 Interrupt 
                DCD     I2C0INT_Handler           ; I2C0 Interrupt 
                DCD     TC0INT_Handler            ; Timer/Counter0 Interrupt 
                DCD     PWM0INT_Handler           ; PWM0 Interrupt 
                DCD     WSI2INT_Handler           ; Wakeup source 2 
                DCD     WSI3INT_Handler           ; Wakeup source 3 
                DCD     TC1INT_Handler            ; Timer/Counter1 Interrupt 
                DCD     PWM1INT_Handler           ; PWM1 Interrupt 
                DCD     USART1INT_Handler         ; USART1 Interrupt 
                DCD     SSP1INT_Handler           ; SSP1 Interrupt 
                DCD     I2C1INT_Handler           ; I2C1 Interrupt 
                DCD     CAN1INT_Handler           ; CAN1 Interrupt 
                DCD     STTINT_Handler            ; STT Interrupt 
                DCD     USART2INT_Handler         ; USART2 Interrupt 
                DCD     TC2INT_Handler            ; Timer/Counter2 Interrupt 
                DCD     TC3INT_Handler            ; Timer/Counter3 Interrupt 
                DCD     PWM2INT_Handler           ; PWM2 Interrupt 
                DCD     WSI4INT_Handler           ; Wakeup source 4 
                DCD     WSI5INT_Handler           ; Wakeup source 5 
                DCD     PWM3INT_Handler           ; PWM3 Interrupt 
                DCD     USART3INT_Handler         ; USART4 Interrupt 
                DCD     GPIO0INT_Handler          ; GPIO 0 Interrupt 
                DCD     GPIO1INT_Handler          ; GPIO 1 Interrupt 
                DCD     TC4INT_Handler            ; Timer/Counter4 Interrupt 
                DCD     WSI6INT_Handler           ; Wakeup source 6 
                DCD     WSI7INT_Handler           ; Wakeup source 7 
                DCD     PWM4INT_Handler           ; PWM4 Interrupt 
                DCD     TC5INT_Handler            ; Timer/Counter5 Interrupt 
                DCD     WSI8INT_Handler           ; Wakeup source 8 
                DCD     WSI9INT_Handler           ; Wakeup source 9 
                DCD     WSI10INT_Handler          ; Wakeup source 10 
                DCD     WSI11INT_Handler          ; Wakeup source 11 
                DCD     PWM5INT_Handler           ; PWM5 Interrupt 
                DCD     TC6INT_Handler            ; Timer/Counter6 Interrupt 
                DCD     WSI12INT_Handler          ; Wakeup source 12 
                DCD     WSI13INT_Handler          ; Wakeup source 13 
                DCD     WSI14INT_Handler          ; Wakeup source 14 
                DCD     WSI15INT_Handler          ; Wakeup source 15 
                DCD     PWM6INT_Handler           ; PWM6 Interrupt 
                DCD     TC7INT_Handler            ; Timer/Counter7 Interrupt 
                DCD     PWM7INT_Handler           ; PWM8 Interrupt 
                DCD     GPIO2INT_Handler          ; GPIO 2 Interrupt 
                DCD     GPIO3INT_Handler          ; GPIO 3 Interrupt 
                DCD     OPAMPINT_Handler          ; OP-AMP interrupt 

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

                EXPORT  WDTINT_Handler            [WEAK]
                EXPORT  CMINT_Handler             [WEAK]
                EXPORT  PFCINT_Handler            [WEAK]
                EXPORT  DFCINT_Handler            [WEAK]
                EXPORT  DMAINT_Handler            [WEAK]
                EXPORT  FRTINT_Handler            [WEAK]
                EXPORT  WSI0INT_Handler           [WEAK]
                EXPORT  WSI1INT_Handler           [WEAK]
                EXPORT  IMC0INT_Handler           [WEAK] 
                EXPORT  ENC0INT_Handler           [WEAK]
                EXPORT  IMC1INT_Handler           [WEAK] 
                EXPORT  ENC1INT_Handler           [WEAK]
                EXPORT  CAN0INT_Handler           [WEAK]
                EXPORT  USART0INT_Handler         [WEAK]
                EXPORT  ADC0INT_Handler           [WEAK]
                EXPORT  ADC1INT_Handler           [WEAK]
                EXPORT  SSP0INT_Handler           [WEAK]
                EXPORT  I2C0INT_Handler           [WEAK]
                EXPORT  TC0INT_Handler            [WEAK]
                EXPORT  PWM0INT_Handler           [WEAK]
                EXPORT  WSI2INT_Handler           [WEAK]
                EXPORT  WSI3INT_Handler           [WEAK]
                EXPORT  TC1INT_Handler            [WEAK]
                EXPORT  PWM1INT_Handler           [WEAK]
                EXPORT  USART1INT_Handler         [WEAK]
                EXPORT  SSP1INT_Handler           [WEAK]
                EXPORT  I2C1INT_Handler           [WEAK]
                EXPORT  CAN1INT_Handler           [WEAK]
                EXPORT  STTINT_Handler            [WEAK]
                EXPORT  USART2INT_Handler         [WEAK]
                EXPORT  TC2INT_Handler            [WEAK]
                EXPORT  TC3INT_Handler            [WEAK]
                EXPORT  PWM2INT_Handler           [WEAK]
                EXPORT  WSI4INT_Handler           [WEAK]
                EXPORT  WSI5INT_Handler           [WEAK]
                EXPORT  PWM3INT_Handler           [WEAK]
                EXPORT  USART3INT_Handler         [WEAK]
                EXPORT  GPIO0INT_Handler          [WEAK]
                EXPORT  GPIO1INT_Handler          [WEAK]
                EXPORT  TC4INT_Handler            [WEAK]
                EXPORT  WSI6INT_Handler           [WEAK]
                EXPORT  WSI7INT_Handler           [WEAK]
                EXPORT  PWM4INT_Handler           [WEAK]
                EXPORT  TC5INT_Handler            [WEAK]
                EXPORT  WSI8INT_Handler           [WEAK]
                EXPORT  WSI9INT_Handler           [WEAK]
                EXPORT  WSI10INT_Handler          [WEAK]
                EXPORT  WSI11INT_Handler          [WEAK]
                EXPORT  PWM5INT_Handler           [WEAK]
                EXPORT  TC6INT_Handler            [WEAK]
                EXPORT  WSI12INT_Handler          [WEAK]
                EXPORT  WSI13INT_Handler          [WEAK]
                EXPORT  WSI14INT_Handler          [WEAK]
                EXPORT  WSI15INT_Handler          [WEAK]
                EXPORT  PWM6INT_Handler           [WEAK]
                EXPORT  TC7INT_Handler            [WEAK]
                EXPORT  PWM7INT_Handler           [WEAK]
                EXPORT  GPIO2INT_Handler          [WEAK]
                EXPORT  GPIO3INT_Handler          [WEAK]
                EXPORT  OPAMPINT_Handler          [WEAK]


WDTINT_Handler          
CMINT_Handler           
PFCINT_Handler          
DFCINT_Handler          
DMAINT_Handler          
FRTINT_Handler          
WSI0INT_Handler         
WSI1INT_Handler         
IMC0INT_Handler         
ENC0INT_Handler         
IMC1INT_Handler         
ENC1INT_Handler         
CAN0INT_Handler         
USART0INT_Handler       
ADC0INT_Handler         
ADC1INT_Handler         
SSP0INT_Handler         
I2C0INT_Handler         
TC0INT_Handler          
PWM0INT_Handler         
WSI2INT_Handler         
WSI3INT_Handler         
TC1INT_Handler          
PWM1INT_Handler         
USART1INT_Handler       
SSP1INT_Handler         
I2C1INT_Handler         
CAN1INT_Handler         
STTINT_Handler          
USART2INT_Handler       
TC2INT_Handler          
TC3INT_Handler          
PWM2INT_Handler         
WSI4INT_Handler         
WSI5INT_Handler         
PWM3INT_Handler         
USART3INT_Handler       
GPIO0INT_Handler        
GPIO1INT_Handler        
TC4INT_Handler          
WSI6INT_Handler         
WSI7INT_Handler         
PWM4INT_Handler         
TC5INT_Handler          
WSI8INT_Handler         
WSI9INT_Handler         
WSI10INT_Handler        
WSI11INT_Handler        
PWM5INT_Handler         
TC6INT_Handler          
WSI12INT_Handler        
WSI13INT_Handler        
WSI14INT_Handler        
WSI15INT_Handler        
PWM6INT_Handler         
TC7INT_Handler          
PWM7INT_Handler         
GPIO2INT_Handler        
GPIO3INT_Handler        
OPAMPINT_Handler        

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
