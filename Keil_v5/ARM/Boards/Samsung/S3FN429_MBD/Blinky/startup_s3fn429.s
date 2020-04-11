;/*******************************************************************************
; * @file     startup_s3fn429.s
; * @brief    CMSIS Cortex-M0 Core Device Startup File
; *           for S3FN429 Device                                                 
; * @version  V1.0                                                               
; * @date     01. May 2012                                                  
; *                                                                              
; * @note                                                                        
; * Copyright (C) 2012 by MCU Application Team, Samsung Electronics CO.,LTD..    
; *			  All rights reserved.                                                   
; *                                                                              
; * @attention                                                                   
; * The software is delivered "AS IS" without warranty or condition of any kind, 
; * either express, implied or statutory. This includes without limitation       
; * any warranty or condition with respect to merchantability or fitness for     
; * any particular purpose, or against the infringements of intellectual property
; * rights of others.                                                            
; *                                                                              
; ******************************************************************************/
;/*
;//-------- <<< Use Configuration Wizard in Context Menu >>> ------------------
;*/

; <h> Stack Configuration
;   <o> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Stack_Size      EQU     0x00000100

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


; <h> SO_CSR (Smart Option Configuration Status Register)
;   <o0.0>    POCCS  
;     <i> Power-On System Clock Source Selection Field
;          <0=> EMCLK <1=> IMCLK
;   <o0.2>    XIN  
;     <i> External Main Clock Input Pin Configuration Field
;          <0=> GPIO (P0.31) pin <1=> XIN pin
;   <o0.3>    XOUT  
;     <i> External Main Clock Output Pin Configuration Field
;          <0=> GPIO (P0.30) pin <1=> XOUT pin
;   <o0.12..15> BTDIV  
;     <i> Basic timer divider selection bit in the reset time
;          <3=> 1 <4=> 2 <5=> 4 <6=> 8 <7=> 16 <8=> 32 <9=> 64 <10=> 128 <11=> 256 <12=> 512 <13=> 1024 <14=> 2048 <15=> 4096
; </h>
SO_CSR          EQU     0xFFFFFFFF   ; Configuration Smart Option
nSO_CSR         EQU     SO_CSR:OR:0xFFFF0FF2   ;Keep reserved Bits

; <h> SO_PSR (Smart Option Protection Status Register)
;   <o0.8> nSWDP  
;     <i> SWD Protection Configuration Field
;     <i> WARNING: If nSWDP field is configured as "Enable SWD Protection", access via the debugger is blocked !!!!!!!
;          <0=> Enable SWD Protection<1=> Disable SWD Protection
;   <o0.17> nHWP  
;     <i> Hardware Protection Configuration Field
;	  <i> nHWP field should be configured together with HWPA0, HWPA1, HWPA2, and HWPA3.
;     <i> WARNING: If nHWP field is configured as "Enable Hardware Protection", access can be blocked !!!!!!!
;          <0=> Enable Hardware Protection <1=> Disable Hardware Protection
;   <o0.4> HWPA0  
;     <i> Hardware Protection Area 0 Selection Field
;          <0=> Include SECTOR 0 in Hardware Protection Area<1=> Don't include SECTOR 0 in Hardware Protection Area
;	<o0.5> HWPA1  
;     <i> Hardware Protection Area 1 Selection Field
;          <0=> Include SECTOR 1 in Hardware Protection Area<1=> Don't include SECTOR 1 in Hardware Protection Area
;   <o0.6> HWPA2  
;     <i> Hardware Protection Area 2 Selection Field
;          <0=> Include SECTOR 2 in Hardware Protection Area<1=> Don't include SECTOR 2 in Hardware Protection Area
;   <o0.7> HWPA3  
;     <i> Hardware Protection Area 3 Selection Field
;          <0=> Include SECTOR 3 in Hardware Protection Area<1=> Don't include SECTOR 3 in Hardware Protection Area
;   <o0.27> nSRP  
;     <i> Serial Read Protection Configuration Field
;          <0=> Enable Serial Read Protection <1=> Disable Serial Read Protection
; </h>
SO_PSR          EQU     0xFFFFFFFF   ; Protection Smart Option
nSO_PSR         EQU     SO_PSR:OR:0xF7FDFE0F     ;Keep reserved Bits


                PRESERVE8
                THUMB			   

