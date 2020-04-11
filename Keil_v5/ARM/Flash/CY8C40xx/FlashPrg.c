/* -----------------------------------------------------------------------------
 * Copyright (c) 2014 ARM Ltd.
 *
 * This software is provided 'as-is', without any express or implied warranty. 
 * In no event will the authors be held liable for any damages arising from 
 * the use of this software. Permission is granted to anyone to use this 
 * software for any purpose, including commercial applications, and to alter 
 * it and redistribute it freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not 
 *    claim that you wrote the original software. If you use this software in
 *    a product, an acknowledgment in the product documentation would be 
 *    appreciated but is not required. 
 * 
 * 2. Altered source versions must be plainly marked as such, and must not be 
 *    misrepresented as being the original software. 
 * 
 * 3. This notice may not be removed or altered from any source distribution.
 *   
 *
 * $Date:        15. April 2014
 * $Revision:    V1.00
 *  
 * Project:      Flash Programming Functions for CY8C40xx Device 16kB Flash
 * --------------------------------------------------------------------------- */

/* History:
 *  Version 1.00
 *    Initial release
 */ 

#include "..\FlashOS.H"        // FlashOS Structures
#include "CY8C40xx.H"		   //Registers/constants of CY8C40xx device

//Results constants
#define SUCCESS 0x00
#define FAIL    0x01

//Chip characteristics constants
#define ROW_SIZE		64
#define ROWS_COUNT		256

/*************************************************************************
*	Purpose:	Checks if all bytes in given row are 0s
*	Parameters:  
*				IN	addr		- address of row
*				IN	dataSize	- size of row
*	Return:		
*				0 - empty,	1 - non-empty (at least one bit is HIGH)
*************************************************************************/
int IsBlockEmpty (unsigned long addr, unsigned long blockSize) 
{
	int i;
	unsigned char* chipData = (unsigned char*)addr; //read directly from FLASH
	for(i = 0; i < blockSize; i++)
	{
		if(chipData[i] != 0x00) return FAIL;
	}
	return SUCCESS;
}

/*************************************************************************
*	Purpose:	Write data to register by address
*	Parameter:
*				IN addr		- Device Base Address
*               IN data		- Data
*    Return:
*				0 - empty,	1 - non-empty (at least one bit is HIGH)
*************************************************************************/
int WriteIO (unsigned int addr, unsigned int data)
{
	unsigned int *address = (unsigned int*)addr;
	*address = (unsigned int)data;
	return (0);
}

/*************************************************************************
*	Purpose:	Read data from register by address
*	Parameter:
*				IN addr		- Device Base Address
*				IN data		- Returns data
*	Return:
*			0 - empty,	1 - non-empty (at least one bit is HIGH)
*************************************************************************/
int ReadIO (unsigned int addr, unsigned int *data)
{
	unsigned int *address = (unsigned int*)addr;
	*data = *address;
	return (0);
}

/*************************************************************************
*	Purpose:	Delays execution for given number of ms
*	Parameter:	
*				IN ms	- Miliseconds
*	Return:
*			0 - empty,	1 - non-empty (at least one bit is HIGH)
*************************************************************************/
void Delay(int ms)
{
	int i, time;
  
	for (time = 0; time < ms; time++)
	{
		for (i=0; i<3333; i++); //Delay = 1ms, mesured by scope for CY8C40xx (CPU = 48MHz)
	}
}

/*************************************************************************
*	Purpose:	Polling CPUSS_SYSREQ register for completion of SROM call
*	Parameter:
*				IN timeOut	- timeout interval in ms
*	Return:
*				0 - empty,	1 - non-empty (at least one bit is HIGH)
*************************************************************************/
int PollSromStatus(long timeOut)
{
	int timeElapsed = 0x00;	
    unsigned int baseAddr = CPUSS_SYSREQ;
    unsigned int readData = 0x00;

	while(timeElapsed < timeOut)
	{
		ReadIO(baseAddr, &readData);	  
		readData &= (SROM_SYSREQ_BIT | SROM_PRIVILEGED_BIT);
		//readData = 0x00;
		if(readData == 0x00) return (SUCCESS); //success
		
		Delay(0x01); // wait for 1 ms, and increment current time
		timeElapsed++;
	}
    return (FAIL); //fail
}

