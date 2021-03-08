/**
 * @file uart.c
 * @author Mohamed Hassanin
 * @brief A UART driver header file.
 * @version 0.1
 * @date 2021-03-08
 */
/******************************************************************************
 * Includes
 ******************************************************************************/
#include "uart.h"
#include "circ_buffer.h"
/******************************************************************************
* Module Variable Definitions
 ******************************************************************************/
/**
 * brief the UART data buffers
 */
static uint8_t UartData[UART_MAX][UART_BUFF_SIZE];

/**
 * brief the UART buffers structures
 */
static CircBuff_t UartBuff[UART_MAX];

/*****************************End of File ************************************/