; Vector Table Mapped to Address 0 at Reset

                AREA    RESET, DATA, READONLY
                EXPORT  __Vectors
                EXPORT  __Vectors_End
                EXPORT  __Vectors_Size		

__Vectors		DCD     __initial_sp			  ; Top of Stack
                DCD     Reset_Handler             ; Reset Handler      
                DCD     NMI_Handler               ; NMI Handler        
                DCD     HardFault_Handler         ; Hard Fault Handler 
                DCD     0                    	  ; Reserved           
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
				; IRQx - user application interrupts from here
				DCD     WDTHandler				  ;  0) addr 0x40 Watchdog timer vector
				DCD     IMC_FAULTHandler          ;  1) addr 0x44 IMC(Fault interrupt) vector
				DCD     COMP0Handler      	 	  ;  2) addr 0x48 Comparator0(edgedetect interrupt) vector
				DCD     COMP1Handler       		  ;  3) addr 0x4C Comparator1(edgedetect interrupt) vector
				DCD     COMP2Handler    	   	  ;  4) addr 0x50 Comparator2(edgedetect interrupt) vector
				DCD     COMP3Handler 	      	  ;  5) addr 0x54 Comparator3(edgedetect interrupt) vector
				DCD     IMC_ZEROTOPHandler    	  ;  6) addr 0x58 IMC(Zero or Top interrupt) vector
				DCD     IMC_ADCMAT0Handler    	  ;  7) addr 0x5C IMC(ADCmatch0 (R/F) interrupt) vector
				DCD     IMC_ADCMAT1Handler    	  ;  8) addr 0x60 IMC(ADCmatch1 (R/F) interrupt) vector
				DCD     IMC_ADCMAT2Handler    	  ;  9) addr 0x64 IMC(ADCmatch2 (R/F) interrupt) vector
				DCD     ADCHandler            	  ; 10) addr 0x68 ADC vector
				DCD     PPDPHandler           	  ; 11) addr 0x6C PPD(Position interrupt) vector
				DCD     PPDSHandler               ; 12) addr 0x70 PPD(Speed interrupt) vector
				DCD     USARTRXHandler        	  ; 13) addr 0x74 USART(RX interrupt) vector
				DCD     CMHandler             	  ; 14) addr 0x78 Clock Manager vector
				DCD     IFCHandler            	  ; 15) addr 0x7C Internal Flash Controller vector
				DCD     EEIAHandler           	  ; 16) addr 0x80 External Event Source Interrupt0 vector
				DCD     EEIBHandler           	  ; 17) addr 0x84 External Event Source Interrupt1 vector
				DCD     EEICHandler           	  ; 18) addr 0x88 External Event Source Interrupt2 vector
				DCD     EEIDHandler           	  ; 19) addr 0x8C External Event Source Interrupt3 vector
				DCD     EEIEHandler           	  ; 20) addr 0x90 External Event Source Interrupt4/5 vector
				DCD     EEIFHandler           	  ; 21) addr 0x94 External Event Source Interrupt6/7 vector
				DCD     TC0Handler            	  ; 22) addr 0x98 TC0 vector
				DCD     TC1Handler            	  ; 23) addr 0x9C TC1 vector
				DCD     TC2Handler            	  ; 24) addr 0xA0 TC2 vector
				DCD     PWM0Handler           	  ; 25) addr 0xA4 PWM0 vector
				DCD     PWM1Handler           	  ; 26) addr 0xA8 PWM1 vector
				DCD     PWM2Handler           	  ; 27) addr 0xAC PWM2 vector
				DCD     PWM3Handler           	  ; 28) addr 0xB0 PWM3 vector
				DCD     SPI0Handler            	  ; 29) addr 0xB4 SPI vector
				DCD     USARTHandler          	  ; 30) addr 0xB8 UART vector
				DCD     GPIOHandler           	  ; 31) addr 0xBC GPIO vector	
