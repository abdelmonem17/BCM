
#include "BCM.h"
#include "Utilites.h"
#include "Queue.h"
#include "BCM_Wrapper.h"

#define LOCKED							0
#define UNLOCKED						1
#define BCM_CODE						'2'
#define TX_DATA_LENGTH_SIZE				5
#define TX_DATA_LENGTH_DATATYPE_SIZE	(sizeof(uint16))
#define RX_DATA_LENGTH_SIZE				5
#define RX_DATA_LENGTH_DATATYPE_SIZE	(sizeof(uint16))
#define RX_BUFFER_LENGTH				10



typedef enum
{
	SectionBCMCode=0,
	SectionLength=1,
	SectionData=2,
	SectionChecksum=3
}EnmBCM_FrameSection_t;
typedef enum
{
	BCM_Idle,
	BCM_StartOperation,
	BCM_OperationProcssing
}EnmBCMState_t;


typedef struct
{
	uint16			u8_Checksum;
	uint8			u8_lock_flag;
	uint16			u16_BufferLength;
	char *			pch_Buffer;
	
	
}ST_ComProfile_t;






static ST_queueInfo_Type gSTQueueInfo_RecivedDatabufferInfo;
static char gstr_RxBuffer[RX_BUFFER_LENGTH];
static ST_ComProfile_t	gSTComProfile_RxProfile;
static ST_ComProfile_t	gSTComProfile_TxProfile;;
static uint16 gu16_DatabufferIndex_Rx = NUMBER_ZERO;
static uint16 * gpu16_ReceivedDataLength;




/*		hock function to store received data	*/
BCM_COM_INT_RECEIVE(void)
{
	QueueEnqueue(&gSTQueueInfo_RecivedDatabufferInfo,BCM_GET_COM_REG() );
}



/****************************************************************************
*
*   @brief function initiates BCM
*   this function  takes BCM configuration and return the enum for error
*   @params  name : ConfigPtr
*			 type :	const BCM_ConfigType *
*   @return EnmBCMError_t
****************************************************************************/
EnmBCMError_t BCM_Init (const BCM_ConfigType * ConfigPtr )
{


	queueCreate(&gSTQueueInfo_RecivedDatabufferInfo,RX_BUFFER_LENGTH,gstr_RxBuffer);

	EnmBCMError_t EnmMCMError_errorStatus=BCM_NoError;
	BCM_COM_SET_CLK(ConfigPtr->baudrate);
	if( Uart_Init(&uart_Conf) == OK)
	{

		gSTComProfile_RxProfile.u8_lock_flag=UNLOCKED;
		gSTComProfile_RxProfile.pch_Buffer=NULL;
		gSTComProfile_RxProfile.u16_BufferLength=NUMBER_ZERO;
		
		gSTComProfile_TxProfile.u8_lock_flag=UNLOCKED;
		gSTComProfile_TxProfile.pch_Buffer=NULL;
		gSTComProfile_TxProfile.u16_BufferLength=NUMBER_ZERO;
		BCM_COM_STOP_RECIVING();

	}
	else
	{
		EnmMCMError_errorStatus=BCM_BaudtrateError;
	}
	return EnmMCMError_errorStatus;
}



/****************************************************************************
*
*   @brief function initiates BCM to receive data
*   this function  takes BCM buffer and the length to initiates the receiver
*   @params  name : apch_RxBuffer
*			 type :	char *
*   @params  name : au16_RxBufferLength
*			 type :	uint16
*	@params  name : apu16_ReceivedDataLength
*			 type :	uint16 *
*   @return EnmBCMError_t
****************************************************************************/
EnmBCMError_t BCM_Receive(char *apch_RxBuffer,uint16 au16_RxBufferMaxLength , uint16 * apu16_ReceivedDataLength)
{
	EnmBCMError_t EnmMCMError_errorStatus=BCM_NoError;
	if (gSTComProfile_RxProfile.u8_lock_flag ==	UNLOCKED)
	{
		BCM_COM_START_RECIVING();
		gSTComProfile_RxProfile.pch_Buffer = apch_RxBuffer;
		gSTComProfile_RxProfile.u16_BufferLength = au16_RxBufferMaxLength;
		gu16_DatabufferIndex_Rx = NUMBER_ZERO;
		gpu16_ReceivedDataLength = apu16_ReceivedDataLength;
		
	}
	else
	{
		EnmMCMError_errorStatus=BCM_ErrorLocked;
	}
	return EnmMCMError_errorStatus;
}