/*************************************************************************
*	Purpose:	Puts row data into SRAM buffer
*	Parameter:
*				IN arrayID	- array number
*               IN data		- page data
*				IN dataSize - size of page data
*	Return:
*				0 - empty,	1 - non-empty (at least one bit is HIGH)
*************************************************************************/
int LoadLatchRow (unsigned char arrayID, unsigned char* data, int dataSize)
{
	int i = 0;
	long Params1, Params2;

	//Put command's parameters into SRAM buffer
	Params1 = (SROM_KEY1 << 0) +  										//KEY1
			  ((SROM_KEY2 + SROM_CMD_LOAD_LATCH) << 8) +	//KEY2
			   (0x00 << 16) +															//Byte number in latch from what to write
			   (arrayID << 24);	                          //Flash Macro ID (0 or 1)

	Params2 = ((dataSize - 1) << 0) +        					//Number of Bytes to load minus 1 (only 64 bytes per row for CY8C40xx)
			(0x00 << 8) +																	//Pad Byte to next 4b boundary    (so 1 byte is sufficient)
			(0x00 << 16) +																//Pad Byte to next 4b boundary
			(0x00 << 24);                                 //Pad Byte to next 4b boundary
	WriteIO(SRAM_PARAMS_BASE + 0x00, Params1);  			//Write params is SRAM
	WriteIO(SRAM_PARAMS_BASE + 0x04, Params2); 				//Write params is SRAM

	//Put row data into SRAM buffer
	for (i = 0; i < dataSize; i += 4)
	{
		Params1 = (data[i] << 0) + (data[i + 1] << 8) + (data[i + 2] << 16) + (data[i + 3] << 24);
		WriteIO(SRAM_PARAMS_BASE + 0x08 + i, Params1);//Write params is SRAM
	}

	//Call "Load Latch" SROM API
	WriteIO(CPUSS_SYSARG, SRAM_PARAMS_BASE); //Set location of parameters
	WriteIO(CPUSS_SYSREQ, SROM_SYSREQ_BIT | SROM_CMD_LOAD_LATCH);//Request SROM operation

	//Wait until SROM call is completed
	return PollSromStatus(SROM_SYSREQ_TIMEOUT);
}

/*************************************************************************
*	Purpose: Gets device Flash size depending on the Wounding register's value
*	Parameter:
*				OUT flashSize - Returns flash size
*	Return:
*				0 - empty,	1 - non-empty (at least one bit is HIGH)
*************************************************************************/
int GetFlashSize(unsigned int *flashSize)
{
  unsigned int value = 0;
  /*unsigned long wounding_table[8];
  
  wounding_table[0] = 0x004000; //16kB
  wounding_table[1] = 0x002000; //8kB
  wounding_table[2] = 0x001000; //4kB
  wounding_table[3] = 0x000800; //2kB
  wounding_table[4] = 0x000400; //1kB
  wounding_table[5] = 0x000200; //512B
  wounding_table[6] = 0x000100; //256B
  wounding_table[7] = 0x000080; //128B*/

  ReadIO(SFLASH_CPUSS_WOUNDING, &value);
  
  value = (value >> 20) & 0x07;
	
	*flashSize = 0x004000 >> value;
  
  /*if(value < 8)
  {
    *flashSize = wounding_table[value];
  }
  else
    *flashSize = 0x004000; //32kB*/
    
  return SUCCESS;
}

