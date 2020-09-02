/*
 * UART.c
 *
 *  Created on: Dec 31, 2019
 *      Author: DiaaEldeen
 */


#include "UART.h"


extern uint32_t F_SysClk;
extern void UART0_Handler();

//returns 0 if no error
uint8_t UART_receive_line(char* bfr, uint8_t bfrSize){

	char receivedChar =0;

	int8_t i =0;
	for (i =0; i<bfrSize+2; i++) {

		receivedChar = UARTCharGet(UART0_BASE);
		ASSERT(UARTRxErrorGet(UART0_BASE) == 0);	//error checking

		if (receivedChar =='\r' || receivedChar == '\n'){
			bfr[i] = '\0';
			return 0;
		}
		else if(i == bfrSize){
			bfr[i] = '\0';
			while(receivedChar !='\r' && receivedChar != '\n'){
				receivedChar = UARTCharGet(UART0_BASE);
			}
			return 1;
		}

		bfr[i] = receivedChar;
	}

	return 0;
}


void UART_put_strLine(char* str){
	while(*str)	UARTCharPut(UART0_BASE, *str++);
	UARTCharPut(UART0_BASE, '\n');
}


void UART_put_string(char* str){
	while(*str)	UARTCharPut(UART0_BASE, *str++);
}


void UART_put_PackedBCD(unsigned char DATA){
	UARTCharPut(UART0_BASE, '0'+(DATA>>4) ) ;
	UARTCharPut(UART0_BASE, '0'+(DATA & 0x0f) )  ;
}




void UART0_init_interrupts(){
	//GPIO Config for UART0
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOA));

	GPIOPinConfigure(GPIO_PA0_U0RX);
	GPIOPinConfigure(GPIO_PA1_U0TX);
	GPIOPinTypeUART(GPIO_PORTA_AHB_BASE, 0x3);

	//UART0 Config
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_UART0));

	UARTClockSourceSet(UART0_BASE, UART_CLOCK_SYSTEM);
	UARTConfigSetExpClk(UART0_BASE, F_SysClk, 9600, UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE );

	//UART0 interrupt config
	UARTFIFOEnable(UART0_BASE);
	UARTFIFOLevelSet(UART0_BASE, UART_FIFO_TX1_8, UART_FIFO_RX1_8);

	IntPendClear(INT_UART0);
	UARTIntEnable(UART0_BASE, UART_INT_RX);	//receive flag interrupt
	UARTIntRegister(UART0_BASE, UART0_Handler);
//	UARTFIFODisable(UART0_BASE);


	IntEnable(INT_UART0);	//enable UART0 interrupts
	IntMasterEnable();	//enable global interrupts
}
