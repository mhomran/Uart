/**
 * @file uart.h
 * @author Mohamed Hassanin
 * @brief A UART driver header file.
 * @version 0.1
 * @date 2021-03-07
 */
#ifndef UART_H
#define UART_H

/******************************************************************************
 * Includes
 ******************************************************************************/
#include <inttypes.h>
#include "uart_cfg.h"
/******************************************************************************
 * Function prototypes
 ******************************************************************************/
#ifdef __cplusplus
extern "C"{
#endif

void Uart_Init(const UartConfig_t * const Config);

void Uart_SendUpdate(void);
void Uart_ReceiveUpdate(void);

uint8_t Uart_SendByte(const Uart_t Uart, const uint8_t Data);
uint8_t Uart_ReceiveByte(const Uart_t Uart, uint8_t* const Data);

uint8_t Uart_SendString(const Uart_t Uart, const uint8_t * const Data, const uint8_t DataSize);
uint8_t Uart_ReceiveString(const Uart_t Uart, uint8_t * const Data, const uint8_t DataSize);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* UART_H */
/*****************************End of File ************************************/