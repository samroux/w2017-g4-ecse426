/*******************************************************************************
  * @file    Thread_LED.c
  * @author  Ashraf Suyyagh
	* @version V1.0.0
  * @date    17-January-2016
  * @brief   This file initializes one LED as an output, implements the LED thread 
  *					 which toggles and LED, and function which creates and starts the thread	
  ******************************************************************************
  */
	
#include "cmsis_os.h"                   // ARM::CMSIS:RTOS:Keil RTX
#include "stm32f4xx_hal.h"
#include "keypad.h"
#include "tilt_detect.h"
#include "main.h"

int desiredPitch = 0;
int desiredRoll = 0;

state_type state = TEMP_MODE;


void Thread_Keyboard (void const *argument);                 // thread function
osThreadId tid_Thread_Keyboard;                              // thread id
osThreadDef(Thread_Keyboard, osPriorityNormal, 1, 0);

/*----------------------------------------------------------------------------
 *      Create the thread within RTOS context
 *---------------------------------------------------------------------------*/
int start_Thread_Keyboard (void) {

  tid_Thread_Keyboard = osThreadCreate(osThread(Thread_Keyboard ), NULL); // Start Keyboard_Thread
  if (!tid_Thread_Keyboard) return(-1); 
  return(0);
}

 /*----------------------------------------------------------------------------
*      Thread  'Keyboard_Thread'
 *---------------------------------------------------------------------------*/
	void Thread_Keyboard (void const *argument) {
		while(1){
				//osDelay(1000);
				//HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
			
				if(keypad_interpret() == ENTER){
					//Get out of input mode, go to temp mode
					state = TEMP_MODE;
				}
				else if (keypad_interpret() != STANDBY ){
						state = KEYPAD;
						//Interact with keypad to get desired roll angle
						//need a way to stop pinting temp value on debug
					
						printf("Please enter roll angle on Keypad...\n");
						desiredRoll = keypad_input();
						printf ("Desired Roll = %d\n", desiredRoll);
						
						//That's a way of a few seconds (2-3)
						osDelay(1000);
						
						//Interact with keypad to get desired pitch angle
						printf("Please enter pitch angle on Keypad...\n");
						desiredPitch = keypad_input();
						printf ("Desired Pitch = %d\n", desiredPitch);
					
						state = TEMP_MODE;
				}
				else{
					//nothing is pressed, just wait in current state
					
				}
			
				
				
			}
	}
	
/*----------------------------------------------------------------------------*/ 
 
	
	