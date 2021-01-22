/*
 * main.c
 */



#include "handy.h"
#include "UART.h"

uint32_t F_SysClk = 80000000;

uint32_t my_num0 = 5;

uint32_t my_num1 = 6;

uint32_t my_num2;


int main(void) {

	clock_setup_PIOSC(F_SysClk/1000000UL);


	UART_initialize_polling();
	UART_send_stringL("working");

	LEDs_EK_setup();
	buttons_EK_setup();
	__enable_irq();



	while(1)
	{

		UART_send_stringL("OK");
		LED1_ON
		delay_ms(500);
		LED1_OFF
		delay_ms(500);
		//blink_EK_LED();


	}
}

void GPIOJ_Handler()
{
	/*char interruptStatus = GPIO_button->RIS;
	if (interruptStatus == P0)
	{
		GPIO_button->ICR |= P0;
		//maybe you should read ICR to force it to clear the flag
		LED1_ON;
		delay_ms(1000);
		LED1_OFF;

	}
	else if (interruptStatus == P1)
	{
		GPIO_button->ICR |= P1;
		LED4_ON;
		delay_ms(1000);
		LED4_OFF;
	}
	else if (interruptStatus == (P0 | P1) )
		GPIO_button->ICR |= P0 | P1;

	*/
	GPIO_button->ICR |= P0 | P1;
}

