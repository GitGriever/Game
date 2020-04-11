/******************** (C) COPYRIGHT 2012 Triad Semiconductor, Inc.**************
* File Name          : mocha_ctrl.h
* Version            : V1.00
* Date               : 23 April 2012
* Description        : MOCHA Header File for
*                      Device TSX1001
*
*******************************************************************************/

#ifndef __MOCHA_CTRL_H__
#define __MOCHA_CTRL_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "TSX1001.h"

extern uint32_t Mocha_EEPROMPageWrite (uint32_t, uint32_t, EEPROM_Page_TypeDef *);
extern void     Mocha_EEPROMWriteEnable(void);
extern void     Mocha_EEPROMWriteDisable(void);
extern void     Mocha_EEPROMPageModeEnable(void);
extern void     Mocha_EEPROMPageModeDisable(void);
extern void     Mocha_EEPROMWaitStatesWrite(uint32_t);
extern uint32_t Mocha_EEPROMWaitStatesRead(void);
extern void     Mocha_EEPROMSpeculativeReadEnable(void);
extern void     Mocha_EEPROMSpeculativeReadDisable(void);
extern void     Mocha_EEPROMWriteStallEnable(void);
extern void     Mocha_EEPROMWriteStallDisable(void);
extern void     Mocha_EEPROMSpecMaskWrite(uint32_t);
extern uint32_t Mocha_EEPROMSpecMaskRead(void);
extern void     Mocha_ApbClockRateWrite (uint32_t);
extern uint32_t Mocha_ApbClockRateRead (void);
extern void     Mocha_ClockRateWrite (uint32_t);
extern uint32_t Mocha_ClockRateRead (void);
extern void     Mocha_IRQLatencyWrite (uint32_t);
extern uint32_t Mocha_IRQLatencyRead (void);
extern void     Mocha_KPAEnable(void);
extern void     Mocha_KPADisable(void);
extern void     Mocha_StopReqEnable(void);
extern void     Mocha_StopReqDisable(void);
extern void     Mocha_WICReqEnable(void);
extern void     Mocha_WICReqDisable(void);
extern uint32_t Mocha_WakeIndicatorRead(void);
extern uint32_t Mocha_RAMOverlayWrite(uint32_t);
extern uint32_t Mocha_RAMOverlayRead(void);
#ifdef __cplusplus
}
#endif

#endif /* __MOCHA_CTRL_H__ */
