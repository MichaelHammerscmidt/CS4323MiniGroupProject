#ifndef PROCESS_DOT_H
#define PROCESS_DOT_H
#include "unique_record_struct.h"

char* handleChildProcess(int msgID, struct uniqueRecordStruct recordArray, char* column, int t, int* running);
void handleParentProcess(int msgID, char* uniqueValue, int t);
void manageIPC(char* filename, char* column);

#endif