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
    {NULL,      NULL }
};





char bfr[40];
_Bool newOp =0;
_Bool done =0;




void process_CLI_command() {

	char command[40];
	uint32_t i =0, count;
	for(i=0; bfr[i] != '\0' && bfr[i] != ' ' ; ++i)
		command[i] = to_upper(bfr[i]);	//convert all to upper case

	command[i] = '\0';
	count = i;

	for(i=0; gCommandTable[i].name != NULL; ++i) {

		if(!strcmp(command, gCommandTable[i].name)) {	//compare it with defined commands

			newOp = 0;
			gCommandTable[i].function(&bfr[count+1]);		//Perform the corresponding function
			done = 1;
			UART_put_string(command);
			UART_put_strLine(" command done");
		}

	}

	if(!done) {
		UART_put_strLine("Undefined command, [Help] to get some hints");
	}

	newOp =0;
	done =0;
}

