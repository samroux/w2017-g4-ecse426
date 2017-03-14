/**
  ******************************************************************************
  * @file    Pulse Width Modulation Demo
  * @author  Lee Dyche - Tecsploit.com
  * @version V1.0.0
  * @date    18/03/2014
  * @brief   3 LEDS go light to dim, and the GPIO pin PB6 has its voltage ramped up and down
  * and could be used to control the brightness of a LED / power of a motor
  ******************************************************************************
  * @attention
  * This is program is provided as is with no warranty!!
  ******************************************************************************
  */
 
#include "stm32f4xx_hal.h"
 
void Delay(__IO uint32_t nCount);
 
int main(void)
{
    /*Structures used in the configuration*/
  TIM_HandleTypeDef  TIM_TimeBaseStructure;
  TIM_OC_InitTypeDef  TIM_OCInitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
 
  /* Enable TIM4 Clock */
  //RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	__TIM4_CLK_ENABLE();
	
 
  /* Enable GPIOD Pins that are used for on board LED's */
  //RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	__HAL_RCC_GPIOD_CLK_ENABLE();
 
  //Enabled GPIOB we are going to use PB6 which is linked to TIM4_CH1 according to the
  //documentation
  //RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	__HAL_RCC_GPIOD_CLK_ENABLE();
 
  /* Initialise  pins 13, 14 and 15 D - relating to on board LED's*/
  GPIO_InitStructure.Pin = GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
  GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStructure.Pull = GPIO_NOPULL;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
 
  //initalise pin 6 B - relating to timer 4 channel 1
  GPIO_InitStructure.Pin = GPIO_PIN_6;
  GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_LOW;
  //GPIO_InitStructure.OType = GPIO_OType_PP;
  //GPIO_InitStructure.PuPd = GPIO_PuPd_UP ;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
 
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_TIM4);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource13, GPIO_AF_TIM4);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_TIM4);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_TIM4);
 
  /* Setup PWM */
  uint16_t PrescalerValue = 0;
  PrescalerValue = (uint16_t) ((SystemCoreClock /2) / 21000000) - 1;
 
  /* Setup timer defaults */
  TIM_TimeBaseStructure.TIM_Period = 665;
  TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
 
  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
 
  /* Configure timer for PWM - channel 1*/
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 0;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
 
  //notice the number 1 in TIM_OC1Init
  TIM_OC1Init(TIM4, &TIM_OCInitStructure);
  TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
 
  /* Configure timer for PWM - channel 2 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 0;
  TIM_OC2Init(TIM4, &TIM_OCInitStructure);
  TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);
 
  /* Configure timer for PWM - channel 3*/
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 0;
  TIM_OC3Init(TIM4, &TIM_OCInitStructure);
  TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);
 
  /* Configure timer for PWM - channel 4 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 0;
  TIM_OC4Init(TIM4, &TIM_OCInitStructure);
  TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);
 
  TIM_ARRPreloadConfig(TIM4, ENABLE);
 
  /* Start timer*/
  TIM_Cmd(TIM4, ENABLE);
 
  int power = 0;
  //for this timer configuration a CCR (power) value of 700 yields approximately 3V on the pin (and is the max)
  while(1)  //Loop forever
  {
      //The power calculations here are only for demonstration and are very rough!!!
      //I've devided teh power of the on board LED so you can see the differnt brightnesses
      TIM4->CCR1 = power; //CCR1 controls channel 1
      TIM4->CCR2 = power /2;//CCR2 controls channel 1
      TIM4->CCR3 = power /8;//CCR3 controls channel 1
      TIM4->CCR4 = power /16;//CCR4 controls channel 1
 
      Delay(80000);
      power++;
      if(power > 699){
          power = 0;
          Delay(900000);
      }
    }
}
 
/**
  * @brief  Delay Function.
  * @param  nCount:specifies the Delay time length.
  * @retval None
  */
void Delay(__IO uint32_t nCount)
{
  while(nCount--)
  {
  }
}