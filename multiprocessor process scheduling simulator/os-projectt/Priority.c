#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "Priority.h"
/************************** Functions Definations *********************************/
void PrioritySchedular (struct node **readyHead, Core *cores) {
     int i;
    priorityQueue(&(*readyHead));//queue head ->priority
     (*readyHead) = NULL;
    while (queueHead != NULL || waitingHead != NULL || runningHead != NULL || (*readyHead) != NULL) 
    {  
        
        struct node *new = queueHead;
        while (new != NULL){
            if(new->p.arrivalTime <= Time){
                Priority(readyHead,new->p);
                deleteNode(&queueHead,new->p.ID);
            }
             
            new = new->next;
        }
        printf("Ready   %d: ", Time);
        printList(*readyHead);
        printf("Running %d: ", Time);
        printList(runningHead);
        printf("Waiting %d: ", Time);
        printList(waitingHead);
        printf("----------------------------------\n");
        for (i = 1; i <= NumberOfCores; i++) 
        {
            if (cores[i].state == EMPTY && (*readyHead) != NULL)
            {   
                
                if((*readyHead)->p.startTime==-1)
                {
                    (*readyHead)->p.startTime=Time;
                }
                cores[i].state = BUSY;
                (*readyHead)->p.coreID = i;
                (*readyHead)->p.state = RUNNING;
                insertAtLast((*readyHead)->p, &runningHead);
                deleteFirst(readyHead);
                
            }
        }
        runningCurrent = runningHead;
        while (runningCurrent != NULL)
        {
            if (runningCurrent->p.brustTime == 0)
            {
                runningCurrent->p.endTime =Time;
                cores[runningCurrent->p.coreID].state = EMPTY;
                insertAtLast((runningCurrent)->p,&remainHead);
                deleteNode(&runningHead, runningCurrent->p.ID);
                runningCurrent = runningHead;  
                continue;
            }
            if (runningCurrent->p.checkTime == runningCurrent->p.cpuTime && runningCurrent->p.IOtime != 0) {
                runningCurrent->p.checkTime = 0;
                cores[runningCurrent->p.coreID].state = EMPTY;
                runningCurrent->p.state = WAITING;
                insertAtLast(runningCurrent->p, &waitingHead);

                if ((*readyHead) != NULL) 
                {  
                    if((*readyHead)->p.startTime==-1)
                {
                    (*readyHead)->p.startTime=Time;
                }
                    cores[runningCurrent->p.coreID].state = BUSY;
                    (*readyHead)->p.coreID = runningCurrent->p.coreID;
                    (*readyHead)->p.state = RUNNING;
                    (*readyHead)->p.brustTime--;
                    (*readyHead)->p.checkTime++;
                    insertAtLast((*readyHead)->p, &runningHead);
                    deleteFirst(readyHead);
                }
                 deleteNode(&runningHead, runningCurrent->p.ID);
            } else if( (*readyHead)!= NULL && (*readyHead)->p.Priority > runningCurrent->p.Priority ){
                 runningCurrent->p.state = READY;
                 Priority(readyHead,runningCurrent->p);
                 deleteNode(&runningHead, runningCurrent->p.ID);
                 if((*readyHead)->p.startTime==-1)
                {
                    (*readyHead)->p.startTime=Time;
                }
                    (*readyHead)->p.coreID = runningCurrent->p.coreID;
                    (*readyHead)->p.state = RUNNING;
                    (*readyHead)->p.brustTime--;
                    (*readyHead)->p.checkTime++;
                    insertAtLast((*readyHead)->p, &runningHead);
                    deleteFirst(readyHead);
                }
            else
            {
                runningCurrent->p.brustTime--;
                runningCurrent->p.checkTime++;
            }
            runningCurrent = runningCurrent->next;
          
        }
        waitingCurrent = waitingHead;
        while (waitingCurrent != NULL)
        {
            if (waitingCurrent->p.brustTime == 0)
            {
                waitingCurrent->p.endTime = Time;
                cores[waitingCurrent->p.coreID].state = EMPTY;
                insertAtLast((waitingCurrent)->p,&remainHead);
                deleteNode(&waitingHead, waitingCurrent->p.ID);
                waitingCurrent = waitingHead; 
                continue;
            }
            if (waitingCurrent->p.IOtime == waitingCurrent->p.checkTime) 
            {        waitingCurrent->p.checkTime = 0;
                if(cores[waitingCurrent->p.coreID].state != EMPTY){
                    current = runningHead;
                    while(current != NULL){
                        if(current->p.coreID == waitingCurrent->p.coreID && waitingCurrent->p.Priority > current->p.Priority ){
                            current->p.state = READY;
                            Priority(readyHead,current->p);
                            deleteNode(&runningHead,current->p.ID);
                            waitingCurrent->p.state = RUNNING;
                            waitingCurrent->p.brustTime--;
                            waitingCurrent->p.checkTime++;
                            cores[waitingCurrent->p.coreID].state = BUSY;
                            insertAtLast(waitingCurrent->p, &runningHead);
                            deleteNode(&waitingHead, waitingCurrent->p.ID); 
                            break;
                        }
                        current = current->next;
                    }
                     if(current == NULL){
                            for (i = 1; i <= NumberOfCores; i++) {
                               if (cores[i].state == EMPTY){
                                 waitingCurrent->p.state = RUNNING;
                                 cores[i].state = BUSY;
                                  waitingCurrent->p.brustTime--;
                                 waitingCurrent->p.checkTime++;
                                 insertAtLast(waitingCurrent->p, &runningHead);
                                  deleteNode(&waitingHead, waitingCurrent->p.ID);
                                  break;
                               }
                            }
                            if(waitingCurrent->p.state == WAITING){
                              waitingCurrent->p.state = READY;
                             Priority(readyHead,waitingCurrent->p);
                             deleteNode(&waitingHead, waitingCurrent->p.ID);
                            }
                        }
                         
                    } else{
                    waitingCurrent->p.checkTime = 0;
                    waitingCurrent->p.state = RUNNING;
                    waitingCurrent->p.brustTime--;
                    waitingCurrent->p.checkTime++;
                    cores[waitingCurrent->p.coreID].state = BUSY;
                    insertAtLast(waitingCurrent->p, &runningHead);
                    deleteNode(&waitingHead, waitingCurrent->p.ID); 
                    
                }   
                    
            } 
            else {
                waitingCurrent->p.checkTime++;
                waitingCurrent->p.brustTime--;
            }
            waitingCurrent = waitingCurrent->next;
        }
        ++Time;
        sleep(1);
    }
        printf("Ready   %d: ", Time);
        printList(*readyHead);
        printf("Running %d: ", Time);
        printList(runningHead);
        printf("Waiting %d: ", Time);
        printList(waitingHead);
        printf("----------------------------------\n");
}
