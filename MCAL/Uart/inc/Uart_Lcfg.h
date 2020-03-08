/****************************************************************************
 * @file  UART_Lcfg.h
 * @brief this file includes some functions prototype to manipulate GPIO
 *
 * @author Abdelmonem Mostafa
 * @date  21th NOV 2019
 *
 ****************************************************************************/


#ifndef UART_LCFG_H_
#define UART_LCFG_H_
#include "Types.h"

#define	USART_INTERRUPT		0x80
#define USART_POLLING		0





typedef struct  
{
	uint32 clk;
	uint32 baudrate;
	uint8  interrupt;
}Uart_Lconfg_S;

extern Uart_Lconfg_S uart_Conf;

#endif /* UART_LCFG_H_ */
