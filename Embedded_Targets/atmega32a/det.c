/**
 * @file det.c
 * @author Mohamed Hassanin
 * @brief a default error tracer for error handling
 * @version 0.1
 * @date 2021-03-12
 */
/******************************************************************************
 * Includes
 ******************************************************************************/
#include "det.h"
#include "uart.h"
/******************************************************************************
 * Function prototypes
 ******************************************************************************/
static void Det_DefaultHandler(void);
//TODO: add any helper functions
/******************************************************************************
 * Function definitions
 ******************************************************************************/
extern void 
Det_ReportError(
  uint16_t ModuleId,
  uint8_t InstanceId, 
  uint8_t ApiId, 
  uint8_t ErrorId)
{
  //TODO: handle the UART errors
  if(ModuleId == UART_MODULE_ID)
    {
      switch(ApiId)
      {
        case UART_INIT_ID:
          Det_DefaultHandler();
        break;
        
        case UART_SEND_UPDATE_ID:
        break;

        case UART_RECEIVE_UPDATE_ID:
        break;
        
        case UART_SEND_BYTE_ID:
        break;
        
        case UART_RECEIVE_BYTE_ID:
        break;

        case UART_SEND_STRING_ID:
        break;

        case UART_RECEIVE_STRING_ID:
        break;

        default:
        break;
      }
    } 
}

/**
 * @brief A default error handler 
 * 
 */
static void 
Det_DefaultHandler(void) 
{
  //TODO: implement Det_DefaultHandler
  while(1);
}
/*****************************End of File ************************************/
