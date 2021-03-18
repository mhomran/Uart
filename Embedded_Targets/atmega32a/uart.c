/**
 * @file uart.c
 * @author Mohamed Hassanin
 * @brief A UART driver header file.
 * @version 0.1
 * @date 2021-03-08
 */
/**********************************************************************
* Preprocessor constants
**********************************************************************/
#define SYSTEM_FREQ (12000000ul) /**< system frequency */
/******************************************************************************
 * Includes
 ******************************************************************************/
#include "uart.h"
#include "circ_buffer.h"
#include "uart_memmap.h"
#include "det.h"
/******************************************************************************
* Module Variable Definitions
 ******************************************************************************/
/**
 * brief the UART send data buffers
 */
static uint8_t UartSendData[UART_MAX][UART_BUFF_SIZE];
/**
 * brief the UART receive data buffers
 */
static uint8_t UartReceiveData[UART_MAX][UART_BUFF_SIZE];

/**
 * brief the UART send buffers structures
 */
static CircBuff_t UartSendBuff[UART_MAX];

/**
 * brief the UART receive buffers structures
 */
static CircBuff_t UartReceiveBuff[UART_MAX];

/**
 * brief the UART data registers
 */
static volatile uint8_t* const UartDataRegs[UART_MAX] =
{
  //TODO: change uint8_t according to register size
  (volatile uint8_t*) UDR
};

/******************************************************************************
 * Function Definitions
 ******************************************************************************/
/******************************************************************************
* Function : Uart_Init()
*//**
* \b Description:
* This function is used to initialize the Uart based on the configuration
* table defined in uart_cfg file. <br>
* PRE-CONDITION: Configuration table needs to populated (sizeof > 0) <br>
* PRE-CONDITION: The MCU clocks must be configured and enabled. <br>
* PRE-CONDITION: The UART DIO pins are configured properly. <br>
* POST-CONDITION: The Uart peripherals are set up with the configuration 
* settings.<br>
* @param Config is a pointer to the configuration table that
* contains the initialization for the peripheral.
* @return void
*
* \b Example:
* @code
* const UartConfig_t *UartConfig = Uart_ConfigGet();
* Uart_Init(UartConfig);
* @endcode
* @see Uart_ConfigGet
*******************************************************************************/
extern void 
Uart_Init(const UartConfig_t * const Config)
{
  if(!(Config != 0x00))
    {
      Det_ReportError(UART_MODULE_ID, 0, UART_INIT_ID, UART_E_PARAM);
      return;
    }

  for(uint8_t i = 0; i < UART_MAX; i++)
    {
      UartSendBuff[i] = CircBuff_Create(UartSendData[i], UART_BUFF_SIZE);
      UartReceiveBuff[i] = CircBuff_Create(UartReceiveData[i], UART_BUFF_SIZE);
    
      *UBRRL = 0;
      *UBRRH = 0;
      *UCSRB = 0;
      *UCSRC = 0;
      //baud rate
      *UBRRL = (((SYSTEM_FREQ)/16/Config[i].Baudrate)-1) & 0xFF;
      *UBRRH = (((SYSTEM_FREQ)/16/Config[i].Baudrate)-1) >> 8;
      
      //enable UART and interrupt events 8-bit mode
      *UCSRB = 1 << TXEN | 1 << RXEN;
      

      // When the function writes to the UCSRC Register, the URSEL bit
      // (MSB) must be set due to the sharing of I/O location by UBRRH
      //and UCSRC.
      *UCSRC = 1 << UCSZ0 | 1 << UCSZ1 | 1 << URSEL;

      if(Config[i].StopBit == UART_STOP_BIT_2)
        {
          *UCSRC |= 1 << USBS;
        }

      if(Config[i].Parity == UART_PARTIY_EVEN)
        {
          *UCSRC |= 1 << UPM1;
        }
      else if(Config[i].Parity == UART_PARTIY_ODD)
        {
          *UCSRC |= 1 << UPM1 | 1 << UPM0;
        }
    }
}

