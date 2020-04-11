
/*****************************************************************************
 * (c) Copyright 2010, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ***************************************************************************//*!
 * @file      LCDHDW.h
 * @author    R55013
 * @version   1.0.1.0
 * @date      Dec-6-2010
 * @brief     LCD driver configuration file for LCD type IS18080P..
 ******************************************************************************/
#ifndef __LCDHWD_H
    #define __LCDHWD_H

#define LCD_BASE_PTR LCD
/* LCD - Register accessors */
#define LCD_GCR_REG(base)                        ((base)->GCR)
#define LCD_AR_REG(base)                         ((base)->AR)
#define LCD_FDCR_REG(base)                       ((base)->FDCR)
#define LCD_FDSR_REG(base)                       ((base)->FDSR)
#define LCD_PEN_REG(base,index)                  ((base)->PEN[index])
#define LCD_BPEN_REG(base,index)                 ((base)->BPEN[index])
#define LCD_WF_REG(base,index2)                  ((base)->WF[index2])
#define LCD_WF8B_REG(base,index2)                ((base)->WF8B[index2])


/* LCD - Register instance definitions */
/* LCD */

#define LCD_GCR                                  LCD_GCR_REG(LCD_BASE_PTR)
#define LCD_AR                                   LCD_AR_REG(LCD_BASE_PTR)
#define LCD_FDCR                                 LCD_FDCR_REG(LCD_BASE_PTR)
#define LCD_FDSR                                 LCD_FDSR_REG(LCD_BASE_PTR)
#define LCD_PENL                                 LCD_PEN_REG(LCD_BASE_PTR,0)
#define LCD_PENH                                 LCD_PEN_REG(LCD_BASE_PTR,1)
#define LCD_BPENL                                LCD_BPEN_REG(LCD_BASE_PTR,0)
#define LCD_BPENH                                LCD_BPEN_REG(LCD_BASE_PTR,1)
#define LCD_WF0                                  LCD_WF8B_REG(LCD_BASE_PTR,0)
#define LCD_WF3TO0                               LCD_WF_REG(LCD_BASE_PTR,0)
#define LCD_WF1                                  LCD_WF8B_REG(LCD_BASE_PTR,1)
#define LCD_WF2                                  LCD_WF8B_REG(LCD_BASE_PTR,2)
#define LCD_WF3                                  LCD_WF8B_REG(LCD_BASE_PTR,3)
#define LCD_WF4                                  LCD_WF8B_REG(LCD_BASE_PTR,4)
#define LCD_WF7TO4                               LCD_WF_REG(LCD_BASE_PTR,1)
#define LCD_WF5                                  LCD_WF8B_REG(LCD_BASE_PTR,5)
#define LCD_WF6                                  LCD_WF8B_REG(LCD_BASE_PTR,6)
#define LCD_WF7                                  LCD_WF8B_REG(LCD_BASE_PTR,7)
#define LCD_WF11TO8                              LCD_WF_REG(LCD_BASE_PTR,2)
#define LCD_WF8                                  LCD_WF8B_REG(LCD_BASE_PTR,8)
#define LCD_WF9                                  LCD_WF8B_REG(LCD_BASE_PTR,9)
#define LCD_WF10                                 LCD_WF8B_REG(LCD_BASE_PTR,10)
#define LCD_WF11                                 LCD_WF8B_REG(LCD_BASE_PTR,11)
#define LCD_WF12                                 LCD_WF8B_REG(LCD_BASE_PTR,12)
#define LCD_WF15TO12                             LCD_WF_REG(LCD_BASE_PTR,3)
#define LCD_WF13                                 LCD_WF8B_REG(LCD_BASE_PTR,13)
#define LCD_WF14                                 LCD_WF8B_REG(LCD_BASE_PTR,14)
#define LCD_WF15                                 LCD_WF8B_REG(LCD_BASE_PTR,15)
#define LCD_WF16                                 LCD_WF8B_REG(LCD_BASE_PTR,16)
#define LCD_WF19TO16                             LCD_WF_REG(LCD_BASE_PTR,4)
#define LCD_WF17                                 LCD_WF8B_REG(LCD_BASE_PTR,17)
#define LCD_WF18                                 LCD_WF8B_REG(LCD_BASE_PTR,18)
#define LCD_WF19                                 LCD_WF8B_REG(LCD_BASE_PTR,19)
#define LCD_WF20                                 LCD_WF8B_REG(LCD_BASE_PTR,20)
#define LCD_WF23TO20                             LCD_WF_REG(LCD_BASE_PTR,5)
#define LCD_WF21                                 LCD_WF8B_REG(LCD_BASE_PTR,21)
#define LCD_WF22                                 LCD_WF8B_REG(LCD_BASE_PTR,22)
#define LCD_WF23                                 LCD_WF8B_REG(LCD_BASE_PTR,23)
#define LCD_WF24                                 LCD_WF8B_REG(LCD_BASE_PTR,24)
#define LCD_WF27TO24                             LCD_WF_REG(LCD_BASE_PTR,6)
#define LCD_WF25                                 LCD_WF8B_REG(LCD_BASE_PTR,25)
#define LCD_WF26                                 LCD_WF8B_REG(LCD_BASE_PTR,26)
#define LCD_WF27                                 LCD_WF8B_REG(LCD_BASE_PTR,27)
#define LCD_WF28                                 LCD_WF8B_REG(LCD_BASE_PTR,28)
#define LCD_WF31TO28                             LCD_WF_REG(LCD_BASE_PTR,7)
#define LCD_WF29                                 LCD_WF8B_REG(LCD_BASE_PTR,29)
#define LCD_WF30                                 LCD_WF8B_REG(LCD_BASE_PTR,30)
#define LCD_WF31                                 LCD_WF8B_REG(LCD_BASE_PTR,31)
#define LCD_WF32                                 LCD_WF8B_REG(LCD_BASE_PTR,32)
#define LCD_WF35TO32                             LCD_WF_REG(LCD_BASE_PTR,8)
#define LCD_WF33                                 LCD_WF8B_REG(LCD_BASE_PTR,33)
#define LCD_WF34                                 LCD_WF8B_REG(LCD_BASE_PTR,34)
#define LCD_WF35                                 LCD_WF8B_REG(LCD_BASE_PTR,35)
#define LCD_WF36                                 LCD_WF8B_REG(LCD_BASE_PTR,36)
#define LCD_WF39TO36                             LCD_WF_REG(LCD_BASE_PTR,9)
#define LCD_WF37                                 LCD_WF8B_REG(LCD_BASE_PTR,37)
#define LCD_WF38                                 LCD_WF8B_REG(LCD_BASE_PTR,38)
#define LCD_WF39                                 LCD_WF8B_REG(LCD_BASE_PTR,39)
#define LCD_WF40                                 LCD_WF8B_REG(LCD_BASE_PTR,40)
#define LCD_WF43TO40                             LCD_WF_REG(LCD_BASE_PTR,10)
#define LCD_WF41                                 LCD_WF8B_REG(LCD_BASE_PTR,41)
#define LCD_WF42                                 LCD_WF8B_REG(LCD_BASE_PTR,42)
#define LCD_WF43                                 LCD_WF8B_REG(LCD_BASE_PTR,43)
#define LCD_WF44                                 LCD_WF8B_REG(LCD_BASE_PTR,44)
#define LCD_WF47TO44                             LCD_WF_REG(LCD_BASE_PTR,11)
#define LCD_WF45                                 LCD_WF8B_REG(LCD_BASE_PTR,45)
#define LCD_WF46                                 LCD_WF8B_REG(LCD_BASE_PTR,46)
#define LCD_WF47                                 LCD_WF8B_REG(LCD_BASE_PTR,47)
#define LCD_WF48                                 LCD_WF8B_REG(LCD_BASE_PTR,48)
#define LCD_WF51TO48                             LCD_WF_REG(LCD_BASE_PTR,12)
#define LCD_WF49                                 LCD_WF8B_REG(LCD_BASE_PTR,49)
#define LCD_WF50                                 LCD_WF8B_REG(LCD_BASE_PTR,50)
#define LCD_WF51                                 LCD_WF8B_REG(LCD_BASE_PTR,51)
#define LCD_WF52                                 LCD_WF8B_REG(LCD_BASE_PTR,52)
#define LCD_WF55TO52                             LCD_WF_REG(LCD_BASE_PTR,13)
#define LCD_WF53                                 LCD_WF8B_REG(LCD_BASE_PTR,53)
#define LCD_WF54                                 LCD_WF8B_REG(LCD_BASE_PTR,54)
#define LCD_WF55                                 LCD_WF8B_REG(LCD_BASE_PTR,55)
#define LCD_WF56                                 LCD_WF8B_REG(LCD_BASE_PTR,56)
#define LCD_WF59TO56                             LCD_WF_REG(LCD_BASE_PTR,14)
#define LCD_WF57                                 LCD_WF8B_REG(LCD_BASE_PTR,57)
#define LCD_WF58                                 LCD_WF8B_REG(LCD_BASE_PTR,58)
#define LCD_WF59                                 LCD_WF8B_REG(LCD_BASE_PTR,59)
#define LCD_WF60                                 LCD_WF8B_REG(LCD_BASE_PTR,60)
#define LCD_WF63TO60                             LCD_WF_REG(LCD_BASE_PTR,15)
#define LCD_WF61                                 LCD_WF8B_REG(LCD_BASE_PTR,61)
#define LCD_WF62                                 LCD_WF8B_REG(LCD_BASE_PTR,62)
#define LCD_WF63                                 LCD_WF8B_REG(LCD_BASE_PTR,63)

