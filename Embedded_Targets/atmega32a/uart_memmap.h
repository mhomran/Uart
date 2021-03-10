/**
 * @file uart_memmap.h
 * @author Mohamed Hassanin
 * @brief A memory map for the UART peripherals
 * @version 0.1
 * @date 2021-03-08
 */

#ifndef UART_MEMMAP_H
#define UART_MEMMAP_H

#define UART_UPPER_BOUND_ADDRESS_0 UDR
#define UDR ((volatile uint8_t*) 0x002C)

#define UCSRA ((volatile uint8_t*) 0x002B)
#define UCSRB ((volatile uint8_t*) 0x002A)
#define UBRRL ((volatile uint8_t*) 0x0029)
#define UART_LOWER_BOUND_ADDRESS_0 UBRRL

#define UART_UPPER_BOUND_ADDRESS_1 UBRRH
#define UBRRH ((volatile uint8_t*) 0x0040)
#define UCSRC ((volatile uint8_t*) 0x0040)
#define UART_LOWER_BOUND_ADDRESS_1 UBRRH

/* UCSRA */
#define RXC     7
#define TXC     6
#define UDRE    5
#define FE      4
#define DOR     3
#define PE      2
#define U2X     1
#define MPCM    0

/* UCSRB */
#define RXCIE   7
#define TXCIE   6
#define UDRIE   5
#define RXEN    4
#define TXEN    3
#define UCSZ2   2
#define RXB8    1
#define TXB8    0

/* UCSRC */
#define URSEL   7
#define UMSEL   6
#define UPM1    5
#define UPM0    4
#define USBS    3
#define UCSZ1   2
#define UCSZ0   1
#define UCPOL   0

#endif
/*****************************End of File ************************************/
