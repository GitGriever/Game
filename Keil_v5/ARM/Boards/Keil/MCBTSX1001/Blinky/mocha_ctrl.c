/******************** (C) COPYRIGHT 2012 Triad Semiconductor, Inc.**************
* File Name          : mocha_ctrl.c
* Version            : V1.00
* Date               : 23 April 2012
* Description        : MOCHA Configuration File for Device TSX1001
*
*******************************************************************************/

#include <TSX1001.h>
#include "mocha_ctrl.h"

//-----------------------------------------------------------------------------
// HSS routines Mocha1 platform
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// HSS EEPROM routines
// page write need to run in RAM ... not sure how to get Keil to do that automatically
//-----------------------------------------------------------------------------
// Subroutine to write data to EEPROM page
// Inputs: page_number - index of page to write data to (32 word page)
//         page_wr_map - bit map of EEPROM word address to write to with page
//         page_data - 32x32 array of data to write to EEPROM
uint32_t Mocha_EEPROMPageWrite (uint32_t page_number, uint32_t page_wr_map, EEPROM_Page_TypeDef *page_data) //__ram
{
  uint32_t index_mask;
  uint32_t index_count;
  uint32_t address;
  const uint32_t EEPROMPageUpperAddress  = 0x000000FFUL;

  // check address for valid EEPROM address range
  if (page_number > EEPROMPageUpperAddress)
  {
    return 1;
  }

  index_mask = 1;
  address = (page_number << 5);

  for (index_count = 0; index_count < 32; index_count++)
  {
    if ((index_mask & page_wr_map) > 0)
    {
      EEPROM->DATA[address] = page_data->DATA[index_count];
    }
    address++;
    index_mask = index_mask << 1;
  }

  return 0;
}


// Subroutine to enable EEPROM writability
void Mocha_EEPROMWriteEnable ()
{
  SYSCTRL->DATA[0] |= EE_WREN;
}

// Subroutine to disable EEPROM writability
void Mocha_EEPROMWriteDisable ()
{
  SYSCTRL->DATA[0] &= !EE_WREN;
}

// Subroutine to disable EEPROM page write feature
void Mocha_EEPROMPageModeDisable ()
{
  SYSCTRL->DATA[0] |= EE_WORD ;
}

// Subroutine to enable EEPROM page write feature
void Mocha_EEPROMPageModeEnable ()
{
  SYSCTRL->DATA[0] &= !EE_WORD;
}



// Subroutine to set number of wait state for EEPROM accesses
// Input: number of wait states
// 0 - Zero wait state
// 1 - One wait state
// 2 - Two wait states
// 3 - Three wait states
void Mocha_EEPROMWaitStatesWrite(uint32_t wait_states)
{
  SYSCTRL->DATA[0] = (SYSCTRL->DATA[0] & 0xFFFCFFFFUL) | (wait_states << 16);
}

// Subroutine to read number of EEPROM wait states
// Returns: Number of EEPROM wait states
// Range of 0 to 3
uint32_t Mocha_EEPROMWaitStatesRead()
{
  return ((SYSCTRL->DATA[0] & 0x00030000UL) >> 16);
}

// Subroutine to enable speculative EEPROM reading
void Mocha_EEPROMSpeculativeReadEnable()
{
  SYSCTRL->DATA[0] = SYSCTRL->DATA[0] | 0x00400000UL;
}

// Subroutine to disable speculative EEPROM reading
void Mocha_EEPROMSpeculativeReadDisable()
{
  SYSCTRL->DATA[0] = SYSCTRL->DATA[0] & 0xFFBFFFFFUL;
}

// Subroutine to disable EEPROM write stalling
void Mocha_EEPROMWriteStallDisable()
{
  SYSCTRL->DATA[0] = SYSCTRL->DATA[0] | 0x00800000UL;
}

// Subroutine to enable EEPROM write stalling
void Mocha_EEPROMWriteStallEnable()
{
  SYSCTRL->DATA[0] = SYSCTRL->DATA[0] & 0xFF7FFFFFUL;
}

// Subroutine to set APB bus clock rate
// Input: clock ratio of range 0 to 3
// 0 - 1:1 clock divider ratio
// 1 - 1:2 clock divider ratio
// 2 - 1:3 clock divider ratio
// 3 - 1:4 clock divider ratio
void Mocha_ApbClockRateWrite (uint32_t clock_rate)
{
  SYSCTRL->DATA[0] = (SYSCTRL->DATA[0] & 0xFFFFFCFFUL) | (clock_rate << 8);
}

// Subroutine to read APB bus clock rate
// Returns: clock ratio of range 0 to 3
// 0 - 1:1 clock divider ratio
// 1 - 1:2 clock divider ratio
// 2 - 1:3 clock divider ratio
// 3 - 1:4 clock divider ratio
uint32_t Mocha_ApbClockRateRead ()
{
  return ((SYSCTRL->DATA[0] & 0x00000300UL) >> 8);
}

// Subroutine to set Mocha System clock rate
// Input: clock ratio
// Range of 0(1:16), 1(1:8), 2(1:4), 3(1:8), 4->7(1:1)
void Mocha_ClockRateWrite (uint32_t clock_rate)
{
  SYSCTRL->DATA[0] = (SYSCTRL->DATA[0] & 0xFFFFFFF8UL) | clock_rate;
}

// Subroutine to read Mocha System clock rate
// Returns: clock ratio
// Range of 0(1:16), 1(1:8), 2(1:4), 3(1:8), 4->7(1:1)
uint32_t Mocha_ClockRateRead ()
{
  return (SYSCTRL->DATA[0] & 0x00000007UL);
}

