#ifndef IPC_DOT_H
#define IPC_DOT_H

#include "process_data_struct.h"

void createProcesses(int nProcess, char *filename, char *column, char uniques[][15]);
void sendViaQueue(int msgid, struct processDataStruct processData);
struct processDataStruct receiveViaQueue(int msgid);

void sendDatToParent(struct processDataStruct data);
void receiveDatFromProcess(char uniqueValue[20]);
struct uniqueRecordStruct fetchRecords(char column[20], char uniqueValue[20]);


#endif