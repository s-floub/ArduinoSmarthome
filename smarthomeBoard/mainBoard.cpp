#ifndef MAINBOARD_H
#define MAINBOARD_H
#include "mainBoard.h"
#endif


// Function to create a new node
Device* createDeviceNode (productType productWhat, char productNum[3], deviceType sensor) {
    // Declaration of local node pointer, with mem allocation
    Device *p_new = (Device*) malloc (sizeof (Device));

    if (p_new != NULL) {
        // Now fill with data. Notice the syntax. You can read this as "assign 
        // value in d to data_val field in Node pointed to by p_new".
        p_new->productWhat = productWhat;

        p_new->productNum[0] = productNum[0];
        p_new->productNum[1] = productNum[1];

        p_new->sensor = sensor;

        p_new->pNextDevice = NULL; 
    }
    
    // Get here with pn pointing to a filled-in structure, or with p_new equal
    // to NULL because malloc() failed. In either case, return p_new.
    return p_new;
}


// Function to insert node at front of the list
int insertNode (List ourList, productType productWhat, char productNum[3], deviceType sensor){  //Function from Lab 1

    Device *p_new = createDeviceNode (productWhat, productNum, sensor);
    // Make sure node creation worked

    if (p_new == NULL) {
        return -1;
    }
    // New node goes at back of list. If the list is currently empty,
    // then both the head and back pointers need to be updated.

    if (ourList.head == NULL) {
          ourList.head = p_new;
          ourList.tail = p_new;
          return 0;
    }
    // If list is empty, inserting at tail is same as at head

    ourList.tail->pNextDevice = p_new;
    ourList.tail = p_new;
    // Point the old tail to our new Node, then point the tail pointer to the new Node
  
    return 0;
}


// Find node that holds the value val, and remove the node from the list, 
// without breaking the list of course. Returns ITEM_ERROR if the number 
// could not be found, and EXIT_OK if the item was found and deleted.
int deleteDevice (List ourList, productType productWhat, char productNum[3], deviceType sensor) {
    Device *p_temp;
    Device *p_node;

    p_temp = ourList.head;   // pointer to first node
    p_node = ourList.head->pNextDevice; // pointer to pointer that points to first node

    Device p_compare;

    p_compare.productWhat = productWhat;

    p_compare.productNum[0] = productNum[0];
    p_compare.productNum[1] = productNum[1];

    p_compare.sensor = sensor;

    p_compare.pNextDevice = NULL; 

    if(!compareDevices(ourList.head, &p_compare)){
      ourList.head = p_node;
      free(p_temp);
      return 0;
    }
        // If the head matches the value, redirect the head pointer to the next Node and then free the old head Node
    
    // Loop through all nodes
    while (p_temp != NULL) {
        // Does ptemp point to node with sought value?

      
       if (!compareDevices(p_node, &p_compare)) {
           // Yes it does. This is node we want to delete from list.
           
           // Remember: it's possible that we're deleting the first, a middle,
           // or the last node in the list. It is also possible we're about to
           // delete the only item in the list. Handle each of these cases. 
            

           if(p_node == ourList.tail){
            ourList.tail = p_temp;
            // point tail pointer to Node before
           }

           p_temp->pNextDevice = p_node->pNextDevice;
           // Point Node behind deleting Node to Node ahead of deleting Node
           free(p_node); // frees that node from memory
           
           return 0; 

        }
       
        // if we've reached here, node was not found yet - move on to next node
    
       p_temp = p_temp->pNextDevice;
       p_node = p_node->pNextDevice;
       // Inciment pointers
    }
    // get here only if we searched whole list and found nothing
    return -1;
}

//Compares two device pointers, returns 0 if same
int compareDevices(Device* first, Device* second){

    if(first->productWhat == second->productWhat && first->sensor == second->sensor && !strcmp(first->productNum, second->productNum)){
        return 0;
    }

    return 1;

}

int queryDevice(Device* device){
    Request ourRequest;

    ourRequest.type = request;

    ourRequest.destination[0] = device->productWhat;
    ourRequest.destination[1] = device->productNum[0];
    ourRequest.destination[2] = device->productNum[1];
    ourRequest.destination[3] = '\0';

    ourRequest.device = device->sensor;

    sendMessage(createMessage(main, request, ourRequest));
    int sentTime = millis();

    while(sentTime - millis() < MESSAGETIMEOUT && HC12.available() < 8){ //While waiting for return message
        delay(100);
    }

    if (HC12.available() >= 8){
        //Fill up queue and process all Messages in Queue
    }

}

int queryList(List list){
    
    Device* device = list.head;

    int nodesReturned = 0;

    while(device != NULL){
        queryDevice(device);

        nodesReturned++;
        device = device->pNextDevice;
        delay(250);
    }

    return nodesReturned;
}
