/*
 * BL.c
 *
 *  Created on: Oct 29, 2020
 *      Author: Diaa Eldeen
 */


#include "BL.h"


extern uint32_t __blrom_start;
extern uint32_t __blrom_size;
extern uint32_t __app0rom_start;
extern uint32_t __app0rom_size;
extern uint32_t __app1rom_start;
extern uint32_t __app1rom_size;
extern uint32_t SRAM_END;
extern uint32_t SRAM_START;


#define __set_MSP(topOfMainStack)	\
{	\
	__asm("MSR msp, %0" : : "r" (topOfMainStack) : );	\
}



int32_t BLDownloadImage(uint32_t pui32Buffer[], uint32_t ui32Addr, uint32_t ui32Size)
{
	uint32_t ui32RejoinAddr, ui32RejoinSize;

	if(ui32Addr == (uint32_t) & __blrom_start)
	{
		ui32RejoinAddr = (uint32_t) & __blrom_start;
		ui32RejoinSize = (uint32_t) & __blrom_size;
	}
	else if(ui32Addr == (uint32_t) & __app0rom_start)
	{
		ui32RejoinAddr = (uint32_t) & __app0rom_start;
		ui32RejoinSize = (uint32_t) & __app0rom_size;
	}
	else if(ui32Addr == (uint32_t) & __app1rom_start)
	{
		ui32RejoinAddr = (uint32_t) & __app1rom_start;
		ui32RejoinSize = (uint32_t) & __app1rom_size;
	}
	else
		return Error;



	// Erase flash
	uint32_t ui32NoOfSectors = ui32RejoinSize / 16384; // Divide by 16K
	uint32_t i = 0;
	for(i=0; i<ui32NoOfSectors; ++i)
	{
		if(FlashErase(ui32RejoinAddr + 16384 * i))
		{
			FlashClearErrors();
			return SectorEraseError;
		}
		else
		{
			// Success
		}
	}

	// Write to flash
	if( FlashProgramBuffering(pui32Buffer, ui32RejoinAddr, ui32Size) )
	{
		FlashClearErrors();
		return FlashProgError;
	}
	else
	{
		// Success
	}

	return Success;
}


int32_t BLJump(uint32_t ui32Addr)
{
	// Check the address
	uint32_t ui32RejoinAddr;
	if(ui32Addr == (uint32_t) & __blrom_start)
		ui32RejoinAddr = (uint32_t) & __blrom_start;

	else if(ui32Addr == (uint32_t) & __app0rom_start)
		ui32RejoinAddr = (uint32_t) & __app0rom_start;

	else if(ui32Addr == (uint32_t) & __app1rom_start)
		ui32RejoinAddr = (uint32_t) & __app1rom_start;

	else
		return Error;


	uint32_t ui32MSP = * (uint32_t*) ui32RejoinAddr;

	// Check valid stack pointer
	ASSERT_TRUE(ui32MSP > (uint32_t) &SRAM_START && ui32MSP <= (uint32_t) &SRAM_END);

	// Reallocate the vector table the application location
	ASSERT_TRUE(ui32RejoinAddr % 1024 == 0);
	NVIC_VTABLE_R = ui32RejoinAddr;

	void (*jump_address)(void) = (void*)(*((uint32_t*)(ui32RejoinAddr+4)));

	// Uninitialization

	// Disable interrupts
	IntMasterDisable();

	// Reset GPIO
	SYSCTL_SRGPIO_R = 0xffff;
	SYSCTL_SRGPIO_R = 0;

	// Reset UART
	SYSCTL_SRUART_R = 0xffff;
	SYSCTL_SRUART_R = 0;

	// Reset Timer
	SYSCTL_SRTIMER_R = 0xffff;
	SYSCTL_SRTIMER_R = 0;

	// Reset system control and clock settings
	SYSCTL_MISC_R = 0xffff;	// Clear system control interrupt flags
	SYSCTL_MOSCCTL_R = 0x000C;	// Reset value
	SYSCTL_MEMTIM0_R = 0x00300030;	// Reset value
	SYSCTL_PLLFREQ0_R = 0;	// Reset value
	SYSCTL_PLLFREQ1_R = 0;	// Reset value
	SYSCTL_RSCLKCFG_R = 0;	// Reset value
	SYSCTL_DIVSCLK_R = 0;	// Reset value

	// Jump
	__set_MSP(ui32MSP);
	jump_address();	// Jump to addr + 4


	// Should never reach here
	while(1);

}
