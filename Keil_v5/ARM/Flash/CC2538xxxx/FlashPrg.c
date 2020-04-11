/***********************************************************************/
/*  This file is part of the ARM Toolchain package                     */
/*  Copyright KEIL ELEKTRONIK GmbH 2003 - 2013                         */
/***********************************************************************/
/*                                                                     */
/*  FlashDev.C:  Flash Programming Functions adapted                   */
/*               for TI CC2538xx Flash                                 */
/*                                                                     */
/***********************************************************************/

#include "..\FlashOS.H"        // FlashOS Structures
#include "hapi.H"              // HAPI.H

typedef volatile unsigned char  vu8;
typedef volatile unsigned long  vu32;
typedef volatile unsigned short vu16;

#define M8(adr)  (*((vu8  *) (adr)))
#define M16(adr) (*((vu16 *) (adr)))
#define M32(adr) (*((vu32 *) (adr)))


#define PAGE_SIZE       (2048)
#if defined (CC2538_512KB)
  #define ADR_VALID_IMAGE_PAGE (0x27F800)
  #define VALID_IMAGE_VAL (M32(0x27FFD8))
#elif defined (CC2538_256KB)
  #define ADR_VALID_IMAGE_PAGE (0x23F800)
  #define VALID_IMAGE_VAL (M32(0x23FFD8))
#elif defined (CC2538_128KB)
  #define ADR_VALID_IMAGE_PAGE (0x21F800)
  #define VALID_IMAGE_VAL (M32(0x21FFD8))
#endif
unsigned long base_adr;


/*
 *  Initialize Flash Programming Functions
 *    Parameter:      adr:  Device Base Address
 *                    clk:  Clock Frequency (Hz)
 *                    fnc:  Function Code (1 - Erase, 2 - Program, 3 - Verify)
 *    Return Value:   0 - OK,  1 - Failed
 */

int Init (unsigned long adr, unsigned long clk, unsigned long fnc) {
  //int ret = 0;
  switch (fnc) 
  {
    case 1:
      //if (VALID_IMAGE_VAL == 0) {
      //  ret = HapiPageErase(ADR_VALID_IMAGE_PAGE,PAGE_SIZE);
      //  return(ret);
      //}
      break;
    default:
      break;
  }
  return (0);
}



/*
 *  De-Initialize Flash Programming Functions
 *    Parameter:      fnc:  Function Code (1 - Erase, 2 - Program, 3 - Verify)
 *    Return Value:   0 - OK,  1 - Failed
 */

int UnInit (unsigned long fnc) {
  return (0);
}


/*
 *  Erase complete Flash Memory
 *    Return Value:   0 - OK,  1 - Failed
 */

//int EraseChip (void) {
//  return (0);                                   // Done
//}



/*
 *  Erase Sector in Flash Memory
 *    Parameter:      adr:  Sector Address
 *    Return Value:   0 - OK,  1 - Failed
 */

int EraseSector (unsigned long adr) {
  int ret;
  ret = HapiPageErase(adr,PAGE_SIZE);
  return (ret);                                   // Done
}



/*  
 *  Blank Check Checks if Memory is Blank
 *    Parameter:      adr:  Block Start Address
 *                    sz:   Block Size (in bytes)
 *                    pat:  Block Pattern
 *    Return Value:   0 - OK,  1 - Failed
 */


/*
 *  Program Page in Flash Memory
 *    Parameter:      adr:  Page Start Address
 *                    sz:   Page Size
 *                    buf:  Page Data
 *    Return Value:   0 - OK,  1 - Failed
 */

int ProgramPage (unsigned long adr, unsigned long sz, unsigned char *buf) {
  int ret;
  ret = HapiProgramFlash((unsigned long *)buf,adr, sz);
  return (ret);                                  // Done
}

