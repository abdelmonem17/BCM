
#ifndef BCM_WRAPER_H_
#define BCM_WRAPER_H_

#define BCM_COM_SET_CLK(clk)      			Uart_setBaudrate(clk)
#define BCM_COM_IS_READY_TO_SEND()	  		Uart_SendIsReadyInterrupt()
#define BCM_COM_SEND(data)					Uart_Send(data)
#define BCM_COM_INT_RECEIVE(input)		    void Uart_Int_Receive(input)
#define BCM_COM_START_RECIVING()			Uart_EnableInterruptReceiving()
#define BCM_COM_STOP_RECIVING()			    Uart_DisableInterruptReceiving()

#endif
