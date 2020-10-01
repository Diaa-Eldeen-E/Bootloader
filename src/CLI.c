/*
 * CLI.c
 *
 *  Created on: Sep 1, 2020
 *      Author: Diaa Eldeen
 */


#include "CLI.h"




#define to_upper(c)		(c >= 'a' && c <= 'z') ? c-'a'+'A' : c



void command_help(char* bfr) {

	UART_put_strLine("List of commands:");

	UART_put_strLine("[Help] for commands list");
	UART_put_strLine("[LED_ON] for turning on the LED");
	UART_put_strLine("[LED_OFF] for turning off the LED");
	UART_put_strLine("[Erase [address]] to erase a sector");
	UART_put_strLine("[Write [address] [data]] to write to a location in flash");
	UART_put_strLine("[Read [address]] to read to a location from flash");
	UART_put_strLine("[Write_Protection [address]] Sets a 2 KB block at this address to read only");
	UART_put_strLine("[PROG] to flash a hex file to the MCU");
	UART_put_strLine("[Jump] to run the loaded flash hex file");

}

void command_LED_ON(char* bfr) {
	LED4_ON;
}

void command_LED_OFF(char* bfr) {
	LED4_OFF;
}


void command_FLASH_ERASE(char* bfr) {

	// Convert the address from string to long int
	int32_t addr = strtol(bfr, &bfr, 16);

	if(FlashErase(addr))
	{
		UART_put_strLine("Sector Erasing error");
		FlashClearErrors();
	}
	else
	{
		// Success
	}
}


void command_FLASH_WRITE(char* bfr){

	// Extract the address and data in integer form
	int32_t addr = strtol(bfr, &bfr, 16);
	uint32_t data = strtol(bfr, &bfr, 16);

	if(FlashProgramBuffering(&data, addr, sizeof(uint32_t) * 1))
	{
		UART_put_strLine("Flash programming error");
		FlashClearErrors();
	}
	else
	{
		// Success
	}
}


void command_FLASH_READ(char* bfr) {

	// Extract the address in integer form
	int32_t addr = strtol(bfr, &bfr, 16);

	// Assert valid address
	if(addr >= FLASH_TOP_ || addr < FLASH_BASE_) {	// Flash rejoin
		UART_put_strLine("The entered flash address is out of bounds");
		return;
	}

	UART_send_uintL(* (uint32_t*) addr);
}


void command_FLASH_WriteProtection(char* bfr) {

	// Extract the address in integer form
	int32_t addr = strtol(bfr, &bfr, 16);

	if(FlashProtectSet(addr, FlashReadOnly))
	{
		UART_put_strLine("Write protection setting error");
		FlashClearErrors();
	}
	else
	{
		// Success
	}
}


uint8_t pui8RxBuffer[20000] __attribute((aligned(1024)));


extern uint32_t __app0rom_start;
extern uint32_t __app0rom_size;


//
void command_ReceiveBin(char* bfr)
{
	// Extract the address and size in integer form
	uint32_t ui32Addr = strtol(bfr, &bfr, 10);
	uint32_t ui32Size = strtol(bfr, &bfr, 10);


	// Check valid address and size
	if(ui32Addr != (uint32_t) &__app0rom_start)
	{
		UART_put_strLine("The image flashing address is not valid");
		UART_send_uintL(__app0rom_start);
		return;
	}

	if(ui32Size >= (uint32_t) &__app0rom_size)
	{
		UART_put_strLine("No enough space for the image");
		return;
	}


	// Receive data

	UART_put_strLine("Receiving the application image");

	char receivedChar =0;
	uint32_t i = 0;

	for(i=0; i<ui32Size; ++i)
	{
		receivedChar = UARTCharGet(UART0_BASE);
		pui8RxBuffer[i] = receivedChar;
	}




	// Erase flash

	// Write to flash
//	FlashProgramBuffering((uint32_t*) pui8RxBuffer, APP_ADDRESS, size/4);


}





#define __set_MSP(topOfMainStack)	\
{	\
	__asm("MSR msp, %0" : : "r" (topOfMainStack) : );	\
}

void command_Jump(char* bfr)
{
	// Take addr where to jump

	uint32_t addr = (uint32_t) __app0rom_start;
	uint32_t ui32MSP = * (uint32_t*) addr;

	// Check valid stack pointer

	// uninitialization
	// Disable interrupts
	// Reset GPIO and UART
	// Reset RCC
	// Reset config register
	// Disable PLL

	// Reallocate vector table if the addr is not 0
	if(addr % 1024 != 0)
		while(1);

	NVIC_VTABLE_R = addr;



	void (*jump_address)(void) = (void*)(*((uint32_t*)(addr+4)));
	// copy addr to MSP
	__set_MSP(ui32MSP);

	// Jump to addr + 4
	jump_address();


	while(1);
}



typedef struct
{
    char const    *name;
    void          (*function)(char*);
} command_t;

command_t const gCommandTable[] =
{
    {"HELP",    command_help,},
    {"LED_ON",     command_LED_ON, },
    {"LED_OFF",  command_LED_OFF, },
	{"ERASE",	command_FLASH_ERASE},
	{"WRITE",	command_FLASH_WRITE},
	{"READ",	command_FLASH_READ},
	{"WRITE_PROTECTION",	command_FLASH_WriteProtection},
	{"PROG",	command_ReceiveBin},
	{"JUMP",	command_Jump},
    {NULL,      NULL }
};





char bfr[40];
_Bool newOp =0;
_Bool done =0;




void process_CLI_command() {

	// Disable receiving commands until the current command is processed
	UARTIntDisable(UART0_BASE, UART_INT_RX);

	char command[40];
	uint32_t i =0, count;

	for(i=0; bfr[i] != '\0' && bfr[i] != ' ' ; ++i)
		command[i] = to_upper(bfr[i]);	// Convert all to upper case

	command[i] = '\0';
	count = i;

	// Search the command in the lookup commands table
	for(i=0; gCommandTable[i].name != NULL; ++i)
	{
		if(!strcmp(command, gCommandTable[i].name))
		{
			//Perform the corresponding function
			gCommandTable[i].function(&bfr[count+1]);

			newOp = 0;
			done = 1;

			// Command success message
			UART_put_string(command);
			UART_put_strLine(" command done");
		}

	}

	if(!done) {
		UART_put_strLine("Undefined command, [Help] to get some hints");
	}

	newOp =0;
	done =0;

	// Enable receiving commands
	UARTIntEnable(UART0_BASE, UART_INT_RX);
}

