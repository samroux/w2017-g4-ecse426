
#ifndef __temp_H
#define __temp_H
#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f4xx_hal.h"
#include "lis3dsh.h"

float doTempStuff(void);

void stopAlarm(void);

float ConvertTemp(int ADCValue);

void displayTemperature(float temp, int mode);

	 
#ifdef __cplusplus
}
#endif
#endif /*__ pinoutConfig_H */
