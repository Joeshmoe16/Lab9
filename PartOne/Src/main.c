#include "config.h"
#include "stm32f4xx.h"
#include "stdint.h"
#include "stepper.h"

int main(void){
	printf("Code Start\n");
	setup();
	stepGPIO();
	const int numSteps = 100;
	char delay = 25;
	for(;;){
		for(int i=0; i<=numSteps; i++){
			stepFull(0, 1);
			SysTick_msdelay(delay);
		}
		stepFull(1, 0);
		for(int i=0; i<=numSteps; i++){
			stepFull(0, 0);
			SysTick_msdelay(delay);
		}
		stepFull(1, 1);
	}
}