/******************************************************************************
* Function : Uart_SendUpdate()
*//**
* \b Description:
* This function is used to send the next byte (if existed) in the UART send 
* data buffers. <br>
* PRE-CONDITION: Uart_Init called properly <br>
* POST-CONDITION: The next byte (if existed) in the UART data buffers is sent <br>
* @param Uart the Uart Id 
* @return void
*
* @see Uart_Init
*******************************************************************************/
extern void
Uart_SendUpdate(const Uart_t Uart)
{
  if(!(Uart < UART_MAX))
    {
      Det_ReportError(UART_MODULE_ID, Uart, UART_SEND_UPDATE_ID, UART_E_PARAM);
      return;
    }

  uint8_t Result;
  uint8_t Data;
  
  Result = CircBuff_Dequeue(&UartSendBuff[Uart], &Data);
  if(Result == 1)
    {
      //Transmit buffer empty ?
      if(*UCSRA & (1<<UDRE))
        {
          *UartDataRegs[Uart] = Data;
        }
      else
        {
          Det_ReportError(UART_MODULE_ID, Uart, UART_SEND_UPDATE_ID, UART_E_TB_NEMPTY);
        }
    }

}

/******************************************************************************
* Function : Uart_ReceiveUpdate()
*//**
* \b Description:
* This function is used to receive a byte (if existed) in the UART receive 
* data registers and store them in the UART receive buffers. <br>
* PRE-CONDITION: Uart_Init called properly <br>
* POST-CONDITION: The next byte (if existed) in the UART data registers is 
* received <br>
* @param Uart the Uart Id 
* @return void
*
* @see Uart_Init
*******************************************************************************/
extern void
Uart_ReceiveUpdate(const Uart_t Uart)
{
  if(!(Uart < UART_MAX))
    {
      Det_ReportError(UART_MODULE_ID, Uart, UART_SEND_UPDATE_ID, UART_E_PARAM);
      return;
    }

  uint8_t Result;
  uint8_t Data;
  uint8_t error = 0;

  //received something ?
  if(*UCSRA & (1 << RXC))
    {
      //error bits are valid until the receive buffer (UDR) is read
      if(*UCSRA & (1 << FE))
        {
          Det_ReportError(UART_MODULE_ID, Uart, UART_RECEIVE_UPDATE_ID, UART_E_FRAME);
          error = 1;
        }

      if(*UCSRA & (1 << DOR))
        {
          Det_ReportError(UART_MODULE_ID, Uart, UART_RECEIVE_UPDATE_ID, UART_E_OVERRUN);
          error = 1;              
        }

      if(*UCSRA & (1 << PE))
        {
          Det_ReportError(UART_MODULE_ID, Uart, UART_RECEIVE_UPDATE_ID, UART_E_PARITY);
          error = 1;
        }

      if(error == 0)
        {
          Data = *UartDataRegs[Uart];
          Result = CircBuff_Enqueue(&UartReceiveBuff[Uart], Data);
        }
      else
        {
          //clear RXC flag
          Data = *UartDataRegs[Uart];
        }
    }
}

/******************************************************************************
* Function : Uart_SendByte()
*//**
* \b Description:
* This function is used to store a byte in the UART send data buffers so it can 
* be sent later when Uart_SendUpdate is called 
* PRE-CONDITION: Uart_Init called properly <br>
* @param Uart the Uart Id 
* @param Data the byte to store 
* @return uint8_t 1 if the byte is stored and 0 otherwise.
*
* @see Uart_Init
* @see Uart_SendUpdate
*******************************************************************************/
extern uint8_t
Uart_SendByte(const Uart_t Uart, const uint8_t Data)
{
  if(!(Uart < UART_MAX))
    {
      Det_ReportError(UART_MODULE_ID, Uart, UART_SEND_BYTE_ID, UART_E_PARAM);
      return 0;
    }

  uint8_t res = CircBuff_Enqueue(&UartSendBuff[Uart], Data);
  return res;
}