// Subroutine to set IRQ latency
// Inputs: latency of range 0->255
void Mocha_IRQLatencyWrite (uint32_t latency)
{
  SYSCTRL->DATA[1] = (SYSCTRL->DATA[1] & 0xFFFFFF00UL) | latency;
}

// Subroutine to read IRQ latency
// Returns: latency of range 0->255
uint32_t Mocha_IRQLatencyRead ()
{
  return (SYSCTRL->DATA[1]  & 0x000000FFUL);
}

// Subroutine to enable Keep Peripherals indicator
void Mocha_KPAEnable()
{
  SYSCTRL->DATA[1] = SYSCTRL->DATA[1] | 0x00200000UL;
}

// Subroutine to disable Keep Peripherals indicator
void Mocha_KPADisable()
{
  SYSCTRL->DATA[1] = SYSCTRL->DATA[1] & 0xFFDFFFFFUL;
}

// Subroutine to enable Stop Req indicator
void Mocha_StopReqEnable()
{
  SYSCTRL->DATA[1] = SYSCTRL->DATA[1] | 0x00100000UL;
}

// Subroutine to disable Stop Req indicator
void Mocha_StopReqDisable()
{
  SYSCTRL->DATA[1] = SYSCTRL->DATA[1] & 0xFFEFFFFFUL;
}

// Subroutine to enable WIC Req indicator
void Mocha_WICReqEnable()
{
  SYSCTRL->DATA[1] = SYSCTRL->DATA[1] | 0x00010000UL;
}

// Subroutine to disable WIC Req indicator
void Mocha_WICReqDisable()
{
  SYSCTRL->DATA[1] = SYSCTRL->DATA[1] & 0xFFFEFFFFUL;
}

// Subroutine to read WIC line wake indicator
uint32_t Mocha_WakeIndicatorRead()
{
  return (SYSCTRL->DATA[2] & 0x0003FFFFUL);
}

// Subroutine to write RAM overlay configuration
// Inputs: ram_ovly - RAM_OVLY defines how much of the EEPROM
//                    starting at address 0 is overlaid with RAM.
//                    The overlay increment is 256 bytes allowing:
// 0   -  no RAM overlay
// 1   -  lower 256 bytes of RAM aliases starting at address 0
// 2   -  lower 512 bytes of RAM aliases starting at address 0
// 3   -  lower 768 bytes of RAM aliases starting at address 0
// 4   -  lower 1024 bytes of RAM aliases starting at address 0
// 5   -  lower 1280 bytes of RAM aliases starting at address 0
// 6   -  lower 1536 bytes of RAM aliases starting at address 0
// 7   -  lower 1792 bytes of RAM aliases starting at address 0
// 8   -  lower 2048 bytes of RAM aliases starting at address 0
// 9   -  lower 2304 bytes of RAM aliases starting at address 0
// 10  -  lower 2560 bytes of RAM aliases starting at address 0
// 11  -  lower 2816 bytes of RAM aliases starting at address 0
// 12  -  lower 3072 bytes of RAM aliases starting at address 0
// 13  -  lower 3328 bytes of RAM aliases starting at address 0
// 14  -  lower 3584 bytes of RAM aliases starting at address 0
// 15  -  lower 3840 bytes of RAM aliases starting at address 0
//
// Returns: 1 - configuration value not assigned
//          0 - valid configuration value assigned
uint32_t Mocha_RAMOverlayWrite(uint32_t ram_ovly)
{
  volatile uint32_t CalcValue;

  // check for valid configuration value
  // if value larger than 0xF then return error condition
  if ((ram_ovly & 0xFFFFFFF0UL) > 0)
  {
    return 1;
  }

  // assign RAM overlay configuration value
  CalcValue = (SYSCTRL->DATA[0] & 0xF0FFFFFFUL) | (0x0F000000UL & (ram_ovly << 24));
  SYSCTRL->DATA[0] = CalcValue;

  // check that value assigned
  // if not equivalent then return error condition
  if (SYSCTRL->DATA[0] != CalcValue)
  {
    return 1;
  }

  // assigned value
  return 0;
}

// Subroutine to read RAM overlay configuration
// Returns: 0-based configuration value
uint32_t Mocha_RAMOverlayRead()
{
  volatile uint32_t CalcValue;

  CalcValue = ((SYSCTRL->DATA[0] >> 24) & 0x0000000FUL);
  return CalcValue;
}

// Subroutine to set EESPECMASK
// Input: speculative reads suppression mask
// EESPECMASK[2]
//   0 ignore branch hint when EESPEC=1 (speculative reads are enabled)
//   1 suppress speculative reads when branch instruction are in decode
// EESPECMASK[1]
//   0 ignore backward conditional branch hint when EESPEC=1 (speculative reads are enabled)
//   1 suppress speculative reads when backward unresolved conditional branch instruction are in decode
// EESPECMASK[0]
//   0 ignore forward conditional branch hint when EESPEC=1 (speculative reads are enabled)
//   1 suppress speculative reads when forward unresolved conditional branch instruction are in decode

void Mocha_EEPROMSpecMaskWrite(uint32_t mask)
{
  SYSCTRL->DATA[0] = (SYSCTRL->DATA[0] & 0x3FFFFFFFUL) | (mask << 28);
}

// Subroutine to read EESPECMASK
// Returns: EESPECMASK
// Range of 0 to 7
uint32_t Mocha_EEPROMSpecMaskRead(void)
{
  return ((SYSCTRL->DATA[0] & 0x30000000UL) >> 28);
}
