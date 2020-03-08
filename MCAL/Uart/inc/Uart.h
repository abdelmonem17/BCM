/****************************************************************************
 * @file  Uart.h
 * @brief this file includes some functions prototype to manipulate UART
 *
 * @author Abdelmonem Mostafa
 * @date  NOV 12 2019
 *
 ****************************************************************************/

#ifndef UART_H_
#define UART_H_
#include "Uart_Lcfg.h"



/************************************************************************/
/*                    UART functions                                     */
/************************************************************************/


/****************************************************************************
*
*   @brief function initializes the UART to send and receive data 
*   this function takes UART configuration	 and return if the the conf.. succeed or not (OK or NOT_OK)
*   @params  name : uart_Lcfg
*			 type :	Uart_Lconfg_S
*
*   @return the status if the configuration succeed the function will return OK else will return NOT_OK
************************************************************************************/
uint8 Uart_Init(Uart_Lconfg_S *);

/****************************************************************************
*
*   @brief function sends 8 bits using the UART 
*   this function takes the data to be sent and return void
*   @params  name : data
*			 type :	char
*
*   @return void
************************************************************************************/
void Uart_Send(char data);



/****************************************************************************
*
*   @brief function receives 8 bits using the UART
*   this function takes pointer to the received data and return if this succeed or not
*   @params  name : pData
*			 type :	
*
*   @return the status if the data received function will return OK else will return NOT_OK
************************************************************************************/
uint8 Uart_Receive(char *);
uint8 Uart_SendIsReadyInterrupt(void);
void Uart_setBaudrate(uint32 baudrate);
void Uart_EnableInterruptReceiving() ;
void Uart_DisableInterruptReceiving() ;




#endif /* UART_H_ */
