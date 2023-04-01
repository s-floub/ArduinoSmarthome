#include "messageQueue.h"


pQueue CreateQueue (void) {
  pQueue pnew = (pQueue)malloc(sizeof (struct queue));
  if (pnew != NULL) {
    pnew->front = NULL;
    pnew->back =  NULL;
    pnew->count = 0;
  }
  return pnew;
}


pQueueItem CreateItem (Message message) {
	pQueueItem pnew = (pQueueItem) malloc(sizeof(QueueItem));
	if (pnew != NULL)	{
		pnew->message = message;
        pnew->next = NULL;
	}

	return pnew;
}

bool IsQEmpty (pQueue queue) {

  if(queue->front == NULL || queue->count == 0){return true;} //"or" or "and"

  return false;
}


int Enqueue (pQueue queue, pQueueItem item) {

  if(IsQEmpty(queue)){
    queue->front = item;
  }

  item->next = queue->back;
  queue->back = item;
  queue->count++;
  return 0;
  
}


int Dequeue (pQueue queue, Message &messageReturn) {
  if(IsQEmpty(queue)){return -1;}

  messageReturn = queue->front->message;
  
  if(queue->count == 1){
    free(queue->front);
    queue->back = NULL;
    queue->front = NULL;
    queue->count = 0;
    return 0;
  }

  if(queue->count >= 2){
    QueueItem *tempItem = queue->back;
    for(int i = 2; i < queue->count; i++){
      tempItem = tempItem->next;
    }
    free(queue->front);
    queue->front = tempItem;
    queue->count--;
    return 0;
  }

  return -1;
}


int DequeueAll (pQueue queue) {
  if(IsQEmpty(queue)){return -1;}

  Message temp;

  while(!IsQEmpty(queue)){
    if (Dequeue(queue, temp) == -1){
      return -1;
    }
  }

  return 0; 
}