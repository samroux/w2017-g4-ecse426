
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __keypad_H
#define __keypad_H
#ifdef __cplusplus
 extern "C" {
#endif
	 
	 
extern GPIO_InitTypeDef GPIO_InitDef_KP;
	 
#define NO_KEY_PRESSED -1
#define DELETE 10
#define ENTER 11
#define STANDBY 100
	 
	 
/* USER CODE BEGIN Prototypes */
	 
void KP_GPIO_Init(void);
int keypad_interpret(void);
int keypad_input(void);

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ pinoutConfig_H */

/**
  * @}
  */

/**
  * @}
  */