__Vectors_End

__Vectors_Size  EQU  __Vectors_End - __Vectors		


				IF      :LNOT::DEF:RAM_TARGET
                AREA    |.ARM.__at_0x0C0|, CODE, READONLY
				DCD     nSO_CSR
				DCD     nSO_PSR                
				 
				; Virtual address 0x1FFFFFC0 -- 0x1FFFFFC7 is used from KEIL Tool Flashloader 
				IF      :DEF:SMART_OPT  
				AREA    |.ARM.__AT_0x1FFFFFC0|, CODE, READONLY
				DCD     nSO_CSR
				DCD     nSO_PSR
				ENDIF
				
				ENDIF	
 
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
  				EXPORT  WDTHandler			[WEAK];  0) addr 0x40 Watchdog timer vector
			 	EXPORT  IMC_FAULTHandler    [WEAK];  1) addr 0x44 IMC(Fault interrupt) vector
			 	EXPORT  COMP0Handler     	[WEAK];  2) addr 0x48 Comparator0(edgedetect interrupt) vector
			 	EXPORT 	COMP1Handler     	[WEAK];  3) addr 0x4C Comparator1(edgedetect interrupt) vector
			 	EXPORT  COMP2Handler    	[WEAK];  4) addr 0x50 Comparator2(edgedetect interrupt) vector
			 	EXPORT  COMP3Handler 	    [WEAK];  5) addr 0x54 Comparator3(edgedetect interrupt) vector
			 	EXPORT  IMC_ZEROTOPHandler  [WEAK];  6) addr 0x58 IMC(Zero or Top interrupt) vector
			 	EXPORT  IMC_ADCMAT0Handler  [WEAK];  7) addr 0x5C IMC(ADCmatch0 (R/F) interrupt) vector
			 	EXPORT  IMC_ADCMAT1Handler  [WEAK];  8) addr 0x60 IMC(ADCmatch1 (R/F) interrupt) vector
			 	EXPORT  IMC_ADCMAT2Handler  [WEAK];  9) addr 0x64 IMC(ADCmatch2 (R/F) interrupt) vector
			 	EXPORT  ADCHandler          [WEAK]; 10) addr 0x68 ADC vector
			 	EXPORT  PPDPHandler         [WEAK]; 11) addr 0x6C PPD(Position interrupt) vector
			 	EXPORT  PPDSHandler         [WEAK]; 12) addr 0x70 PPD(Speed interrupt) vector
			 	EXPORT  USARTRXHandler      [WEAK]; 13) addr 0x74 USART(RX interrupt) vector
			 	EXPORT  CMHandler           [WEAK]; 14) addr 0x78 Clock Manager vector
			 	EXPORT  IFCHandler          [WEAK]; 15) addr 0x7C Internal Flash Controller vector
			 	EXPORT  EEIAHandler         [WEAK]; 16) addr 0x80 External Event Source Interrupt0 vector
			 	EXPORT  EEIBHandler         [WEAK]; 17) addr 0x84 External Event Source Interrupt1 vector
			 	EXPORT  EEICHandler         [WEAK]; 18) addr 0x88 External Event Source Interrupt2 vector
			 	EXPORT  EEIDHandler         [WEAK]; 19) addr 0x8C External Event Source Interrupt3 vector
			 	EXPORT  EEIEHandler         [WEAK]; 20) addr 0x90 External Event Source Interrupt4/5 vector
			 	EXPORT  EEIFHandler         [WEAK]; 21) addr 0x94 External Event Source Interrupt6/7 vector
			 	EXPORT  TC0Handler          [WEAK]; 22) addr 0x98 TC0 vector
			 	EXPORT  TC1Handler          [WEAK]; 23) addr 0x9C TC1 vector
			 	EXPORT  TC2Handler          [WEAK]; 24) addr 0xA0 TC2 vector
			 	EXPORT  PWM0Handler         [WEAK]; 25) addr 0xA4 PWM0 vector
			 	EXPORT  PWM1Handler         [WEAK]; 26) addr 0xA8 PWM1 vector
			 	EXPORT  PWM2Handler         [WEAK]; 27) addr 0xAC PWM2 vector
			 	EXPORT  PWM3Handler         [WEAK]; 28) addr 0xB0 PWM3 vector
			 	EXPORT  SPI0Handler         [WEAK]; 29) addr 0xB4 SPI vector
			 	EXPORT  USARTHandler        [WEAK]; 30) addr 0xB8 UART (Except RX interrupt) vector
			 	EXPORT  GPIOHandler         [WEAK]; 31) addr 0xBC GPIO vector

