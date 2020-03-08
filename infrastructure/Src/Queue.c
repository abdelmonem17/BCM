/*
 * Queue.c
 *
 *  Created on: Oct 30, 2019
 *      Author: AVE-LAP-062
 */
#include "Queue.h"
#include "stdlib.h"
void queueCreate(ST_queueInfo_Type* info, queue_size_Type maxSize,void * dataBuffer)
{

	//info->queue_pointer=(queue_data_Type *)malloc(sizeof(queue_data_Type) * maxSize);

	info->queue_pointer =(queue_data_Type *) dataBuffer;
	info->start=info->end=0;
	info->queue_maxSize=maxSize;
	info->size=0;

}
enm_QueueOperationStatus_Type QueueEnqueue(ST_queueInfo_Type *info, queue_data_Type data)
{

	enm_QueueOperationStatus_Type operation_status= queue_operationSucceeded;

   if( (info->queue_maxSize) != info->size )
		{

		if(info->end == info->queue_maxSize)
		{
				info->end=0;
		}
			info->queue_pointer[info->end]=data;
			++info->end;
			++info->size;
		}
	else
	{

					//printf("error queue is full\n");
				operation_status=queue_ErrorBufferFull;
	}

	return operation_status;
}

enm_QueueOperationStatus_Type QueueDequeue(ST_queueInfo_Type *info, queue_data_Type* data)
{

	enm_QueueOperationStatus_Type operation_status= queue_operationSucceeded;
	if( info->size != 0 )
	 {
		if(info->start == info->queue_maxSize)
		{
			info->start=0;
		}
				*data =info->queue_pointer[info->start];
				++info->start;
				--info->size;
	  }
	else
	{
		operation_status= queue_ErrorBufferEmpty;
			//printf("error empty is empty\n");
	}
	return operation_status;
}

inline queue_size_Type __attribute__((always_inline)) QueueGetSize(ST_queueInfo_Type *info)
{
	return info->size;
}
