


#ifndef BCM_H_
#define BCM_H_
#include "Types.h"
#include "BCM_LCfg.h"
#include "Uart.h"
#include "Mega32_Registers.h"

#define BCM_GET_COM_REG()	USART_DATA_REG




typedef enum
{
	BCM_NoError=0,
	BCM_BaudtrateError=-1,
	BCM_ErrorLocked =-2
	
}EnmBCMError_t;




typedef enum
{
	BCM_RxDispatchStatusInProgress=0,
	BCM_RxDispatchStatusSuccessful=1,
	BCM_RxDispatchStatusLengthError=-1,
	BCM_RxDispatchStatusCodeError=-2,
	BCM_RxDispatchStatusChecksumError=-3,
	BCM_RxDispatchStatusIdel=-4,
	BCM_RxDispatchStatusRequetLocked=-5,
	
}EnmBCMStatus_t;


/****************************************************************************
*
*   @brief function initiates BCM
*   this function  takes BCM configuration and return the enum for error
*   @params  name : ConfigPtr
*			 type :	const BCM_ConfigType *
*   @return EnmBCMError_t
****************************************************************************/
EnmBCMError_t BCM_Init (const BCM_ConfigType * ConfigPtr );

/****************************************************************************
*
*   @brief function monitors BCM for receiving
*   this function  takes void return  error type
*   @return EnmBCMStatus_t
****************************************************************************/
EnmBCMStatus_t BCM_RxDispatch(void);

/****************************************************************************
*
*   @brief function initiates BCM to receive data
*   this function  takes BCM buffer and the length to initiates the receiver
*   @params  name : apch_RxBuffer
*			 type :	char * 
*   @params  name : au16_RxBufferLength
*			 type :	uint16
*   @return EnmBCMError_t
****************************************************************************/
EnmBCMError_t BCM_Receive(char *apch_RxBuffer,uint16 au16_RxBufferMaxLength , uint16 * apu16_ReceivedDataLength);

/****************************************************************************
*
*   @brief function unlocks BCM
*   this function  takes void and unlock the buffer
*   @return void
****************************************************************************/
 void BCM_RxDispatchUnlock(void);
 /****************************************************************************
 *
 *   @brief function monitors BCM for transmitting
 *   this function  takes void return void
 *   @return void
 ****************************************************************************/
void BCM_TxDispatch(void);

/****************************************************************************
*
*   @brief function initiates BCM to send data
*   this function  takes BCM buffer and the length to initiates the transmitter
*   @params  name : apch_TxBuffer
*			 type :	char *
*   @params  name : au16_TxBufferLength
*			 type :	uint16
*   @return EnmBCMError_t
****************************************************************************/
EnmBCMError_t BCM_Send(char *apch_TxBuffer,uint16 au8_TxBufferLength );

#endif /* BCM_H_ */
