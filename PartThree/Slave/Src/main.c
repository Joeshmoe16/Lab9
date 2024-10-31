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

	PINB0_TOGGLE;
	SysTick_msdelay(500);
	PINB0_TOGGLE;

	for(;;){
		//incriment steps in the correct direction
		if(steps){
			steps-=1;
			for(int i=0; i<4; i++){
				stepFull(0, dir);
				SysTick_msdelay(10);
			}
		};
	}
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

	//0x01 -> keypad data entry 0-9 on bits 4-7
	//0x02 -> incriment num steps in main
	//0x03 -> decrement num steps in main
	if((data & 0x3)==0x1){
		data = (data&0xF0)>>4;
		stdout_putchar(data+48);
		stdout_putchar('\n');
		steps += data;
	}
	else if((data & 0x3)==0x03){
		dir = 0;
		stdout_putchar('R');
		stdout_putchar('\n');
	}
	else if((data & 0x3)==0x02){
		dir = 1;
		stdout_putchar('F');
		stdout_putchar('\n');
	}
	
	//if true write to CR1 to clear it
    I2C1->CR1 |= 0x0001;
	I2C1->SR1 &= ~BIT(6); //read sr2 to clear bit

}
