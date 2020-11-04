/*
 * CLI_Engine.h
 *
 *  Created on: Oct 29, 2020
 *      Author: Diaa Eldeen
 */

#ifndef INCLUDE_CLI_ENGINE_H_
#define INCLUDE_CLI_ENGINE_H_


#include "string.h"

#include "handy.h"
#include "UART.h"


// The size of the buffer in which the commands get received
#define CLI_BUFFER_SIZE	40


typedef struct
{
    char const    *pcName;
    char const    *pcHelp;
    void          (*pFunction)(char*);

}command_t;





void process_CLI_command();


#endif /* INCLUDE_CLI_ENGINE_H_ */
