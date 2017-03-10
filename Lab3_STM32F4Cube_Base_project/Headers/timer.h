#ifndef TIMER_H
#define TIMER_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "stm32f4xx_hal.h"
#include "lis3dsh.h"
#include "math.h"
#include "main.h"

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef* htim);

void init_TIM4(void);

void controlRollLED(float desiredRoll, float currentRoll);
	
void controlPitchLED(float desiredPitch, float currentPitch);

void Timer_Init(void);

void PWM_Init(void);

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *Timer);

void initTimer();

#endif