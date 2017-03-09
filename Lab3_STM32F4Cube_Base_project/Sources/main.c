/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program subroutine
	* Author						 : Ashraf Suyyagh
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

/* Private variables ---------------------------------------------------------*/
float accelerometer_data[3];	
TIM_HandleTypeDef tim3_handle;


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

uint32_t filterResult(uint32_t* p) {//FIR filter for noise reduction 
	uint32_t res = 0;
	
	for(int i = 4; i >= 0; i--) {
		res += *(p+i)*(coeff.b[4-i]);
	}
	return res;
}

//Input capture callback
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	
	//Compilation warning
  __IO uint32_t tmpreg = 0x00;
  UNUSED(tmpreg); 
	
	// If callback is associated with the given pin, then read the data
	if(GPIO_Pin == GPIO_PIN_0) {
		LIS3DSH_ReadACC(accelerometer_data);
	}	
}	

//Timer callback
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef* htim) {
	//Prevent comilation warning
  UNUSED(htim);
	
}

/* Might not be needed, dont really know what it does tbh
void init_TIM3(void) {
	TIM_Base_InitTypeDef initTIM3;
	
	// Enable clock for TIM3 
	__HAL_RCC_TIM3_CLK_ENABLE();
	
	// Desired Rate = ClockFrequency / (prescaler * period)
	// Rate = 1000Hz, frequency = 42MHz																		 TODO: FIX to 500 HZ
	// need to setup period and prescaler
	// set rate to 500Hz
	
	// Initialize timer 3 initialization struct 
	initTIM3.Period = 42;			 								// Period is in MHz
	initTIM3.Prescaler = 2000;
	initTIM3.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	initTIM3.CounterMode = TIM_COUNTERMODE_UP;
	
	// Initialize timer 3 handle struct
	tim3_handle.Instance = TIM3;
	tim3_handle.Init = initTIM3;
	tim3_handle.Channel = HAL_TIM_ACTIVE_CHANNEL_CLEARED;
	tim3_handle.Lock = HAL_UNLOCKED;
	tim3_handle.State = HAL_TIM_STATE_READY;

	// Initialize timer 3 handle and enable interrupts
	HAL_TIM_Base_MspInit(&tim3_handle);
	HAL_TIM_Base_Init(&tim3_handle);
	HAL_TIM_Base_Start_IT(&tim3_handle);
		
	// Configure NVIC 
	HAL_NVIC_EnableIRQ(TIM3_IRQn);
	HAL_NVIC_SetPriority(TIM3_IRQn, 0,0);
	//HAL_NVIC_ClearPendingIRQ(TIM3_IRQn);
	
}
*/

int main(void)
{	
  /* MCU Configuration----------------------------------------------------------*/

  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();
	
  /* Initialize all configured peripherals */
	printf("begin");
	
	//init_TIM3();
	
	printf("Initialized TIM3\n");
	
	init_accelerometer();
	
	printf("Initialized accelerometer\n");
	
	accel_ready = 0;
	
	while (1){
		if (accel_ready == 1){
			accel_ready = 0;
			
			printf("%f, %f, %f\n", accelerometer_data[0], accelerometer_data[1], accelerometer_data[2]);	
			
			update_accel(accelerometer_data[0], accelerometer_data[1], accelerometer_data[2]);
			
			printf("%f, %f, %f\n", accelerometer_data[0], accelerometer_data[1], accelerometer_data[2]);	
			
			float roll = calc_roll();
			float pitch = calc_pitch();
			
			printf("Roll: %f, pitch: %f\n", roll, pitch); 
		}			
	}
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

