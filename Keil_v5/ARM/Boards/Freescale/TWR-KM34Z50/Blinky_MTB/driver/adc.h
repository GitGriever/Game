/*****************************************************************************
 * (c) Copyright 2010-2012, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ***************************************************************************//*!
 * @file      adc.h
 * @version   1.0.2.0
 * @date      May-14-2012
 * @brief     Analog-to-Digital Converter (ADC) driver header file.
 *
 * Revision History:
 *  1.	01/01/2010   R55013   Initially written
 *  2.	01/08/2012   B34185   Ported to Kinetis L
 *  3.	01/08/2012   R55013   Ported to Kinetis M
 *  4.	16/04/2013   By ARM   Modifed for RVCT & CMSIS adaptation
 ******************************************************************************/
 /******************************************************************************
 *//*! @addtogroup adc_example
 * @{
 * @par       Source code:
 * @include   adc_test\adc_test.c
 * @par       Appconfig.h:
 * @include   adc_test\appconfig.h  
 ******************************************************************************/
 /*! @} End of adc_example                                                    */
#ifndef __ADC_H
#define __ADC_H 

#include "MKM34Z5.h"

/* ADC - Register accessors */
#define ADC_SC1_REG(base,index)                  ((base)->SC1[index])
#define ADC_CFG1_REG(base)                       ((base)->CFG1)
#define ADC_CFG2_REG(base)                       ((base)->CFG2)
#define ADC_R_REG(base,index)                    ((base)->R[index])
#define ADC_CV1_REG(base)                        ((base)->CV1)
#define ADC_CV2_REG(base)                        ((base)->CV2)
#define ADC_SC2_REG(base)                        ((base)->SC2)
#define ADC_SC3_REG(base)                        ((base)->SC3)
#define ADC_OFS_REG(base)                        ((base)->OFS)
#define ADC_PG_REG(base)                         ((base)->PG)
#define ADC_CLPD_REG(base)                       ((base)->CLPD)
#define ADC_CLPS_REG(base)                       ((base)->CLPS)
#define ADC_CLP4_REG(base)                       ((base)->CLP4)
#define ADC_CLP3_REG(base)                       ((base)->CLP3)
#define ADC_CLP2_REG(base)                       ((base)->CLP2)
#define ADC_CLP1_REG(base)                       ((base)->CLP1)
#define ADC_CLP0_REG(base)                       ((base)->CLP0)

/* ADC - Peripheral instance base addresses */
/** Peripheral ADC base pointer */
#define ADC_BASE_PTR                             (ADC)

/* ----------------------------------------------------------------------------
   -- ADC - Register accessor macros
   ---------------------------------------------------------------------------- */

/**
 * @addtogroup ADC_Register_Accessor_Macros ADC - Register accessor macros
 * @{
 */


/* ADC - Register instance definitions */
/* ADC */
#define ADC_SC1A                                 ADC_SC1_REG(ADC_BASE_PTR,0)
#define ADC_SC1B                                 ADC_SC1_REG(ADC_BASE_PTR,1)
#define ADC_SC1C                                 ADC_SC1_REG(ADC_BASE_PTR,2)
#define ADC_SC1D                                 ADC_SC1_REG(ADC_BASE_PTR,3)
#define ADC_CFG1                                 ADC_CFG1_REG(ADC_BASE_PTR)
#define ADC_CFG2                                 ADC_CFG2_REG(ADC_BASE_PTR)
#define ADC_RA                                   ADC_R_REG(ADC_BASE_PTR,0)
#define ADC_RB                                   ADC_R_REG(ADC_BASE_PTR,1)
#define ADC_RC                                   ADC_R_REG(ADC_BASE_PTR,2)
#define ADC_RD                                   ADC_R_REG(ADC_BASE_PTR,3)
#define ADC_CV1                                  ADC_CV1_REG(ADC_BASE_PTR)
#define ADC_CV2                                  ADC_CV2_REG(ADC_BASE_PTR)
#define ADC_SC2                                  ADC_SC2_REG(ADC_BASE_PTR)
#define ADC_SC3                                  ADC_SC3_REG(ADC_BASE_PTR)
#define ADC_OFS                                  ADC_OFS_REG(ADC_BASE_PTR)
#define ADC_PG                                   ADC_PG_REG(ADC_BASE_PTR)
#define ADC_CLPD                                 ADC_CLPD_REG(ADC_BASE_PTR)
#define ADC_CLPS                                 ADC_CLPS_REG(ADC_BASE_PTR)
#define ADC_CLP4                                 ADC_CLP4_REG(ADC_BASE_PTR)
#define ADC_CLP3                                 ADC_CLP3_REG(ADC_BASE_PTR)
#define ADC_CLP2                                 ADC_CLP2_REG(ADC_BASE_PTR)
#define ADC_CLP1                                 ADC_CLP1_REG(ADC_BASE_PTR)
#define ADC_CLP0                                 ADC_CLP0_REG(ADC_BASE_PTR)

