#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "MLFQ.h"
void MLFQ (struct node **readyHead, Core *cores)
{
    int i;
    priorityQueue(&(*readyHead));
while(queueHead != NULL || High_Level !=NULL || Mid_Level !=NULL || Low_Level != NULL || runningHead != NULL || waitingHead !=NULL)
{
    while(queueHead != NULL  || High_Level !=NULL || runningHead != NULL || waitingHead !=NULL)
    {   printf("Ready   %d: ",Time);
        printready(queueHead);
        printf("High_Level   %d: ",Time);
        printList(High_Level);
        printf("Mid_Level   %d: ",Time);
        printList(Mid_Level);
        printf("Low_Level   %d: ",Time);
        printList(Low_Level);
        printf("Running %d: ",Time);
        printList(runningHead);
        printf("Waiting %d: ",Time);
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
                    cores[i].state = BUSY;
                    queueHead->p.coreID = i;
                    queueHead->p.state = RUNNING;
                    insertAtLast(queueHead->p, &runningHead);
                    deleteFirst(&queueHead);
                }
                else if(High_Level !=NULL)
                {
                    cores[i].state = BUSY;
                    High_Level->p.coreID = i;
                    High_Level->p.state = RUNNING;
                    insertAtLast(High_Level->p, &runningHead);
                    deleteFirst(&High_Level);
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
                insertAtLast((runningCurrent)->p,&remainHead);
                deleteNode(&runningHead, runningCurrent->p.ID);
                runningCurrent = runningHead;  
                continue;

            }
            else if(runningCurrent->p.RRcheckTime > quantum)
            {
                runningCurrent->p.RRcheckTime=0;
                cores[runningCurrent->p.coreID].state = EMPTY;
                runningCurrent->p.state = READY;
                insertAtLast((runningCurrent)->p,&Mid_Level);
                deleteNode(&runningHead, runningCurrent->p.ID);
                runningCurrent = runningHead; 
                continue;
            }
            else
            {
                if (runningCurrent->p.checkTime == runningCurrent->p.cpuTime)
                {
                    runningCurrent->p.checkTime = 0;
                    runningCurrent->p.RRcheckTime=0;
                    cores[runningCurrent->p.coreID].state = EMPTY;
                    runningCurrent->p.state = WAITING;
                    insertAtLast(runningCurrent->p, &waitingHead);
                    deleteNode(&runningHead, runningCurrent->p.ID);
                    if (queueHead != NULL && queueHead->p.arrivalTime <= Time) 
                    {  
                        if(queueHead->p.startTime==-1)
                        {
                            queueHead->p.startTime=Time;
                        }
                        cores[runningCurrent->p.coreID].state = BUSY;
                        queueHead->p.coreID = runningCurrent->p.coreID;
                        queueHead->p.state = RUNNING;
                        queueHead->p.brustTime--;
                        queueHead->p.checkTime++;
                        queueHead->p.RRcheckTime++;
                        insertAtLast(queueHead->p, &runningHead);
                        deleteFirst(&queueHead);
                    }
                    else if(High_Level !=NULL)
                    {
                        cores[runningCurrent->p.coreID].state = BUSY;
                        High_Level->p.coreID = runningCurrent->p.coreID;
                        High_Level->p.state = RUNNING;
                        High_Level->p.brustTime--;
                        High_Level->p.checkTime++;
                        High_Level->p.RRcheckTime++;
                        insertAtLast(High_Level->p, &runningHead);
                        deleteFirst(&High_Level);
                    }
                    
                }
                else
                {
                    runningCurrent->p.RRcheckTime++;
                    runningCurrent->p.brustTime--;
                    runningCurrent->p.checkTime++;
                }
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
            {       waitingCurrent->p.checkTime =0;
                    if(cores[waitingCurrent->p.coreID].state != EMPTY)
                    {
                        waitingCurrent->p.state = READY;
                        insertAtLast((waitingCurrent)->p,&High_Level);
                        deleteNode(&waitingHead, waitingCurrent->p.ID);
        
                    }
                    else
                    {
                        cores[waitingCurrent->p.coreID].state = BUSY;
                        waitingCurrent->p.state = RUNNING;
                         waitingCurrent->p.checkTime++;
                         waitingCurrent->p.brustTime--;
                        insertAtLast((waitingCurrent)->p,&runningHead);
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
       
        Time++;
        if(Time % Time_Level == 0)
        {
            struct node * level = Mid_Level;
            while(level != NULL)
            {
                insertAtLast(level->p,&High_Level);
                deleteFirst(&Mid_Level);
                level =level->next;
            }
            level = Low_Level;
            while(level != NULL)
            {
                insertAtLast(level->p,&High_Level);
                deleteFirst(&Low_Level);
                level =level->next;
            }
        }
        sleep(1);
        
    }
    while( (queueHead == NULL || queueHead->p.arrivalTime >Time) && Mid_Level !=NULL || runningHead != NULL || waitingHead !=NULL)
    {   
           printf("Ready   %d: ",Time);
        printready(queueHead);
        printf("High_Level   %d: ",Time);
        printList(High_Level);
        printf("Mid_Level   %d: ",Time);
        printList(Mid_Level);
        printf("Low_Level   %d: ",Time);
        printList(Low_Level);
        printf("Running %d: ",Time);
        printList(runningHead);
        printf("Waiting %d: ",Time);
        printList(waitingHead);
        printf("----------------------------------\n");
        for (i = 1; i <= NumberOfCores; i++) 
        {
            if (cores[i].state == EMPTY)
            {   
                if(Mid_Level !=NULL)
                {
                    cores[i].state = BUSY;
                    Mid_Level->p.coreID = i;
                    Mid_Level->p.state = RUNNING;
                    insertAtLast(Mid_Level->p, &runningHead);
                    deleteFirst(&Mid_Level);
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
                insertAtLast((runningCurrent)->p,&remainHead);
                deleteNode(&runningHead, runningCurrent->p.ID);
                runningCurrent = runningHead; 
                continue;

            }
            else if(runningCurrent->p.RRcheckTime > quantum)
            {
                runningCurrent->p.RRcheckTime=0;
                // runningCurrent->p.checkTime = 0;
                cores[runningCurrent->p.coreID].state = EMPTY;
                runningCurrent->p.state = READY;
                insertAtLast((runningCurrent)->p,&Low_Level);
                deleteNode(&runningHead, runningCurrent->p.ID);
                runningCurrent = runningHead; 
                continue;
            }
            else
            {
                if (runningCurrent->p.checkTime == runningCurrent->p.cpuTime)
                {
                    runningCurrent->p.checkTime = 0;
                    runningCurrent->p.RRcheckTime=0;
                    cores[runningCurrent->p.coreID].state = EMPTY;
                    runningCurrent->p.state = WAITING;
                    insertAtLast(runningCurrent->p, &waitingHead);
                    
                    if(Mid_Level !=NULL)
                    {
                        cores[runningCurrent->p.coreID].state = BUSY;
                        Mid_Level->p.coreID = runningCurrent->p.coreID;
                        Mid_Level->p.state = RUNNING;
                        Mid_Level->p.brustTime--;
                        Mid_Level->p.checkTime++;
                        Mid_Level->p.RRcheckTime++;
                        insertAtLast(Mid_Level->p, &runningHead);
                        deleteFirst(&Mid_Level);
                    }
                    deleteNode(&runningHead, runningCurrent->p.ID);
                }
                else
                {
                    runningCurrent->p.RRcheckTime++;
                    runningCurrent->p.brustTime--;
                    runningCurrent->p.checkTime++;
                }
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
                waitingCurrent = waitingHead;  // Reset to the head after deletion
                continue;
            }
            if (waitingCurrent->p.IOtime == waitingCurrent->p.checkTime) 
            {    waitingCurrent->p.checkTime =0;
                    if(cores[waitingCurrent->p.coreID].state != EMPTY)
                    {
                        waitingCurrent->p.state = READY;
                        insertAtLast((waitingCurrent)->p,&Mid_Level);
                        deleteNode(&waitingHead, waitingCurrent->p.ID);
                    }
                    else
                    {
                        cores[waitingCurrent->p.coreID].state = BUSY;
                        waitingCurrent->p.state = RUNNING;
                         waitingCurrent->p.checkTime++;
                         waitingCurrent->p.brustTime--;
                        insertAtLast((waitingCurrent)->p,&runningHead);
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
      
        Time++;
        if(Time % Time_Level == 0)
        {
            struct node * level = Mid_Level;
            while(level != NULL)
            {
                insertAtLast(level->p,&High_Level);
                deleteFirst(&Mid_Level);
                level =level->next;
            }
            level = Low_Level;
            while(level != NULL)
            {
                insertAtLast(level->p,&High_Level);
                deleteFirst(&Low_Level);
                level =level->next;
            }
            break;
        }
        sleep(1);
        
    }
    while((queueHead == NULL || queueHead->p.arrivalTime >Time)  && Low_Level !=NULL || runningHead != NULL || waitingHead !=NULL)
    {    
           printf("Ready   %d: ",Time);
        printready(queueHead);
        printf("High_Level   %d: ",Time);
        printList(High_Level);
        printf("Mid_Level   %d: ",Time);
        printList(Mid_Level);
        printf("Low_Level   %d: ",Time);
        printList(Low_Level);
        printf("Running %d: ",Time);
        printList(runningHead);
        printf("Waiting %d: ",Time);
        printList(waitingHead);
        printf("----------------------------------\n");
        for (i = 1; i <= NumberOfCores; i++) 
        {
            if (cores[i].state == EMPTY)
            {   
                if(Low_Level !=NULL)
                {
                    cores[i].state = BUSY;
                    Low_Level->p.coreID = i;
                    Low_Level->p.state = RUNNING;
                    insertAtLast(Low_Level->p, &runningHead);
                    deleteFirst(&Low_Level);
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
                insertAtLast((runningCurrent)->p,&remainHead);
                deleteNode(&runningHead, runningCurrent->p.ID);
                runningCurrent = runningHead;  // Reset to the head after deletion
                continue;

            }
            else
            {
                if (runningCurrent->p.checkTime == runningCurrent->p.cpuTime)
                {
                    runningCurrent->p.checkTime = 0;
                    runningCurrent->p.RRcheckTime=0;
                    cores[runningCurrent->p.coreID].state = EMPTY;
                    runningCurrent->p.state = WAITING;
                    insertAtLast(runningCurrent->p, &waitingHead);
                    
                    if(Low_Level !=NULL)
                    {
                        cores[runningCurrent->p.coreID].state = BUSY;
                        Low_Level->p.coreID = runningCurrent->p.coreID;
                        Low_Level->p.state = RUNNING;
                        Low_Level->p.brustTime--;
                        Low_Level->p.checkTime++;
                        Low_Level->p.RRcheckTime++;
                        insertAtLast(Low_Level->p, &runningHead);
                        deleteFirst(&Low_Level);
                    }
                    deleteNode(&runningHead, runningCurrent->p.ID);
                }
                else
                {
                    runningCurrent->p.RRcheckTime++;
                    runningCurrent->p.brustTime--;
                    runningCurrent->p.checkTime++;
                }
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
                waitingCurrent = waitingHead;  // Reset to the head after deletion
                continue;
            }
            if (waitingCurrent->p.IOtime == waitingCurrent->p.checkTime) 
            {         waitingCurrent->p.checkTime =0;
                    if(cores[waitingCurrent->p.coreID].state != EMPTY)
                    {
                        waitingCurrent->p.state = READY;
                       
                        insertAtLast((waitingCurrent)->p,&Low_Level);
                        deleteNode(&waitingHead, waitingCurrent->p.ID);
                    }
                    else
                    {
                        cores[waitingCurrent->p.coreID].state = BUSY;
                        waitingCurrent->p.state = RUNNING;
                          waitingCurrent->p.checkTime++;
                         waitingCurrent->p.brustTime--;
                        insertAtLast((waitingCurrent)->p,&runningHead);
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
        
        Time++;
        if(Time % Time_Level == 0)
        {
            struct node * level = Mid_Level;
            while(level != NULL)
            {
                insertAtLast(level->p,&High_Level);
                deleteFirst(&Mid_Level);
                level =level->next;
            }
            level = Low_Level;
            while(level != NULL)
            {
                insertAtLast(level->p,&High_Level);
                deleteFirst(&Low_Level);
                level =level->next;
            }
            break;
        }
        sleep(1);
        
    }
}
        printf("Ready   %d: ",Time);
        printready(queueHead);
        printf("High_Level   %d: ",Time);
        printList(High_Level);
        printf("Mid_Level   %d: ",Time);
        printList(Mid_Level);
        printf("Low_Level   %d: ",Time);
        printList(Low_Level);
        printf("Running %d: ",Time);
        printList(runningHead);
        printf("Waiting %d: ",Time);
        printList(waitingHead);
        printf("----------------------------------\n");

}
