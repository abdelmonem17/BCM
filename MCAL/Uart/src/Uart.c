#include "Uart.h"
#include "mega32_registers.h"
#include "interrupt.h"
#include "avr/io.h"

/************************************************************************/
/*				Global variables                                        */
/************************************************************************/
static char sent_Data_Buffer[100];
static uint8 sent_Data_index=0;


static char received_Data_Buffer[100];
static uint8 received_Data_index=0;

/************************************************************************/
/*						UART macros and  defines                        */
/************************************************************************/
#define	USART_RX_EN									(1<<4)
#define	USART_TX_EN									(1<<3)
#define	USART_8_BIT_DATA							(0x86)
#define USART_WRITE_CONTROL_C						(0x7F)
#define	USART_DATA_REGISTER_EMPTY					(1<<5)
#define	USART_DATA_RECEIVE_COMPELET					(1<<7)
#define USART_DATA_EMPTY_INT_EN()					 USART_CONTROL_STATUS_B_REG |=(1<<5)
#define USART_DATA_EMPTY_INT_DIS()					 USART_CONTROL_STATUS_B_REG &=~ (1<<5)
#define USART_DOUBLE_SPEED							0x02

#define BAUDRATE_REG_CALCULATION(clk,baudrate)				( ( clk / 16 / baudrate ) - 1 )
#define BAUDRATE_DOUBLE_REG_CALCULATION(clk,baudrate)		( ( clk / 8 / baudrate ) -  1  )

/*		to handle sending data using interrupt     */
__attribute__((weak)) void Uart_Int_Send()
{
	if (sent_Data_index)
	{

		USART_DATA_REG=sent_Data_Buffer[--sent_Data_index];
		
	}
	else
	{
		USART_DATA_EMPTY_INT_DIS();
	}
	
}
/*			to handle receiving data using interrupt		*/
__attribute__((weak)) void Uart_Int_Receive(void)
{
	
	received_Data_Buffer[received_Data_index++]=USART_DATA_REG;
}

void Uart_setBaudrate(uint32 baudrate)
{
	uart_Conf.baudrate=baudrate;
}

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
****************************************************************************/
uint8 Uart_Init(Uart_Lconfg_S *uart_Lcfg)
{
	
	uint16 baudrate;
	/*				stop usart				*/
	//USART_CONTROL_STATUS_B_REG=0;
	//USART_CONTROL_STATUS_C_REG=0;
	//USART_CONTROL_STATUS_A_REG=0;
	
	///*				baudrate configuration				*/
	 //USART_BAUDRATED_LOW_REG	=REG_LOW_BYTE( BAUDRATE_REG_CALCULATION(uart_Lcfg.clk	,	uart_Lcfg.baudrate) );
	 //USART_BAUDRATED_HIGH_REG	=REG_HIGH_BYTE(	BAUDRATE_REG_CALCULATION(uart_Lcfg.clk	,	uart_Lcfg.baudrate) );
	 
	 USART_CONTROL_STATUS_B_REG		=	( USART_RX_EN | USART_TX_EN |uart_Lcfg->interrupt);
	 /*				set data size						*/
	 USART_CONTROL_STATUS_C_REG		=USART_8_BIT_DATA;
	 
	 /*				baudrate configuration				*/
	 baudrate=BAUDRATE_REG_CALCULATION(uart_Lcfg->clk	,	uart_Lcfg->baudrate);
	
	 if (baudrate <= 0x0fff)
	 {
		 
		 USART_BAUDRATED_LOW_REG	=REG_LOW_BYTE(baudrate);
		 USART_BAUDRATED_HIGH_REG	=REG_HIGH_BYTE(baudrate);
	 }
	 else
	 {
		 return NOT_OK;
	 }
	
	
	
	//	USART_CONTROL_STATUS_A_REG |=							USART_DOUBLE_SPEED;
	
	
	
	if (uart_Lcfg->interrupt==USART_INTERRUPT)
	{
		INT_GLOBAL_FLAG_ENABLE();
	}
	return OK;
	
}


/****************************************************************************
*
*   @brief function sends 8 bits using the UART
*   this function takes the data to be sent and return void
*   @params  name : data
*			 type :	char
*
*   @return void
************************************************************************************/
inline void Uart_Send(char data)
{
	
	
		USART_DATA_REG=data;
	
	
}

inline uint8 Uart_SendIsReadyInterrupt(void)
{
	return ( ( USART_CONTROL_STATUS_A_REG & USART_DATA_REGISTER_EMPTY ) == USART_DATA_REGISTER_EMPTY ? OK :NOT_OK );
}

/****************************************************************************
*
*   @brief function receives 8 bits using the UART
*   this function takes pointer to the received data and return if this succeed or not
*   @params  name : pData
*			 type :
*
*   @return the status if the data received function will return OK else will return NOT_OK
************************************************************************************/
uint8 Uart_Receive(char *pData)
{
	
	if (uart_Conf.interrupt == USART_INTERRUPT)
	{
		
		
		if (received_Data_index)
		{

			*pData=received_Data_Buffer[--received_Data_index];
			return OK;
			//USART_DATA_EMPTY_INT_EN();
		}
		else
		{
			return NOT_OK;
		}
	}
	else
	{
		if (( USART_CONTROL_STATUS_A_REG & USART_DATA_RECEIVE_COMPELET )==USART_DATA_RECEIVE_COMPELET)
		{
			*pData=USART_DATA_REG;
			return OK;
		}
		else
		{
			return NOT_OK;
		}
	}
	//return OK;
}

inline void __attribute__((always_inline)) Uart_DisableInterruptReceiving()
{
	USART_CONTROL_STATUS_B_REG &=(~USART_INTERRUPT);
}

inline void __attribute__((always_inline)) Uart_EnableInterruptReceiving()
{
	USART_CONTROL_STATUS_B_REG |=(USART_INTERRUPT);
}

INT_HANDLER(USART_DATA_EMPTY_VECT)
{
	/*
	USART_DATA_REG=sent_Data;
	USART_DATA_EMPTY_INT_DIS();
	*/
	//Uart_Int_Send();
	
}
INT_HANDLER(USART_RXC_VECT)
{
	
	Uart_Int_Receive();
}
