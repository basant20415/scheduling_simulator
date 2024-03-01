#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "common.h"
/************************** Global Variables *********************************/
struct node *readyHead = NULL;
struct node *runningHead = NULL;
struct node *remainHead = NULL;
struct node *waitingHead = NULL;
struct node *queueHead = NULL;
struct node *current = NULL;
struct node* stridehead=NULL;
struct node *runningCurrent = NULL;
struct node *waitingCurrent = NULL;
struct node *High_Level = NULL;
struct node *Mid_Level = NULL;
struct node *Low_Level = NULL;
unsigned int Time  = 0;
float avg_response= 0.0,avg_turnround= 0.0;
int NumberOfCores;
/************************** Functions Definations *********************************/
/* Description: 
     Function used to insert Process in the last of certain list
*/
void insertAtLast(Process vv, struct node **head) {

    struct node *link = (struct node *)malloc(sizeof(struct node));
    if (link == NULL) {
        exit(EXIT_FAILURE);
    }

    link->p = vv;
    link->next = NULL;

    if (*head == NULL) {
        *head = link;
        return;
    }

    current = *head;

    while (current->next != NULL) {
        current = current->next;
    }

    current->next = link;
}
/* Description: 
     Function used to delete first Process in the last of certain list
*/
void deleteFirst(struct node **head) {
    if (*head == NULL) {
        return;
    }

    struct node *temp = *head;
    *head = (*head)->next;
    free(temp);
}
/*Description:
      Function used to delete any required process from certain list
*/
void deleteNode(struct node **head, int key) {
    if (*head == NULL) {
        return;
    }

    if ((*head)->p.ID == key) {
        struct node *temp = *head;
        *head = (*head)->next;
        free(temp);
        return;
    }

    struct node *current_del = *head;
    struct node *prev = NULL;

    while (current_del != NULL && current_del->p.ID != key) {
        prev = current_del;
        current_del = current_del->next;
    }

    if (current_del == NULL) {
        return;
    }

    prev->next = current_del->next;
    free(current_del);
}
/* Description:
       Function used to display certain list
*/
void printList(struct node *head) {
    struct node *ptr = head;

    printf("[head] =>");

    while (ptr != NULL) {
        printf(" %d =>", ptr->p.ID);
        ptr = ptr->next;
    }

    printf(" [null]\n");
}
/* Description:
       Function used to insert Process in correct location based on arrival Time
*/
void enqueuePriority(struct node** readyHead) {
    struct node* current_list = *readyHead;
    while (current_list!=NULL)
    {
    struct node* newProcess =(struct node *)malloc(sizeof(struct node));

    if (queueHead == NULL || current_list->p.arrivalTime < queueHead->p.arrivalTime) {
        newProcess -> p = current_list->p;
        newProcess->next = queueHead;
        queueHead = newProcess;
        current_list = current_list->next;
        continue;
    }

    struct node* q_current = queueHead;

    while (q_current->next != NULL && q_current->next->p.arrivalTime <= current_list->p.arrivalTime) {
        q_current = q_current->next;
    }
    newProcess -> p = current_list->p;
    newProcess->next = q_current->next;
    q_current->next = newProcess;
    current_list = current_list->next; 
    }

}
/*Description:
       Function used to run shortest Job within certain time interval
*/
void shortest (struct node** queueHead, int time,int i)
{
    struct node* current_short = *queueHead;
    int min = current_short->p.brustTime;
    int ans =current_short->p.ID;
    while(current_short->next != NULL && current_short->p.arrivalTime <= time )
    {
            if( min>current_short->next->p.brustTime )
            {
                min =current_short->next->p.brustTime;
                ans=current_short->next->p.ID;
            }
            current_short = current_short->next;
    }
    current_short = *queueHead;
    while(current_short!= NULL)
    {
        if(current_short->p.ID == ans)
        {
            if(current_short->p.startTime==-1)
            {
                current_short->p.startTime=time;
            }
            current_short->p.coreID = i;
            current_short->p.state = RUNNING;
            insertAtLast(current_short->p, &runningHead);
            deleteNode(queueHead,ans);
            break;
        }
        current_short = current_short->next;
    }
}
/*Description:
      Function used to insert process in correct position based on arrival time in required list.
*/
void enqPriority (struct node** queueHead , Process vv)
{
    struct node* newProcess =(struct node *)malloc(sizeof(struct node));
    newProcess->p = vv;
    if (*queueHead == NULL || newProcess->p.arrivalTime < (*queueHead)->p.arrivalTime) {
        newProcess->next = *queueHead;
        *queueHead = newProcess;
        return;
    }

    struct node* q_current = *queueHead;

    while (q_current->next != NULL && q_current->next->p.arrivalTime <= newProcess->p.arrivalTime)
    {
        q_current = q_current->next;
    }
    newProcess->next = q_current->next;
    q_current->next = newProcess;
    
}
/* Description:
       Function used to insert Process in correct location based on Priority
*/
void priorityQueue(struct node** readyHead){
    struct node* current_list = *readyHead;
    while (current_list!=NULL)
    {
    struct node* newProcess =(struct node *)malloc(sizeof(struct node));

    if (queueHead == NULL || current_list->p.Priority > queueHead->p.Priority) {
        newProcess -> p = current_list->p;
        newProcess->next = queueHead;
        queueHead = newProcess;
        current_list = current_list->next;
        continue;
    }

    struct node* q_current = queueHead;

    while (q_current->next != NULL && q_current->next->p.Priority >= current_list->p.Priority) {
        q_current = q_current->next;
    }
    newProcess -> p = current_list->p;
    newProcess->next = q_current->next;
    q_current->next = newProcess;
    current_list = current_list->next; 
    }
}