/* LCD - Register array accessors */
#define LCD_PEN(index)                           LCD_PEN_REG(LCD_BASE_PTR,index)
#define LCD_BPEN(index)                          LCD_BPEN_REG(LCD_BASE_PTR,index)
#define LCD_WF(index2)                           LCD_WF_REG(LCD_BASE_PTR,index2)
#define LCD_WF8B(index2)                         LCD_WF8B_REG(LCD_BASE_PTR,index2)

    #define DIM(x)                (sizeof(x)/sizeof(x[0]))

    #define _LCD_CHARNUM          (9)                   // number of LCD segment
    #define _LCDTYPE              (1)                   // 1 byte to complete one character
    #define  CharacterPlace(LCDn) *((unsigned char *)&LCD_WF3TO0 + LCDn)

/*
    #define _LCD_CHARNUM          (9)                   // number of LCD segment
    #define _LCDTYPE              (2)                   // 2 bytes to complete one character
    #define  CharacterPlace(LCDn) *((unsigned char *)&LCD_WF3TO0 + LCDn)
*/

    #define SymbolON(LCDn, mask)                                              \
    {                                                                         \
      *((unsigned char *)&LCD_WF3TO0 + LCDn) |= (1<<(mask)); ;                \
    }
    
    #define SymbolOFF(LCDn, mask)                                             \
    {                                                                         \
      *((unsigned char *)&LCD_WF3TO0 + LCDn) &= ~(1<<(mask));                 \
    }
    
    #define SegmentsON(LCDn, mask)                                            \
    {                                                                         \
      *((unsigned char *)&LCD_WF3TO0 + LCDn) |= (mask);                       \
    }

    #define SegmentsOFF(LCDn, mask)                                           \
    {                                                                         \
      *((unsigned char *)&LCD_WF3TO0 + LCDn) &= ~(mask);                      \
    }
    
    /* Indicate how character is connected to LCDn          */
    #define   Char0     CharacterPlace(27)
    #define   Char1     CharacterPlace(26)
    #define   Char2     CharacterPlace(25)
    #define   Char3     CharacterPlace(24)
    #define   Char4     CharacterPlace(23)
    #define   Char5     CharacterPlace(22)
    #define   Char6     CharacterPlace(21)
    #define   Char7     CharacterPlace(20)
    #define   Char8     CharacterPlace(19)

