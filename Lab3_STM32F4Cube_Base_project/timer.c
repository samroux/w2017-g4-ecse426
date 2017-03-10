/**
  ******************************************************************************
  * File Name          : timer.c
  * Description        : Main program subroutine
	* Author						 : Chris Di Betta and Samuel Roux
	* Version            : 1.0.0
	* Date							 : January 14th, 2016
  ******************************************************************************
  */
	
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "supporting_functions.h"
#include "lis3dsh.h"
#include "main.h"
#include "tilt_detect.h"

TIM_HandleTypeDef tim4_handle;
TIM_HandleTypeDef TIM3_handle;
int tim_counter;

void initTimer()
{
	TIM_Base_InitTypeDef initTIM4;
	TIM_OC_InitTypeDef Timer_init;
	
	// Desired Rate = ClockFrequency / (prescaler * period)
	// Rate = 2000Hz, frequency = 42MHz, precaler = 1000																	 
	
	// Initialize timer 4 initialization struct 
	initTIM4.Period = 42;			 								// Period is in MHz
	initTIM4.Prescaler = 1000;
	initTIM4.CounterMode = TIM_COUNTERMODE_UP;
	
	// Initialize timer 4 handle struct
	tim4_handle.Instance = TIM4;
	tim4_handle.Init = initTIM4;
	tim4_handle.Channel = HAL_TIM_ACTIVE_CHANNEL_1;
	Timer_init.OCMode = TIM_OCMODE_PWM1;
	Timer_init.Pulse = 42;

	// Initialize timer 4 handle and enable interrupts
	HAL_TIM_PWM_MspInit(&tim4_handle);
	HAL_TIM_PWM_Init(&tim4_handle);
	HAL_TIM_PWM_ConfigChannel(&tim4_handle, &Timer_init, TIM_CHANNEL_1);
}

void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef* pwm)
{
	// Enable clock for TIM4 
	__TIM4_CLK_ENABLE();
	
	__GPIOD_CLK_ENABLE();
	GPIO_InitTypeDef GPIO_InitDef_LED;
	GPIO_InitDef_LED.Pin = GPIO_PIN_12;
	GPIO_InitDef_LED.Mode 	= GPIO_MODE_AF_PP;
	//GPIO_InitDef_LED.Pull 	= GPIO_PULLDOWN;
	GPIO_InitDef_LED.Speed 	= GPIO_SPEED_FREQ_MEDIUM; 
	GPIO_InitDef_LED.Alternate = GPIO_AF2_TIM4;
  HAL_GPIO_Init(GPIOD, &GPIO_InitDef_LED);
}


//Can be used to cause a software delay
void delay(uint32_t time)
{
  TimingDelay = time;
  while(TimingDelay !=0);
}

//Exception Handling for timer
/*
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *Timer){

	if (Timer->Instance == TIM4){
		int input = KeyPadGetValue();
		
    keyPadInput(input);		

		if(rollValue > 0) {
			controlRollLED();
		}
		 if (pitchValue > 0 ){
			controlPitchLED();
		}
		
	}
	  
}
*/

//Timer callback
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef* htim) {
	//Prevent compilation warning
  UNUSED(htim);
	
}


void init_TIM4(void) {
	TIM_Base_InitTypeDef initTIM4;
	
	// Enable clock for TIM4 
	__HAL_RCC_TIM4_CLK_ENABLE();
	
	// Desired Rate = ClockFrequency / (prescaler * period)
	// Rate = 2000Hz, frequency = 42MHz, precaler = 1000																	 
	
	// Initialize timer 4 initialization struct 
	initTIM4.Period = 42;			 								// Period is in MHz
	initTIM4.Prescaler = 1000;
	//initTIM4.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	initTIM4.CounterMode = TIM_COUNTERMODE_UP;
	
	// Initialize timer 4 handle struct
	tim4_handle.Instance = TIM4;
	tim4_handle.Init = initTIM4;
	tim4_handle.Channel = HAL_TIM_ACTIVE_CHANNEL_CLEARED;
	tim4_handle.Lock = HAL_UNLOCKED;
	tim4_handle.State = HAL_TIM_STATE_READY;

	// Initialize timer 4 handle and enable interrupts
	HAL_TIM_Base_MspInit(&tim4_handle);
	HAL_TIM_Base_Init(&tim4_handle);
	HAL_TIM_Base_Start_IT(&tim4_handle);
		
	// Configure NVIC 
	HAL_NVIC_SetPriority(TIM4_IRQn, 0,1);
	HAL_NVIC_EnableIRQ(TIM4_IRQn);
	
	//HAL_NVIC_ClearPendingIRQ(TIM4_IRQn);
	
}


