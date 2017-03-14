/**
  ******************************************************************************
  * @file		keypad.c
  * @author	Chris Di Betta and Samuel Roux
	* @date		2017/03/01
  ****************************************************************************** 
*/
#include <stdio.h>
#include <string.h>
#include "stm32f4xx.h"                  // Device header
#include "lis3dsh.h"
#include "keypad.h"

#define NO_KEY_PRESSED -1
#define DELETE 10
#define ENTER 11
#define STANDBY 100

GPIO_InitTypeDef GPIO_InitDef_KP;

int check_column (void);
int check_row (void);
int KP_Translation(int column, int row);

/**
	* @brief function interpretting and catching keypad input
	*	@retval int angle requested
*/
int keypad_input (){
	
	int angle, req_angle, i, temp;
	int digits, pressed;
	
	digits = 3; //max number of digits that can be entered to request angle
	pressed = 0;
	angle = 0;
	req_angle = 0;
	i = 0;
	
	
	while(i<digits){
		temp = keypad_interpret();
		
		if( temp !=STANDBY && temp != ENTER && temp !=DELETE && pressed == 0){
			//a key is pressed (no longer in standby mode)
			//printf ("key: %d\n", temp);
			//printf("Angle (Init): %d\n",angle);
			
			pressed = 1;
			angle += temp;
			//printf("Angle (Add): %d\n",angle);
			angle *= 10;	//shift left
			//printf("Angle (shift): %d\n",angle);
			i += 1;
			printf ("Angle (number): %d\n", angle/10);
		}
		
		if(temp == DELETE && pressed == 0){
				pressed = 1;
				angle /= 100;				//since integer division always give floor, it will remove the last digit
				angle *= 10;
				if(i>0){
					i -= 1;		//since one digit is removed, reduce value of i
				}else{
					i = 0;
				}
				printf ("Angle(Delete): %d\n", angle/10);
		}
		
		if ((temp == ENTER && pressed == 0) || i == 3){
			pressed = 1;
			req_angle = angle/10;
			angle = 0;	//reset angle
			i = 0;			//reset i
			//printf ("Requested Angle is: %d\n", req_angle);
			pressed = 0;
			break;
		}
		
		if(temp == STANDBY  && pressed == 1){
			//back to stanby mode (nothing pressed)
			pressed = 0;
		}
		

	}
		
	return req_angle;
}

/**
	* @brief checking and interpretting key entries
	* @retval key
	*/
int keypad_interpret (){
	
	int column = check_column();
	int row = check_row();
	
	//printf("c: %d r: %d\n", column, row);
	
	int key = KP_Translation (column, row);
	
	//printf ("key: %d\n",key);
	
	return key;
}

/**
	* @brief function doing mapping between row and column and actual key
	*	@retval int key_id
*/
int KP_Translation (int column, int row){
	

	
	int key_code = row*10 + column;
	
	int key;

	switch (key_code){
										//row		//column
		
		//--------Row 1---------
		case 11:	//1110		1100
			key = 1;
		break;
		
		case 12:	//1110		1010
			key = 2;
		break;
		
		case 13:	//1110		0110
			key = 3;
		break;
		
		//--------Row 2---------
		
		case 21:	//1101		1100
			key = 4;
		break;
		
		case 22:	//1101		1010
			key = 5;
		break;
		
		case 23:	//1101		0110
			key = 6;
		break;
		
		//--------Row 3---------
		
		case 31:	//1011		1100
			key = 7;
		break;
		
		case 32:	//1011		1010
			key = 8;
		break;
		
		case 33:	//1011		0110
			key = 9;
		break;
		
		//--------Row 4---------
		
		case 41:	//0111		1100
			key = 10; //DELETE (*)
		break;
		
		case 42:	//0111		1010
			key = 0;
		break;
		
		case 43:	//0111		0110
			key = 11;	//ENTER (#)
		break;
		
		default:
			key = STANDBY;
		
	}
	
	return key;

}

/** @brief function checking on which column is the pressed key
		@retval int column number
*/
int check_column(){
	
	
	//Columns
	GPIO_InitDef_KP.Pin 		= GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14;
	GPIO_InitDef_KP.Mode 	= GPIO_MODE_INPUT;	// Input Floating
	GPIO_InitDef_KP.Pull 	= GPIO_PULLUP;
	GPIO_InitDef_KP.Speed 	= GPIO_SPEED_MEDIUM;
	HAL_GPIO_Init(GPIOB, &GPIO_InitDef_KP);
	
	//Set rows as output to make sure there's no floating values
	GPIO_InitDef_KP.Pin 		=  GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 |GPIO_PIN_12;
	GPIO_InitDef_KP.Mode 	= GPIO_MODE_OUTPUT_PP;	// combined the previous Mode(OUT) & OType(PP) (Output Push Pull)
	GPIO_InitDef_KP.Pull 	= GPIO_PULLUP;
	GPIO_InitDef_KP.Speed 	= GPIO_SPEED_MEDIUM;
	HAL_GPIO_Init(GPIOD, &GPIO_InitDef_KP);
	
	//set rows to 1
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_10, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET);
	
	//read columns
	if (!HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_11)){ //column 3
		return 3;
	}
	if (!HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_12)){ //column 2
		return 2;
	}
	if (!HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_13)){ //column 1
		return 1;
	}
	else {
		return NO_KEY_PRESSED;
	}
}

/** 
	* @brief function checking on which row is the pressed key
	*	@retval int row number
*/
int check_row(){
	
		//Rows
		GPIO_InitDef_KP.Pin 		= GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 |GPIO_PIN_12;
		GPIO_InitDef_KP.Mode 	= GPIO_MODE_INPUT; // Input Floating
		GPIO_InitDef_KP.Pull 	= GPIO_PULLUP;
		GPIO_InitDef_KP.Speed 	= GPIO_SPEED_MEDIUM;
		HAL_GPIO_Init(GPIOD, &GPIO_InitDef_KP);
		
		//Columns
		GPIO_InitDef_KP.Pin 		= GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14;
		GPIO_InitDef_KP.Mode 	= GPIO_MODE_OUTPUT_PP; // combined the previous Mode(OUT) & OType(PP) (Output Push Pull)
		GPIO_InitDef_KP.Pull 	= GPIO_PULLUP;
		GPIO_InitDef_KP.Speed 	= GPIO_SPEED_MEDIUM;
		HAL_GPIO_Init(GPIOB, &GPIO_InitDef_KP);
		
		//set columns to 1
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);
		
		//read rows
		if(!HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_9)){
			return 4;
		}
		if(!HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_10)){
			return 3;
		}
		if(!HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_11)){
			return 2;
		}
		if(!HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_12)){
			return 1;
		}
		else{
			return NO_KEY_PRESSED;
		}
}



