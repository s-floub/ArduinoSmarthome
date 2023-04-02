#ifndef MESSAGEQUEUE_H
#define MESSAGEQUEUE_H
#include "transmission.h"

//Queue function definitions are more or less taken from MREN 178 lab 2

typedef struct qItem	{   //struct of items to add to linked list implimented queue
	struct qItem	*next;  //Next item in the queue
	Message message;        //The actual message portion of the item
} QueueItem, *pQueueItem;

typedef struct queue{
	pQueueItem front;	// Pointer to front queue element
	pQueueItem back;	// Pointer to back queue element
	int		count;		// Number of items in queue
} Queue, *pQueue;


// creates and returns pointer to a queue 
pQueue CreateQueue (void);

//creates and returns pointer to a queueItem
pQueueItem CreateItem (Message message);

//checks to see if queue is empty
bool IsQEmpty (pQueue queue);

// enqueues an item into queue, returning 0 if succeeding, and -1 if not.
int Enqueue (pQueue queue, pQueueItem item);

// dequeues an item from the queue, returning 0 if succeeding, and 
// -1 if not.
int Dequeue (pQueue queue, Message &messageReturn);

// dequques all items from a queue, returning 0 if succeeding, and 
// -1 if not.
int DequeueAll (pQueue queue);

#endif