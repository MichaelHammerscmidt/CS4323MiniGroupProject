#ifndef PIPE_DOT_H
#define PIPE_DOT_H
#include "unique_record_struct.h"

void sendDataViaPipe(char* myPipe, struct uniqueRecordStruct uniqueRecord);
struct uniqueRecordStruct receiveDataViaPipe(char* myPipe);
char* concatenate(char recordArray[700][7][300], int row, int col);
struct uniqueRecordStruct unwrap(char* arrayString);

#endif
