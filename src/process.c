#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "processStruct.h"

/*
This file contains the method for storing all the necessaray information about each process in a data structure which acts as a PCB
*/

ProcessStruct *processInit(int id, char **attr){

    ProcessStruct *process = malloc(sizeof(ProcessStruct));
    process->id = id;   //process id
    process->priority = atoi(attr[0]);   //priority number of the process
    process->path = strdup(attr[1]);   //path where the program is stored
    process->parameters = strdup(attr[2]);   //the parameter to pass to the program

    return process;
}