/*
 * UART.h
 *
 *  Created on: Dec 31, 2019
 *      Author: DiaaEldeen
 */

#ifndef UART_H_
#define UART_H_

#ifndef DEBUG
#define DEBUG
#endif

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/rom.h"

#include "driverlib/interrupt.h"
//#include "inc/hw_ints.h"
#include "driverlib/pin_map.h"
#include "inc/tm4c1294ncpdt.h"

#include "driverlib/uart.h"
#include "inc/hw_uart.h"

#include "handy.h"


uint8_t UART_receive_line(char* bfr, uint8_t bfrSize);
void UART_put_strLine(char* str);
void UART_put_string(char* str);
void UART0_init_interrupts();

void UART_put_PackedBCD(unsigned char DATA);

#endif /* UART_H_ */