/*
    #define   Char4a    CharacterPlace(38)
    #define   Char4b    CharacterPlace(37)
    #define   Char5a    CharacterPlace(36)
    #define   Char5b    CharacterPlace(35)
    #define   Char6a    CharacterPlace(34)
    #define   Char6b    CharacterPlace(33)
    #define   Char7a    CharacterPlace(32)
    #define   Char7b    CharacterPlace(31)
    #define   Char8a    CharacterPlace(30)
    #define   Char8b    CharacterPlace(29)
    #define   Char9a    CharacterPlace(28)
    #define   Char9b    CharacterPlace(27)
    #define   Char10a   CharacterPlace(26)
    #define   Char10b   CharacterPlace(25)
    #define   Char11a   CharacterPlace(24)
    #define   Char11b   CharacterPlace(23)
    #define   Char12a   CharacterPlace(22)
    #define   Char12b   CharacterPlace(21)
*/
    /* Special symbols ON */
/***************************************************************************//*!
 * @brief   Macro displays _LCD_CHAR_V_S3_ON segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define _LCD_CHAR_V_S3_ON    SymbolON(31,0)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_CHAR_V_S6_ON segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define _LCD_CHAR_V_S6_ON    SymbolON(31,5)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_CHAR_V_S7_ON segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define _LCD_CHAR_V_S7_ON    SymbolON(32,1)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_CHAR_V_S9_ON segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define _LCD_CHAR_V_S9_ON    SymbolON(31,4)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_CHAR_V_S11_ON segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define _LCD_CHAR_V_S11_ON   SymbolON(32,4)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_CHAR_V_S12_ON segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define _LCD_CHAR_V_S12_ON   SymbolON(32,2)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_CHAR_V_S13_ON segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define _LCD_CHAR_V_S13_ON   SymbolON(32,3)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_CHAR_I_S8_ON segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define _LCD_CHAR_I_S8_ON    SymbolON(31,2)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_CHAR_I_S12_ON segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define _LCD_CHAR_I_S12_ON   SymbolON(32,2)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_DOT_P3_ON segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define _LCD_DOT_P3_ON       SymbolON(18,1)

   
//------------------------------------------------------------------------------
/***************************************************************************//*!
 * @brief   Macro displays _LCD_CHAR_T_ON segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define _LCD_CHAR_T_ON       SymbolON(38,0)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_COLS_ON segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define _LCD_COLS_ON         SymbolON(20,0)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_COMMA_ON segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define _LCD_COMMA_ON         SymbolON(20,2)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_SYM_T1_ON segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define _LCD_SYM_T1_ON        SymbolON(28,0)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_SYM_T2_ON segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define _LCD_SYM_T2_ON        SymbolON(26,0)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_SYM_T3_ON segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define _LCD_SYM_T3_ON        SymbolON(24,0)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_SYM_T4_ON segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define _LCD_SYM_T4_ON        SymbolON(22,0)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_SYM_kW_ON segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define _LCD_SYM_kW_ON        SymbolON(19,0)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_SYM_h_ON segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define _LCD_SYM_h_ON         SymbolON(17,3)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_SYM_BATT_ON segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define _LCD_SYM_BATT_ON      SymbolON(36,0)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_SYM_WRN_ON segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define _LCD_SYM_WRN_ON       SymbolON(34,0)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_SYM_LOCK_ON segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define _LCD_SYM_LOCK_ON      SymbolON(32,0)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_SYM_OBIS_DOTS_ON segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define _LCD_SYM_OBIS_DOTS_ON SymbolON(19,3)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_SYM_OBIS_ONE_ON segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
#define _LCD_SYM_OBIS_ONE_ON      SymbolON(18,1)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_SYM_OBIS_D1B_C_ON segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
#define _LCD_SYM_OBIS_D1B_C_ON    SymbolON(20,1)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_SYM_OBIS_D1A_E_ON segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
#define _LCD_SYM_OBIS_D1A_E_ON    SymbolON(20,3)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_SYM_OBIS_D2B_ON segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
#define _LCD_SYM_OBIS_D2B_ON      SymbolON(19,1)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_SYM_OBIS_D2E_ON segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
#define _LCD_SYM_OBIS_D2E_ON      SymbolON(19,2)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_SYM_OBIS_D3B_ON segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
#define _LCD_SYM_OBIS_D3B_ON    SymbolON(19,3)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_SYM_OBIS_D3A_ON segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
#define _LCD_SYM_OBIS_D3A_ON         SymbolON(18,0)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_SYM_OBIS_D3C_ON segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
#define _LCD_SYM_OBIS_D3C_ON         SymbolON(18,2)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_SYM_OBIS_D3D_ON segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
#define _LCD_SYM_OBIS_D3D_ON         SymbolON(18,3)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_SYM_OBIS_D3E_ON segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
#define _LCD_SYM_OBIS_D3E_ON         SymbolON(17,0)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_SYM_OBIS_D3F_ON segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
#define _LCD_SYM_OBIS_D3F_ON         SymbolON(17,1)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_SYM_OBIS_D3G_ON segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
#define _LCD_SYM_OBIS_D3G_ON         SymbolON(17,2)


    /* Special symbols OFF */
