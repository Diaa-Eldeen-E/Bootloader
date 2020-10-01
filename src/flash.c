/*
 * flash.c
 *
 *  Created on: Sep 3, 2020
 *      Author: Diaa Eldeen
 */



#include "flash.h"


void FlashClearErrors()
{
	FLASH_FCMISC_R |= FLASH_FCMISC_PROGMISC | FLASH_FCMISC_VOLTMISC
			| FLASH_FCMISC_PMISC | FLASH_FCMISC_INVDMISC | FLASH_FCMISC_ERMISC
			| FLASH_FCMISC_AMISC;
}


int32_t FlashErase(uint32_t ui32Address)
{
	// Assert valid address
	ASSERT_TRUE(ui32Address < FLASH_TOP_ && ui32Address >= FLASH_BASE_);	// Flash rejoin
	ASSERT_TRUE(ui32Address % 16384 == 0);	// 16 KB alignment


	// Write address
	FLASH_FMA_R = ui32Address;

	// Write key to FMC and enable the erase bit
	FLASH_FMC_R = FLASH_FMC_WRKEY | FLASH_FMC_ERASE;

	// Poll for the erase bit
	while(FLASH_FMC_R & FLASH_FMC_ERASE);


	// Check errors
	if((FLASH_FCRIS_R & (FLASH_FCRIS_ARIS | FLASH_FCRIS_VOLTRIS |
			FLASH_FCRIS_INVDRIS | FLASH_FCRIS_ERRIS)))
	{
		return(-1);
	}

	return 0;
}



int32_t FlashProgram(uint32_t *pui32Data, uint32_t ui32Address, uint32_t ui32Count)
{

	// Assert valid address
	ASSERT_TRUE(ui32Address < FLASH_TOP_ && ui32Address >= FLASH_BASE_);	// Flash rejoin
	ASSERT_TRUE(!(ui32Address & 0x3));	// 4 Bytes alignment
	ASSERT_TRUE(!(ui32Count & 0x3));	// 4 Bytes alignment

	uint32_t i = 0;

	while(ui32Count)
	{
		// Write data
		FLASH_FMD_R = pui32Data[i];

		//Write address
		FLASH_FMA_R = ui32Address;

		//Write key to FMC and enable the erase bit
		FLASH_FMC_R = FLASH_FMC_WRKEY | FLASH_FMC_WRITE;

		// Poll for the writing bit
		while(FLASH_FMC_R & FLASH_FMC_WRITE);

		ui32Count -=4;
		ui32Address += 4;
		i++;
	}


	// Check errors
	if((FLASH_FCRIS_R & (FLASH_FCRIS_ARIS | FLASH_FCRIS_VOLTRIS |
			FLASH_FCRIS_INVDRIS | FLASH_FCRIS_PROGRIS)))
	{
		return(-1);
	}

	return 0;

}


//((ui32Address & 0x7c) || FLASH_FWBVAL_R == 0)
int32_t FlashProgramBuffering(uint32_t *pui32Data, uint32_t ui32Address, uint32_t ui32Count)
{

	// Assert valid address
	ASSERT_TRUE(ui32Address < FLASH_TOP_ && ui32Address >= FLASH_BASE_);	// Flash rejoin
	ASSERT_TRUE(!(ui32Address & 0x3));	// 4 Bytes alignment
	ASSERT_TRUE(!(ui32Count & 0x3));	// 4 Bytes alignment

	uint32_t i = 0;

	while(ui32Count)
	{
		//Write address
		uint32_t ui32PageAddress = ui32Address & (~0x7f);
		uint32_t ui32RegOffSet = ui32Address & 0x7c;

		FLASH_FMA_R = ui32PageAddress;	// Must be page (128 byte) aligned

		// Write a 32-word page to the flash memory buffer registers
		while( (ui32RegOffSet < 128) && (ui32Count) )
		{
			HWREG(FLASH_FWBN + ui32RegOffSet) = pui32Data[i];

			ui32Count -=4;
			ui32RegOffSet += 4;
			i++;
		}

		// Start the buffered flash write
		FLASH_FMC2_R = FLASH_FMC_WRKEY | FLASH_FMC2_WRBUF;

		// Poll for the buffering write bit
		while(FLASH_FMC2_R & FLASH_FMC2_WRBUF);

		ui32Address += ui32RegOffSet;
	}



	// Check errors
	if((FLASH_FCRIS_R & (FLASH_FCRIS_ARIS | FLASH_FCRIS_VOLTRIS |
			FLASH_FCRIS_INVDRIS | FLASH_FCRIS_PROGRIS)))
	{
		return(-1);
	}

	return 0;

}



int32_t FlashProtectSet(uint32_t ui32Address, tFlashProtection eProtect)
{
	ASSERT_TRUE(ui32Address % 2048 == 0);	// 2 KB alignment

	ui32Address /= 2048;	// Convert the address into a block number.
	uint32_t ui32RegOffSet = (ui32Address / 32) * 4;
	uint32_t ui32RegBit = (1 << (ui32Address % 32));

	ASSERT_TRUE(ui32Address < NO_OF_BLOCKS);

	// R\W	-> both 1
	if(eProtect == FlashReadWrite)
	{
		// Cannot be Read/Write if is not already Read/Write
		if( !(HWREG(FLASH_FMPRE0 + ui32RegOffSet) & ui32RegBit)
				&& !(HWREG(FLASH_FMPRE0 + ui32RegOffSet) & ui32RegBit) )
		{
			return -1;
		}

		return 0;
	}

	// Read only	-> Read 1 and Program 0
	else if(eProtect == FlashReadOnly)
	{
		// Cannot be read only if it is already execute only
		if( !(HWREG(FLASH_FMPRE0 + ui32RegOffSet) &  ui32RegBit) )
		{
			return -1;
		}

		HWREG(FLASH_FMPPE0 + ui32RegOffSet) &= ~ui32RegBit;
	}

	// Execute only	-> Read 0 Program 0
	else if(eProtect == FlashExecuteOnly)
	{
		HWREG(FLASH_FMPRE0 + ui32RegOffSet) &= ~ui32RegBit;
		HWREG(FLASH_FMPPE0 + ui32RegOffSet) &= ~ui32RegBit;
	}

	else
		ASSERT_TRUE(0);


	return 0;
}


tFlashProtection FlashProtectGet(uint32_t ui32Address)
{
	ui32Address /= 2048;	// Convert the address into a block number.
	uint32_t ui32RegOffSet = (ui32Address / 32) * 4;
	uint32_t ui32RegBit = (1 << (ui32Address % 32));

	if( (HWREG(FLASH_FMPRE0 + ui32RegOffSet) & ui32RegBit)
					&& (HWREG(FLASH_FMPRE0 + ui32RegOffSet) & ui32RegBit) )
	{
		return FlashReadWrite;
	}

	else if( !(HWREG(FLASH_FMPRE0 + ui32RegOffSet) &  ui32RegBit) )
	{
		return FlashReadOnly;
	}

	else
		return FlashExecuteOnly;

}