WDTHandler				;  0) addr 0x40 Watchdog timer vector
IMC_FAULTHandler       	;  1) addr 0x44 IMC(Fault interrupt) vector
COMP0Handler       		;  2) addr 0x48 Comparator0(edgedetect interrupt) vector
COMP1Handler       		;  3) addr 0x4C Comparator1(edgedetect interrupt) vector
COMP2Handler       		;  4) addr 0x50 Comparator2(edgedetect interrupt) vector
COMP3Handler       		;  5) addr 0x54 Comparator3(edgedetect interrupt) vector
IMC_ZEROTOPHandler    	;  6) addr 0x58 IMC(Zero or Top interrupt) vector
IMC_ADCMAT0Handler    	;  7) addr 0x5C IMC(ADCmatch0 (R/F) interrupt) vector
IMC_ADCMAT1Handler    	;  8) addr 0x60 IMC(ADCmatch1 (R/F) interrupt) vector
IMC_ADCMAT2Handler    	;  9) addr 0x64 IMC(ADCmatch2 (R/F) interrupt) vector
ADCHandler            	; 10) addr 0x68 ADC vector
PPDPHandler           	; 11) addr 0x6C PPD(Position interrupt) vector
PPDSHandler             ; 12) addr 0x70 PPD(Speed interrupt) vector
USARTRXHandler        	; 13) addr 0x74 USART(RX interrupt) vector
CMHandler             	; 14) addr 0x78 Clock Manager vector
IFCHandler            	; 15) addr 0x7C Internal Flash Controller vector
EEIAHandler           	; 16) addr 0x80 External Event Source Interrupt0 vector
EEIBHandler           	; 17) addr 0x84 External Event Source Interrupt1 vector
EEICHandler           	; 18) addr 0x88 External Event Source Interrupt2 vector
EEIDHandler           	; 19) addr 0x8C External Event Source Interrupt3 vector
EEIEHandler           	; 20) addr 0x90 External Event Source Interrupt4/5 vector
EEIFHandler           	; 21) addr 0x94 External Event Source Interrupt6/7 vector
TC0Handler            	; 22) addr 0x98 TC0 vector
TC1Handler            	; 23) addr 0x9C TC1 vector
TC2Handler            	; 24) addr 0xA0 TC2 vector
PWM0Handler           	; 25) addr 0xA4 PWM0 vector
PWM1Handler           	; 26) addr 0xA8 PWM1 vector
PWM2Handler           	; 27) addr 0xAC PWM2 vector
PWM3Handler           	; 28) addr 0xB0 PWM3 vector
SPI0Handler            	; 29) addr 0xB4 SPI vector
USARTHandler          	; 30) addr 0xB8 UART (Except RX interrupt) vector
GPIOHandler           	; 31) addr 0xBC GPIO vector
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

											