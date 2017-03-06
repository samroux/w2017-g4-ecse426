/**
  ******************************************************************************
  * File Name          : gpio.c
  * Description        : This file provides code for the configuration
  *                      of all used GPIO pins.
  ******************************************************************************
  *
  * COPYRIGHT(c) 2017 STMicroelectronics
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

/* Includes ------------------------------------------------------------------*/
#include "gpio.h"
/* USER CODE BEGIN 0 */


/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
     PXX   ------> YYYYYY
		 
		 Continue your GPIO configuration information as abow.

*/

/**
GPIOE:
	4 -> A
	5 -> B
	6 -> C
	7 -> D
	8 -> E
	9 -> F
	10 -> G
	11 -> Decimal-Point
	12 -> 10
	13 -> 1
	14 -> dec
	15 -> degree unit

*/



void MX_GPIO_Init(){
	GPIO_InitDef.Pin 		= GPIO_PIN_4 | GPIO_PIN_5 |GPIO_PIN_6 |GPIO_PIN_7 |GPIO_PIN_8 |GPIO_PIN_9 |GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12| GPIO_PIN_13|GPIO_PIN_14 | GPIO_PIN_15;
	GPIO_InitDef.Mode 	= GPIO_MODE_OUTPUT_PP; // combined the previous Mode(OUT) & OType(PP)
	GPIO_InitDef.Pull 	= GPIO_PULLUP;
	GPIO_InitDef.Speed 	= GPIO_SPEED_HIGH; // no more HZ value.
	HAL_GPIO_Init(GPIOE, &GPIO_InitDef);
}

void LED_GPIO_Init(){
	GPIO_InitDef_LED.Pin 		= GPIO_PIN_12| GPIO_PIN_13|GPIO_PIN_14 | GPIO_PIN_15;
	GPIO_InitDef_LED.Mode 	= GPIO_MODE_OUTPUT_PP; // combined the previous Mode(OUT) & OType(PP)
	GPIO_InitDef_LED.Pull 	= GPIO_NOPULL;
	GPIO_InitDef_LED.Speed 	= GPIO_SPEED_HIGH; // no more HZ value.
	HAL_GPIO_Init(GPIOD, &GPIO_InitDef_LED);
}


void oneDigitDisplay(int num, uint16_t digit){ // E4-E10(A-G); E11(DP); E12(x100),E13(x10),E14(1), E15 (Degree Unit)

	switch(digit){
		case TEN:
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_12,GPIO_PIN_SET);  	// 10
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_13,GPIO_PIN_RESET);	//1
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_11,GPIO_PIN_RESET); //Decimal_point
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_14,GPIO_PIN_RESET);	//dec
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_15,GPIO_PIN_RESET);	//Degree_unit
		break;

		case ONE:
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_12,GPIO_PIN_RESET); // 10
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_13,GPIO_PIN_SET);	//1
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_11,GPIO_PIN_SET); //Decimal_point
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_14,GPIO_PIN_RESET);	//dec
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_15,GPIO_PIN_RESET);	//Degree_unit
		break;

		case DEC:
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_12,GPIO_PIN_RESET); // 10
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_13,GPIO_PIN_RESET);	//1
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_11,GPIO_PIN_RESET); //Decimal_point
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_14,GPIO_PIN_SET);	//dec
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_15,GPIO_PIN_RESET);	//Degree_unit
		break;

		case DEG:
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_12,GPIO_PIN_RESET); // 10
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_13,GPIO_PIN_RESET);	//1
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_11,GPIO_PIN_RESET); //Decimal_point
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_14,GPIO_PIN_RESET);	//dec
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_15,GPIO_PIN_SET);	//Degree_unit
		break;

	}
	switch(num){
		default:
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_4,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_5,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_6,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_7,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_8,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_9,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_10,GPIO_PIN_RESET);

		case 0: //A,B,C,D,E,F	// G
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_4,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_5,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_6,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_7,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_8,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_9,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_10,GPIO_PIN_RESET);
		break;

		case 1: // B,C 				// A,D,E,F,G
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_4,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_5,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_6,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_7,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_8,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_9,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_10,GPIO_PIN_RESET);
		break;

		case 2: // A,B,E,D,G 	// C,F
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_4,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_5,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_6,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_7,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_8,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_9,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_10,GPIO_PIN_SET);
		break;

		case 3: // A,B,C,D,G 	// E,F
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_4,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_5,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_6,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_7,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_8,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_9,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_10,GPIO_PIN_SET);
		break;

		case 4: // F,G,B,C		// A,D,E
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_4,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_5,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_6,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_7,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_8,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_9,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_10,GPIO_PIN_SET);
		break;

		case 5: // A,F,G,C,D	// B,E
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_4,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_5,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_6,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_7,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_8,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_9,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_10,GPIO_PIN_SET);
		break;

		case 6: // A,F,G,C,E,D // B
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_4,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_5,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_6,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_7,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_8,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_9,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_10,GPIO_PIN_SET);
		break;

		case 7: // A,B,C			//D,E,F,G
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_4,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_5,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_6,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_7,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_8,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_9,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_10,GPIO_PIN_RESET);
		break;

		case 8: // A,B,C,D,E,F,G //
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_4,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_5,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_6,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_7,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_8,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_9,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_10,GPIO_PIN_SET);
		break;

		case 9: // A,B,F,G,C,D // E
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_4,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_5,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_6,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_7,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_8,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_9,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_10,GPIO_PIN_SET);
		break;

		case 100: // degree C : ABGF//CDE
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_4,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_5,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_6,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_7,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_8,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_9,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_10,GPIO_PIN_RESET);
		break;
		
		case 101: // degree F : ABGF//CDE
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_4,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_5,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_6,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_7,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_8,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_9,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_10,GPIO_PIN_SET);
		break;
	}
}


/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
