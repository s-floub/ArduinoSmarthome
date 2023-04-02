#include "mainBoard.h"


pList initialiseList(){
    pList list = (pList) malloc(sizeof(List));
    list->head = NULL;  //I don't know why this is required as it's defined default
    list->tail = NULL;  //in the struct but the linked list breaks without it so it stays

    return list;
}


Device* createDeviceNode (productType productWhat, char productNum[3], deviceType sensor) {
    // Declaration of local node pointer, with mem allocation
    Device *p_new = (Device *) malloc (sizeof (Device));

    if (p_new != NULL) {
        //If malloc was successful, assign data to Device
        p_new->productWhat = productWhat;

        p_new->productNum[0] = productNum[0];
        p_new->productNum[1] = productNum[1];
        p_new->productNum[2] = '\0';

        p_new->sensor = sensor;

        p_new->pNextDevice = NULL;

        if(DEBUG) Serial.println("Device Init");
        if(DEBUG) Serial.print("p_new->productWhat ");
        if(DEBUG) Serial.println((char) p_new->productWhat);
        if(DEBUG) Serial.print("p_new->productNum ");
        if(DEBUG) Serial.println(p_new->productNum);
        if(DEBUG) Serial.print("p_new->sensor ");
        if(DEBUG) Serial.println((char) p_new->sensor);

    }

    return p_new;
}


int insertNode (pList ourList, productType productWhat, char productNum[3], deviceType sensor){

    Device *p_new = createDeviceNode (productWhat, productNum, sensor);

    //Check Device was created successfully
    if (p_new == NULL) {
        if(DEBUG) Serial.println("p_new == NULL");
        return RETURN_ERR;
    }

    //If list is empty, new Device becomes head and tail
    if (ourList->head == NULL) {
          ourList->head = p_new;
          ourList->tail = p_new;
          if(DEBUG) Serial.println("INSERTED AT HEAD AND TAIL");
          return RETURN_OK;
    }

    ourList->tail->pNextDevice = p_new;
    ourList->tail = p_new;
    // Point the old tail to our new Node, then point the tail pointer to the new Node
  
        if(DEBUG) Serial.println("Device Inserted");

    return RETURN_OK;
}


int deleteDevice (pList ourList, productType productWhat, char productNum[3], deviceType sensor) {
    Device *p_temp;
    Device *p_node;

    p_temp = ourList->head;   // pointer to first node
    p_node = ourList->head->pNextDevice; // pointer to node after head node

    //Assemple Device to compare data with
    //Not using createDeviceNode() as that would use unnessasary heap allocation
    Device p_compare;

    p_compare.productWhat = productWhat;

    p_compare.productNum[0] = productNum[0];
    p_compare.productNum[1] = productNum[1];

    p_compare.sensor = sensor;

    p_compare.pNextDevice = NULL; 

    //Check if head matches device to delete
    if(!compareDevices(ourList->head, &p_compare)){
        //If matches delete and reasign head
      ourList->head = p_node;
      free(p_temp);
      return RETURN_OK;
    }
    
    // Loop through all nodes
    while (p_temp != NULL) {

       //Check if p_node matches comparison Device 
       if (!compareDevices(p_node, &p_compare)) {
           // Case matched, delete p_node

           if(p_node == ourList->tail){
            ourList->tail = p_temp;
            // point tail pointer to Node before
           }

           // Point Node behind deleting Node to Node ahead of deleting Node
           p_temp->pNextDevice = p_node->pNextDevice;

           free(p_node); // frees that node from memory
           
           return RETURN_OK; 

        }
       
        // if we've reached here, node was not found yet Inciment pointers
    
       p_temp = p_temp->pNextDevice;
       p_node = p_node->pNextDevice;
    }
    // get here only if we searched whole list and found nothing
    return RETURN_ERR;
}


int compareDevices(Device* first, Device* second){

    if(first->productWhat == second->productWhat && first->sensor == second->sensor && !strcmp(first->productNum, second->productNum)){
        return 0;
    }

    return 1;

}


int queryDevice(Device* pDevice, pQueue messageQueue){

        if(DEBUG) Serial.println("Device Query");
        if(DEBUG) Serial.print("pDevice->productWhat ");
        if(DEBUG) Serial.println((char) pDevice->productWhat);
        if(DEBUG) Serial.print("pDevice->productNum ");
        if(DEBUG) Serial.println(pDevice->productNum);
        if(DEBUG) Serial.print("pDevice->sensor ");
        if(DEBUG) Serial.println((char) pDevice->sensor);

    //Create int to keep track of how many messages we get
    int howManyReturns = 0;

    //Assemble Request with desired paramaters
    Request ourRequest;

    ourRequest.type = request;

    ourRequest.destination[0] = (char) pDevice->productWhat;
    ourRequest.destination[1] = (char) pDevice->productNum[0];
    ourRequest.destination[2] = (char) pDevice->productNum[1];
    ourRequest.destination[3] = '\0';

    ourRequest.device = pDevice->sensor;

    if(DEBUG) Serial.print("Sending destination ");
    if(DEBUG) Serial.println(ourRequest.destination);

    //Send Request on HC12
    sendMessage(createMessage(main, request, ourRequest));

    //Log when sent (relative to Arduino running time so far)
    int sentTime = millis();

    //While waiting for return message
    while(sentTime - millis() < MESSAGETIMEOUT && HC12.available() < MINMESSAGELEN){
        delay(100);
    }

    //While there are messages avalible, recive those messages to our queue
    while (HC12.available() >= MINMESSAGELEN){
        reciveMessageToQueue(messageQueue);
    }

    //Read all messages in the queue
    while(messageQueue->count > 0){
        Message incomingMessage;
        Dequeue(messageQueue, incomingMessage);
        dealWithMessage(incomingMessage);
        howManyReturns++;
    }

    if(DEBUG) Serial.println("Query Done");

    return howManyReturns;

}


int queryList(pList list, pQueue messageQueue){
    
    //Start with head
    Device* pDevice = list->head;

    //Keep track of how many messages are returned
    int messagesReturned = 0;

    if(DEBUG) Serial.println("Function Called");

    //while we have not reached the end of the list
    while(pDevice != NULL){

        if(DEBUG) Serial.println("Sending Query");

        //queryDevice and add to messagesReturned
        messagesReturned += queryDevice(pDevice, messageQueue);

        //incriment device pointer
        pDevice = pDevice->pNextDevice;

        //delay for allowing time for other boards to process / preventing message overlap
        delay(300);
    }

    return messagesReturned;
}
