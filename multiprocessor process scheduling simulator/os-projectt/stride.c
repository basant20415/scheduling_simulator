#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include "stride.h"
void stride (struct node **readyHead, Core *cores){

 enqueuePriority(&(*readyHead));
 struct node *loop = queueHead;
    int i;
    while (queueHead != NULL || waitingHead != NULL || runningHead != NULL || stridehead!=NULL ) 
    {
        printf("Ready   %d: ", Time);
        printready(queueHead);
        printf("Running %d: ", Time);
        printList(runningHead);
        printf("stride %d: ", Time);
        printList(stridehead);
        printf("Waiting %d: ", Time);
        printList(waitingHead);
        printf("----------------------------------\n");
        for (i = 1; i <= NumberOfCores; i++) 
        {
             if (cores[i].state == EMPTY)
            {   
                if(queueHead != NULL && queueHead->p.arrivalTime <= Time)
                {
                    if(queueHead->p.startTime==-1)
                    {
                        queueHead->p.startTime=Time;
                    }
                    queueHead->p.coreID = i;
                    queueHead->p.state = RUNNING;
                    insertAtLast(queueHead->p, &runningHead);
                    deleteFirst(&queueHead);
                    cores[i].state = BUSY;
                }
                else if(stridehead !=NULL)
                {
                    cores[i].state = BUSY;
                    stridehead->p.coreID = i;
                    stridehead->p.state = RUNNING;
                    insertAtLast(stridehead->p, &runningHead);
                    deleteFirst(&stridehead);
                }
            }
        }
        runningCurrent = runningHead;
        while (runningCurrent != NULL)
        {    
            if (runningCurrent->p.brustTime == 0)
            {
                runningCurrent->p.endTime = Time;
                cores[runningCurrent->p.coreID].state = EMPTY;
                insertAtLast(runningCurrent->p,&remainHead);
                deleteNode(&runningHead, runningCurrent->p.ID);
                runningCurrent = runningHead; 
                continue;
            }
            if (runningCurrent->p.checkTime == runningCurrent->p.cpuTime && runningCurrent->p.IOtime != 0) {
                runningCurrent->p.checkTime = 0;
                cores[runningCurrent->p.coreID].state = EMPTY;
                runningCurrent->p.state = WAITING;
                insertAtLast(runningCurrent->p, &waitingHead);
                if(queueHead!=NULL && queueHead->p.arrivalTime<=Time ){
                     printf("brust = %d , id = %d\n",runningCurrent->p.brustTime , runningCurrent->p.ID);
                   if(queueHead->p.startTime==-1)
                    {
                        queueHead->p.startTime=Time;
                    }
                    cores[runningCurrent->p.coreID].state = BUSY;
                    queueHead->p.coreID = runningCurrent->p.coreID;
                    queueHead->p.state = RUNNING;
                     queueHead->p.pass += queueHead->p.stride;
                     queueHead->p.brustTime--;
                     queueHead->p.checkTime++;
                     insertAtLast(queueHead->p, &runningHead);
                     deleteFirst(&queueHead);
                       
                }else if(stridehead!=NULL){

                 
                    cores[runningCurrent->p.coreID].state = BUSY;
                    stridehead->p.coreID = runningCurrent->p.coreID;
                    stridehead->p.state = RUNNING;
                    stridehead->p.pass += stridehead->p.stride;
                    stridehead->p.brustTime--;
                    stridehead->p.checkTime++;
                    insertAtLast(stridehead->p, &runningHead);
                    deleteFirst(&stridehead);
                }
                 deleteNode(&runningHead, runningCurrent->p.ID);
                  
            }
            else
            {
                runningCurrent->p.pass+= runningCurrent->p.stride;
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
                insertAtLast(waitingCurrent->p,&remainHead);
                deleteNode(&waitingHead, waitingCurrent->p.ID);
                waitingCurrent = waitingHead; 
                continue;
            }
            if (waitingCurrent->p.IOtime == waitingCurrent->p.checkTime) 
            {
                    waitingCurrent->p.checkTime = 0;
                    stridePriority(&waitingCurrent,waitingCurrent->p.pass) ;
                    deleteNode(&waitingHead, waitingCurrent->p.ID);
            }
            
            else {
                waitingCurrent->p.checkTime++;
                waitingCurrent->p.brustTime--;
                }
            waitingCurrent = waitingCurrent->next;
        }
        Time++;
        sleep(1);
    }
        printf("Ready   %d: ",Time);
        printList(queueHead);
        printf("Running %d: ",Time);
        printList(runningHead);
        printf("stride %d: ",Time);
        printList(stridehead);
        printf("Waiting %d: ",Time);
        printList(waitingHead);
        printf("----------------------------------\n");
}