/* ADC - Register array accessors */
#define ADC_SC1(index)                           ADC_SC1_REG(ADC_BASE_PTR,index)
#define ADC_R(index)                             ADC_R_REG(ADC_BASE_PTR,index)

/******************************************************************************
 * ADC calibration data structure definition                                  *
 ******************************************************************************/
typedef struct { uint32 OFS, PG, MG, CLPD, CLPS, CLP[5]; } tADC_CALIB;
 
/******************************************************************************
 * definition configuration structure                                         *
 ******************************************************************************/
typedef struct { uint32 CFG1, CFG2, CV1, CV2, SC2, SC3, PGA; } tADC;
typedef struct { uint32 SC1; } tADC_CH;

/******************************************************************************
* ADC hardware averaging factors definition
*
*//*! @addtogroup adc_avg_list
* @{
*******************************************************************************/
#define HWAVG_OFF   (uint8)0xff ///< hardware averaging disabled
#define HWAVG_04    (uint8)0x00 ///< 4 samples averaged.
#define HWAVG_08    (uint8)0x01 ///< 8 samples averaged.
#define HWAVG_16    (uint8)0x02 ///< 16 samples averaged.
#define HWAVG_32    (uint8)0x03 ///< 32 samples averaged.
/*! @} End of adc_avg_list                                                        */

/******************************************************************************
* ADC input channel definition
*
*//*! @addtogroup adc_channel
* @{
* This device contains 12 single ended conversion channels tied to the package pins
* (number varies on each package).
*******************************************************************************/
#define AD3         (uint8)0    ///< Single ended input from pin
#define AD6         (uint8)1    ///< Single ended input from pin
#define AD8         (uint8)2    ///< Single ended input from pin
#define AD9         (uint8)3    ///< Single ended input from pin
#define VLL1        (uint8)4    ///< LCD VLL1 Voltage
#define VCAP1       (uint8)5    ///< LCD VCAP1 Voltage
#define AD0         (uint8)6    ///< Single ended input from pin
#define AD1         (uint8)7    ///< Single ended input from pin
#define VLL2        (uint8)8    ///< LCD VLL2 Voltage
#define VCAP2       (uint8)9    ///< LCD VCAP2 Voltage
#define AD2         (uint8)10   ///< Single ended input from pin
#define AD4         (uint8)11   ///< Single ended input from pin
#define PMC_VDD     (uint8)12   ///< VDD supply from PMC
#define PMC_VDD_SW  (uint8)13   ///< Switched VDD supply from PMC
#define SAR_VDDA    (uint8)14   ///< VDDA supply for SAR
#define AD5         (uint8)15   ///< Single ended input from pin
#define AD7         (uint8)16   ///< Single ended input from pin
#define HSCMP0DAC   (uint8)17   ///< HSCMP 0 DAC output voltage
#define HSCMP1DAC   (uint8)18   ///< HSCMP 1 DAC output voltage
#define AD10        (uint8)19   ///< Single ended input from pin
#define AD11        (uint8)20   ///< Single ended input from pin
#define TEMP_SENSOR (uint8)26   ///< Internal to SAR ADC
#define PMC1VBGPSRC (uint8)27   ///< Internal to SAR ADC
#define AD_OFF      (uint8)31   ///< channel disabled
/*! @} End of adc_channel                                                     */

/******************************************************************************
* ADC module default configurations used by ADC_Init() function.
*
*//*! @addtogroup adc_module_config
* @{
*******************************************************************************/
/**************************************************************************/ /*!
 * @brief Selects 16-bit software triggered mode with internal voltage reference.
 *****************************************************************************/
