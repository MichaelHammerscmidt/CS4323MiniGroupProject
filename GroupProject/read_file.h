#ifndef READ_FILE_DOT_H
#define READ_FILE_DOT_H

#include "process_data_struct.h"

struct uniqueRecordStruct readFile(char* filename);
char** getUniqueValues(struct uniqueRecordStruct records, char* column, int* returned_size);
struct uniqueRecordStruct getRecordsByUniqueValue(struct uniqueRecordStruct records, char* column, char* uniqueValue);

#endif