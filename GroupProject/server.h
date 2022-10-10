#ifndef SERVER_DOT_H
#define SERVER_DOT_H
#include "unique_record_struct.h"

void displayOrSave(struct uniqueRecordStruct uniqueRecord, bool isDisplay);
void display(struct uniqueRecordStruct uniqueRecord);
void save(struct uniqueRecordStruct uniqueRecord);

#endif