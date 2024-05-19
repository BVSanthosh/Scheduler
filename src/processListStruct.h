/*
header file which contains the linked list struct 
*/

typedef struct ProcessListStruct{
    int processNum;   //numbe for identifying the process
    bool rearranged;    //flag for whether the  position of the process has been changed in the queue for the priority scheduler
    struct ProcessStruct *process;    //pointer to the process
    struct ProcessListStruct *next;   //pointer to the next linked-list node

}ProcessListStruct;

void addProcess(int processNum, ProcessStruct *process);

ProcessStruct *nextProcess();

void rearrangeList();

void initialisePrioirtyQueue(int size);

ProcessStruct *getPriorityQueue();