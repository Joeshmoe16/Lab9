/*******************************************************************************
* Name:             Joseph Shotts
* Course:           EGR 326 Embedded System Design
* Project:          DemoProject
* File:             config.c
* Description:      Includes functions to initialize GPIO pins and systick.
********************************************************************************/

#include "config.h"

/***| setup() |*****************************//*
*Brief: 
*	Runs Setup Functions
*Params:
*            None
*Returns:
*           None
**********************************************/
void setup(void){
	gpio_setup();
	SysTick_Init();
}

/***| gpio_setup() |*****************************//*
*Brief: 
*	Initializes LEDs and Buttons
*Params:
*            None
*Returns:
*           None
********************************************************/
void gpio_setup(void)
{
	//Enable GPIO B
	RCC -> AHB1ENR |= BIT(1);

	//MODERx[0:1} (x is pins 0, 2, 4, 6)
	GPIOB -> MODER |= BIT(0);
	GPIOB -> MODER |= BIT(2);
	GPIOB -> MODER |= BIT(4);
	
	//Turn all LEDs Off
	PINB012_OFF;
	
	//MODER8[0:0] button pin is 6, add a pull up resistor using PUPDR[1:1]
	GPIOB -> MODER &= ~(BIT(12)|BIT(13));
	GPIOB -> PUPDR  |= BIT(12);
	
	//MODER6[0:0] button pin is 7, add a pull up resistor using PUPDR[1:1]
	GPIOB -> MODER &= ~(BIT(14)|BIT(15));
	GPIOB -> PUPDR  |= BIT(14);
}

/***| Systick_Init() |*****************************//*
*Brief: 
*	Initializes and resets Systick
*Params:
*            None
*Returns:
*           None
********************************************************/
void SysTick_Init (void)
{                                                       // initialization of SysTick timer
    SysTick -> CTRL     = 0;                            // disable SysTick during step
    SysTick -> LOAD     = 0x00FFFFFF;                   // max reload value
    SysTick -> VAL      = 0;                            // any write to current clears it
    SysTick -> CTRL     = 0x00000005;                   // enable SysTick, 16MHz, No Interrupts
}


/***|debounce(uint16_t delay) |*****************************//*
*Brief: 
*	Denounces the button: If button is on after ~10 cycles return true. 
* If button is off after ~10 cycles return false. 
*Params:
*          
*Returns:
*           True/False if button is on
********************************************************/
unsigned char debounce(void)
{
	static uint32_t State = 0; // Current debounce status
	static char output = 0;   //output value, stored for debouncing release
	
	//checks if button was previously off
	if(!output){
		// read switch, upper 3 bits of State are don't cares
		State=(State<<1) | PINB6_READ | 0xe0000000; 
		if (State == 0xf0000000) {output = 1; State = 0;}
		else output = 0;
	}
	else{
		// read switch, upper 3 bits of State are don't cares
		State=(State<<1) | !PINB6_READ | 0xe0000000; 
		if (State == 0xf0000000) {output = 0; State = 0;}
		else output = 1;
	}
	
	return output;
}

/***|debounce2(uint16_t delay) |*****************************//*
*Brief: 
*	Denounces the button: If button is on after ~10 cycles return true. 
* If button is off after ~10 cycles return false. 
*Params:
*          
*Returns:
*           True/False if button is on
********************************************************/
unsigned char debounce2(void)
{
	static uint32_t State = 0; // Current debounce status
	static char output = 0;   //output value, stored for debouncing release
	
	//checks if button was previously off
	if(!output){
		// read switch, upper 3 bits of State are don't cares
		State=(State<<1) | PINB7_READ | 0xe0000000; 
		if (State == 0xf0000000) {output = 1; State = 0;}
		else output = 0;
	}
	else{
		// read switch, upper 3 bits of State are don't cares
		State=(State<<1) | !PINB7_READ | 0xe0000000; 
		if (State == 0xf0000000) {output = 0; State = 0;}
		else output = 1;
	}
	
	return output;
}