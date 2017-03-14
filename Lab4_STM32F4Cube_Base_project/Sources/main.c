/*******************************************************************************
  * @file    main.c
  * @author  Ashraf Suyyagh
	* @version V1.2.0
  * @date    17-January-2016
  * @brief   This file demonstrates flasing one LED at an interval of one second
	*          RTX based using CMSIS-RTOS 
  ******************************************************************************
  */

#include "stm32f4xx_hal.h"              // Keil::Device:STM32Cube HAL:Common
#include "cmsis_os.h"                   // ARM::CMSIS:RTOS:Keil RTX
#include "RTE_Components.h"             // Component selection
#include "main.h"
#include "tilt_detect.h"
#include "keypad.h"
#include "temp.h"
#include "adc.h"

extern void initializeLED_IO			(void);
extern void start_Thread_LED			(void);
extern void Thread_LED(void const *argument);
extern osThreadId tid_Thread_LED;


/* Private variables ---------------------------------------------------------*/

#define OVERHEAT 33.0f

float accelerometer_data[3];
float rolls[5];
float pitches[5];
int accel_ready;

int desiredPitch = 0;
int desiredRoll = 0;

const int length = 20;
const int order = 5;


//ADC_HandleTypeDef ADC1_Handle;


/* Private function prototypes -----------------------------------------------*/
void CLKStart (void);

//tets function for 7-segments display
void testGPIO_Digits(void);


/**
	These lines are mandatory to make CMSIS-RTOS RTX work with te new Cube HAL
*/
#ifdef RTE_CMSIS_RTOS_RTX
extern uint32_t os_time;

uint32_t HAL_GetTick(void) { 
  return os_time; 
}
#endif


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

/**
  * System Clock Configuration
  */
void SystemClock_Config(void) {
  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();

  /* The voltage scaling allows optimizing the power consumption when the
     device is clocked below the maximum system frequency (see datasheet). */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
     clocks dividers */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 |
                                RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);
}

/**
  * Main function
  */
int main (void) {

	/* Initialize all configured peripherals */
	printf("begin\n");
	
	osKernelInitialize();                     /* initialize CMSIS-RTOS          */

	HAL_Init();                               /* Initialize the HAL Library     */

	SystemClock_Config();                     /* Configure the System Clock     */
	
	CLKStart();
	
	/*Cnfigure ADC instance*/
	ConfigADC();
	
	/* Start ADC*/
	HAL_ADC_Start(&ADC1_Handle);
	
	/*Initialize GPIO*/
	MX_GPIO_Init();
	LED_GPIO_Init();
 
	// set sensor data acqusition frequency to 100hz
	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/100); // where argument is Number of ticks between two interrupts


	uint32_t tickStart;
	
	int LED_length = 300;
	float temperature_c = 0;
	
	int green, orange, red, blue;
	
	
	//Interact with keypad to get desired roll angle
	printf("Please enter roll angle on Keypad...\n");
	desiredRoll = keypad_input();
	printf ("Desired Roll = %d\n", desiredRoll);
	
	int j =0;
	
	//That's a way of a few seconds (2-3)
	while (j < 10000000){
		j+=1;
	}
	
	//Interact with keypad to get desired pitch angle
	printf("Please enter pitch angle on Keypad...\n");
	desiredPitch = keypad_input();
	printf ("Desired Pitch = %d\n", desiredPitch);
	

	/* User codes goes here*/
  initializeLED_IO();                       /* Initialize LED GPIO Buttons    */
  start_Thread_LED();                       /* Create LED thread              */
	/* User codes ends here*/
	
	while(1){
		temperature_c = doTempStuff();
	}
  
  osKernelStart();                          /* start thread execution         */
}

/**Start all necessary clocks*/
void CLKStart(void){
	
	//Enable System Clock
	__HAL_RCC_SYSCFG_CLK_ENABLE();
	// Enable Clock process of ADC1
	__HAL_RCC_ADC1_CLK_ENABLE();
	//Enable GPIOH (A, B, C , H) clock
	__HAL_RCC_GPIOH_CLK_ENABLE();

	__HAL_RCC_GPIOA_CLK_ENABLE();		//Enable GPIOA clock (used for button)
	__HAL_RCC_GPIOB_CLK_ENABLE(); 	//Enable GPIOB clock (used for pins)
	__HAL_RCC_GPIOD_CLK_ENABLE();		//Enable GPIOD clock (used for LED and pins)
	__HAL_RCC_GPIOE_CLK_ENABLE(); 	//Enable GPIOE clock (used for pins)
}

/**
  * @brief  This function handles accelerometer interrupt requests
  * @param  None
  * @retval None
  */
void EXTI0_IRQHandler(void){
	
	// Listen to pin 0
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
	
	// Flag for interrupt
	accel_ready = 1;

}
