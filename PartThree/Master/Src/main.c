#include "config.h"
#include "stm32f4xx.h"
#include "stdint.h"
#include "i2c.h"

int main(void){
	setup();
	Keypad_Init();
	I2C1_init();
	
	char FWD_BTN = 0;
	char RVS_BTN = 0;
	uint16_t steps = 0;

	for(;;){
		//0x01 -> keypad data entry 0-9 on bits 4-7
		//0x02 -> incriment num steps in main
		//0x03 -> decrement num steps in main
		if(debounce()&!FWD_BTN){
			FWD_BTN = 1;
			PINB0_TOGGLE;
			I2C1_byteWrite_stm32(0x22, 0x02);
		}
		if(!debounce()){
			FWD_BTN = 0;
		}
		if(debounce2()&!RVS_BTN){
			RVS_BTN = 1;
			PINB0_TOGGLE;
			I2C1_byteWrite_stm32(0x22, 0x03);
		}
		if(!debounce2()){
			RVS_BTN = 0;
		}

		Read_Keypad_Single(&steps);
		
		if(steps){
			/*for(int i=0; i<steps; i++){
				PINB0_TOGGLE;
				SysTick_Delay(250);
			}
			steps=0;*/
			I2C1_byteWrite_stm32(0x22, ((steps<<4)|(0x01)));
			steps=0;
			SysTick_Delay(300);
		}
	}
}
