//Group G
//Rebecca Wasserott
//rebecca.wasserott@okstate.edu
//09/25
//The logic to enable the server to display the contents of the struct array on the client
//Not finished. This is for the progress report.
 
#include "process.c"
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
int main() {
 
   //struct definition
   typedef struct{
       //dynamic array of strings
       char **data;
   }
   DataColumn;
 
   typedef struct{
       //dynamic array of strings for the column headers
       char** colHeaders;
       //dynamic array of datacolumns
       DataColumn *cloumns;
 
       int numRows;
       int numCols;
   }
   DataTable;
 
   char *filename = "bookInfo1.txt";
   char *column = "Stock";
   char *uniqueValue = "Out of stock";
 
   int row = 0;
   int col = 0;
 
   char*** recordsArray = getRecordsByUniqueValue(filename, column, uniqueValue, &row, &col);
 
   for (int i = 1; i < row; i++) {
       for (int j = 0; j < col; j++) {
           printf("%d. %s\n", (j + 1), recordsArray[0][j]);
          
           printf("%s\n\n", recordsArray[i][j]);
       }
       printf("---------------------------------\n");
   }
 
}

