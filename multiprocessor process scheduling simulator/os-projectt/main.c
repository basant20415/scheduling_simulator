#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>

#include "FCFS.h"
#include "SJF.h"
#include "STCF.h"
#include "Priority.h"
#include "RR.h"
#include "MLFQ.h"
#include "stride.h"

int NumberOfProcesses;
int main(int argc, char *argv[])
{
   if (argc != 2) {
        fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }


    int p;
    fscanf(file, "%d %d", &NumberOfProcesses, &NumberOfCores);
    printf("Number of processes is: %d\n",NumberOfProcesses);
    printf("Number of cores is: %d\n",NumberOfCores);
    Core cores[NumberOfCores + 1];
    for (int i = 1; i <= NumberOfProcesses; i++) {
        Process x;
        x.ID = i;
        fscanf(file, "%d %d %d %d", &x.arrivalTime, &x.IOtime, &x.cpuTime, &x.brustTime);
        printf("\nProcess %d : ArrivalTime = %d\tIOtime = %d\tCpuTime = %d\tBrustTime = %d \tPriority",i,x.arrivalTime,x.IOtime,x.cpuTime,x.brustTime,x.Priority);
        x.tickets = random() % 100;
        x.pass = 0;
        x.stride = 10000 / x.tickets;
        x.coreID = 0;
        x.checkTime = 0;
        x.RRcheckTime = 0;
        x.startTime = -1;
        x.state = READY;
        insertAtLast(x, &readyHead);
    }
    printf("\n");
    for (int j = 1; j <= NumberOfCores; j++) {
        cores[j].ID = j;
        cores[j].state = EMPTY;
    }

     int choice;
    printf("\n*************************************\n");
    printf("*****  Please choose an option  *****\n");
    printf("*************************************\n");
    printf("1. First Come First Serve.\n");
    printf("2. Shortest Jop First.\n");
    printf("3. Shortest Time To Complelion First.\n");
    printf("4. Round Robin.\n");
    printf("5. Multi Level Feedback Queue.\n");
    printf("6. PriorityQueue.\n");
    printf("7. Stride.\n");
    printf("8. Exit.\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            printf("You choose First Come First Serve.\n");
            FCFS(&readyHead,cores);
            calc(&remainHead,NumberOfProcesses);
            
            break;
        case 2:
            printf("You choose Shortest Jop First.\n");
            SJF(&readyHead,cores);
           calc(&remainHead,NumberOfProcesses);
            break;
        case 3:
            printf("You choose Shortest Time To Complelion First.\n");
            STCF(&readyHead,cores);
            calc(&remainHead,NumberOfProcesses);
            break;
        case 4:
            printf("You choose Round Robin.\n");
            RR(&readyHead,cores);
            calc(&remainHead,NumberOfProcesses);
            break;
        case 5:
            printf("You choose Multi Level Feedback Queue.\n");
            MLFQ(&readyHead,cores);
            calc(&remainHead,NumberOfProcesses);
            break;
         case 6:
            printf("You choose PriorityQueue.\n");
           PrioritySchedular(&readyHead,cores);
           calc(&remainHead,NumberOfProcesses);
            break;
         case 7:
            printf("You choose Stride.\n");
            stride(&readyHead,cores);
            calc(&remainHead,NumberOfProcesses);
            break;
         case 8:
            printf("You choose Exit.\n");
           exit(0);
            break;
        default:
            printf("Invalid choice.\n");
            break;
    }
    fclose(file);
    return 0;
}
