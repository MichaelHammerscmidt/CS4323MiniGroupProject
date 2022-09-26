#include <stdio.h>


int saveFile(DataTable *ptable, DataColumn *pcolumn)
{


int i, j, k; //initializing variables for later for loops

int n = gets(DataTable.numCols); //initialize n for the number of columns in the table
int z = gets(DataTable.numRows); //initialize z for the number of rows in the table

FILE *output; //creating file

output = fopen ("output.txt", "w"); //open the file for writing



for(k = 0; k < n, k++) //loop for adding the data headers to the file
{
	gets((ptable.colHeaders);
	printf("\t\t");



}

for(j = 0; j < z; j++) //loop for adding the rest of the data to the file. This loop is for the rows
{
	for(i = 0; i < n; i++) //this loop is for the columns
	{
		gets(*pcolumn.columns);
		printf("\t\t"); //formatting

	
	}

	printf("\n"); //formatting

}


/*
fwrite (&input, sizeof(struct DataTable), 1, outfile);
this is commented out because I think im gonna stop trying to use gets and change to fwrite to get things to work better.

}
*/
