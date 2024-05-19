#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "processListStruct.h"

/*
This file contains the linked list which acts as a ready queue, storing the PCB of each process 
*/

ProcessListStruct *head = NULL;   //the head of the linked list
ProcessStruct *rearrangedQueue;   //priority queue array 
int numOfProcesses;   //the total number of processes in the list
int curretProcessNum;   //the number to identify the current process in the list 

//adds each process to the linked list 
void addProcess(int processNum, ProcessStruct *process){

    //the following code has been taken from https://www.log2base2.com/data-structures/linked-list/inserting-a-node-at-the-end-of-a-linked-list.html

    ProcessListStruct *newProcess = (ProcessListStruct *)malloc(sizeof(ProcessListStruct)); 
    newProcess->processNum = processNum;   //the number of the current process
    newProcess->rearranged = false;
    newProcess->process = process;   //the process
    newProcess->next = NULL;   //pointer to the next process

    //check is the current process to add is the first one
    if(head == NULL){   // if so it is the head of the linked list
        head = newProcess;
    }
    else{   // if not it is added to the end 
        ProcessListStruct *lastProcess = head;

        //goes through the linked list until it finds the end
        while(lastProcess->next != NULL){
            lastProcess = lastProcess->next;
        }

        lastProcess->next = newProcess;
    }

    numOfProcesses++;
}

//sends the current process to be executed from the ready queue
ProcessStruct *nextProcess(){

    int temp = 0;
    ProcessListStruct *current = head;   
    curretProcessNum++;   //increments the variable so that the it points to the current process to be executed
    temp = curretProcessNum;

    //goes through the linked list until the current process to be executed is found
    while (current != NULL){

        //checks if this process in the linked list is the current process to be executed
        if(current->processNum == curretProcessNum){

            //checks if the current process is the last process in the list
            if(curretProcessNum == numOfProcesses){
                curretProcessNum = 0;   //if so the process pointer points back to the start of the list (gives the impression that the linked list is circular)
            }
            
            return current->process;
        }
        else{   //if not the next process in the linked list is retrieved
            current = current->next;
        }
    }
}

//initialises the priority queue array
void initialisePrioirtyQueue(int size){
    rearrangedQueue = (ProcessStruct *)malloc(sizeof(ProcessStruct) * size);
}

//sends the priority queue array
ProcessStruct *getPriorityQueue(){
    return rearrangedQueue;
}

//rearranges the original linked list 
void rearrangeList(){
    
    ProcessListStruct *current;
    ProcessStruct *highestPriorityProcess;
    bool repeat = true;
    int count = 0;

    while(repeat == true){

        for(int counter1 = 0; counter1 < numOfProcesses; counter1++){

            current = head;

            for(int counter2 = 0; counter2 < counter1; counter2++){
                current = current->next;
            }

            if(current->rearranged != true){

                highestPriorityProcess = current->process;
                current = head;

                while(current != NULL){

                    if(!current->rearranged){

                        if(highestPriorityProcess->priority > current->process->priority){
                            highestPriorityProcess = current->process;
                        }
                        else{
                            current = current->next;
                        }
                    }
                    else{
                        current = current->next;
                    }
                }

                current = head;

                while(current != NULL){

                    if(current->process->id == highestPriorityProcess->id){
                        current->rearranged = true;
                        break;
                    }
                    else{
                        current = current->next;
                    }
                }

                rearrangedQueue[count] = *highestPriorityProcess;
                count++;
            }
        }

        current = head;
        int tempCounter = 0;

        while(current != NULL){

            if(current->rearranged == false){
                break;
            }
            else{
                tempCounter++;
                current = current->next;
            }

            if(tempCounter == numOfProcesses){
                repeat = false;
            }
        }

    }
}