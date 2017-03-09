#ifndef TILT_DETECT_H
#define TILT_DETECT_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "stm32f4xx_hal.h"
#include "lis3dsh.h"
#include "math.h"

/* Private typedef -----------------------------------------------------------*/

typedef struct {
	float x; 
	float y; 
	float z;
} accelerometer_axes;

#define PI 3.14159265358
#define ACC11 4.3252
#define ACC12 0.5604
#define ACC13 0.1812
#define ACC10 0.000333
#define ACC21 -0.03287
#define ACC22 4.4725
#define ACC23 0.23041
#define ACC20 0.000006
#define ACC31 0.06875
#define ACC32 0.2117
#define ACC33 4.1983
#define ACC30 0.0005 
#define conv_to_deg(x) (180.0*x/PI)

//Updates accelerometer values
void update_accel(float Ax, float Ay, float Az);

//Returns pitch angle
float calc_pitch(void);

//Returns roll angle
float calc_roll(void);


//Initialize accelerometer
void init_accelerometer(void);



#endif
