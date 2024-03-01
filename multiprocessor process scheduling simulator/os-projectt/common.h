/*Guarding File*/
#ifndef COMMON_DATA
#define COMMON_DATA
/********************** Definations ************************************/
/* boolean Values*/
#define TRUE 1
#define FALSE 0

/*  Time Slice */
#define quantum     2
/*used to make all processes in high level after every time level*/
#define Time_Level  5
/************************** Data Types *********************************/
typedef unsigned char boolean;

typedef enum { WAITING ,
                READY ,
                RUNNING ,
                TERMINATED 
}processState;

typedef enum {  BUSY,
                EMPTY 
}coreState;

 typedef struct {
    int ID;
    coreState state;
}Core;

typedef struct {
    int ID ;
    int arrivalTime;
    int IOtime;
    int cpuTime;
    int endTime;
    int brustTime;
    int startTime;
    int checkTime;
    int coreID;
    processState state;
    int Priority;
    int RRcheckTime;
    int tickets;
    int pass;
    int stride;
}Process;

struct node {
    Process p;
   struct node *next;
};
/************************** Global Variables *********************************/
extern struct node *readyHead;
extern struct node *runningHead;
extern struct node *remainHead;
extern struct node *waitingHead;
extern struct node *queueHead;
extern struct node *current;
extern struct node* stridehead;
extern struct node *runningCurrent;
extern struct node *waitingCurrent;
extern struct node *High_Level;
extern struct node *Mid_Level;
extern struct node *Low_Level;
extern unsigned int Time;
extern float avg_response,avg_turnround;
extern int NumberOfCores;
/************************** Functions ProtoTypes *********************************/
/* Description: 
     Function used to insert Process in the last of certain list
*/
void insertAtLast(Process vv, struct node **head);
/* Description: 
     Function used to delete first Process in certain list
*/
void deleteFirst(struct node **head);
/*Description:
      Function used to delete any required process from certain list
*/
void deleteNode(struct node **head, int key);
/* Description:
       Function used to insert Process in certain location based on arrival Time
*/
void enqueuePriority(struct node** readyHead) ;
/* Description:
       Function used to display certain list
*/
void printList(struct node *head);
/*Description:
       Function used to run shortest Job within certain time interval
*/
void shortest (struct node** queueHead, int time,int i);
/*Description:
      Function used to insert process in correct position based on arrival time in required list.
*/
void enqPriority (struct node** queueHead , Process vv);
/* Description:
       Function used to insert Process in correct location based on Priority
*/

void priorityQueue(struct node** readyHead);

/* Description:
      Function used to update which process run when new process entre on system.
*/

boolean update(struct node** queueHead, int time, struct node** current_Node);

void Priority (struct node** queueHead , Process vv);
void printready (struct node *head);
void stridePriority(struct node** running, int pass);
void calc ( struct node **head , int num );

#endif