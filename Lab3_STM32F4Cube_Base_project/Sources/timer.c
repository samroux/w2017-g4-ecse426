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
TIM_ClockConfigTypeDef ClockConfig;
TIM_MasterConfigTypeDef sMasterConfig;
TIM_OC_InitTypeDef sConfigOC;
void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

void Timer_Init(void)
{

  tim4_handle.Instance = TIM4;
  tim4_handle.Init.Prescaler = 0;
  tim4_handle.Init.CounterMode = TIM_COUNTERMODE_UP;
  tim4_handle.Init.Period = 500;
  tim4_handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  
	HAL_TIM_Base_Init(&tim4_handle);
	
  ClockConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	
  HAL_TIM_ConfigClockSource(&tim4_handle, &ClockConfig);

  HAL_TIM_PWM_Init(&tim4_handle);


  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  
	HAL_TIMEx_MasterConfigSynchronization(&tim4_handle, &sMasterConfig);

  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 500;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  
	HAL_TIM_PWM_ConfigChannel(&tim4_handle, &sConfigOC, TIM_CHANNEL_1);

  HAL_TIM_PWM_ConfigChannel(&tim4_handle, &sConfigOC, TIM_CHANNEL_2);

  HAL_TIM_PWM_ConfigChannel(&tim4_handle, &sConfigOC, TIM_CHANNEL_3);

  HAL_TIM_PWM_ConfigChannel(&tim4_handle, &sConfigOC, TIM_CHANNEL_4);

  HAL_TIM_MspPostInit(&tim4_handle);
	
	HAL_TIM_Base_Start(&tim4_handle); 
	HAL_TIM_PWM_Start(&tim4_handle,TIM_CHANNEL_1); 
	HAL_TIM_PWM_Start(&tim4_handle,TIM_CHANNEL_2); 
	HAL_TIM_PWM_Start(&tim4_handle,TIM_CHANNEL_3); 
	HAL_TIM_PWM_Start(&tim4_handle,TIM_CHANNEL_4); 

}

void LEDs_Init(void){
		
	GPIO_InitTypeDef D_GPIO;
	GPIO_InitTypeDef B_GPIO;
	
	D_GPIO.Pin = GPIO_PIN_12;
	
	D_GPIO.Mode = GPIO_MODE_AF_PP;
	D_GPIO.Pull = GPIO_PULLUP;
	D_GPIO.Speed = GPIO_SPEED_FREQ_HIGH;
	D_GPIO.Alternate = GPIO_AF2_TIM4;
	
	HAL_GPIO_Init(GPIOD, &D_GPIO);	//Initialize GPIO D
	
	
	B_GPIO.Pin = GPIO_PIN_6;
	
  B_GPIO.Mode = GPIO_MODE_AF_PP;
  B_GPIO.Pull = GPIO_PULLUP;
  B_GPIO.Speed = GPIO_SPEED_FREQ_HIGH;
	
	HAL_GPIO_Init(GPIOB, &B_GPIO);
	
	D_GPIO.Pin = GPIO_PIN_13;
	HAL_GPIO_Init(GPIOD, &D_GPIO);
	
	D_GPIO.Pin = GPIO_PIN_14;
	HAL_GPIO_Init(GPIOD, &D_GPIO);
	
	D_GPIO.Pin = GPIO_PIN_15;
	HAL_GPIO_Init(GPIOD, &D_GPIO);
	
	B_GPIO.Pin = GPIO_PIN_7;
	HAL_GPIO_Init(GPIOB, &B_GPIO);
	
}

void setRollDC(int value){
	sConfigOC.Pulse = value;
	

  HAL_TIM_PWM_ConfigChannel(&tim4_handle, &sConfigOC, TIM_CHANNEL_2);


  HAL_TIM_PWM_ConfigChannel(&tim4_handle, &sConfigOC, TIM_CHANNEL_4);

  HAL_TIM_MspPostInit(&tim4_handle);
	
	HAL_TIM_Base_Start(&tim4_handle);
	
	HAL_TIM_PWM_Start(&tim4_handle,TIM_CHANNEL_2); 
	HAL_TIM_PWM_Start(&tim4_handle,TIM_CHANNEL_4); 

}

void setPitchDC(int value){
	sConfigOC.Pulse = value;
	
	HAL_TIM_PWM_ConfigChannel(&tim4_handle, &sConfigOC, TIM_CHANNEL_1);


  HAL_TIM_PWM_ConfigChannel(&tim4_handle, &sConfigOC, TIM_CHANNEL_3);


  HAL_TIM_MspPostInit(&tim4_handle);
	
	HAL_TIM_Base_Start(&tim4_handle);
	
	HAL_TIM_PWM_Start(&tim4_handle,TIM_CHANNEL_1); 
	//HAL_TIM_PWM_Start(&tim4_handle,TIM_CHANNEL_2); 
	HAL_TIM_PWM_Start(&tim4_handle,TIM_CHANNEL_3); 
	//HAL_TIM_PWM_Start(&tim4_handle,TIM_CHANNEL_4); 
}