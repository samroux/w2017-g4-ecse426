/**
  ******************************************************************************
  * File Name          : main.c
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
#include "timer.h"

/* Private variables ---------------------------------------------------------*/
float accelerometer_data[3];
float rolls[5];
float pitches[5];
float desiredPitch = 55;
float desiredRoll = 33;


/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config	(void);

typedef struct {
	float b[5];
}FIR_coeff;

FIR_coeff coeff = {//FIR Filter coefficients
	.b[0] = 0.1,
	.b[1] = 0.15,
	.b[2] = 0.5,
	.b[3] = 0.15,
	.b[4] = 0.1
};

float filterResult(float* p) {//FIR filter for noise reduction 
	float res = 0;
	
	for(int i = 4; i >= 0; i--) {
		res += *(p+i)*(coeff.b[4-i]);
	}
	return res;
}

//Input capture callback
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	
	//Prevent Compilation warning
  __IO uint32_t tmpreg = 0x00;
  UNUSED(tmpreg); 
	
	// If callback is associated with the given pin, then read the data
	if(GPIO_Pin == GPIO_PIN_0) {
		LIS3DSH_ReadACC(accelerometer_data);
	}	
}	

int main(void)
{	
  /* MCU Configuration----------------------------------------------------------*/

  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();
	
  /* Initialize all configured peripherals */
	printf("begin\n");
	
	/*
	Timer_Init();
	PWM_Init();
	//init_TIM4();
	
	printf("Initialized tim4\n");
	
	
	init_accelerometer();
	
	printf("Initialized accelerometer\n");
	
	accel_ready = 0;
	*/
	
	/*
	// LED stuff
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitTypeDef GPIO_InitDef;
  GPIO_InitTypeDef GPIO_InitDef_LED;
	
	__HAL_RCC_GPIOD_CLK_ENABLE();
	
	GPIO_InitDef_LED.Pin 		= GPIO_PIN_12| GPIO_PIN_13|GPIO_PIN_14 | GPIO_PIN_15;
	GPIO_InitDef_LED.Mode 	= GPIO_MODE_AF_PP;
	GPIO_InitDef_LED.Pull 	= GPIO_PULLDOWN;
	GPIO_InitDef_LED.Speed 	= GPIO_SPEED_FREQ_MEDIUM; 
  HAL_GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	GPIO_InitDef_LED.Pin 		= GPIO_PIN_13;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	GPIO_InitDef_LED.Pin 		= GPIO_PIN_14;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	GPIO_InitDef_LED.Pin 		= GPIO_PIN_15;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStructure);
	*/
	
	initTimer();
	
	/*
	
	while (1){
		if (accel_ready == 1){
			accel_ready = 0;
			
			//printf("%f, %f, %f\n", accelerometer_data[0], accelerometer_data[1], accelerometer_data[2]);	
			
			update_accel(accelerometer_data[0], accelerometer_data[1], accelerometer_data[2]);
			
			//Uncomment to see uncalibrated version
			//update_accel2(accelerometer_data[0], accelerometer_data[1], accelerometer_data[2]);
			
			rolls[0] = calc_roll();
			pitches[0] = calc_pitch();
			
			float roll = filterResult(&rolls[0]);
			float pitch = filterResult(&pitches[0]);
			
			controlRollLED(desiredRoll, roll);
			controlPitchLED(desiredPitch, pitch);
			
			for(int i = 3; i > -1; i--){
				rolls[i+1] = rolls[i];
				pitches[i+1] = pitches[i];
			}
			printf("Roll: %f, pitch: %f, no filt roll: %f, no filt pitch: %f\n", roll, pitch, rolls[0], pitches[0]); 
		}			
	}
	*/
}

/** System Clock Configuration*/
void SystemClock_Config(void){

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

  __PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  RCC_OscInitStruct.OscillatorType 	= RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState 			 	= RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState 		= RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource 	= RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM 				= 8;
  RCC_OscInitStruct.PLL.PLLN 				= 336;
  RCC_OscInitStruct.PLL.PLLP 				= RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ 				= 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK){Error_Handler(RCC_CONFIG_FAIL);};

  RCC_ClkInitStruct.ClockType 			= RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource 		= RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider 	= RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider 	= RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider 	= RCC_HCLK_DIV2;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5)!= HAL_OK){Error_Handler(RCC_CONFIG_FAIL);};
	
	/*Configures SysTick to provide 1ms interval interrupts. SysTick is already 
	  configured inside HAL_Init, I don't kow why the CubeMX generates this call again*/
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

	/* This function sets the source clock for the internal SysTick Timer to be the maximum,
	   in our case, HCLK is now 168MHz*/
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line){
}
#endif

