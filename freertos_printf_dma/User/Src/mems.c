/******************************************************************************
 * Developed by: gslm
 *
 * Copyright 2021
 *
 * All rights are reserved. Reproduction in whole or part is prohibited without
 * the written consent of the copyright owner.
 *****************************************************************************/


/*******************************************************************************
    Includes.
*******************************************************************************/
#include <stdio.h>
#include "mpu6050.h"
#include "FreeRTOS.h"
#include "task.h"
#include "mems.h"
#include "vcom.h"


/*******************************************************************************
    Private constants and types.
*******************************************************************************/
typedef enum
{
  MEMS_STATE_INIT = 0U,
  MEMS_STATE_READ,
  MEMS_STATE_WAIT,
  MEMS_STATE_FAIL
}mems_states_t;


/*******************************************************************************
    Private variables.
*******************************************************************************/

static MPU6050_t		g_MPU6050;
static mems_states_t	g_mems_states;


/*******************************************************************************
    Public function implementations.
*******************************************************************************/

/**
* @brief Reads MPU6050 data.
* @param argument: Not used
* @retval None
*/
void tsken_mpu6050(void *argument)
{
	char mpu_buf[9][8];

	for(;;)
	{
		switch(g_mems_states)
		{
			case MEMS_STATE_INIT:

				/* initializes MPU6050 module*/
				if(MPU6050_Init(&hi2c1) == 0)	g_mems_states = MEMS_STATE_READ;
				else							g_mems_states = MEMS_STATE_FAIL;
				break;

			case MEMS_STATE_READ:

				MPU6050_Read_All(&hi2c1, &g_MPU6050);

				snprintf(mpu_buf[0], sizeof(mpu_buf[0]), "%3.3f", g_MPU6050.Ax);
				snprintf(mpu_buf[1], sizeof(mpu_buf[1]), "%3.3f", g_MPU6050.Ay);
				snprintf(mpu_buf[2], sizeof(mpu_buf[2]), "%3.3f", g_MPU6050.Az);

				snprintf(mpu_buf[3], sizeof(mpu_buf[3]), "%3.3f", g_MPU6050.Gx);
				snprintf(mpu_buf[4], sizeof(mpu_buf[4]), "%3.3f", g_MPU6050.Gy);
				snprintf(mpu_buf[5], sizeof(mpu_buf[5]), "%3.3f", g_MPU6050.Gz);

				snprintf(mpu_buf[6], sizeof(mpu_buf[6]), "%3.3f",
						g_MPU6050.KalmanAngleX);
				snprintf(mpu_buf[7], sizeof(mpu_buf[7]), "%3.3f",
						g_MPU6050.KalmanAngleY);

				snprintf(mpu_buf[8], sizeof(mpu_buf[8]), "%3.3f",
						g_MPU6050.Temperature);

				vcom_printf("\033[2J\e[31mAX %s\e[32m\tAY %s\e[33m\tAZ %s\r\n"
						"\e[34mGX %s\e[35m\tGY %s\e[36m\tGZ %s\r\n"
						"\e[37mKX %s\e[38m\tKY %s\e[39m\tTE %s\e[0m\r\n",
						mpu_buf[0],mpu_buf[1],mpu_buf[2],
						mpu_buf[3],mpu_buf[4],mpu_buf[5],
						mpu_buf[6],mpu_buf[7],
						mpu_buf[8]);

				g_mems_states = MEMS_STATE_WAIT;
				break;

			case MEMS_STATE_WAIT:
				osDelay(100);
				g_mems_states = MEMS_STATE_READ;
				break;

			case MEMS_STATE_FAIL:
				vcom_printf("\e[33m failed to initialize MPU6050...\e[0m\r\n");
				vTaskDelete(NULL);
				break;
		}
	}
}

