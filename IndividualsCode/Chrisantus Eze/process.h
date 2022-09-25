#ifndef PROCESS_DOT_H
#define PROCESS_DOT_H

char*** readFile(char* filename, char* column, int* row, int* col);

char ***readFileByUniqueValue(char* filename, char* column, char* uniqueValue, int* row_, int* col_);

char*** getByUniqueValue(struct recordStruct *recordArray, char* column, char* uniqueValue, int row, int col, int *count);

char*** getRecordsByUniqueValue(char* filename, char* column, char* uniqueValue, int *row_, int *col_);

#endif