#define ADC_MODULE_16B_SWTRG_IREF_CONFIG                                      \
(tADC){                                                                       \
/* CFG1   */  CLR(ADC_CFG1_ADLPC_MASK)|SET(ADC_CFG1_ADIV(3))|                 \
              SET(ADC_CFG1_ADLSMP_MASK)|SET(ADC_CFG1_MODE(3))|                \
              SET(ADC_CFG1_ADICLK(0)),                                        \
/* CFG2   */  CLR(ADC_CFG2_MUXSEL_MASK)|CLR(ADC_CFG2_ADACKEN_MASK)|           \
              SET(ADC_CFG2_ADHSC_MASK)|SET(ADC_CFG2_ADLSTS(0)),               \
/* CV1    */  0l,                                                             \
/* CV2    */  0l,                                                             \
/* SC2    */  CLR(ADC_SC2_ADTRG_MASK)|CLR(ADC_SC2_ACFE_MASK)|                 \
              CLR(ADC_SC2_ACFGT_MASK)|CLR(ADC_SC2_ACREN_MASK)|                \
              CLR(ADC_SC2_DMAEN_MASK)|SET(ADC_SC2_REFSEL(1)),                 \
/* SC3    */  CLR(ADC_SC3_CAL_MASK)|CLR(ADC_SC3_ADCO_MASK)|                   \
              CLR(ADC_SC3_AVGE_MASK)|SET(ADC_SC3_AVGS(0)),                    \
}
/**************************************************************************/ /*!
 * @brief Selects 16-bit software triggered mode with external voltage reference.
 *****************************************************************************/
#define ADC_MODULE_16B_SWTRG_XREF_CONFIG                                      \
(tADC){                                                                       \
/* CFG1   */  CLR(ADC_CFG1_ADLPC_MASK)|SET(ADC_CFG1_ADIV(3))|                 \
              SET(ADC_CFG1_ADLSMP_MASK)|SET(ADC_CFG1_MODE(3))|                \
              SET(ADC_CFG1_ADICLK(0)),                                        \
/* CFG2   */  CLR(ADC_CFG2_MUXSEL_MASK)|CLR(ADC_CFG2_ADACKEN_MASK)|           \
              SET(ADC_CFG2_ADHSC_MASK)|SET(ADC_CFG2_ADLSTS(0)),               \
/* CV1    */  0l,                                                             \
/* CV2    */  0l,                                                             \
/* SC2    */  CLR(ADC_SC2_ADTRG_MASK)|CLR(ADC_SC2_ACFE_MASK)|                 \
              CLR(ADC_SC2_ACFGT_MASK)|CLR(ADC_SC2_ACREN_MASK)|                \
              CLR(ADC_SC2_DMAEN_MASK)|SET(ADC_SC2_REFSEL(0)),                 \
/* SC3    */  CLR(ADC_SC3_CAL_MASK)|CLR(ADC_SC3_ADCO_MASK)|                   \
              CLR(ADC_SC3_AVGE_MASK)|SET(ADC_SC3_AVGS(0)),                    \
}
/**************************************************************************/ /*!
 * @brief Selects 16-bit hardware triggered mode with internal voltage reference.
 *****************************************************************************/
#define ADC_MODULE_16B_HWTRG_IREF_CONFIG                                      \
(tADC){                                                                       \
/* CFG1   */  CLR(ADC_CFG1_ADLPC_MASK)|SET(ADC_CFG1_ADIV(3))|                 \
              SET(ADC_CFG1_ADLSMP_MASK)|SET(ADC_CFG1_MODE(3))|                \
              SET(ADC_CFG1_ADICLK(0)),                                        \
/* CFG2   */  CLR(ADC_CFG2_MUXSEL_MASK)|CLR(ADC_CFG2_ADACKEN_MASK)|           \
              SET(ADC_CFG2_ADHSC_MASK)|SET(ADC_CFG2_ADLSTS(0)),               \
/* CV1    */  0l,                                                             \
/* CV2    */  0l,                                                             \
/* SC2    */  SET(ADC_SC2_ADTRG_MASK)|CLR(ADC_SC2_ACFE_MASK)|                 \
              CLR(ADC_SC2_ACFGT_MASK)|CLR(ADC_SC2_ACREN_MASK)|                \
              CLR(ADC_SC2_DMAEN_MASK)|SET(ADC_SC2_REFSEL(1)),                 \
/* SC3    */  CLR(ADC_SC3_CAL_MASK)|CLR(ADC_SC3_ADCO_MASK)|                   \
              CLR(ADC_SC3_AVGE_MASK)|SET(ADC_SC3_AVGS(0)),                    \
}
/**************************************************************************/ /*!
 * @brief Selects 16-bit hardware triggered mode with external voltage reference.
 *****************************************************************************/