void controlRollLED(float desiredRoll, float currentRoll){
	float rollDifference = fabs(desiredRoll - currentRoll);
	
	if (rollDifference < 5){
	__HAL_TIM_SetCompare(&tim4_handle, TIM_CHANNEL_1, 0);
	__HAL_TIM_SetCompare(&tim4_handle, TIM_CHANNEL_3, 0);
	
	}
	else {
	__HAL_TIM_SetCompare(&tim4_handle, TIM_CHANNEL_1, rollDifference);
	__HAL_TIM_SetCompare(&tim4_handle, TIM_CHANNEL_3, rollDifference);
	}

}

void controlPitchLED(float desiredPitch, float currentPitch){
	float pitchDifference = fabs(desiredPitch - currentPitch);
	
	 if(pitchDifference < 5){
	__HAL_TIM_SetCompare(&tim4_handle, TIM_CHANNEL_2, 0);
	__HAL_TIM_SetCompare(&tim4_handle, TIM_CHANNEL_4, 0);
	}
	else {
	__HAL_TIM_SetCompare(&tim4_handle, TIM_CHANNEL_2, pitchDifference);
	__HAL_TIM_SetCompare(&tim4_handle, TIM_CHANNEL_4, pitchDifference);
	
	}
}


void Timer_Init(void){
	
	TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_MasterConfigTypeDef sMasterConfig;
  TIM_OC_InitTypeDef Timer_init;
	
	__TIM4_CLK_ENABLE();
	
	tim4_handle.Instance = TIM4;
	
	tim4_handle.Init.Period = 360;
	tim4_handle.Init.Prescaler = 117;
	tim4_handle.Init.RepetitionCounter = 0;
	tim4_handle.Init.ClockDivision = 0;
	tim4_handle.Channel = TIM_CHANNEL_ALL;
	tim4_handle.Init.CounterMode = TIM_COUNTERMODE_UP;
	
	HAL_TIM_Base_Init(&tim4_handle);
	Timer_init.OCMode = TIM_OCMODE_PWM1;
	Timer_init.OCPolarity = TIM_OCPOLARITY_HIGH;
	Timer_init.OCIdleState = TIM_OCIDLESTATE_RESET;
	Timer_init.Pulse = 360;
	
//	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
//	
//	HAL_TIM_ConfigClockSource(&tim4_handle, &sClockSourceConfig);

//	HAL_TIM_PWM_Init(&tim4_handle);
//	
//	
//	HAL_NVIC_EnableIRQ(TIM4_IRQn);
//	HAL_NVIC_SetPriority(TIM4_IRQn, 0, 0);
}

void PWM_Init(void){
	HAL_TIM_PWM_Init(&tim4_handle);
	
	HAL_TIM_PWM_Start(&tim4_handle, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&tim4_handle, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&tim4_handle, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&tim4_handle, TIM_CHANNEL_4);
}
/*
void timer_configure(void){
	// set pointer to the flag
	// enable clock
	__TIM3_CLK_ENABLE();
	
	segments_configure();
	
	// initialize the timer 
	TIM3_handle.Init.Prescaler = 84;
	TIM3_handle.Init.CounterMode = TIM_COUNTERMODE_UP;
	TIM3_handle.Init.Period = 999;
	// Desired rate = TimerClockingFrequency / ((Period+1) x prescalar)
	// 84 Mhz / 84 / 1000 = 1000Hz
	
	TIM3_handle.Instance = TIM3;
	HAL_TIM_Base_Init(&TIM3_handle);     // Init timer
	HAL_TIM_Base_Start_IT(&TIM3_handle); // start timer interrupts
	
	HAL_NVIC_SetPriority(TIM3_IRQn, 0, 1);
	HAL_NVIC_EnableIRQ(TIM3_IRQn);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	if (htim->Instance == TIM3){
		if (tim_counter % MOUSE_CONTROL_PERIOD == 0){
			mouse_control_tim_elapsed_handler(htim);
		}
		if (tim_counter % MOUSE_CONTROL_PERIOD == 0){
			mouse_tim_elapsed_handler(htim);
		}
		tim_counter++;
		TIM3_IRQHandler();
	}
}
*/