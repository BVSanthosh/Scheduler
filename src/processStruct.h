/*
header file which contains the process struct 
*/

typedef struct ProcessStruct{
    int id;    //process id
    int priority;    //priority
    char *path;    //filepath of the program
    char *parameters;   //parameters to pass to thr program 
}ProcessStruct;

ProcessStruct *processInit(int id, char *attr[]);