/****************************************************************************
*
*   @brief function monitors BCM for receiving
*   this function  takes void return  error type
*   @return EnmBCMStatus_t
****************************************************************************/
EnmBCMStatus_t BCM_RxDispatch(void)
{
	static EnmBCMStatus_t lsEnmBCMDispatchStatus_RxDispatchCurrentStatus=BCM_RxDispatchStatusIdle;
	static EnmBCM_FrameSection_t lsEnmBCM_FrameSection_Rx=SectionBCMCode;
	static EnmBCMState_t lsEnmBCMState_Rx=BCM_Idle;
	static uint8 lsu8_RxChecksumCalculated=NUMBER_ZERO;
	static char lsStr_DataLength[RX_DATA_LENGTH_SIZE];
	static uint16 lsu16_DataLengthReceived=NUMBER_ZERO;
	static uint8 lsu8_StrDataLengthIndex=NUMBER_ZERO;
	static char lsch_ReceiveData;
	static uint16 lsu16_DatabufferIndex=NUMBER_ZERO;

	if (gSTComProfile_RxProfile.u8_lock_flag == UNLOCKED)
	{
	
		switch(lsEnmBCMState_Rx)
		{
			case BCM_StartOperation:
				
					switch(lsEnmBCM_FrameSection_Rx)
					{
						case SectionData :
						  if ( lsu16_DatabufferIndex < lsu16_DataLengthReceived )
							{
							  	  /* storing data and calculating the check sum */
								lsu8_RxChecksumCalculated +=	lsch_ReceiveData;
								gSTComProfile_RxProfile.pch_Buffer[lsu16_DatabufferIndex ++]=lsch_ReceiveData;

								lsEnmBCMState_Rx= BCM_OperationProcssing;

							}
							else
							{

								/*	stay in same state and receive checksum */

								lsEnmBCM_FrameSection_Rx=SectionChecksum;
							}

				
							break;
						case SectionLength :

							if (RX_DATA_LENGTH_DATATYPE_SIZE  !=	lsu8_StrDataLengthIndex	)
							{
								/* receiving bytes of length*/

								lsStr_DataLength[lsu8_StrDataLengthIndex++]=lsch_ReceiveData;
								lsEnmBCMState_Rx= BCM_OperationProcssing;

							}
							else
							{
								lsu16_DataLengthReceived=hexStrToInt(lsStr_DataLength,RX_DATA_LENGTH_DATATYPE_SIZE);
								if (lsu16_DataLengthReceived >	gSTComProfile_RxProfile.u16_BufferLength)
								{
								   /* received lenght will not fit to data buffer */
									lsEnmBCMState_Rx = BCM_Idel;
									BCM_COM_STOP_RECIVING();
									/*	   initiate the queue for next use		*/
									queueCreate(&gSTQueueInfo_RecivedDatabufferInfo,RX_BUFFER_LENGTH,gstr_RxBuffer);
									lsEnmBCMDispatchStatus_RxDispatchCurrentStatus = BCM_RxDispatchStatusLengthError;
								}
								else
								{
									/* stay in same state and process data section */
									lsEnmBCM_FrameSection_Rx=SectionData;

								}

							}
							break;
						case SectionChecksum :
							 if (lsch_ReceiveData == lsu8_RxChecksumCalculated )
							 {
								 /* valid frame received*/
								 * gpu16_ReceivedDataLength = lsu16_DataLengthReceived;
								 lsEnmBCMDispatchStatus_RxDispatchCurrentStatus = BCM_RxDispatchStatusSuccessful;
								 gSTComProfile_RxProfile.u8_lock_flag=LOCKED;

							 }
							 else
							 {
								 /*	invalid frame received	*/
								 lsEnmBCMDispatchStatus_RxDispatchCurrentStatus = BCM_RxDispatchStatusChecksumError;
							 }
							 lsEnmBCMState_Rx=BCM_Idel;
							 BCM_COM_STOP_RECIVING();
							 /*	   initiate the queue for next use		*/
							 queueCreate(&gSTQueueInfo_RecivedDatabufferInfo,RX_BUFFER_LENGTH,gstr_RxBuffer);

							break;
						default:
						/*	 section not valid	*/
						break;
					}
					break;
			
			case BCM_OperationProcssing:

				if(QueueDequeue(&gSTQueueInfo_RecivedDatabufferInfo,&lsch_ReceiveData) == queue_operationSucceeded)
				{
					lsEnmBCMState_Rx=BCM_StartOperation;

				}
				else
				{
					//stay here
				}
				
				break;
			case BCM_Idle:

				if( QueueGetSize(&gSTQueueInfo_RecivedDatabufferInfo) !=0)
				{
					/* data recived for a request */
					lsEnmBCMDispatchStatus_RxDispatchCurrentStatus=BCM_RxDispatchStatusInProgress;
					lsEnmBCMState_Rx=BCM_OperationProcssing;
					lsEnmBCM_FrameSection_Rx=SectionBCMCode;
					lsu8_RxChecksumCalculated=NUMBER_ZERO;
					lsu16_DatabufferIndex = NUMBER_ZERO;
					lsu8_StrDataLengthIndex=NUMBER_ZERO;

				}
				else
				{
					/* stay here untill request and data come */

				}

				break;
			
			default:
			/*						error  state option								*/
				break;
		}
	}
	else
	{
		/* the request is locked nothing	*/
		lsEnmBCMDispatchStatus_RxDispatchCurrentStatus = BCM_RxDispatchStatusRequetLocked;
	}
	return lsEnmBCMDispatchStatus_RxDispatchCurrentStatus;
}