#define ADC_MODULE_16B_HWTRG_XREF_CONFIG                                      \
(tADC){                                                                       \
/* CFG1   */  CLR(ADC_CFG1_ADLPC_MASK)|SET(ADC_CFG1_ADIV(3))|                 \
              SET(ADC_CFG1_ADLSMP_MASK)|SET(ADC_CFG1_MODE(3))|                \
              SET(ADC_CFG1_ADICLK(0)),                                        \
/* CFG2   */  CLR(ADC_CFG2_MUXSEL_MASK)|CLR(ADC_CFG2_ADACKEN_MASK)|           \
              SET(ADC_CFG2_ADHSC_MASK)|SET(ADC_CFG2_ADLSTS(0)),               \
/* CV1    */  0l,                                                             \
/* CV2    */  0l,                                                             \
/* SC2    */  SET(ADC_SC2_ADTRG_MASK)|CLR(ADC_SC2_ACFE_MASK)|                 \
              CLR(ADC_SC2_ACFGT_MASK)|CLR(ADC_SC2_ACREN_MASK)|                \
              CLR(ADC_SC2_DMAEN_MASK)|SET(ADC_SC2_REFSEL(0)),                 \
/* SC3    */  CLR(ADC_SC3_CAL_MASK)|CLR(ADC_SC3_ADCO_MASK)|                   \
              CLR(ADC_SC3_AVGE_MASK)|SET(ADC_SC3_AVGS(0)),                    \
}
/*! @} End of adc_module_config                                           */

/******************************************************************************
* ADC module default configurations used by ADC_Init() function.
*
*//*! @addtogroup adc_chan_config
* @{
*******************************************************************************/
/**************************************************************************/ /*!
 * @brief Single ended conversion, interrupt enabled.
 * @param   chan    Select channel @ref adc_channel
 *****************************************************************************/
#define ADC_CH_SE_IRQ_CONFIG(chan)                                            \
(tADC_CH){                                                                    \
/* SC1    */ SET(ADC_SC1_AIEN_MASK)|chan,                                     \
}
/**************************************************************************/ /*!
 * @brief Single ended conversion, pooling mode (interrupt disabled).
 * @param   chan    Select channel @ref adc_channel
 *****************************************************************************/
#define ADC_CH_SE_POLL_CONFIG(chan)                                           \
(tADC_CH){                                                                    \
/* SC1    */ CLR(ADC_SC1_AIEN_MASK)|chan,                                     \
}
/**************************************************************************/ /*!
 * @brief Channel disabled.
 *****************************************************************************/
#define ADC_CH_DISABLE_CONFIG    			                                        \
(tADC_CH){                                                                    \
/* SC1    */ CLR(ADC_SC1_AIEN_MASK)|AD_OFF,                                   \
}
/*! @} End of adc_chan_config                                                 */


/******************************************************************************
* ADC callback registered by ADC_Init() function
*
*//*! @addtogroup adc_callback
* @{
*******************************************************************************/
/*! ADC_CALLBACK type declaration                                             */
typedef enum 
{
  CHA_CALLBACK=(1<<0),  ///< Status and Control Register A Conversion Complete
  CHB_CALLBACK=(1<<1),  ///< Status and Control Register B Conversion Complete
  CHC_CALLBACK=(1<<2),  ///< Status and Control Register C Conversion Complete
  CHD_CALLBACK=(1<<3),  ///< Status and Control Register D Conversion Complete
} ADC_CALLBACK_TYPE;

/*! @brief ADC_CALLBACK function declaration                                  */
typedef void (*ADC_CALLBACK)(ADC_CALLBACK_TYPE type, register int16 result);
/*! @} End of adc_callback                                                    */

/******************************************************************************
* ADC function and macro definitions
*
*//*! @addtogroup adc_macro
* @{
*******************************************************************************/
#define ADC_CHA_Start(inp)      REG_SetVal(ADC_SC1A,ADC_SC1_ADCH,inp)
/***************************************************************************//*!
 * @brief   Macro starts conversion by software.
 * @details This macro triggers conversion by software. Note that only conversion 
 *          upon SC1A register (CHA) can be triggered by both software and hardware. 
 *          The conversions upon SC1B (CHB),SC1C (CHC) and SC1D (CHD) registers can 
 *          only be triggered by HW trigger source.
 * @param   chan    CHA
 * @param   inp     Use one of following channel @ref adc_channel.                 
 * @note    Implemented as inlined macro.
 ******************************************************************************/
#define ADC_Start(chan,inp)     ADC_##chan##_Start(inp)

