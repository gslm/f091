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
#include <stdarg.h>
#include <string.h>
#include "usart.h"
#include "vcom.h"


/*******************************************************************************
    Private constants and types.
*******************************************************************************/
#define VCOM_BUFSIZE 		(256)


/*******************************************************************************
    Private variables.
*******************************************************************************/
static char g_buf[VCOM_BUFSIZE];


/*******************************************************************************
    Public function implementations.
*******************************************************************************/
void vcom_printf(char *fmt,...)
{
    va_list arg;
    va_start(arg,fmt);
    vsprintf((char*)g_buf,fmt,arg);
    va_end(arg);

    /* starts DMA buffer transmission */
    HAL_UART_Transmit_DMA(&huart2, g_buf, strlen((const char *)g_buf));
}
