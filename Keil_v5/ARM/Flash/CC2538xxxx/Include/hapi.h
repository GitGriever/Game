//------------------------------------------------------------------------------
// TI Confidential – NDA Restrictions
//
// Copyright (c) 2011 Texas Instruments, Inc.
//
//    This is an unpublished work created in the year stated above.
//    Texas Instruments owns all rights in and to this work and
//    intends to maintain and protect it as an unpublished copyright.
//    In the event of either inadvertent or deliberate publication,
//    the above stated date shall be treated as the year of first
//    publication. In the event of such publication, Texas Instruments
//    intends to enforce its rights in the work under the copyright
//    laws as a published work.
//
//
//  Content        : This file contains misc functions residing in ROM.
//
//  The following functions are available for ROM and FLASH firmware through a
//  lookup table:
//
//  - Flash write access
//     - PageErase
//     - ProgramFlash
//  - Utility functions
//     - Ccrc32
//     - GetFlashSize
//     - GetChipId
//  - Standard library functions
//     - memset
//     - memcpy
//     - memcmp
//     - memmove
//
//  Created By     : Low Power RF Wireless Business Unit
//
//------------------------------------------------------------------------------
//  File           : $Id: hapi.h 35061 2011-06-16 12:12:48Z a0181173 $
//------------------------------------------------------------------------------

#ifndef __HAPI_H__
#define __HAPI_H__

//
// Start address of the ROM FW rev field (located after the ROM exception table)
//
#define ROM_FW_REV_FIELD_ADDR (0x00000000 + (17 * sizeof(unsigned long)))
#define ROM_FW_REV_FIELD_SIZE       4
typedef unsigned long ROM_FW_REV_FIELD_T;


//
// ROM Hard-API function interface types
//
// TBD: AMN do we use no return in rambist and reset function below?
typedef unsigned long       (* volatile FPTR_CRC32_T)       (unsigned char* /*pData*/, unsigned long /*byteCount*/);
typedef unsigned long       (* volatile FPTR_GETFLSIZE_T)   (void);
typedef unsigned long       (* volatile FPTR_GETCHIPID_T)   (void);
typedef long                (* volatile FPTR_PAGEERASE_T)   (unsigned long /*FlashAddr*/, unsigned long /*Size*/);
typedef long                (* volatile FPTR_PROGFLASH_T)   (unsigned long* /*pRamData*/, unsigned long /*FlashAdr*/, unsigned long /*ByteCount*/);
typedef void                (* volatile FPTR_RESETDEV_T)    (void);
typedef void*               (* volatile FPTR_MEMSET_T)      (void* /*s*/, signed long /*c*/, unsigned long /*n*/);
typedef void*               (* volatile FPTR_MEMCPY_T)      (void* /*s1*/, const void* /*s2*/, unsigned long /*n*/);
typedef signed long         (* volatile FPTR_MEMCMP_T)      (const void* /*s1*/, const void* /*s2*/, unsigned long /*n*/);
typedef void*               (* volatile FPTR_MEMMOVE_T)     (void* /*s1*/, const void* /*s2*/, unsigned long /*n*/);
typedef void                (* volatile FPTR_RAMBIST_T)     (void);
typedef long                (* volatile FPTR_ROMBIST_T)     (void);

//
// ROM Hard-API access table type
//
typedef struct
    {
        FPTR_CRC32_T        Crc32;
        FPTR_GETFLSIZE_T    GetFlashSize;
        FPTR_GETCHIPID_T    GetChipId;
        FPTR_PAGEERASE_T    PageErase;
        FPTR_PROGFLASH_T    ProgramFlash;
        FPTR_RESETDEV_T     ResetDevice;
        FPTR_MEMSET_T       memset;
        FPTR_MEMCPY_T       memcpy;
        FPTR_MEMCMP_T       memcmp;
        FPTR_MEMMOVE_T      memmove;
        FPTR_RAMBIST_T      RamBist;
        FPTR_ROMBIST_T      RomBist;
    } HARD_API_T;

//
// Start address of the ROM hard API access table (located after the ROM FW rev field)
//
#define ROM_HAPI_TABLE_ADDR (ROM_FW_REV_FIELD_ADDR + ROM_FW_REV_FIELD_SIZE)

//
// Pointer to the ROM HAPI table
//
#define P_HARD_API              ((HARD_API_T*) ROM_HAPI_TABLE_ADDR)

#define HapiCrc32(a,b)          P_HARD_API->Crc32(a,b)
#define HapiGetFlashSize()      P_HARD_API->GetFlashSize()
#define HapiGetChipId()         P_HARD_API->GetChipId()
#define HapiPageErase(a,b)      P_HARD_API->PageErase(a,b)
#define HapiProgramFlash(a,b,c) P_HARD_API->ProgramFlash(a,b,c)
#define HapiResetDevice()       P_HARD_API->ResetDevice()
#define HapiMemset(a,b,c)       P_HARD_API->memset(a,b,c)
#define HapiMemcpy(a,b,c)       P_HARD_API->memcpy(a,b,c)
#define HapiMemcmp(a,b,c)       P_HARD_API->memcmp(a,b,c)
#define HapiMemmove(a,b,c)      P_HARD_API->memmove(a,b,c)
#define HapiRamBist()           P_HARD_API->RamBist()
#define HapiRomBist()           P_HARD_API->RomBist()

//
// Start address of the certificates table (located after the ROM exception table)
//
#define ROM_CERTIFICATES_ADDR (ROM_HAPI_TABLE_ADDR + sizeof(HARD_API_T))
// TBD FIXME UPDATE TO ACTUAL LEN!!!
#define ROM_CERTIFICATES_LEN 128

typedef unsigned long ROM_CERTIFICATES_ELEM_T;
typedef ROM_CERTIFICATES_ELEM_T ROM_CERTIFICATES_T[ROM_CERTIFICATES_LEN];

//
// Pointer to the certificates table
//
#define P_ROM_CERTIFICATES ((ROM_CERTIFICATES_T*) ROM_CERTIFICATES_ADDR)

// CRC check value for ROM crc32 calculation.
// Note: Value should be patched based on actual CRC32 value of ROM contents
#define ROM_CRC_CHECK_ADDR  (0x2000 - 4)

#define ROM_CRC_CHECK_VALUE (0xdeadbeef)
#define P_ROM_CRC_CHECK     ((unsigned long *) ROM_CRC_CHECK_ADDR)

#endif // __HAPI_H__
