#ifndef UNIQUE_RECORD_STRUCT_DOT_H
#define UNIQUE_RECORD_STRUCT_DOT_H

struct uniqueRecordStruct {
    char uniqueValue[30];
    int rowSize;
    int colSize;
    char recordArray[700][7][300];
};

#endif