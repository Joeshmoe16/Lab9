
/*******************************************************************************
* Name:
* Course:           EGR 226 - Microcontroller Programming and Applications
* Project:          Lab 05 - Keypad Introduction
* File:             keypad.c
* Description:      Includes functions to initialize and sense the keypad, print
*                   the pressed key
* Pin Configs:      PC0-PC3=> Rows and PC4-PC6=> Cols
*                   Keypad pin7-PC0
*                   Keypad pin1-PC6
********************************************************************************/

/***| Standard Library Includes |***/
#include "stm32f4xx.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "keypad.h"


/***| Systick_Delay() |*********************************//*
*Brief: Systick Millisecond Delay Function, gets 16MHz clock
*Brief: 1/16 e-6s is one period. Max load is 2^24
*Params:
*           (uint16) msdelay: delay in the units of milliseconds
*Returns:
*           None
********************************************************/
void SysTick_Delay(uint16_t delay)                      // ---- Configurable Systick delay fromm 1 ms to 5592 ms
{
    SysTick -> LOAD = ((delay*16000) - 1);               // 1ms count down to zero
    SysTick -> VAL = 0;                                 // any write to CVR clears it and COUNTFLAG in CSR
    while((SysTick -> CTRL & 0x00010000) == 0);         // Wait for flag to be SET (Timeout happened)
}

/***| Keypad_Init() |***********************************//*
*Brief: Standard Port Initializations, select the responsible
*Brief: pins and assign them as GPIO input with rows pull up enabled
*Brief: Since the reading will be done over the rows, rows are set up
*Brief: as pull up resistors
*Params:
*           None
*Returns:
*           None
********************************************************/
void Keypad_Init (void)
{
    RCC->AHB1ENR |= BIT(C);
		GPIOC->MODER &=~(uint32_t) (SHIFT(2*R0,3)|SHIFT(2*R1,3)|SHIFT(2*R2,3)|SHIFT(2*R3,3)|SHIFT(2*C0,3)|SHIFT(2*C1,3)|SHIFT(2*C2,3));  // 2y+1:2y bits reset
		GPIOC->PUPDR &=~(uint32_t) (SHIFT(2*R0,3)|SHIFT(2*R1,3)|SHIFT(2*R2,3)|SHIFT(2*R3,3));	// 2y+1:2y bits reset
		GPIOC->PUPDR |= (uint32_t) (SHIFT(2*R0,1)|SHIFT(2*R1,1)|SHIFT(2*R2,1)|SHIFT(2*R3,1));	// 2y+1:2y bits set to pullup	
	  // R0-R3: pull up, C0-C2: High Z (no pull up/down)
}

/***| Read_Keypad() |***********************************//*
*Brief: Checks for the button press by grounding each col as
*Brief: assigning each col as GND one by one while the rest
*Brief: are floating input with infinite impedance. As a result,
*Brief: if any press is hit, that row will be grounded over the
*Brief: col that is assigned as GND.
*|--|--| -> Row0=> GND if 1 is pressed
*|--|--| -> Row1=> GND if 4 is pressed
*|--|--| -> Row2=> GND if 7 is pressed
*|--|--| -> Row3=> GND if * is pressed
*|=>GND
*Brief: Caution: If two cols are hit at the same time and they
*Brief: are assigned as both OUT, there will be a short circuit
*Brief: that can fry MCU. Hence, multiple repeating IN settings
*Brief: are placed for safety.
*Params:
*           (uint16) *numptr: pressed number pointer
*Returns:
*           None
********************************************************/
uint8_t Read_Keypad(uint16_t *numptr)
{
    uint8_t col,row;                                    // will need to loop for rows and cols
    for(col=0; col<3; col++)
    {
        GPIOC->MODER &=~(uint32_t)(SHIFT(2*R0,3)|SHIFT(2*R1,3)|SHIFT(2*R2,3)|SHIFT(2*R3,3)|SHIFT(2*C0,3)|SHIFT(2*C1,3)|SHIFT(2*C2,3));        // Set Columns to inputs for safety not to burn MCU
        GPIOC->MODER |= (uint32_t) SHIFT(2*(col+4),1);        // Set Column i to Output
				GPIOC->ODR   &=~(uint32_t) BIT((col+4));						   	// Set Column i to GND
				//SysTick_Delay(1);                                    // Delay the while loop
        row =  GPIOC->IDR & (BIT(R0)|BIT(R1)|BIT(R2)|BIT(R3));// read all rows: Read must be done before the debouncing
			  while( !(GPIOC->IDR & BIT(R0))||!(GPIOC->IDR & BIT(R1))||!(GPIOC->IDR & BIT(R2))||!(GPIOC->IDR & BIT(R3))){;}
        // debouncing loop: if any of the buttons are pressed just put it into the loop

        if (row != 0x0F) break;                         // if one of the input is GND, some key is pressed.
    }
    GPIOC->MODER &=~(uint32_t) (SHIFT(2*R0,3)|SHIFT(2*R1,3)|SHIFT(2*R2,3)|SHIFT(2*R3,3)|SHIFT(2*C0,3)|SHIFT(2*C1,3)|SHIFT(2*C2,3));           // Set Columns to inputs
		if (col == 3)  return 0;                            // none pressed: col went out of for loop max. value of 3
    if (row == 0x0E) *numptr = 3 + col + 1;             // key in row 0, PC3-0=1110
    if (row == 0x0D) *numptr =     col + 1;             // key in row 1, PC3-0=1101
    if (row == 0x0B) *numptr = 6 + col + 1;             // key in row 2, PC3-0=1011
    if (row == 0x07) *numptr = 9 + col + 1;             // key in row 3, PC3-0=0111
    return 1;
}

