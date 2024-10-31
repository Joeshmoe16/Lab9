#ifndef stepper
#define stepper

#include "stm32f4xx.h"
#include "stdint.h"

#define BIT(x) (1<<(x)) 

//configurable, defines port and pins
#define PORT GPIOC
#define P1 2
#define P2 4
#define P3 6
#define P4 8

//Not configurable, used for GPIO Control
#define COILS_OFF (PORT -> ODR &= ~(BIT(P1) | BIT(P2) | BIT(P3) | BIT(P4)))

#define COILA1_ON (PORT -> ODR |= BIT(P1))
#define COILA2_ON (PORT -> ODR |= BIT(P2))
#define COILB1_ON (PORT -> ODR |= BIT(P4))
#define COILB2_ON (PORT -> ODR |= BIT(P3))

#define COILA1_OFF (PORT -> ODR &= ~BIT(P1))
#define COILA2_OFF (PORT -> ODR &= ~BIT(P2))
#define COILB1_OFF (PORT -> ODR &= ~BIT(P3))
#define COILB2_OFF (PORT -> ODR &= ~BIT(P4))


/// @brief Step motor with wave drive
//void stepWave(char reset, char CounterC);

/// @brief Step motor with full step drive
void stepFull(char reset, char CounterC);

/// @brief Step motor with half step drive
//void stepHalf(char reset, char CounterC);

void stepGPIO(void);

#endif //end stepper