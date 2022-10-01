#ifndef PROCESS_DATA_STRUCT_DOT_H
#define PROCESS_DATA_STRUCT_DOT_H

#define MAX_LENGTH 128   //maximum length of the message that can be sent allowed

struct processDataStruct {
    int pid;
    char uniqueValue[MAX_LENGTH];
    char filename[MAX_LENGTH];
    char column[MAX_LENGTH];
    char tdata[MAX_LENGTH];
};

#endif