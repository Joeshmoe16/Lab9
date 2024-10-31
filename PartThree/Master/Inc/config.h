#ifndef config
#define config

#include "stm32f4xx.h"
//#include "stdio.h"
#include "stdint.h"

#define BIT(x) (1<<(x)) 

#define PINB6_READ (GPIOB->IDR & BIT(6))
#define PINB7_READ (GPIOB->IDR & BIT(7))

#define PINB0_OFF (GPIOB -> ODR &= ~BIT(0))
#define PINB1_OFF (GPIOB -> ODR &= ~BIT(1))
#define PINB2_OFF (GPIOB -> ODR &= ~BIT(2))
#define PINB012_OFF (GPIOB -> ODR &= ~(BIT(0) | BIT(1) | BIT(2)))
#define PINB0_ON (GPIOB -> ODR |= BIT(0))
#define PINB1_ON (GPIOB -> ODR |= BIT(1))
#define PINB2_ON (GPIOB -> ODR |= BIT(2))

#define PINB0_TOGGLE (GPIOB -> ODR ^= BIT(0))
#define PINB1_TOGGLE (GPIOB -> ODR ^= BIT(1))
#define PINB2_TOGGLE (GPIOB -> ODR ^= BIT(2))

void setup(void);
void gpio_setup(void);
void SysTick_Init(void);


#endif