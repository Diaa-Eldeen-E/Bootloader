/*
 * main.c
 *
 *  Created on: Dec 30, 2019
 *      Author: DiaaEldeen
 */

#ifndef DEBUG
#define DEBUG
#endif

#include "UART.h"



#include "CLI.h"


uint32_t F_SysClk = 82000000;



extern char bfr[];
extern _Bool newOp;



int main(){
	uint32_t i = 1;


	F_SysClk = SysCtlClockFreqSet(SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480
									| SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN, F_SysClk);
		delay_us(1);

	LEDs_EK_setup();
	UART0_init_interrupts();

	UART_put_strLine("Hello");

	command_help("");



	while(1){

		if(newOp){
			process_CLI_command();
		}
		else{
			LED_EK_blink(200);
		}



	}
}



void GPIOJ_Handler(){
//	if (interruptStatus == P0)
//	{
//		GPIO_button->ICR |= P0;
//		//maybe you should read ICR to force it to clear the flag
//		LED1_ON;
//		delay_ms(1000);
//		LED1_OFF;
//
//	}
//	else if (interruptStatus == P1)
//	{
//		GPIO_button->ICR |= P1;
//		LED4_ON;
//		delay_ms(1000);
//		LED4_OFF;
//	}
//	else if (interruptStatus == (P0 | P1) )
//		GPIO_button->ICR |= P0 | P1;
//
//
//	GPIO_button->ICR |= P0 | P1;
	while(1){

	}
}

void UART0_Handler(){

	if(UART_receive_line(bfr, 40))	//UARTCharGet automatically clears the interrupt
		UART_put_strLine("Error, too long string, try again");	//buffer overflow
	else{
//		UART_put_strLine(bfr);
		newOp =1;
	}

//	IntPendClear(INT_UART0);
}

