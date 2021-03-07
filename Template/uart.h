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
 * Function prototypes
 ******************************************************************************/
#ifdef __cplusplus
extern "C"{
#endif

void Uart_Init(const UartConfig_t * constConfig);
void Uart_SendByte(Uart_t Uart, uint8_t data);
uint8_t Uart_ReceiveByte(Uart_t Uart);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* UART_H */
/*****************************End of File ************************************/
