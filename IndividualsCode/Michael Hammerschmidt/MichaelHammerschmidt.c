#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define LINE_LENGTH 256
#define INITIAL_NUM_ROWS 200
#define ROW_INCRIMENT 50

typedef struct
{
    char **data;
} DataColumn;

typedef struct
{
    DataColumn *columns;
    int numRows;
    int numCols;
} DataTable;

bool readFile(char filesName[], DataTable *saveTable)
{
    FILE *fp;

    printf("Read file: %s", &filesName[0]);
    // fopen_s(&fp, filesName, "r");
    // if (fp == NULL)
    fopen(filesName, "r");

    if (&filesName == NULL)
    {
        printf("%s", "Error: The file is Null");
        return false;
    }

    char bufferIn[LINE_LENGTH];
    char *tokenizedBuffer;
    int rowsSoFar = 0;
    const char delim[] = ",";

    fgets(bufferIn, LINE_LENGTH, fp);
    char *bufferPt = bufferIn;
    saveTable->numCols = 0;
    printf("\nBuffer pointer: %s", bufferPt);
    while (*bufferPt)
    {
        if (*bufferPt != ',' && *bufferPt != '\t')
        {
            saveTable->numCols++, bufferPt++;
            // printf("\nnumCol: %d", saveTable->numCols);
        }
        while (*bufferPt && *bufferPt != ',' && *bufferPt != '\t')
        {
            bufferPt++;
        }
        while (*bufferPt && (*bufferPt == ',' || *bufferPt == '\t'))
        {
            bufferPt++;
        }
    }

    printf("\nNumber of col in file = %d\n\n", saveTable->numCols);

    saveTable->numRows = INITIAL_NUM_ROWS;
    saveTable->columns = calloc(saveTable->numCols, sizeof(DataColumn));
    for (int i = 0; i < saveTable->numCols; i++)
    {
        saveTable->columns[i].data = calloc(INITIAL_NUM_ROWS, sizeof(char *));
    }

    while (fgets(bufferIn, LINE_LENGTH, fp))
    {
        if (bufferIn[0] == '\0')
        {
            continue;
        }

        if (bufferIn[strlen(bufferIn) - 1] == '\n')
        { // Get rid of the newline character in the buffer
            bufferIn[strlen(bufferIn) - 1] = '\0';
        }

        if (rowsSoFar == saveTable->numRows)
        {
            saveTable->numRows = saveTable->numRows + ROW_INCRIMENT;
            for (int i = 0; i < saveTable->numCols; i++)
            {
                saveTable->columns[i].data = realloc(saveTable->columns[i].data, saveTable->numRows * sizeof(char *));
                for (int r = rowsSoFar; r < saveTable->numRows; r++)
                {
                    saveTable->columns[i].data[r] = NULL;
                }
            }
        }

        tokenizedBuffer = strtok(bufferIn, delim);
        for (int i = 0; i < saveTable->numCols; i++)
        {
            saveTable->columns[i].data[rowsSoFar] = malloc(strlen(tokenizedBuffer) + 1);
            strcpy(saveTable->columns[i].data[rowsSoFar], tokenizedBuffer);

            // For testing:
            // printf("%s\n", tokenizedBuffer);
            printf("%s  ", saveTable->columns[i].data[rowsSoFar]);

            tokenizedBuffer = strtok(NULL, delim);

            if (i % saveTable->numCols == 0)
            {
                printf("\n");
            }
        }

        rowsSoFar++;
    }

    printf("\nClosed file\n");
    fclose(fp);

    saveTable->numRows = rowsSoFar;
    for (int i = 0; i < saveTable->numCols; i++)
    {
        saveTable->columns[i].data =
            realloc(saveTable->columns[i].data, saveTable->numRows * sizeof(char *));
        for (int r = rowsSoFar; r < saveTable->numRows; r++)
        {
            saveTable->columns[i].data[r] = NULL;
        }
    }

    return true;
}

int main(int argc, char *argv[])
{

    bool end = false;
    char userInput[256];
    DataTable saveTable;

    while (end == false)
    {
        printf("\nEnter the name of the list to filter through,\nbookInfo or amazonBestsellers\nor enter 'end' to exit the program.\n\n");

        printf("Input: ");
        fgets(userInput, 256, stdin);

        if ((strncmp(userInput, "end", 3) == 0) || (strncmp(userInput, "End", 3) == 0))
        {
            printf("\nEnding program...");
            end = true;
        }
        else if ((strncmp(userInput, "amazonBestsellers", 17) == 0) || (strncmp(userInput, "AmazonBestsellers", 17) == 0) || (strncmp(userInput, "amazonBestsellers.txt", 21) == 0) || (strncmp(userInput, "AmazonBestsellers.txt", 21) == 0))
        {
            printf("\nReading amazonBestsellers file\n");
            readFile("amazonBestsellers.txt", &saveTable);
            end = true;
        }
        else if ((strncmp(userInput, "bookInfo", 8) == 0) || (strncmp(userInput, "BookInfo", 8) == 0) || (strncmp(userInput, "BookInfo.txt", 12) == 0) || (strncmp(userInput, "BookInfo.txt", 12) == 0))
        {
            printf("\nReading bookInfo file\n");
            readFile("bookInfo.txt", &saveTable);
            end = true;
        }
        else if ((strncmp(userInput, "options", 7) == 0))
        {
            printf("\nReading bookInfo file\n");
            readFile("options.txt", &saveTable);
            end = true;
        }
        else
        {
            printf("\nNot valid input\n");
        }
    }
}