/*************************************************************************
*	Purpose: Initialize Flash for programming/erase operations
*	Parameter: NONE
*	Return:
*				0 - SUCCESS,	1 - FAIL
*************************************************************************/
int FMLT_ClkConfig()
{
	long Params;
	
	//Set IMO=48 MHz to enable program/erase operations for FLASH (Flash Lite IP)
	
	Params =(SROM_KEY1 << 0) +				    	    				//KEY1    	   
         ((SROM_KEY2+SROM_CMD_SET_IMO_48MHZ) << 8);  	//KEY2
	
	WriteIO(CPUSS_SYSARG, Params);
	WriteIO(CPUSS_SYSREQ, SROM_SYSREQ_BIT | SROM_CMD_SET_IMO_48MHZ);//Request SROM operation
	
	return PollSromStatus(SROM_SYSREQ_TIMEOUT);	
}

/*************************************************************************
*	Purpose: Initialize Flash Programming Functions
*	Parameter:
*				IN adr	- Device Base Address
*				IN clk	- Clock Frequency (Hz)
*				IN fnc	- Function Code (1 - Erase, 2 - Program, 3 - Verify)
*	Return:
*				0 - empty,	1 - non-empty (at least one bit is HIGH)
*************************************************************************/
int Init (unsigned long adr, unsigned long clk, unsigned long fnc) 
{
	int hr = SUCCESS;
	
	//Set IMO=48MHz before Erase/Program requests
	if ((fnc == 0x01) || (fnc == 0x02))
	{
			hr = FMLT_ClkConfig();
  }
	
	return hr;	
}

/*************************************************************************
*	Purpose: De-Initialize Flash Programming Functions
*	Parameter:
*				IN fnc	- Function Code (1 - Erase, 2 - Program, 3 - Verify)
*	Return:
*				0 - empty,	1 - non-empty (at least one bit is HIGH)
*************************************************************************/
int UnInit (unsigned long fnc) 
{
	return (0);                                  // Finished without Errors
}

/*************************************************************************
*	Purpose: Erase complete Flash Memory
*	Return:
*			0 - empty,	1 - non-empty (at least one bit is HIGH)
*************************************************************************/
int EraseChip (void) 
{
	return EraseSector(0x00); // Finished without Errors
}

/*************************************************************************
*	Purpose: Erase Sector in Flash Memory*    
*	Parameter:
*				IN adr	- Sector Address
*	Return:
*				0 - empty,	1 - non-empty (at least one bit is HIGH)
*************************************************************************/
int EraseSector (unsigned long adr) 
{	
	int i;
	int RowID, addr;
	unsigned char data[ROW_SIZE];
	unsigned int flashSize = 0;
	
	GetFlashSize(&flashSize); //Get Flash size using Wounding register

	//Create empty row
	for(i = 0; i < ROW_SIZE; i++)
	{
		data[i] = 0x00;
	}
	
	//Erase every row one-by-one
	//There is now way to call EraseAll SROM API from CM0 mode. 
	//It is only available from DAP interface. FW cannot erase itself completely (Flash and its Protection)
	//See "M0S8 SROM FW specification", 001-60700
	for(RowID = 0, addr = 0; RowID < ROWS_COUNT; RowID++, addr+=ROW_SIZE)
	{
		if(addr >= flashSize) return SUCCESS; //Do not erase rows if the incoming address is out of flash size
		if(FAIL == IsBlockEmpty(addr, ROW_SIZE)) //optimization, erase only non-0 rows
		{
			//if block not empty then program it with all zeros
			if(SUCCESS != ProgramPage(addr, ROW_SIZE, data)) return FAIL;
	 	}
  }

	return SUCCESS; // Finished without Errors
}

