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

GPIO_InitTypeDef GPIO_InitDef_KP;


void KP_GPIO_Init(){
	
	int r0,r1,r2,r3;
	int c0,c1,c2,c3;
	int reading = 0;
	
	__HAL_RCC_GPIOE_CLK_ENABLE();
	
	//Columns
	GPIO_InitDef_KP.Pin 		= GPIO_PIN_10 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14;
	GPIO_InitDef_KP.Mode 	= GPIO_MODE_INPUT;	// Input Floating
	GPIO_InitDef_KP.Pull 	= GPIO_PULLUP;
	GPIO_InitDef_KP.Speed 	= GPIO_SPEED_MEDIUM;
	HAL_GPIO_Init(GPIOE, &GPIO_InitDef_KP);
	
	//Rows
	GPIO_InitDef_KP.Pin 		=  GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 |GPIO_PIN_11;
	GPIO_InitDef_KP.Mode 	= GPIO_MODE_OUTPUT_PP;	// combined the previous Mode(OUT) & OType(PP) (Output Push Pull)
	GPIO_InitDef_KP.Pull 	= GPIO_PULLUP;
	GPIO_InitDef_KP.Speed 	= GPIO_SPEED_MEDIUM;
	HAL_GPIO_Init(GPIOE, &GPIO_InitDef_KP);
	
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_11, GPIO_PIN_RESET);
	
	//reading columns initial states
	c0 = HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_12);
	c1 = HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_14);
	c2 = HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_10);
	c3 = HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_13);
	
	printf("c: %d%d%d%d\n",c0,c1,c2,c3);
	
	
	if((c0 && c1 && c2 && c3) != 1){ // a key has been pressed
		//check which key by getting output of rows.
		
		//Rows
		GPIO_InitDef_KP.Pin 		= GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 |GPIO_PIN_11;
		GPIO_InitDef_KP.Mode 	= GPIO_MODE_INPUT; // Input Floating
		GPIO_InitDef_KP.Pull 	= GPIO_PULLUP;
		GPIO_InitDef_KP.Speed 	= GPIO_SPEED_MEDIUM;
		HAL_GPIO_Init(GPIOE, &GPIO_InitDef_KP);
		
		//Columns
		GPIO_InitDef_KP.Pin 		= GPIO_PIN_10 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14;
		GPIO_InitDef_KP.Mode 	= GPIO_MODE_OUTPUT_PP; // combined the previous Mode(OUT) & OType(PP) (Output Push Pull)
		GPIO_InitDef_KP.Pull 	= GPIO_PULLUP;
		GPIO_InitDef_KP.Speed 	= GPIO_SPEED_MEDIUM;
		HAL_GPIO_Init(GPIOE, &GPIO_InitDef_KP);

		
		r0 = HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_11);
		r1 = HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_8);
		r2 = HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_7);
		r3 = HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_9);
		
		printf("pressed_r: %d%d%d%d\n",r0,r1,r2,r3);
	
	}else{
		//Rows
	GPIO_InitDef_KP.Pin 		= GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 |GPIO_PIN_11;
	GPIO_InitDef_KP.Mode 	= GPIO_MODE_INPUT; // Input Floating
	GPIO_InitDef_KP.Pull 	= GPIO_PULLUP;
	GPIO_InitDef_KP.Speed 	= GPIO_SPEED_MEDIUM;
	HAL_GPIO_Init(GPIOE, &GPIO_InitDef_KP);
	
	//Columns
	GPIO_InitDef_KP.Pin 		= GPIO_PIN_10 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14;
	GPIO_InitDef_KP.Mode 	= GPIO_MODE_OUTPUT_PP; // combined the previous Mode(OUT) & OType(PP) (Output Push Pull)
	GPIO_InitDef_KP.Pull 	= GPIO_PULLUP;
	GPIO_InitDef_KP.Speed 	= GPIO_SPEED_MEDIUM;
	HAL_GPIO_Init(GPIOE, &GPIO_InitDef_KP);

	
	r0 = HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_11);
	r1 = HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_8);
	r2 = HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_7);
	r3 = HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_9);
	
	printf("not_pressed_r: %d%d%d%d\n",r0,r1,r2,r3);
		
	}
	
}

int KP_Translation (int reading){
	
	int key;

	switch (reading){
										//row		//column
		
		//--------Row 1---------
		case 11101110:	//1110		1110
			key = 1;
		break;
		
		case 11101101:	//1110		1101
			key = 2;
		break;
		
		case 11101011:	//1110		1011
			key = 3;
		break;
		
		//--------Row 2---------
		
		case 11011110:	//1101		1110
			key = 4;
		break;
		
		case 11011101:	//1101		1101
			key = 5;
		break;
		
		case 11011011:	//1101		1110
			key = 6;
		break;
		
		//--------Row 3---------
		
		case 10111110:	//1011		1110
			key = 7;
		break;
		
		case 10111101:	//1011		1101
			key = 8;
		break;
		
		case 10111011:	//1011		1110
			key = 9;
		break;
		
		//--------Row 4---------
		
		case 01111101:	//0111		1101
			key = 0;
		break;
		
	}
	
	return key;

}

