#include "messageQueue.h"

//Queue function definitions are more or less taken from MREN 178 lab 2


pQueue CreateQueue (void) {
  pQueue pnew = (pQueue) malloc (sizeof (struct queue));
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
  return RETURN_OK;
  
}


int Dequeue (pQueue queue, Message &messageReturn) {
  if(IsQEmpty(queue)){return RETURN_ERR;}

  messageReturn = queue->front->message;
  
  //if there is only one item in the queue it is safe to remove item and reset queue to empty state
  if(queue->count == 1){
    free(queue->front);
    queue->back = NULL;
    queue->front = NULL;
    queue->count = 0;
    return RETURN_OK;
  }

  //If there are 2 or more items in the queue
  if(queue->count >= 2){

    //Move everyone up in queue
    QueueItem *tempItem = queue->back;
    for(int i = 2; i < queue->count; i++){
      tempItem = tempItem->next;
    }

    //Return front item of queue
    free(queue->front);
    queue->front = tempItem;
    queue->count--;
    return RETURN_OK;
  }

  return RETURN_ERR;
}


int DequeueAll (pQueue queue) {
  if(IsQEmpty(queue)){return RETURN_ERR;}

  Message temp;

  //Empty queue one by one
  while(!IsQEmpty(queue)){
    if (Dequeue(queue, temp) == RETURN_ERR) return RETURN_ERR;
  }

  return RETURN_OK; 
}