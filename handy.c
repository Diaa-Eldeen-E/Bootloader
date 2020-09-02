/*
 * handy.c
 *
 *  Created on: Dec 31, 2019
 *      Author: DiaaEldeen
 */


#include "handy.h"


extern void GPIOJ_Handler();


void __error__(char *pcFilename, uint32_t ui32Line)
{
    while(1);
}


void LED_EK_blink(uint32_t ms){

	LED1_ON
	delay_ms(ms);
	LED1_OFF
	delay_ms(ms);
}


void LED_EK_blink_forever(uint32_t ms){

	LEDs_EK_setup();

	while(1){
		LED1_ON
		delay_ms(ms);
		LED1_OFF
		delay_ms(ms);
	}
}

void LEDs_EK_setup(){
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPION));
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF));

	GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_0 | GPIO_PIN_1);
	GPIOPinTypeGPIOOutput(GPIO_PORTF_AHB_BASE, GPIO_PIN_0 | GPIO_PIN_4);
}


void buttons_EK_setup(){
	SysCtlPeripheralEnable(SYSCTL_PERIPH_BUTTON);
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_BUTTON));

	GPIODirModeSet(GPIO_BUTTON, BUTTON_PINS, GPIO_DIR_MODE_IN);
	GPIOPadConfigSet(GPIO_BUTTON, BUTTON_PINS, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

	GPIOIntTypeSet(GPIO_BUTTON, BUTTON_PINS, GPIO_FALLING_EDGE);
	GPIOIntClear(GPIO_BUTTON, BUTTON_PINS);
	GPIOIntEnable(GPIO_BUTTON, BUTTON_PINS);
	GPIOIntRegister(GPIO_BUTTON, GPIOJ_Handler);
}

// this function reverses the order of a number of bytes (length) starting from the address (src)
char* reverse(char* src,uint8_t length){
	char temp=0,i;
	for (i=0;i<length/2;i++)
		{
			temp = *(src+i);
			*(src+i) = *(src+length-i-1);
			*(src+length-i-1) = temp;
		}
	return src;
}



// this function converts a 4-byte signed integer (num) to
//its ASCI char array (str) in a numeric system (base)
void itoa(int32_t num, char* str, uint32_t base)
{
	uint8_t counter= 0;    //this counts the number of digits
    uint8_t sign =0;

    if (num<0 && base==10) { sign =1; num*=-1; }

    do
    {
    	if (num%base > 9) //for base 16
    		*(str+counter++) = (num%base) - 10 + 'A';  //transform the numbers (10,11,12,13,14,15) to (A,B,C,D,E,F)

    	else
    		*(str+counter++) = num % base + '0' ;   //add the '0' to convert the digit into ASCI
    	num /=base;
    }
    while(num>0);

    if (sign==1) *(str+counter++)= '-';
    reverse(str,counter);
    *(str+counter)= 0;   //null character
}



// this function converts a 4-byte unsigned integer (num) to
//its ASCI char array (str) in a numeric system (base)
void utoa(uint32_t num, char* str, uint32_t base)
{
	uint8_t counter= 0;    //this counts the number of digits

    do
    {
		if (num%base > 9) //for base 16
    		*(str+counter) = (num%base) - 10 + 'A';  //transform the numbers (10,11,12,13,14,15) to (A,B,C,D,E,F)

        else
			*(str+counter) = num % base + '0' ;   //add the '0' to convert the digit into ASCI
		num /=base;
        counter++;
    }
    while(num>0);

    reverse(str,counter);
    *(str+counter)= 0;   //null character
}


//this function converts float number to an to its ASCI char array, the float can hold nearly 8 digits at max
//more digits will cause lack of precision, so the string passed size typically should be 10
void ftoa (float num,uint8_t precision,char *str)  //requires 2048 stack size
{
	switch (precision){
	case 1:
		sprintf(str, "%.1f", num);
		break;
	case 2:
		sprintf(str, "%.2f", num);
		break;
	case 3:
		sprintf(str, "%.3f", num);
		break;
	case 4:
		sprintf(str, "%.4f", num);
		break;
	case 5:
		sprintf(str, "%.5f", num);
		break;
	case 6:
		sprintf(str, "%.6f", num);
		break;
	default:
		str[0]='0'; str[1]= '\0';
		break;
	}
}




void delay_ms(uint_fast32_t ms){
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER);
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_TIMER));

	TimerConfigure(TIMER_delay, TIMER_CFG_A_PERIODIC);
	TimerControlStall(TIMER_delay, TIMER_A, 1);
	TimerClockSourceSet(TIMER_delay, TIMER_CLOCK_PIOSC);
	TimerLoadSet(TIMER_delay, TIMER_A,  16000);
	TimerIntClear(TIMER_delay, TIMER_TIMA_TIMEOUT);
	TimerEnable(TIMER_delay, TIMER_A);

	while(ms--){
		while ( ! ( TimerIntStatus(TIMER_delay, 0) & 1)  );
		TimerIntClear(TIMER_delay, TIMER_TIMA_TIMEOUT);
	}
	TimerDisable(TIMER_delay, TIMER_A);
}





void delay_us(uint_fast32_t us){
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER);
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_TIMER));

	TimerConfigure(TIMER_delay, TIMER_CFG_A_PERIODIC);
	TimerControlStall(TIMER_delay, TIMER_A, 1);
	TimerClockSourceSet(TIMER_delay, TIMER_CLOCK_PIOSC);
	TimerLoadSet(TIMER_delay, TIMER_A, 16);
	TimerIntClear(TIMER_delay, TIMER_TIMA_TIMEOUT);
	TimerEnable(TIMER_delay, TIMER_A);

	while(us--){
		while ( ! ( TimerIntStatus(TIMER_delay, 0) & 1)  );
		TimerIntClear(TIMER_delay, TIMER_TIMA_TIMEOUT);
	}
	TimerDisable(TIMER_delay, TIMER_A);
}
