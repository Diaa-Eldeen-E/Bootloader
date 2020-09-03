/*
 * CLI.h
 *
 *  Created on: Sep 1, 2020
 *      Author: Diaa Eldeen
 */

#ifndef CLI_H_
#define CLI_H_



#include "string.h"
#include "stdlib.h"

#include "UART.h"



#define FLASH_BASE_	0x2000
#define FLASH_TOP_	0xFFFFC


void command_help(char* bfr);
void process_CLI_command();


#endif /* CLI_H_ */
