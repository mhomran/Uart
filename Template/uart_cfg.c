/**
 * @file uart_cfg.c
 * @author Mohamed Hassanin
 * @brief A UART driver configuration file.
 * @version 0.1
 * @date 2021-03-08
 */

/*****************************************************************************
* Includes
*****************************************************************************/
#include "uart_cfg.h"

/*****************************************************************************
* Module Variable Definitions
*****************************************************************************/
/**
* The following array contains the configuration data for each
* UART Peripheral. Each row represents a UART peripheral. Each column is
* representing a member of the UartConfig_t
* structure. This table is read in by Uart_Init, where each channel is then
* set up based on this table.
*/
static const UartConfig_t UartConfig[] =
{
  //TODO: configure your UART peripherals
  { UART_0, UART_BAUDRATE_9600, UART_STOP_BIT_1, UART_PARTIY_NO }
};
/**********************************************************************
* Function Definitions
**********************************************************************/
/**********************************************************************
* Function : Uart_ConfigGet()
*//**
* \b Description:
* This function is used to get the cofiguration handle of the Uart <br>
* POST-CONDITION: A constant pointer to the first member of the
* configuration table will be returned. <br>
* @return A pointer to the configuration table.
*
* \b Example Example:
* @code
* const Uart_ConfigType *UartConfig = Uart_GetConfig();
* Uart_Init(UartConfig);
* @endcode
* @see Uart_Init
**********************************************************************/
const UartConfig_t * 
Uart_GetConfig(void)
{
  /*
  * The cast is performed to ensure that the address of the first element
  * of configuration table is returned as a constant pointer and NOT a
  * pointer that can be modified.
  */
  return (const UartConfig_t *) UartConfig;
}
/*****************************End of File ************************************/
