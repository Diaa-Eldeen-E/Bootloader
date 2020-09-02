/*
 * CLI.c
 *
 *  Created on: Sep 1, 2020
 *      Author: Diaa Eldeen
 */


#include "CLI.h"




#define to_upper(c)		(c >= 'a' && c <= 'z') ? c-'a'+'A' : c


//#define MAX_COMMAND_LEN             (10)
#define COMMAND_TABLE_SIZE          (5)



void command_help(char* bfr) {

	UART_put_strLine("List of commands:");

	UART_put_strLine("[Help] for commands list");
	UART_put_strLine("[LED_ON] for turning on the LED");
	UART_put_strLine("[LED_OFF] for turning off the LED");
	UART_put_strLine("[Erase] to erase a sector");
	UART_put_strLine("[Write] to write to a location in flash");

}

void command_LED_ON(char* bfr) {
	LED4_ON;
}

void command_LED_OFF(char* bfr) {
	LED4_OFF;
}


void command_FLASH_ERASE(char* bfr) {

	char addr[4];
	int8_t i,j;
	for(i=3, j=0; i>=0; --i, j++) {
		addr[j] = bfr[i];
	}

	// Assert valid address

	//Write address
	FLASH_FMA_R = *(uint32_t*) addr;

	//Write key to FMC and enable the erase bit
	FLASH_FMC_R = FLASH_FMC_WRKEY | FLASH_FMC_ERASE;

	// Poll for the erase bit
	while(FLASH_FMC_R & FLASH_FMC_ERASE);
}

void command_FLASH_WRITE(char* bfr){

	//convert from array to int

	char addr[4];
	char data[4];
	int8_t i,j;
	for(i=3, j=0; i>=0; --i, j++) {
		addr[j] = bfr[i];
		data[j] = bfr[i+5];
	}

	char* rem;
	// Assert valid address
	int32_t a = strtol(bfr, &rem, 16);
	int32_t d = strtol(rem, &rem, 16);
	// Write data
//	FLASH_FMD_R = *(uint32_t*) data;
//	FLASH_FMD_R = atol(&bfr[4]);
	FLASH_FMD_R = d;

	//Write address
//	FLASH_FMA_R = *(uint32_t*) addr;
//	FLASH_FMA_R = atol(bfr);
	FLASH_FMA_R = a;

	//Write key to FMC and enable the erase bit
	FLASH_FMC_R = FLASH_FMC_WRKEY | FLASH_FMC_WRITE;

	// Poll for the erase bit
	while(FLASH_FMC_R & FLASH_FMC_WRITE);
}


typedef struct
{
    char const    *name;
    void          (*function)(char*);
} command_t;

command_t const gCommandTable[COMMAND_TABLE_SIZE] =
{
    {"HELP",    command_help,},
    {"LED_ON",     command_LED_ON, },
    {"LED_OFF",  command_LED_OFF, },
	{"ERASE",	command_FLASH_ERASE},
	{"WRITE",	command_FLASH_WRITE}
//    {NULL,      NULL }
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

	for(i=0; i< COMMAND_TABLE_SIZE; ++i) {

		if(!strcmp(command, gCommandTable[i].name)) {	//compare it with defined commands

			newOp = 0;
			gCommandTable[i].function(&bfr[count+1]);		//Perform the corresponding function
			done = 1;
			UART_put_string(command);
			UART_put_strLine(" Process success");
		}

	}

	if(!done) {
		UART_put_strLine("Undefined command, [Help] to get some hints");
	}

	newOp =0;
	done =0;
}

