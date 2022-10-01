#ifndef PROCESS_FILE_DOT_H
#define PROCESS_FILE_DOT_H

struct uniqueRecordStruct readFileByUniqueValue(char* filename, char* column, char* uniqueValue);

void sendDataToParent(struct processDataStruct data, struct uniqueRecordStruct uniqueRecord);
struct uniqueRecordStruct receiveDataFromProcess(char filename[20], char column[20], char uniqueValue[20]);
#endif