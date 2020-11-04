/*
 * CLI.c
 *
 *  Created on: Sep 1, 2020
 *      Author: Diaa Eldeen
 */


#include "CLI.h"


// Register all the commands here
// Function name (must be upper-case), Help message and Function pointer
command_t const gCommandTable[] =
{
    {"HELP", "[Help] List the available commands", command_help,},
    {"LED_ON", "[LED_ON] Turn on the LED", command_LED_ON, },
    {"LED_OFF", "[LED_OFF] Turn off the LED", command_LED_OFF, },
	{"ERASE", "[Erase [address]] Erase the sector at [address]", command_FLASH_ERASE},
	{"WRITE", "[Write [address] [data]] Write a word in flash",	command_FLASH_WRITE},
	{"READ", "[Read [address]] Read a word from flash", command_FLASH_READ},
	{"WRITE_PROTECTION", "[Write_Protection [address]] Set a 2 KB block at \
			this address to read only",	command_FLASH_WriteProtection},
	{"PROG", "[PROG [address] [size]] Flash a hex file to the MCU",	command_ReceiveBin},
	{"JUMP", "[Jump [address]] Jump to the specified address", command_Jump},
    {NULL,      NULL }
};


void command_help(char* bfr)
{
	UART_put_strLine("Available commands:");

	uint32_t i;
	for(i=0; gCommandTable[i].pcName != NULL; ++i)
	{
		UART_put_strLine(gCommandTable[i].pcHelp);
	}

}


void command_LED_ON(char* bfr)
{
	LED4_ON;
}


void command_LED_OFF(char* bfr)
{
	LED4_OFF;
}


void command_FLASH_ERASE(char* bfr)
{

	// Extract the address in integer form
	uint32_t ui32Addr = strtol(bfr, &bfr, 16);

	if(FlashErase(ui32Addr))
	{
		UART_put_strLine("Sector Erasing error");
		FlashClearErrors();
	}
	else
	{
		// Success
	}
}


void command_FLASH_WRITE(char* bfr)
{

	// Extract the address and data in integer form
	uint32_t ui32Addr = strtol(bfr, &bfr, 16);
	uint32_t ui32Data = strtol(bfr, &bfr, 16);

	if(FlashProgramBuffering(&ui32Data, ui32Addr, sizeof(uint32_t) * 1))
	{
		UART_put_strLine("Flash programming error");
		FlashClearErrors();
	}
	else
	{
		// Success
	}
}


void command_FLASH_READ(char* bfr)
{

	// Extract the address in integer form
	uint32_t ui32Addr = strtol(bfr, &bfr, 16);

	// Assert valid address
	if(ui32Addr >= FLASH_TOP_ || ui32Addr < FLASH_BASE_) {	// Flash rejoin
		UART_put_strLine("The entered flash address is out of bounds");
		return;
	}

	UART_send_uintL(* (uint32_t*) ui32Addr);
}


void command_FLASH_WriteProtection(char* bfr)
{

	// Extract the address in integer form
	uint32_t ui32Addr = strtol(bfr, &bfr, 16);

	if(FlashProtectSet(ui32Addr, FlashReadOnly))
	{
		UART_put_strLine("Write protection setting error");
		FlashClearErrors();
	}
	else
	{
		// Success
	}
}


uint8_t pui8RxBuffer[65536];	// 64 KB


void command_ReceiveBin(char* bfr)
{
	// Extract the address and size in integer form
	uint32_t ui32Addr = strtol(bfr, &bfr, 10);
	uint32_t ui32Size = strtol(bfr, &bfr, 10);

	// Check enough memory for the image size
	if(ui32Size >= sizeof(pui8RxBuffer)) //(uint32_t) &__app0rom_size)
	{
		UART_put_strLine("No enough space for the image");
		return;
	}

	// Receive the image
	UART_put_strLine("Receiving the application image");
	char cReceivedChar =0;
	uint32_t i = 0;
	for(i=0; i<ui32Size; ++i)
	{
		cReceivedChar = UARTCharGet(UART0_BASE);
		pui8RxBuffer[i] = cReceivedChar;
	}

	// Flash the image
	uint32_t ui32RetCode;
	ui32RetCode = BLDownloadImage((uint32_t*) pui8RxBuffer, ui32Addr, ui32Size);

	switch (ui32RetCode)
	{
	case Success:
		UART_put_strLine("Download complete");
		break;

	case SectorEraseError:
		UART_put_strLine("Sector erasing error");
		break;

	case FlashProgError:
		UART_put_strLine("Flash programming error");
		break;

	case Error:
	default:
		UART_put_strLine("Error downloading the image");
		break;
	}

}



void command_Jump(char* bfr)
{
	// Extract the address in integer form
	uint32_t ui32Addr = strtol(bfr, &bfr, 10);

	uint32_t ui32RetCode;
	ui32RetCode = BLJump(ui32Addr);

	switch (ui32RetCode)
	{
	case Error:
	default:
		UART_put_strLine("Jump failed");
		break;
	}
}


