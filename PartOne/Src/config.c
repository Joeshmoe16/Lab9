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
	
	/*
	//MODER8[0:0] button pin is 8, add a pull up resistor using PUPDR[1:1]
	GPIOB -> MODER &= ~(BIT(16)|BIT(17));
	GPIOB -> PUPDR  |= BIT(16);
	
	//MODER6[0:0] button pin is 6, add a pull up resistor using PUPDR[1:1]
	GPIOB -> MODER &= ~(BIT(20)|BIT(21));
	GPIOB -> PUPDR  |= BIT(20);
	*/
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

/***| msDelay(uint16_t n) |*****************************//*
*Brief: msDelay Function, generates a delay using a for loop
*				the delays are not timed, eye balled for debouncing
*Params:
*           uint16_t n
*Returns:
*           None
********************************************************/
void SysTick_msdelay (uint32_t msdelay)                 // SysTick delay function
{
	SysTick -> LOAD = ((msdelay * 16000) - 1);          // delay for 1 ms* delay value
	SysTick -> VAL  = 0;                                // any write to CVR clears it

	while (!(SysTick -> CTRL  &  0x00010000));
	
}


/***|debounce(uint16_t delay) |*****************************//*
*Brief: 
*	Denounces the button: If button is on after ~10 cycles return true. 
* If button is off after ~10 cycles return false. 
* Works same as above, but does it explicitly.
*Params:
*          
*Returns:
*           True/False if button is on
********************************************************/
/*#define DEBOUNCE_LVL 10
unsigned char debounce(){
  static char     output = 0;
  static uint32_t  debounce = 0;
	
	char BTN = !(GPIOB->IDR & IDR8);
	
	if(!output){
		//debounced button input
		if(BTN){
			debounce += 1;
			if(debounce >= DEBOUNCE_LVL){
				output = 1;
				debounce = DEBOUNCE_LVL;
			}
		}
		else{
			debounce = 0;
			output = 0;
		}
	}
	else{
		if(!BTN)
		{
			debounce -= 1;
			if(debounce == 0){
				output = 0;
				debounce = DEBOUNCE_LVL;
			}
		}
		else{
			debounce = DEBOUNCE_LVL;
			output = 1;
		}
	}

  return output;
}*/