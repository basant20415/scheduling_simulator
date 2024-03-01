#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "RR.h"
void RR (struct node **readyHead, Core *cores)
{
    int i;
    enqueuePriority(&(*readyHead));
    *readyHead = NULL;
    while (queueHead != NULL || waitingHead != NULL || runningHead != NULL ||  *readyHead != NULL ) 
    {  
         printf("Ready   %d: ", Time);
        printready(queueHead);
        printf("Ready   %d: ", Time);
        printList(*readyHead);
        printf("Running %d: ", Time);
        printList(runningHead);
        printf("Waiting %d: ", Time);
        printList(waitingHead);
        printf("----------------------------------\n");
       
        for (i = 1; i <= NumberOfCores; i++) {
            if (cores[i].state == EMPTY )
            {      
                if(queueHead != NULL){
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
               if(*readyHead != NULL){
                    (*readyHead)->p.state = RUNNING;
                    (*readyHead)->p.coreID = runningCurrent->p.coreID;
                    (*readyHead)->p.RRcheckTime++;
                    (*readyHead)->p.checkTime++;
                    (*readyHead)->p.brustTime--;
                    cores[runningCurrent->p.coreID].state = BUSY;
                    insertAtLast( (*readyHead)->p, &runningHead);
                    deleteFirst(readyHead);
               }
            }
        }
        runningCurrent = runningHead;
        while (runningCurrent != NULL) {
            if (runningCurrent->p.brustTime == 0) {
                runningCurrent->p.endTime = Time;
                cores[runningCurrent->p.coreID].state = EMPTY;
                insertAtLast((runningCurrent)->p,&remainHead);
                deleteNode(&runningHead, runningCurrent->p.ID);
                runningCurrent = runningHead;
                continue;
            }
            if (runningCurrent->p.checkTime == runningCurrent->p.cpuTime)
            {
                runningCurrent->p.checkTime = 0;
                cores[runningCurrent->p.coreID].state = EMPTY;
                runningCurrent->p.state = WAITING;
                if (queueHead != NULL && queueHead->p.arrivalTime <= Time ) 
                {
                    if(queueHead->p.startTime==-1)
                    {
                        queueHead->p.startTime=Time;
                    }
                    queueHead->p.state = RUNNING;
                    queueHead->p.coreID = runningCurrent->p.coreID;
                    queueHead->p.RRcheckTime++;
                    queueHead->p.checkTime++;
                    queueHead->p.brustTime--;
                    cores[runningCurrent->p.coreID].state = BUSY;
                    insertAtLast(queueHead->p, &runningHead);
                    deleteFirst(&queueHead);
                } else if(*readyHead != NULL){
                    
                    (*readyHead)->p.state = RUNNING;
                    (*readyHead)->p.coreID = runningCurrent->p.coreID;
                    (*readyHead)->p.RRcheckTime++;
                    (*readyHead)->p.checkTime++;
                    (*readyHead)->p.brustTime--;
                    cores[runningCurrent->p.coreID].state = BUSY;
                    insertAtLast( (*readyHead)->p, &runningHead);
                    deleteFirst(readyHead);
                }
                insertAtLast(runningCurrent->p, &waitingHead);
                deleteNode(&runningHead, runningCurrent->p.ID);
            }
            else if(runningCurrent->p.RRcheckTime == quantum)
            {
                runningCurrent->p.RRcheckTime =0;
                runningCurrent->p.state = READY;
                if (queueHead != NULL && queueHead->p.arrivalTime <= Time) 
                {
                    if(queueHead->p.startTime==-1)
                    {
                        queueHead->p.startTime=Time;
                    }
                    queueHead->p.state = RUNNING;
                    queueHead->p.coreID = runningCurrent->p.coreID;
                    queueHead->p.RRcheckTime++;
                    queueHead->p.checkTime++;
                    queueHead->p.brustTime--;
                    cores[runningCurrent->p.coreID].state = BUSY;
                    insertAtLast(queueHead->p, &runningHead);
                    deleteFirst(&queueHead);
                }
                else if(*readyHead != NULL)
                {
                    (*readyHead)->p.state = RUNNING;
                    (*readyHead)->p.coreID = runningCurrent->p.coreID;
                    (*readyHead)->p.RRcheckTime++;
                    (*readyHead)->p.checkTime++;
                    (*readyHead)->p.brustTime--;
                    cores[runningCurrent->p.coreID].state = BUSY;
                    insertAtLast((*readyHead)->p, &runningHead);
                    deleteFirst(readyHead);
                }
                insertAtLast(runningCurrent->p,readyHead);
                deleteNode(&runningHead, runningCurrent->p.ID);
            }
            else
            {
                runningCurrent->p.RRcheckTime++;
                runningCurrent->p.brustTime--;
                runningCurrent->p.checkTime++;
            }
            runningCurrent = runningCurrent->next;
        }
        waitingCurrent = waitingHead;
        while (waitingCurrent != NULL) {
            if (waitingCurrent->p.brustTime == 0) {
                waitingCurrent->p.endTime = Time;
                cores[waitingCurrent->p.coreID].state = EMPTY;
                insertAtLast((waitingCurrent)->p,&remainHead);
                deleteNode(&waitingHead, waitingCurrent->p.ID);
                waitingCurrent = waitingHead;  
                continue;
            }
            if (waitingCurrent->p.IOtime == waitingCurrent->p.checkTime) {
                waitingCurrent->p.checkTime = 0;
                if (cores[waitingCurrent->p.coreID].state == EMPTY && waitingCurrent->p.RRcheckTime != quantum)
                {
                    waitingCurrent->p.state = RUNNING;
                    cores[waitingCurrent->p.coreID].state = BUSY;
                    waitingCurrent->p.RRcheckTime++;
                    waitingCurrent->p.brustTime--;
                    waitingCurrent->p.checkTime++;
                    insertAtLast(waitingCurrent->p, &runningHead);
                    deleteNode(&waitingHead, waitingCurrent->p.ID);
                }else if(cores[waitingCurrent->p.coreID].state != EMPTY && waitingCurrent->p.RRcheckTime != quantum){
                        for (i = 1; i <= NumberOfCores; i++) {
                         if (cores[i].state == EMPTY){
                         waitingCurrent->p.state = RUNNING;
                         cores[i].state = BUSY;
                         waitingCurrent->p.RRcheckTime++;
                         waitingCurrent->p.brustTime--;
                         waitingCurrent->p.checkTime++;
                         insertAtLast(waitingCurrent->p, &runningHead);
                         deleteNode(&waitingHead, waitingCurrent->p.ID);
                          break;
                         }
                        }
                         if(waitingCurrent->p.state == WAITING){
                              waitingCurrent->p.state = READY;
                              waitingCurrent->p.RRcheckTime =0;
                             insertAtLast(waitingCurrent->p, readyHead);
                             deleteNode(&waitingHead, waitingCurrent->p.ID);
                    }
                }
                else
                {
                    waitingCurrent->p.RRcheckTime =0;
                    waitingCurrent->p.state = READY;
                    insertAtLast(waitingCurrent->p,readyHead);
                    deleteNode(&waitingHead, waitingCurrent->p.ID);
                }
            }
            else
            {
                waitingCurrent->p.checkTime++;
                waitingCurrent->p.brustTime--;
            }
            waitingCurrent = waitingCurrent->next;
        }
        ++Time;
        sleep(1);
    }
}