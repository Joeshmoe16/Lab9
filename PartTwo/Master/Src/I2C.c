/*************************************************
 * AUTHOR(s): Tom Stutz
 * 
 * FILE: i2c.c
 * 
 * PROGRAM: 
 * 
 * DESCRIPTION: 
 * 
 * VERSION: 1.0
 * 
 * DATE: (DD-MM-YYYY) 29-08-24
 * 
 * 
 * VERBOSE HEADER
 *************************************************/



///////////////////////////////////////////////////
//User Includes
///////////////////////////////////////////////////
#include "i2c.h"
#include "config.h"
#include <stm32f446xx.h>

///////////////////////////////////////////////////
//User defines
///////////////////////////////////////////////////
#define SCL_pin 8 /*port b*/
#define SCA_pin 9 /*port b*/

/*******
FUNCTION: i2c1_init
INPUTS: 
OUTPUTS: N/A
DESCRIPTION: This function initializes i2c 1
*******/
void I2C1_init(void)
{
    RCC->APB1ENR |= 0x01 << 21;
    //initialize PB8 as SCL and PB9 as SCA
    GPIOB->AFR[1] &= ~0x00FF;
    GPIOB->AFR[1] |= 0x0044;    /*PB 8 and PB9 to alternate functions*/
    GPIOB->MODER &= ~(0x000F0000);
    GPIOB->MODER |= 0x000A0000;

    //initlize as open drains
    GPIOB->OTYPER |= 0x00000300;

    //initialize pullups
    //GPIOB->PUPDR &= ~0x000F0000;
    //GPIOB->PUPDR |= 0x00050000;

    //initialize the i2c1 registers
    I2C1->CR1 = 0x8000;
    I2C1->CR1 &= ~0x8000;
    I2C1->CR2 = 0x0010;
    I2C1->CCR = 80;
    I2C1->TRISE = 17;
    I2C1->CR1 |= 0x0001;
}

/*******
FUNCTION: I2C1_byteWrite
INPUTS: 
OUTPUTS: N/A
DESCRIPTION: This function initializes i2c 1
*******/
int I2C1_byteWrite_stm32(char saddr, char data)
{
    volatile int tmp;
    while(I2C1->SR2 & 2);

    I2C1->CR1 |= 0x100;
    while(!(I2C1->SR1 & 1));

    I2C1->DR = (saddr << 1);
    while(!(I2C1->SR1 & 2));
    tmp = I2C1->SR2;

    while(!(I2C1->SR1 & (1<<7)));
    I2C1->DR = data;

    while(!(I2C1->SR1 & 4));
    I2C1->CR1 |= 0x200;

    return 0;

}
