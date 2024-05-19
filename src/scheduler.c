#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <signal.h>
#include <sys/wait.h>
#include <string.h>
#include <time.h>
#include "process.c"
#include "processList.c"

int main(int argc, char **argv) {

    FILE* fp;
    char fileName[100];     //file to be read
    char processLine[100];     //the lines in the file
    char *arguments[100];   //the attirbutes of each process
    int processCount = 0;    //the total number of processes to be executed
    int currentProcessNum = 0;    //identifies the current process to be executed
    bool emptyQueue = false;    //checks if all the processes have been executed
    int choice;  
    clock_t start, end;
    int total;
    
    //prompts the user to enter the configuration file 
    printf("Enter the configuration file\n");
    scanf("%s", fileName);

    fp = fopen(fileName, "r");

    //checks if there is an error with the opening of the file for reading
    if(!fp){
        printf("An error occurred while opening the file.\n", fileName);
        exit(1);
    }

    //Reads the contents of the file
    while (!feof(fp) && !ferror(fp)){

        char *line;
        char *split;

        //gets each line
        if(fgets(processLine, sizeof(processLine), fp) != NULL){

            int count = 0;

            split = strtok(processLine, " ");

            //splits each line into its arguments
            while(split != NULL){
                arguments[count] = split;
                split = strtok(NULL, " ");
                count++;
            }
            
            pid_t pid = fork();   //starts a child process for each line and pauses it immediately 

            if(pid < 0){   //if there was an error in starting the process
                printf("Something went wrong!\n");
            }
            else if(pid > 0){   //parent process
                kill(pid,SIGSTOP);
            }      
            else{     //child process
                execl(arguments[1], arguments[1], arguments[2], NULL);  
            }  
            
            processCount++;
            ProcessStruct *process = processInit(pid, arguments);   //passes the info about each process to store in the data structure (PCB)
            addProcess(processCount, process);   //passes each process to the linked list (ready queue)
        }
    }

    fclose(fp);

    printf("Choose the type of scheduler to use:\n");
    printf("1: Round-robin scheduler\n");
    printf("2: Priority scheduler\n");
    scanf("%i", &choice);

    
    if(choice == 1){   //executes the round-robin scheduler

        do{     
            int status;
            pid_t result;
            ProcessStruct *process = nextProcess();   //gets the process to execute from the ready queue

            result = waitpid(process->id, &status, WNOHANG);   //gets the current status of the process

            //checks whether the current process to be executed has finished
            if(result != 0){     //if it has finished executing

                currentProcessNum++;
                kill(process->id, SIGTERM);   //terminates the process

                printf("process %i has finished executing", process->id);
                printf("\n");

                //checks whether the ready queue is empty 
                if(currentProcessNum == numOfProcesses){
                    emptyQueue = true;
                }
            }
            else{    //if it still needs to be executed
                printf("started executing process %i", process->id);
                printf("\n");

                kill(process->id, SIGCONT);   //starts executing the prcess

                usleep(500000);     //the time quantum

                kill(process->id, SIGSTOP);   //pauses the execution of the process

                printf("\n");
                printf("paused executing process %i", process->id);
                printf("\n");
            }
        } while(emptyQueue == false);
    }
    else if(choice == 2){   //executes the priority scheduler

        initialisePrioirtyQueue(processCount);   
        rearrangeList();     //all the function to re-arrange the processes in the queue
        ProcessStruct *priorityQueue = getPriorityQueue();    //gets the array or re-arranged processes 

        for(int counter = 0; counter < numOfProcesses; counter++){     

            int status;
            pid_t result;
            ProcessStruct process = priorityQueue[counter];     //calls the next highest priority process
    
            printf("started executing process %i", process.id);
            printf("\n");

            kill(process.id, SIGCONT);   //executes the process

            wait(NULL);   //waits until it finishes executing

            kill(process.id, SIGTERM);   //terminates the process

            printf("\n");
            printf("finished executing process %i", process.id);
            printf("\n");
        }
    }
}