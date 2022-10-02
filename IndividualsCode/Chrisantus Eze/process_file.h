#ifndef PROCESS_FILE_DOT_H
#define PROCESS_FILE_DOT_H

#include "process_data_struct.h"

struct uniqueRecordStruct readFileByUniqueValue(char* filename, char* column, char* uniqueValue);

void snd(struct processDataStruct data);
struct processDataStruct rcv(char uniqueValue[20]);

void msgsnd_(int msgid, struct processDataStruct data, int size, int pms);
struct processDataStruct msgrcv_(int msgid, char uniqueValue[20], int size, int pms, int detr);

void sendDataToParent(struct processDataStruct data);
struct uniqueRecordStruct receiveDataFromProcess(char filename[20], char column[20], char uniqueValue[20]);
#endif