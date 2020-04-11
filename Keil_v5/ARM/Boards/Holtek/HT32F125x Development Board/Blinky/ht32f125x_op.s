;/*****************************************************************************/
;/* ht32f125x_op.s: HT32F125x Flash Option Bytes Block                        */
;/*****************************************************************************/
;/* <<< Use Configuration Wizard in Context Menu >>>                          */
;/*****************************************************************************/

;// <e> Setting of Flash protection
;// <i> Setting of Page erase/program protection and security protection of Flash (configured in Option Byte)
FLASH_OPT     EQU     1

;// <h> Page erase/program protection of Main Flash
;// <i> Enable page erase/program protection to prevent unwanted change of Flash contents
;//   <o0.0 > Enable page 0 erase/Program Protection
;//   <o0.1 > Enable page 1 erase/Program Protection
;//   <o0.2 > Enable page 2 erase/Program Protection
;//   <o0.3 > Enable page 3 erase/Program Protection
;//   <o0.4 > Enable page 4 erase/Program Protection
;//   <o0.5 > Enable page 5 erase/Program Protection
;//   <o0.6 > Enable page 6 erase/Program Protection
;//   <o0.7 > Enable page 7 erase/Program Protection
;//   <o0.8 > Enable page 8 erase/Program Protection
;//   <o0.9 > Enable page 9 erase/Program Protection
;//   <o0.10 > Enable page 10 erase/Program Protection
;//   <o0.11 > Enable page 11 erase/Program Protection
;//   <o0.12 > Enable page 12 erase/Program Protection
;//   <o0.13 > Enable page 13 erase/Program Protection
;//   <o0.14 > Enable page 14 erase/Program Protection
;//   <o0.15 > Enable page 15 erase/Program Protection
;//   <o0.16 > Enable page 16 erase/Program Protection
;//   <o0.17 > Enable page 17 erase/Program Protection
;//   <o0.18 > Enable page 18 erase/Program Protection
;//   <o0.19 > Enable page 19 erase/Program Protection
;//   <o0.20 > Enable page 20 erase/Program Protection
;//   <o0.21 > Enable page 21 erase/Program Protection
;//   <o0.22 > Enable page 22 erase/Program Protection
;//   <o0.23 > Enable page 23 erase/Program Protection
;//   <o0.24 > Enable page 24 erase/Program Protection
;//   <o0.25 > Enable page 25 erase/Program Protection
;//   <o0.26 > Enable page 26 erase/Program Protection
;//   <o0.27 > Enable page 27 erase/Program Protection
;//   <o0.28 > Enable page 28 erase/Program Protection
;//   <o0.29 > Enable page 29 erase/Program Protection
;//   <o0.30 > Enable page 30 erase/Program Protection
;// </h>
PP0     EQU         0x00000000
nPP0    EQU         PP0:EOR:0xFFFFFFFF

;//   <o0.1> Enable Option Byte erase/program protection
;//   <i> Enable Option Byte erase/program protection to prevent unwanted change of Option Byte
;//   <o0.0> Enable security protection of Flash
;//   <i> Enable Security protection to prevent illegal Flash code/data access
CP      EQU         0x00000000
nCP     EQU         CP:EOR:0xFFFFFFFF


    IF nPP0 = 0xFFFFFFFF &&  nCP = 0xFFFFFFFF
CK      EQU         0xFFFFFFFF
    ELSE
CK      EQU         (nPP0 + 0xFFFFFFFF + 0xFFFFFFFF + 0xFFFFFFFF + nCP)
    ENDIF
RES     EQU         0xFFFFFFFF

;// </e>

                IF      FLASH_OPT <> 0
                AREA    |.ARM.__AT_0x1FF00000|, CODE, READONLY
                DCD   nPP0          ; 0x1FF00000
                DCD   RES           ; 0x1FF00004
                DCD   RES           ; 0x1FF00008
                DCD   RES           ; 0x1FF0000C
                DCD   nCP           ; 0x1FF00010
                DCD   RES           ; 0x1FF00014
                DCD   RES           ; 0x1FF00018
                DCD   RES           ; 0x1FF0001C
                DCD   CK            ; 0x1FF00020
                ENDIF

;// <e> Program user's Flash data into Option Byte
;// <i> Address range: from 0x1FF00050 to 0x1FF003FF (Max 944 Bytes)
FLASH_DATA      EQU     1
;// </e>
                IF      FLASH_DATA <> 0
                AREA    |.ARM.__AT_0x1FF00050|, CODE, READONLY
                INCBIN  userdata.dat
                ENDIF

                END