void printready (struct node *head){
    struct node *ptr = head;

    printf("[head] =>");

    while (ptr != NULL && ptr->p.arrivalTime <= Time) {
        printf(" %d =>", ptr->p.ID);
        ptr = ptr->next;
    }

    printf(" [null]\n");
}
/* Description:
      Function used to update which process run when new process entre on system.
*/
boolean update(struct node** queueHead, int time, struct node** current_Node) {
    struct node* current_upddate = *queueHead;
    int min = (*current_Node)->p.brustTime;
    int ans = (*current_Node)->p.ID;
    Process answer = (*current_Node)->p;
    Process Old = (*current_Node)->p;

    while (current_upddate != NULL && current_upddate->p.arrivalTime <= time) {
        if (min > current_upddate->p.brustTime) {
            min = current_upddate->p.brustTime;
            ans = current_upddate->p.ID;
            answer = current_upddate->p;
        }
        current_upddate = current_upddate->next;
    }

    current_upddate = *queueHead;
        while (current_upddate != NULL) {
            if (current_upddate->p.ID == ans) {
                if (current_upddate->p.startTime == -1) {
                    current_upddate->p.startTime = time;
                }
                current_upddate->p.coreID = (*current_Node)->p.coreID;
                current_upddate->p.state = RUNNING;
                enqPriority(queueHead, (*current_Node)->p);
                insertAtLast(answer, &runningHead);
                deleteNode(queueHead, ans);
                deleteNode(&runningHead,(*current_Node)->p.ID);
                (*current_Node)->p.state = READY;
                return TRUE;
            }
            current_upddate = current_upddate->next;
        }
    return FALSE;
}

void Priority (struct node** queueHead , Process vv)
{
    struct node* newProcess =(struct node *)malloc(sizeof(struct node));
    newProcess->p = vv;
    if (*queueHead == NULL || newProcess->p.Priority > (*queueHead)->p.Priority) {
        newProcess->next = *queueHead;
        *queueHead = newProcess;
        return;
    }

    struct node* q_current = *queueHead;

    while (q_current->next != NULL && q_current->next->p.Priority >= newProcess->p.Priority)
    {
        q_current = q_current->next;
    }
    newProcess->next = q_current->next;
    q_current->next = newProcess;
    
}
void stridePriority(struct node** running, int pass) {
  
  struct node* current_list = *running;
    while (current_list!=NULL)
    {

    struct node* newProcess =(struct node *)malloc(sizeof(struct node));

    if (stridehead == NULL || current_list->p.pass < stridehead->p.pass) {
        newProcess -> p = current_list->p;
        newProcess->next = stridehead;
        stridehead = newProcess;
        current_list = current_list->next;
        continue;
    }

    struct node* q_current = stridehead;

    while (q_current->next != NULL && q_current->next->p.pass <= current_list->p.pass) {
        q_current = q_current->next;
    }
    newProcess -> p = current_list->p;
    newProcess->next = q_current->next;
    q_current->next = newProcess;
    current_list = current_list->next; 
    
    }

}
void calc ( struct node **head , int num ){
struct node *current= *head;
while(current!=NULL ){
printf(" Start time :  %d\n",current->p.startTime);
printf(" End time :  %d\n",current->p.endTime);

avg_response += (current->p.startTime - current->p.arrivalTime);
avg_turnround += (current->p.endTime - current->p.arrivalTime);
current=current->next;
}

printf(" Average responce time :  %.2f\n",avg_response/num);
printf(" Average turnround time :  %.2f\n",avg_turnround/num);

}