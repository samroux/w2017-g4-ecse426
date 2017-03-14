#include "stm32f4xx_hal.h"              // Keil::Device:STM32Cube HAL:Common
#include "temp.h"
#include "gpio.h"

int counter = 0;
float display_temp;


ADC_HandleTypeDef ADC1_Handle;



float doTempStuff( ){
	float temperature_c;
	float ADCValue;
	int displayMode = 0;
	
	if (HAL_ADC_PollForConversion (&ADC1_Handle, 100) == HAL_OK){
			
			ADCValue = HAL_ADC_GetValue(&ADC1_Handle);
			printf("ADCvalue: %.6f\n",ADCValue);
			
			temperature_c = ConvertTemp(ADCValue); 
			printf("Temperature_c: %.6f\n",temperature_c);
			
		}
	
	if (counter ==0){
		display_temp = temperature_c;
	}
		
		displayMode = HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0); //Read Blue Button on Board. -> Output 1 if pressed (F). Else, 0 = C
		//printf("DisplayMode : %d \n",displayMode);
		
		counter +=1;
		
		if(counter == 100){
			display_temp = temperature_c;
			displayTemperature (temperature_c, displayMode); //displayTemperature on 4 digit 7-segments display
			counter = 0;
		}
		else{
			displayTemperature (display_temp, displayMode); //displayTemperature on 4 digit 7-segments display
		}
		
		printf("DisplayTemp: %.6f\n",display_temp);
		
		
		return temperature_c;
}	
	

/**
Function to reset all LED set when alarm was turned on
*/
void stopAlarm (){
	HAL_GPIO_WritePin(GPIOD, LED_GREEN,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, LED_ORANGE,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, LED_RED,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, LED_BLUE,GPIO_PIN_RESET);
}
	
/*
Celsius to farhenheit conversion
rvalue = temp_f
*/
float convertToF (float temp_c){

	float temp_f = temp_c * 1.8 + 32;
	
	return temp_f;
}

/**
Convert from ADC input to degree c
*/
float ConvertTemp(int ADCValue){
	
	//Conversion found at STM32F47xx Datasheet section 5.3.22
	//tValueC = (Vsense - V25) / 0.0025 + 25
	
	float tValueC;
	float V25 = 0.760f; //reference voltage value at 25 deg. C
	float avgSlope = 0.0025; // avgSlope = 2.5 mV/C
	float refTemp = 25.0;   // reference temperature in C (room temp)
	float offset = 0;
	
	tValueC = ADCValue;
	tValueC *= 3000; //reference voltage
	tValueC /= 0xfff; //Dividing by because using 12-bit format (0xfff). Reading in mV now. (4095)
	tValueC /= 1000; // Read in Volts
	tValueC -= V25;	//substract ref. voltage
	tValueC /= avgSlope; // multiply by avgSlope
	tValueC += refTemp; // add ref temp (25 deg. C)
	tValueC += offset;
	
	//printf("Divided 4096: %.6f",tValueC);
	
	return tValueC;
}

/**
Display temparature on 4-digits 7 segments display
*/
void displayTemperature(float temp, int mode){
	
/**
Starting all necessary clocks	
*/	
	//mode: 0 = Celsius, 1 = Fahrenheit
	
	if(mode == 1){
		temp = convertToF(temp);
		printf("temp_f: %.6f\n",temp);
	}

	
	int ten, one, dec = 0;
	
	int delay = 3;
	
	int tCal = temp * 10; //multiply by 10 to remove decimal place
	
	dec = tCal % 10;
	tCal = tCal / 10;
	
	one = tCal % 10;
	tCal = tCal / 10;
	
	ten = tCal % 10;
	
	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000); // increase frequency for better rendering
	
	oneDigitDisplay (ten, TEN); //digit at position 10^1
	HAL_Delay(delay);  //delay here is to make sure we see digit long enough
	
	oneDigitDisplay (one, ONE); //digit at position 10^0
	HAL_Delay(delay);
	
	oneDigitDisplay (dec, DEC); //digit at position 10^-1
	HAL_Delay(delay);
	
	if(mode == 0){ //Celsius
		oneDigitDisplay (100, DEG);
	}
	else{ // farheneith
		oneDigitDisplay (101, DEG);
	}
	HAL_Delay(delay);
	
	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/100); // restore frequency

}
