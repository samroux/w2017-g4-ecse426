/**
  ******************************************************************************
  * @file    keypad.c
  * @author  Chris Di Betta and Samuel Roux
  ****************************************************************************** 
*/
#include <stdio.h>
#include <string.h>
#include "stm32f4xx.h"                  // Device header
#include "lis3dsh.h"
#include "keypad.h"


int KP_Translation (reading){
	
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

}
	