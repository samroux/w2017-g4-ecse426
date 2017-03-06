/**
  ******************************************************************************
  * @file    tilt_detect.c
  * @author  Chris Di Betta and Samuel Roux
  ****************************************************************************** 
*/
#include <stdio.h>
#include <string.h>
#include "stm32f4xx.h"                  // Device header
#include "lis3dsh.h"

#define PI 3.14159265358
#define conv_to_deg(x) (180.0*x/PI)

// Initialize accelerometer
void init_accelerometer(void) {
	
	//Configure LIS3DSH accelermoter sensor
	LIS3DSH_InitTypeDef init;
	LIS3DSH_DRYInterruptConfigTypeDef init_it;
	GPIO_InitTypeDef GPIO_InitE;
	
	init.Power_Mode_Output_DataRate = LIS3DSH_DATARATE_25;  	// Active mode with data rate 25HZ
	init.Axes_Enable = LIS3DSH_XYZ_ENABLE; 										// Enable all axes
	init.Continous_Update = LIS3DSH_ContinousUpdate_Enabled;  // Enable continuous update
	init.AA_Filter_BW = LIS3DSH_AA_BW_50; 										
	init.Full_Scale = LIS3DSH_FULLSCALE_2; 										// Use full scale range of 2g
	init.Self_Test = LIS3DSH_SELFTEST_NORMAL; 								// Keep at normal
	LIS3DSH_Init(&init);

	// Accelerometer interrupt configuration
  init_it.Dataready_Interrupt = LIS3DSH_DATA_READY_INTERRUPT_ENABLED; //Enable dataready interrupt
  init_it.Interrupt_signal = LIS3DSH_ACTIVE_HIGH_INTERRUPT_SIGNAL;    //Set interrupt signal to active high              
  init_it.Interrupt_type = LIS3DSH_INTERRUPT_REQUEST_PULSED;      		//Interrupt type as pulsed
	
	/* Set PE0 as the interupt for the accelorometer
	   Enable clocks for ports E */
	__HAL_RCC_GPIOE_CLK_ENABLE();

	// Give initialization values for GPIO E pin sets
	GPIO_InitE.Pin = GPIO_PIN_0 ;
	GPIO_InitE.Mode = GPIO_MODE_IT_RISING; 
	GPIO_InitE.Pull = GPIO_PULLDOWN;
	GPIO_InitE.Speed =  GPIO_SPEED_FREQ_HIGH;
	
	// Initialize GPIO PE0
	HAL_GPIO_Init(GPIOE, &GPIO_InitE);	
	
	// Enable external interrupt line 0
	HAL_NVIC_EnableIRQ(EXTI0_IRQn); 							//Enables a device specific interrupt in the NVIC interrupt controller.
	HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);				//Set priority to 0 -> highest
	HAL_NVIC_ClearPendingIRQ(EXTI0_IRQn);					//Clears the pending bit of an external interrupt.
	LIS3DSH_DataReadyInterruptConfig(&init_it); 	//Set LIS3DSH Interrupt configuration

}

/*
float calc_pitch_angle(void){
	return 90.0 - conv_to_deg(atan(accel.y/sqrt((accel.x*accel.x)+(accel.z*accel.z))));

}
*/