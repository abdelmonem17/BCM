/*
 * Queue.h
 *
 *  Created on: Oct 30, 2019
 *      Author: AVE-LAP-062
 */

#ifndef QUEUE_H_
#define QUEUE_H_
#include <stdint.h>
#include <stdio.h>


typedef char queue_data_Type ;
typedef uint16_t queue_size_Type ;
typedef	struct{

	queue_data_Type *queue_pointer;
	queue_size_Type  queue_maxSize;
	queue_size_Type  start;
	queue_size_Type  end;
	queue_size_Type size;
}ST_queueInfo_Type;

typedef enum
{
	queue_operationSucceeded=0,
	queue_ErrorBufferEmpty=-1,
	queue_ErrorBufferFull=-2
}enm_QueueOperationStatus_Type;

void queueCreate(ST_queueInfo_Type* info, queue_size_Type maxSize,void * dataBuffer);
enm_QueueOperationStatus_Type QueueEnqueue(ST_queueInfo_Type *info, queue_data_Type data);
enm_QueueOperationStatus_Type QueueDequeue(ST_queueInfo_Type *info, queue_data_Type* data);
queue_size_Type QueueGetSize(ST_queueInfo_Type *info);


#endif /* QUEUE_H_ */
