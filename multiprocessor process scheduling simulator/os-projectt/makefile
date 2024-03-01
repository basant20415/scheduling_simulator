# target: prerequisite1 prerequisite2 ...
#	command(s)

# target is the target file you want
# prerequisite is a file required to get compile obtain the target
# command(s) those are the step(s) needed to create the target from the prerequisites

main: main.o FCFS.o SJF.o STCF.o Priority.o RR.o MLFQ.o stride.o common.o
	 gcc main.o FCFS.o SJF.o STCF.o Priority.o RR.o MLFQ.o stride.o   common.o -o main
# SJF.o STCF.o Priority.o RR.o MLFQ.o
# SJF.o STCF.o Priority.o RR.o MLFQ.o
main.o: main.c
	gcc -c -Wall -O main.c -o main.o 

FCFS.o: FCFS.c
	gcc -c -Wall -O FCFS.c -o FCFS.o 

SJF.o: SJF.c
	gcc -c -Wall -O SJF.c -o SJF.o 

STCF.o: STCF.c
	gcc -c -Wall -O STCF.c -o STCF.o 

Priority.o: Priority.c
	gcc -c -Wall -O Priority.c -o Priority.o 

RR.o: RR.c
	gcc -c -Wall -O RR.c -o RR.o 

MLFQ.o: MLFQ.c
	gcc -c -Wall -O MLFQ.c -o MLFQ.o 

stride.o: stride.c
	gcc -c -Wall -O stride.c -o stride.o 

common.o: common.c
	gcc -c -Wall -O common.c -o common.o 







clean:
	rm -f main main.o FCFS.o SJF.o STCF.o Priority.o RR.o MLFQ.o stride.o  Common-data.o
	
# SJF.o STCF.o Priority.o RR.o MLFQ.o
	