/****************************************************************************
*
*   @brief function unlocks BCM
*   this function  takes void and unlock the buffer
*   @return void
****************************************************************************/

void inline __attribute((always_inline)) BCM_RxDispatchUnlock(void)
{
	gSTComProfile_RxProfile.u8_lock_flag=UNLOCKED;
}


/****************************************************************************
 *
 *   @brief function monitors BCM for transmitting
 *   this function  takes void return void
 *   @return EnmBCMError_t
 ****************************************************************************/
EnmBCMError_t BCM_Send(char *apch_TxBuffer,uint16 au16_TxBufferLength )
{
	EnmBCMError_t EnmMCMError_errorStatus=BCM_NoError;
	if (gSTComProfile_TxProfile.u8_lock_flag == UNLOCKED)
	{

		gSTComProfile_TxProfile.pch_Buffer = apch_TxBuffer;
		gSTComProfile_TxProfile.u16_BufferLength = au16_TxBufferLength;
		gSTComProfile_TxProfile.u8_lock_flag=LOCKED;

	}
	else
	{
		EnmMCMError_errorStatus=BCM_ErrorLocked;
	}
	return EnmMCMError_errorStatus;
}


/****************************************************************************
 *
 *   @brief function monitors BCM for transmitting
 *   this function  takes void return void
 *   @return void
 ****************************************************************************/
void BCM_TxDispatch(void)
{
	static uint16 au16_TxBufferIndex=NUMBER_ZERO;
	static EnmBCMState_t aEnmMCMState_Tx=BCM_Idle;
	static uint8 au8_TxChecksum=NUMBER_ZERO;
	static EnmBCM_FrameSection_t aEnmBCM_FrameDataType_FrameSection=SectionBCMCode;
	static char StaticStr_DataLength[TX_DATA_LENGTH_SIZE];
	static uint8 au8_StrDataLengthIndex=NUMBER_ZERO;
	static char ach_sentData;



	switch(aEnmMCMState_Tx)
	{
		case BCM_StartOperation:
			/*			compute checksum ,go to sendingprocessing 					*/

			switch(aEnmBCM_FrameDataType_FrameSection)
			{
				case SectionData :
					if ( au16_TxBufferIndex < gSTComProfile_TxProfile.u16_BufferLength )
					{
						ach_sentData=gSTComProfile_TxProfile.pch_Buffer[ au16_TxBufferIndex];
						au8_TxChecksum +=	ach_sentData;
						ach_sentData=gSTComProfile_TxProfile.pch_Buffer[ au16_TxBufferIndex++];
						aEnmMCMState_Tx=BCM_OperationProcssing;

					}
					else
					{

						/*	stay in same state and send checksum */

						aEnmBCM_FrameDataType_FrameSection=SectionChecksum;
					}

					break;
				case SectionLength :
					if (TX_DATA_LENGTH_DATATYPE_SIZE  !=	au8_StrDataLengthIndex	)
					{

						ach_sentData = StaticStr_DataLength[au8_StrDataLengthIndex++];
						aEnmMCMState_Tx=BCM_OperationProcssing;
					}
					else
					{
						/*	stay in same state and send data section */
						aEnmBCM_FrameDataType_FrameSection=SectionData;
					}


					break;
				case SectionBCMCode :

					ach_sentData=BCM_CODE;
					IntToHexStr((uint16)gSTComProfile_TxProfile.u16_BufferLength,StaticStr_DataLength);
					aEnmBCM_FrameDataType_FrameSection=SectionLength;

					aEnmMCMState_Tx=BCM_OperationProcssing;
					break;
				case SectionChecksum :

					ach_sentData=au8_TxChecksum;
					aEnmMCMState_Tx=BCM_OperationProcssing;

					break;
				default:
					/*	 section not valid	*/
					break;
			}
			break;

		case BCM_OperationProcssing:
			if (BCM_COM_IS_READY_TO_SEND() == OK)
			{
				/*		send and go to Sending state	*/


				BCM_COM_SEND(ach_sentData);
				if (aEnmBCM_FrameDataType_FrameSection !=SectionChecksum)
				{
					aEnmMCMState_Tx=BCM_StartOperation;
				}
				else
				{

					au8_TxChecksum=NUMBER_ZERO;
					au16_TxBufferIndex=NUMBER_ZERO;
					gSTComProfile_TxProfile.u8_lock_flag=UNLOCKED;
					aEnmMCMState_Tx=BCM_OperationProcssing;
					gSTComProfile_TxProfile.u16_BufferLength=NUMBER_ZERO;
					aEnmMCMState_Tx=BCM_Idel;

					aEnmMCMState_Tx=BCM_Idel;
				}
			}

			else
			{
				/*	 	stay in same state (send Processing)	*/
			}
			break;
		case BCM_Idle:
			if (au16_TxBufferIndex < gSTComProfile_TxProfile.u16_BufferLength)
			{
				aEnmMCMState_Tx=BCM_StartOperation;
			}
			else
			{
				/*			stay here			*/
			}
			break;

		default:
		/*				error option			*/
			break;
	}

}


