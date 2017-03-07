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
#define ACC11 0.9523
#define ACC12 -0.0019
#define ACC13 -0.0079
#define ACC10 -0.0297
#define ACC21 -0.0397
#define ACC22 0.9896
#define ACC23 -0.0105
#define ACC20 0.0070
#define ACC31 -0.0047
#define ACC32 -0.0024
#define ACC33 1.0087
#define ACC30 -0.0328 
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