/*************************************************************************
*	Purpose: Program Page in Flash Memory
*	Parameter:
*				IN adr	- Page Start Address
*				IN	sz	- Page Size
*				IN buf	- Page Data
*	Return:
*				0 - empty,	1 - non-empty (at least one bit is HIGH)
*************************************************************************/
int ProgramPage (unsigned long adr, unsigned long sz, unsigned char *buf) 
{
	//Do not use "sz" here since usually it is not = ROW_SIZE for last row.
	//And thus last block can be programmed in other row.
	//The sz = ROW_SIZE for all rows besides last one. For last row it contains only actual number of used bytes there <= ROW_SIZE.

	int hr = 0x00;
	int FlashSize = 16 * 1024; //16 K
	int FlashArrays = 1;
	int FlashRowSize = ROW_SIZE; //64 for CY8C40xx silicon
	int FlashRowsPerArray = FlashSize / (FlashArrays * FlashRowSize);
	int rowID = adr/FlashRowSize;
	int arrayID = 0x00;
	long Param = 0x00;

	//Load Row to page latch buffer
	arrayID = (unsigned char)(rowID / FlashRowsPerArray);    
	hr = LoadLatchRow(arrayID, buf, FlashRowSize);
	if (hr == FAIL) { return (hr); }
	
	//Program Row - call SROM API
	Param =(SROM_KEY1 << 0) +				    	    				//KEY1    	   
				 ((SROM_KEY2+SROM_CMD_WRITE_ROW) << 8) +		//KEY2
         //((SROM_KEY2+SROM_CMD_PROGRAM_ROW) << 8) +		//KEY2
		   ((rowID & 0x00FF) <<  16) +									//ROW_ID_LOW[7:0]
		   ((rowID & 0xFF00) << 16);										//ROW_ID_HIGH[9:8]
	
	WriteIO(SRAM_PARAMS_BASE+0x00,Param);							//Write params is SRAM
	WriteIO(CPUSS_SYSARG, SRAM_PARAMS_BASE);                    //Set location of parameters
	WriteIO(CPUSS_SYSREQ, SROM_SYSREQ_BIT | SROM_CMD_WRITE_ROW);//Request SROM operation
	//WriteIO(CPUSS_SYSREQ, SROM_SYSREQ_BIT | SROM_CMD_PROGRAM_ROW);//Request SROM operation
  	
	//Wait until SROM call is completed
	return PollSromStatus(200);
}

/*************************************************************************
*	Purpose: Verify Page in Flash Memory
*	Parameter:
*				IN adr	- Page Start Address
*				IN sz	- Page Size
*               IN buf	- Page Data
*	Return:
*				the sum of (adr+sz) - on success. 
*				any other number - on failure, and represents the failing address. 
*************************************************************************/
unsigned long Verify (unsigned long adr, unsigned long sz, unsigned char *buf)
{	
	//Note, the "sz" for all rows = ROW_SIZE, but only for last row being verified , 
	//this value typically is < ROW_SIZE. 
	//So, it is necessary to use "sz" for every call, instead of hardcoded ROW_SIZE.
		
	unsigned int i, cpuss_sysreq;
	unsigned char* chipData = (unsigned char*)adr; //read directly from FLASH
	
	//When in CM0 mode, the first 8 bytes of Flash (SP and Reset vectors) during reading
	//are redirected to first 8 bytes of SROM to fetch SP and Reset vectors, unless
	//the CPUSS_SYSREQ.NO_RST_OVR bit is set. The default value upon reset is 0.
	//So, we set it to 1, for proper reading of these 8 bytes.
	//See spec 001-60700 Rev**, sec 7.2.11
	if (adr == 0)
	{
		ReadIO(CPUSS_SYSREQ, &cpuss_sysreq);
		cpuss_sysreq |= NO_RST_OVR; //set NO_RST_OVR bit - disable redirecting of Flash to SROM reads
		WriteIO(CPUSS_SYSREQ, cpuss_sysreq);
	}
  
	//Compare bytes in current row
	for (i=0 ; i < sz; i++) //always use "sz", not 128, since last block can be not shorter.
	{
		if (buf[i] != chipData[i]) break;
	}

	//Return NO_RST_OVR bit in original state
	if (adr == 0)
	{
		ReadIO(CPUSS_SYSREQ, &cpuss_sysreq);
		cpuss_sysreq &= ~NO_RST_OVR; //reset NO_RST_OVR bit - enable redirecting of Flash to SROM reads
		WriteIO(CPUSS_SYSREQ, cpuss_sysreq);
	}
	
	return (adr+i);// success = (adr+sz), otherwise = fail
}
