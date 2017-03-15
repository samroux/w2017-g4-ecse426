/**
  ******************************************************************************
  * File Name          : ADC.c
  * Description        : This file provides code for the configuration
  *                      of the ADC instances.
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
#include "adc.h"
#include "main.h"

osMutexId temperatureMutex;
const void* temperatureMutexPtr;

ADC_ChannelConfTypeDef ADC_Channel;


void ConfigADC(void){
		// Populate the struct ADC Handle

	// Instance
	ADC1_Handle.Instance = ADC1;

	// Init struct (From DOC19 section 4 )
	ADC1_Handle.Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV4; // Clock Prescaler at 168/4Mhz
	ADC1_Handle.Init.Resolution = ADC_RESOLUTION_12B; 							// Biggest resolution for best accuracy (12-bit)
	ADC1_Handle.Init.DataAlign = ADC_DATAALIGN_RIGHT;								// Align the data to the right
	ADC1_Handle.Init.ScanConvMode = DISABLE;												// One channel only so no scan 
	ADC1_Handle.Init.EOCSelection = DISABLE;												// Don't use the EOC flag
	ADC1_Handle.Init.DMAContinuousRequests = DISABLE;								// Not using DMA for now
	
	ADC1_Handle.Init.ContinuousConvMode = ENABLE;										// Continuous conversion for stream of data
	ADC1_Handle.Init.NbrOfConversion = 1;														// Just 1 conversion for continuous is ok
	
	ADC1_Handle.Init.DiscontinuousConvMode = DISABLE;								// Want to be continuous conversion
	ADC1_Handle.Init.NbrOfDiscConversion = 0;												// No discontinuous conversions
	
	ADC1_Handle.Init.ExternalTrigConv= ADC_SOFTWARE_START;					// Disable external edges
	ADC1_Handle.Init.ExternalTrigConvEdge = ADC_SOFTWARE_START;			// Disable external events


	// Set Init
	HAL_ADC_Init(&ADC1_Handle);

	// Channel config
	ADC_Channel.Channel = ADC_CHANNEL_16;														// Set to CH16 (Temperature sensor)
	ADC_Channel.SamplingTime = ADC_SAMPLETIME_480CYCLES; 						// Must be bigger than 15Cycles to work correctly (Ref value, don't know exactly why...)
	ADC_Channel.Rank = 1;																						// Rank 1 (in group sequencer) & no offset
	ADC_Channel.Offset = 0;

	// Set Channel

	HAL_ADC_ConfigChannel(&ADC1_Handle, &ADC_Channel);
	
	HAL_ADC_Start(&ADC1_Handle);
	
	// Initialize temperature sensor mutex
	temperatureMutex = osMutexCreate(temperatureMutexPtr);
	
}



/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
