/*
 * handy.h
 *
 *  Created on: Dec 31, 2019
 *      Author: DiaaEldeen
 */

#ifndef HANDY_H_
#define HANDY_H_


#ifndef DEBUG
#define DEBUG
#endif

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "inc/hw_memmap.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/rom.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "inc/tm4c1294ncpdt.h"
#include "driverlib/timer.h"



// The error routine that is called if the driver library encounters an error.
//#ifdef DEBUG
void __error__(char *pcFilename, uint32_t ui32Line);


//#endif

#define LED1_ON		GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_1, GPIO_PIN_1);
#define LED1_OFF	GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_1, 0);
#define LED2_ON		GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, GPIO_PIN_0);
#define LED2_OFF	GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, 0);
#define LED3_ON		GPIOPinWrite(GPIO_PORTF_AHB_BASE, GPIO_PIN_4, GPIO_PIN_4);
#define LED3_OFF	GPIOPinWrite(GPIO_PORTF_AHB_BASE, GPIO_PIN_4, 0);
#define LED4_ON		GPIOPinWrite(GPIO_PORTF_AHB_BASE, GPIO_PIN_0, GPIO_PIN_0);
#define LED4_OFF	GPIOPinWrite(GPIO_PORTF_AHB_BASE, GPIO_PIN_0, 0);

#define SYSCTL_PERIPH_BUTTON	SYSCTL_PERIPH_GPIOJ
#define GPIO_BUTTON		GPIO_PORTJ_AHB_BASE
#define BUTTON_PINS		GPIO_PIN_0 | GPIO_PIN_1

#define SYSCTL_PERIPH_TIMER		SYSCTL_PERIPH_TIMER6
#define TIMER_delay				TIMER6_BASE

extern uint32_t F_SysClk;

void LED_EK_blink(uint32_t ms);
void LED_EK_blink_forever(uint32_t ms);
void LEDs_EK_setup();
void buttons_EK_setup();

char* reverse(char* src,uint8_t length);
void itoa(int32_t num, char* str, uint32_t base);
void utoa(uint32_t num, char* str, uint32_t base);
void ftoa (float num,uint8_t precision,char *str);	//requires 2048 stack size

void delay_ms(uint_fast32_t ms);
void delay_us(uint_fast32_t us);



#endif /* HANDY_H_ */
