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
    GPIOB->AFR[1] |= 0x0044;    /*PB8 and PB9 to alternate functions*/
    GPIOB->MODER &= ~(0x000F0000);
    GPIOB->MODER |= 0x000A0000;

    //initlize as open drains
    GPIOB->OTYPER |= 0x00000300;
    //GPIOB->OSPEEDR |= 0xF0000; //set PB9&9 to high speed

    //initialize pullups
    //GPIOB->PUPDR &= ~0x000F0000;
    //GPIOB->PUPDR |= 0x00050000;

    //initialize the i2c1 registers
    I2C1->CR1 = 0x8000; 
    I2C1->CR1 &= ~0x8000;
    I2C1->CR2 = 0x0010; //set frequency
    I2C1->CCR = 80; //?
    I2C1->TRISE = 17; //?
    I2C1->CR2 |= (1<<9)|(1<<10); //enabale intertupts
    I2C1->CR1 |= 0x0001; //enable
    I2C1->CR1 |= (1<<10);

    //set addtess mode to 7 bit BIT(16), set address to 0x22, adress must be <0x80,
    I2C1->OAR1 |= (0x00022<<1); 

    __disable_irq(); // global disable IRQs
    NVIC_EnableIRQ(I2C1_EV_IRQn);
    //NVIC_DisableIRQ(I2C1_EV_IRQn);
    __enable_irq(); // global enable IRQs
}

int I2C1_slave_byteRead(void){
    char data = 0;

    //Check if ADDR=1 in SR1
    while(!(I2C1->SR1 & 1));
    volatile int tmp;
	tmp = I2C1->SR2; //read sr2 to clear bit

    //Check if RxNE bit =1 in SR1
    while(!(I2C1->SR1 & (1<<6)));
    
    //If true, read DR register (to get data and clear bit)
    data = I2C1->DR;

    //Check if STOPF=1 in SR1
    while(I2C1->SR2 & (1<<4));

    //if true write to CR1 to clear it
    I2C1->CR1 |= 0x0001;

    //return data as int
    return data;
}



/*******
FUNCTION: I2C1_byteWrite
INPUTS: 
OUTPUTS: N/A
DESCRIPTION: This function initializes i2c 1
*******/
int I2C1_byteWrite(char saddr, char maddr, char data)
{
    volatile int tmp;
    while(I2C1->SR2 & 2);

    I2C1->CR1 |= 0x100;
    while(!(I2C1->SR1 & 1));

    I2C1->DR = saddr << 1;
    while(!(I2C1->SR1 & 2));
    tmp = I2C1->SR2;

    while(!(I2C1->SR1 & 0x80));
    I2C1->DR = maddr;

    while(!(I2C1->SR1 & 0x80));
    I2C1->DR = data;

    while(!(I2C1->SR1 & 4));
    I2C1->CR1 |= 0x200;

    return 0;

}

/*******
FUNCTION: I2C1_byteRead
INPUTS: 
OUTPUTS: N/A
DESCRIPTION: This function initializes i2c 1
*******/
int I2C1_byteRead(char saddr, char maddr, char* data)
{
    volatile int tmp;
	
    while(I2C1->SR2 & 2);

    I2C1->CR1 |= 0x100;
    while(!(I2C1->SR1 & 1));

    I2C1->DR = saddr << 1;
    while(!(I2C1->SR1 & 2));
    tmp = I2C1->SR2;

    while(!(I2C1->SR1 & 0x80));
    I2C1->DR = maddr;

    while(!(I2C1->SR1 & 0x80));
    
    I2C1->CR1 |= 0x100;
    while(!(I2C1->SR1 & 1));
    I2C1->DR = saddr<<1 | 1;

    while(!(I2C1->SR1 & 2));
    I2C1->CR1 &= ~0x400;
    tmp = I2C1->SR2;

    I2C1->CR1 |= 0x200;

    while(!(I2C1->SR1 & 0x40));
    *data++ = I2C1->DR;

    return 0;

}