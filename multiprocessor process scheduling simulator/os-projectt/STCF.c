#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "STCF.h"
/************************** Functions Definations *********************************/
void STCF(struct node **readyHead, Core *cores)
{
    int i;
    enqueuePriority(&(*readyHead));
    (*readyHead) = NULL;

    while (queueHead != NULL || waitingHead != NULL || runningHead != NULL || *readyHead !=NULL) 
    {    
        struct node *new = queueHead;
        while (new != NULL){
            if(new->p.arrivalTime <= Time){
                insertAtLast(new->p,readyHead);
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
                shortest(readyHead,Time,i);
                cores[i].state = BUSY;
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
            if (runningCurrent->p.checkTime == runningCurrent->p.cpuTime && runningCurrent->p.IOtime != 0) {
                runningCurrent->p.checkTime = 0;
                cores[runningCurrent->p.coreID].state = EMPTY;
                runningCurrent->p.state = WAITING;
                insertAtLast(runningCurrent->p, &waitingHead);
                deleteNode(&runningHead, runningCurrent->p.ID);
                if ((*readyHead) != NULL) 
                {  
                    shortest(readyHead,Time,runningCurrent->p.coreID);
                    cores[runningCurrent->p.coreID].state = BUSY;
                    runningCurrent->p.brustTime--;
                    runningCurrent->p.checkTime++;
                }
               
            }   
             else {
                update(readyHead,Time,&runningCurrent);
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
                int result = update(readyHead,Time,&waitingCurrent);
                if(result == FALSE){
                    if (cores[waitingCurrent->p.coreID].state == EMPTY) {
                         waitingCurrent->p.state = RUNNING;
                         cores[waitingCurrent->p.coreID].state = BUSY;
                         waitingCurrent->p.brustTime--;
                         waitingCurrent->p.checkTime++;
                         insertAtLast(waitingCurrent->p, &runningHead);
                        deleteNode(&waitingHead, waitingCurrent->p.ID);
                    }   else {
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
                             insertAtLast(waitingCurrent->p, readyHead);
                             deleteNode(&waitingHead, waitingCurrent->p.ID);
                           }
                    }
                } else {
                    waitingCurrent->p.state = READY;
                    enqPriority(readyHead,waitingCurrent->p);
                    deleteNode(&waitingHead, waitingCurrent->p.ID);
                }
            } else {
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
