#include "config.h"
#include "stm32f4xx.h"
#include "stdint.h"
#include "i2c.h"

char steps = 0;
char dir = 0;

int main(void){
	setup();
	stepGPIO();
	stdout_init();
	I2C1_init();

	PINB0_ON;
	SysTick_msdelay(500);
	PINB0_OFF;
	PINB1_ON;

	for(;;){}
}

void I2C1_EV_IRQHandler(void){
	char data = 0;

    //Check if ADDR=1 in SR1
    while(!(I2C1->SR1 & 2));
	
    volatile int tmp;
	tmp = I2C1->SR2; //read sr2 to clear bit

    //Check if RxNE bit =1 in SR1
    while(!(I2C1->SR1 & (1<<6)));
    
    //If true, read DR register (to get data and clear bit)
    data = I2C1->DR;

    //Check if STOPF=1 in SR1
    while(I2C1->SR2 & (1<<4));

	if(data==0x7){
		PINB0_TOGGLE;
		PINB1_TOGGLE;
	}	
	
	//if true write to CR1 to clear it
    I2C1->CR1 |= 0x0001;
	I2C1->SR1 &= ~BIT(6); //read sr2 to clear bit

}
