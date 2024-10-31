/*************************************************
 * AUTHOR(s): Tom Stutz
 * 
 * FILE: i2c.h
 * 
 * PROGRAM: 
 * 
 * DATE: (DD-MM-YYYY) 29-08-24
 * 
 * 
 * MINIMAL HEADER
 *************************************************/


#ifndef __I2C_H__
#define __I2C_H__

///////////////////////////////////////////////////
//User Includes
///////////////////////////////////////////////////
#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#include <stm32f446xx.h>

///////////////////////////////////////////////////
//User fct prtype
///////////////////////////////////////////////////
void I2C1_init(void);
int I2C1_byteWrite_stm32(char saddr, char data);





#endif


//EOF