/******************************************************************************
* Function : Uart_ReceiveByte()
*//**
* \b Description:
* This function is used to receive the next byte from the UART receive data buffers.
* PRE-CONDITION: Uart_Init called properly <br>
* @param Uart the Uart Id 
* @param Data a pointer to store the received byte
* @return uint8_t 1 if the byte is received and 0 otherwise.
*
* @see Uart_Init
* @see Uart_ReceiveUpdate
*******************************************************************************/
extern uint8_t
Uart_ReceiveByte(const Uart_t Uart, uint8_t* const Data)
{
  if(!(Data != 0x00 && Uart < UART_MAX))
    {
      Det_ReportError(UART_MODULE_ID, Uart, UART_RECEIVE_BYTE_ID, UART_E_PARAM);
      return 0;
    }

  uint8_t res = CircBuff_Dequeue(&UartReceiveBuff[Uart], Data);
  return res;
}

/******************************************************************************
* Function : Uart_SendString()
*//**
* \b Description:
* This function is used to store a string in the UART send data buffers so it can 
* be sent later when Uart_SendUpdate is called 
* PRE-CONDITION: Uart_Init called properly <br>
* @param Uart the Uart Id 
* @param Data a pointer to the data to store in send data buffers
* @param DataSize The size of the string to send
* @return uint8_t the number of stored data in bytes
*
* @see Uart_Init
* @see Uart_SendUpdate
*******************************************************************************/
extern uint8_t
Uart_SendString(
  const Uart_t Uart,
  const uint8_t * const Data,
  const uint8_t DataSize)
{
  if(!(Data != 0x00 && Uart < UART_MAX))
    {
      Det_ReportError(UART_MODULE_ID, Uart, UART_SEND_STRING_ID, UART_E_PARAM);
      return 0;
    }

  if(DataSize == 0) return 0;
  
  uint8_t res;
  uint8_t i = 0;

  do{
    res = CircBuff_Enqueue(&UartSendBuff[Uart], Data[i]);

    i = (res == 1) ? (i + 1) : i;

  } while(res == 1 && i < DataSize);

  return i;
}

/******************************************************************************
* Function : Uart_ReceiveByte()
*//**
* \b Description:
* This function is used to receive a string from the UART receive data buffers.
* PRE-CONDITION: Uart_Init called properly <br>
* @param Uart the Uart Id 
* @param Data a pointer to store the received string in
* @param DataSize The size of the string to receive
* @return uint8_t the number of received data in bytes
*
* @see Uart_Init
* @see Uart_ReceiveUpdate
*******************************************************************************/
extern uint8_t
Uart_ReceiveString(
  const Uart_t Uart, 
  uint8_t * const Data,
  const uint8_t DataSize)
{
  if(!(Data != 0x00 && Uart < UART_MAX))
    {
      Det_ReportError(UART_MODULE_ID, Uart, UART_RECEIVE_STRING_ID, UART_E_PARAM);
      return 0;
    }

  if(DataSize == 0) return 0;

  uint8_t res;
  uint8_t i = 0;
  
  do{
    res = CircBuff_Dequeue(&UartReceiveBuff[Uart], &Data[i]);

    i = (res == 1) ? (i + 1) : i;

  } while(res == 1 && i < DataSize);

  return i;
}

/******************************************************************************
* Function : Uart_PeekLastByte()
*//**
* \b Description:
* This function is used to peak the last received byte from the UART 
* receive data buffers.
* PRE-CONDITION: Uart_Init called properly <br>
* @param Uart the Uart Id 
* @param Data a pointer to store the received byte
* @return uint8_t 1 if the byte is received and 0 otherwise.
*
* @see Uart_Init
*******************************************************************************/
extern uint8_t 
Uart_PeekLastByte(const Uart_t Uart, uint8_t* const Data)
{
  if(!(Data != 0x00 && Uart < UART_MAX))
    {
      Det_ReportError(UART_MODULE_ID, Uart, UART_PEEK_LAST_BYTE_ID, UART_E_PARAM);
      return 0;
    }

  uint8_t res = CircBuff_PeekLast(&UartReceiveBuff[Uart], Data);
  return res;
}

/*****************************End of File ************************************/