#define ADC_CHA_Ready           ((ADC_SC1A & ADC_SC1_COCO_MASK) ? TRUE : FALSE)
#define ADC_CHB_Ready           ((ADC_SC1B & ADC_SC1_COCO_MASK) ? TRUE : FALSE)
#define ADC_CHC_Ready           ((ADC_SC1C & ADC_SC1_COCO_MASK) ? TRUE : FALSE)
#define ADC_CHD_Ready           ((ADC_SC1D & ADC_SC1_COCO_MASK) ? TRUE : FALSE)
/***************************************************************************//*!
 * @brief   Macro returns non zero value when conversion completed.
 * @details This macro returns non zero value when conversion completed and 
 *          new value converted by the ADC resides in the result register. 
 *          Read value from the respective result register by @ref ADC_Read(). 
 * @param   chan    CHA,CHB,CHC,CHD
 * @return  TRUE    conversion completed (COCO flag is asserted)
 * @return  FALSE   conversion pending (COCO flag is deasserted)
 * @note    Implemented as inlined macro.
 ******************************************************************************/
#define ADC_Ready(chan)         ADC_##chan##_Ready

#define ADC_CHA_Read            (int16)ADC_RA
#define ADC_CHB_Read            (int16)ADC_RB
#define ADC_CHC_Read            (int16)ADC_RC
#define ADC_CHD_Read            (int16)ADC_RD
/***************************************************************************//*!
 * @brief   Macro returns value from the result register.
 * @details This macro returns value from the respective result register. 
 * @param   chan    CHA,CHB,CHC,CHD
 * @return  16-bit right-justified value converted by the ADC. The number of 
 *          effective bits depends on the conversion mode.
 * @note    Implemented as inlined macro.
 ******************************************************************************/
#define ADC_Read(chan)          ADC_##chan##_Read

/***************************************************************************//*!
 * @brief   ADC channel initialization function.
 * @details This function initalizes Analogue Digital Converter (ADC). It also 
 *          installs callback function for interrupt vector 32.
 * @param   cfg       Use one of following module configuration structures @ref 
 *                    adc_module_config
 * @param   avg       Use of the following averaging options @ref adc_avg_list
 * @param   CHA       Select channel "A" configuration structure @ref adc_chan_config
 * @param   CHB       Select channel "B" configuration structure @ref adc_chan_config
 * @param   CHC       Select channel "C" configuration structure @ref adc_chan_config
 * @param   CHD       Select channel "D" configuration structure @ref adc_chan_config
 * @param   ip        Interrupt priority 0..3
 * @param   callback  pointer to @ref ADC_CALLBACK function
 * @note    Implemented as function call. 
 * @warning Note that only conversion upon SC1A register (CHA) can be triggered by 
 *          both software and hardware. The conversions upon SC1B (CHB),SC1C (CHC) 
 *          and SC1D (CHD) registers can only be triggered by HW trigger source.
 ******************************************************************************/ 
#define ADC_Init(cfg,avg,CHA,CHB,CHC,CHD,ip,callback)                           \
                                    ADC_Init(cfg,avg,CHA,CHB,CHC,CHD,ip,callback)

/***************************************************************************//*!
 * @brief   ADC calibrationfunction.
 * @details This function runs calibration procedure and updates calibration 
 *          registers of the ADC module.
 * @param   cfg       Select configuration structure:\n
 *                    @ref ADC_MODULE_16B_SWTRG_IREF_CONFIG\n
 *                    @ref ADC_MODULE_16B_SWTRG_XREF_CONFIG\n
 * @param   calib     Pointer to tADC_CALIB structure.
 * @return  FALSE     calibration sucessfull\n
 * @return  TRUE      error occured during calibration\n
 * @note    Implemented as function call. 
 ******************************************************************************/ 
#define ADC_ExecCalib(cfg,calib)    ADC_ExecCalib(cfg,calib)

/***************************************************************************//*!
 * @brief   Function updates register of the ADC by calibration values.
 * @details This updates register of the ADC by calibration values from the structure
 *          passed by pointer.
 * @param   calib     Pointer to tADC_CALIB structure.
 * @note    Implemented as function call. 
 ******************************************************************************/ 
#define ADC_SaveCalib(calib)        ADC_SaveCalib(calib)
/*! @} End of adc_macro                                                       */
  
/******************************************************************************
 * public function prototypes                                                 *
 ******************************************************************************/    
extern int  ADC_ExecCalib (tADC adc, tADC_CALIB *pCALIB);
extern void ADC_SaveCalib (tADC_CALIB *pCALIB);
extern void ADC_Init      (tADC adc, uint8 avg, tADC_CH CHA, tADC_CH CHB, tADC_CH CHC, 
                           tADC_CH CHD, uint8 ip, ADC_CALLBACK pCallback);
   
#endif /* __ADC_H */ 
