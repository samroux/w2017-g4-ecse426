#ifndef TIMER_H
#define TIMER_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "stm32f4xx_hal.h"
#include "lis3dsh.h"
#include "math.h"
#include "main.h"

void Timer_Init(void);
void LEDs_Init(void);
void setRollDC(int value);
void setPitchDC(int value);

#endif