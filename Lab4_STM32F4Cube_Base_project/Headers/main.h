/**
  ******************************************************************************
  * @file    HAL/HAL_TimeBase/Inc/main.h 
  * @author  MCD Application Team
  * @version   V1.2.4
  * @date      13-November-2015
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2015 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "lis3dsh.h"
#include "cmsis_os.h"

#define THREAD_WAIT 0  	 // Signal for thread to wait
#define THREAD_EXECUTE 2   // Signal for thread to execute
#define THREAD_TIMEOUT 100    // Thread timeout value in milliseconds
#define THREAD_DELAY 5

#define OVERHEAT 33.0f

/* Exported types ------------------------------------------------------------*/
extern int accel_ready;
extern uint8_t tim4_flag;
extern uint16_t tim4_ticks;
extern uint32_t TimingDelay;

extern void initializeLED_IO	(void);
extern void start_Thread_LED	(void);
extern void Thread_LED(void const *argument);
extern osThreadId tid_Thread_LED;

// Timer handler
extern TIM_HandleTypeDef handle_tim4;
extern TIM_ClockConfigTypeDef ClockConfig;
extern TIM_MasterConfigTypeDef sMasterConfig;
extern TIM_OC_InitTypeDef sConfigOC;


typedef enum  {TEMP_MODE, ACCEL_MODE, KEYPAD} state_type;
extern state_type state;


// Mutexes
extern osMutexId temperatureMutex;
extern osMutexId tiltAnglesMutex;

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
float filterResult(float* p);

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