/***************************************************************************//*!
 * @brief   Macro displays _LCD_CHAR_V_S3_OFF segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define _LCD_CHAR_V_S3_OFF       SymbolOFF(31,0)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_CHAR_V_S6_OFF segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define _LCD_CHAR_V_S6_OFF       SymbolOFF(31,5)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_CHAR_V_S7_OFF segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define _LCD_CHAR_V_S7_OFF       SymbolOFF(32,1)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_CHAR_V_S9_OFF segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define _LCD_CHAR_V_S9_OFF       SymbolOFF(31,4)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_CHAR_V_S11_OFF segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define _LCD_CHAR_V_S11_OFF      SymbolOFF(32,4)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_CHAR_V_S12_OFF segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define _LCD_CHAR_V_S12_OFF      SymbolOFF(32,2)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_CHAR_V_S1_OFF segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define _LCD_CHAR_V_S13_OFF      SymbolOFF(32,3)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_CHAR_I_S8_OFF segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define _LCD_CHAR_I_S8_OFF       SymbolOFF(31,2)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_CHAR_I_S12_OFF segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define _LCD_CHAR_I_S12_OFF      SymbolOFF(32,2)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_DOT_P3_OFF segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define _LCD_DOT_P3_OFF          SymbolOFF(18,1)

   
//------------------------------------------------------------------------------
/***************************************************************************//*!
 * @brief   Macro displays _LCD_CHAR_T_OFF segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define _LCD_CHAR_T_OFF        SymbolOFF(38,0)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_COLS_OFF segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define _LCD_COLS_OFF         SymbolOFF(20,0)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_COMMA_OFF segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define _LCD_COMMA_OFF         SymbolOFF(20,2)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_SYM_T1_OFF segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define _LCD_SYM_T1_OFF        SymbolOFF(28,0)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_SYM_T2_OFF segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define _LCD_SYM_T2_OFF        SymbolOFF(26,0)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_SYM_T3_OFF segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define _LCD_SYM_T3_OFF        SymbolOFF(24,0)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_SYM_T4_OFF segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define _LCD_SYM_T4_OFF        SymbolOFF(22,0)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_SYM_kW_OFF segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define _LCD_SYM_kW_OFF        SymbolOFF(19,0)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_SYM_h_OFF segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define _LCD_SYM_h_OFF         SymbolOFF(17,3)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_SYM_BATT_OFF segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define _LCD_SYM_BATT_OFF      SymbolOFF(36,0)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_SYM_WRN_OFF segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define _LCD_SYM_WRN_OFF       SymbolOFF(34,0)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_SYM_LOCK_OFF segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define _LCD_SYM_LOCK_OFF      SymbolOFF(32,0)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_SYM_OBIS_DOTS_OFF segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
    #define _LCD_SYM_OBIS_DOTS_OFF  SymbolOFF(19,3)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_SYM_OBIS_ONE_OFF segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
#define _LCD_SYM_OBIS_ONE_OFF       SymbolOFF(18,1)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_SYM_OBIS_D1B_C_OFF segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
#define _LCD_SYM_OBIS_D1B_C_OFF     SymbolOFF(20,1)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_SYM_OBIS_D1A_E_OFF segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
#define _LCD_SYM_OBIS_D1A_E_OFF     SymbolOFF(20,3)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_SYM_OBIS_D2B_OFF segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
#define _LCD_SYM_OBIS_D2B_OFF       SymbolOFF(19,1)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_SYM_OBIS_D2E_OFF segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
#define _LCD_SYM_OBIS_D2E_OFF       SymbolOFF(19,2)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_SYM_OBIS_D3B_OFF segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
#define _LCD_SYM_OBIS_D3B_OFF       SymbolOFF(19,3)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_SYM_OBIS_D3A_OFF segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
#define _LCD_SYM_OBIS_D3A_OFF         SymbolOFF(18,0)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_SYM_OBIS_D3C_OFF segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
#define _LCD_SYM_OBIS_D3C_OFF         SymbolOFF(18,2)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_SYM_OBIS_D3D_OFF segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
#define _LCD_SYM_OBIS_D3D_OFF         SymbolOFF(18,3)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_SYM_OBIS_D3E_OFF segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
#define _LCD_SYM_OBIS_D3E_OFF         SymbolOFF(17,0)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_SYM_OBIS_D3F_OFF segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
#define _LCD_SYM_OBIS_D3F_OFF         SymbolOFF(17,1)

/***************************************************************************//*!
 * @brief   Macro displays _LCD_SYM_OBIS_D3G_OFF segments.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
#define _LCD_SYM_OBIS_D3G_OFF         SymbolOFF(17,2)

    /* Segments ON  */
    #define Segment38_ON    SegmentsON ( 38,0x0f )
    #define Segment37_ON    SegmentsON ( 37,0x0f )
    #define Segment36_ON    SegmentsON ( 36,0x0f )
    #define Segment35_ON    SegmentsON ( 35,0x0f )
    #define Segment34_ON    SegmentsON ( 34,0x0f )
    #define Segment33_ON    SegmentsON ( 33,0x0f )
    #define Segment32_ON    SegmentsON ( 32,0x0f )
    #define Segment31_ON    SegmentsON ( 31,0x0f )
    #define Segment30_ON    SegmentsON ( 30,0x0f )
    #define Segment29_ON    SegmentsON ( 29,0x0f )
    #define Segment28_ON    SegmentsON ( 28,0x0f )
    #define Segment27_ON    SegmentsON ( 27,0x0f )
    #define Segment26_ON    SegmentsON ( 26,0x0f )
    #define Segment25_ON    SegmentsON ( 25,0x0f )
    #define Segment24_ON    SegmentsON ( 24,0x0f )
    #define Segment23_ON    SegmentsON ( 23,0x0f )
    #define Segment22_ON    SegmentsON ( 22,0x0f )
    #define Segment21_ON    SegmentsON ( 21,0x0f )
    #define Segment20_ON    SegmentsON ( 20,0x0f )
    #define Segment19_ON    SegmentsON ( 19,0x0f )
    #define Segment18_ON    SegmentsON ( 18,0x0f )
    #define Segment17_ON    SegmentsON ( 17,0x0f )


     /* Segments OFF  */
    #define Segment38_OFF    SegmentsOFF(38,0x0f )
    #define Segment37_OFF    SegmentsOFF(37,0x0f )
    #define Segment36_OFF    SegmentsOFF(36,0x0f )
    #define Segment35_OFF    SegmentsOFF(35,0x0f )
    #define Segment34_OFF    SegmentsOFF(34,0x0f )
    #define Segment33_OFF    SegmentsOFF(33,0x0f )
    #define Segment32_OFF    SegmentsOFF(32,0x0f )
    #define Segment31_OFF    SegmentsOFF(31,0x0f )
    #define Segment30_OFF    SegmentsOFF(30,0x0f )
    #define Segment29_OFF    SegmentsOFF(29,0x0f )
    #define Segment28_OFF    SegmentsOFF(28,0x0f )
    #define Segment27_OFF    SegmentsOFF(27,0x0f )
    #define Segment26_OFF    SegmentsOFF(26,0x0f )
    #define Segment25_OFF    SegmentsOFF(25,0x0f )
    #define Segment24_OFF    SegmentsOFF(24,0x0f )
    #define Segment23_OFF    SegmentsOFF(23,0x0f )
    #define Segment22_OFF    SegmentsOFF(22,0x0f )
    #define Segment21_OFF    SegmentsOFF(21,0x0f )
    #define Segment20_OFF    SegmentsOFF(20,0x0f )
    #define Segment19_OFF    SegmentsOFF(19,0x0f )
    #define Segment18_OFF    SegmentsOFF(18,0x0f )
    #define Segment17_OFF    SegmentsOFF(17,0x0f )

    /*Map segment to COM mask - LINE0*/
    #define  L0SEGA   0x02
    #define  L0SEGB   0x08
    #define  L0SEGC   0x40
    #define  L0SEGD   0x80
    #define  L0SEGE   0x20
    #define  L0SEGF   0x04
    #define  L0SEGG   0x10
    

    /*Map segment to COM mask - LINE0*/
/*
    #define  L0SEGA   0x02
    #define  L0SEGB   0x04
    #define  L0SEGC   0x08
    #define  L0SEGD   0x01
    #define  L0SEGE   0x02
    #define  L0SEGF   0x04
    #define  L0SEGG   0x08
*/

#endif /* __LCDHWD_H */ 
