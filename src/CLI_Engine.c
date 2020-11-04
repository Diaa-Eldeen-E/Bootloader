/*
 * CLI_Engine.c
 *
 *  Created on: Oct 29, 2020
 *      Author: Diaa Eldeen
 */


#include "CLI_Engine.h"



#define to_upper(c)		(c >= 'a' && c <= 'z') ? c-'a'+'A' : c


extern command_t const gCommandTable[];
char bfr[CLI_BUFFER_SIZE];
uint8_t newOp =0;
uint8_t done =0;


void process_CLI_command() {

	// Disable receiving commands until the current command is processed
	UARTIntDisable(UART0_BASE, UART_INT_RX);

	char pcFirstArg[CLI_BUFFER_SIZE];
	uint32_t i =0, ui32Idx;

	for(i=0; bfr[i] != '\0' && bfr[i] != ' ' ; ++i)
		pcFirstArg[i] = to_upper(bfr[i]);	// Convert all to upper case

	pcFirstArg[i] = '\0';
	ui32Idx = i;

	// Search the command argument in the commands lookup table
	for(i=0; gCommandTable[i].pcName != NULL; ++i)
	{
		if(!strcmp(pcFirstArg, gCommandTable[i].pcName))
		{
			//Perform the corresponding function
			gCommandTable[i].pFunction(&bfr[ui32Idx+1]);

			newOp = 0;
			done = 1;

			// Command success message
			UART_put_string(pcFirstArg);
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






void UART0_Handler()
{

	if(UART_receive_line(bfr, CLI_BUFFER_SIZE))
		UART_put_strLine("Error, too long string, try again");	//buffer overflow
	else
		newOp =1;

	//UARTCharGet automatically clears the interrupt
}