/***| Read_Keypad_Single() |***********************************//*
*Brief: Checks for the button press by grounding each col as
*Brief: assigning each col as GND one by one while the rest
*Brief: are floating input with infinite impedance. As a result,
*Brief: if any press is hit, that row will be grounded over the
*Brief: col that is assigned as GND.
*|--|--| -> Row0=> GND if 1 is pressed
*|--|--| -> Row1=> GND if 4 is pressed
*|--|--| -> Row2=> GND if 7 is pressed
*|--|--| -> Row3=> GND if * is pressed
*|=>GND
*Brief: Caution: If two cols are hit at the same time and they
*Brief: are assigned as both OUT, there will be a short circuit
*Brief: that can fry MCU. Hence, multiple repeating IN settings
*Brief: are placed for safety.
*Params:
*           (uint16) *numptr: pressed number pointer
*Returns:
*           None
********************************************************/
uint8_t Read_Keypad_Single(uint16_t *numptr)
{
    uint8_t col,row;                                    // will need to loop for rows and cols
    for(col=0; col<3; col++)
    {
        GPIOC->MODER &=~(uint32_t)(SHIFT(2*R0,3)|SHIFT(2*R1,3)|SHIFT(2*R2,3)|SHIFT(2*R3,3)|SHIFT(2*C0,3)|SHIFT(2*C1,3)|SHIFT(2*C2,3));        // Set Columns to inputs for safety not to burn MCU
        GPIOC->MODER |= (uint32_t) SHIFT(2*(col+4),1);        // Set Column i to Output
				GPIOC->ODR   &=~(uint32_t) BIT((col+4));						   	// Set Column i to GND
				//SysTick_Delay(1);                                    // Delay the while loop
        row =  GPIOC->IDR & (BIT(R0)|BIT(R1)|BIT(R2)|BIT(R3));// read all rows: Read must be done before the debouncing
			  while( !(GPIOC->IDR & BIT(R0))||!(GPIOC->IDR & BIT(R1))||!(GPIOC->IDR & BIT(R2))||!(GPIOC->IDR & BIT(R3))){;}
        // debouncing loop: if any of the buttons are pressed just put it into the loop

        if (row != 0x0F) break;                         // if one of the input is GND, some key is pressed.
    }
    GPIOC->MODER &=~(uint32_t) (SHIFT(2*R0,3)|SHIFT(2*R1,3)|SHIFT(2*R2,3)|SHIFT(2*R3,3)|SHIFT(2*C0,3)|SHIFT(2*C1,3)|SHIFT(2*C2,3));           // Set Columns to inputs
		if (col == 3)  return 0;                            // none pressed: col went out of for loop max. value of 3
    if (row == 0x0E) *numptr = 3 + col + 1;             // key in row 0, PC3-0=1110
    if (row == 0x0D) *numptr =     col + 1;             // key in row 1, PC3-0=1101
    if (row == 0x0B) *numptr = 6 + col + 1;             // key in row 2, PC3-0=1011
    if (row == 0x07) *numptr = 9 + col + 1;             // key in row 3, PC3-0=0111
    return 1;
}
