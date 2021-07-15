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
#include "led.h"
#include "vcom.h"
#include "gpio.h"


/*******************************************************************************
    Public function implementations.
*******************************************************************************/

/**
* @brief User LED periodic blink for heartbeating.
* @param argument: Not used
* @retval None
*/
void tsken_led(void *argument)
{
	for(;;)
	{
		uint32_t ticks = HAL_GetTick();
		char* s_text = "heartbeat";

		if(!(ticks % 500))
		{
			HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
//			vcom_printf("\e[31m \u2665 %s at %d.%.3d sec\e[0m\r\n",
//			s_text,
//			(ticks / 1000),
//			(ticks % 1000));

			osDelay(20);
			HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
			osDelay(80);
		}
	}
}
