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

#include "CLI_Engine.h"
#include "UART.h"
#include "flash.h"
#include "BL.h"





void command_help(char* bfr);
void process_CLI_command();



void command_LED_ON(char* bfr);
void command_LED_OFF(char* bfr);
void command_FLASH_ERASE(char* bfr);
void command_FLASH_WRITE(char* bfr);
void command_FLASH_READ(char* bfr);
void command_FLASH_WriteProtection(char* bfr);
void command_ReceiveBin(char* bfr);
void command_Jump(char* bfr);

#endif /* CLI_H_ */
