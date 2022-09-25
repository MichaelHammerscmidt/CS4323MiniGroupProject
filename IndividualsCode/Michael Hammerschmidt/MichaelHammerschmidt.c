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

bool readFile(char fileName[], DataTable *ptable)
{
    FILE *file;

    char errorMsg[] = "Error. File Null";

    printf("Read file: %s", &fileName[0]);
    fopen_s(&file, fileName, "r");
    if (file == NULL)
    {
        printf("%s", errorMsg);
        return false;
    }

    char line[LINE_LENGTH];
    char *temp;
    int currentLine = 0, j = 0;
    const char comma[] = ",";

    fgets(line, LINE_LENGTH, file);
    char *bufferPt = line;
    ptable->numCols = 0;
    printf("\nBuffer pointer: %s", bufferPt);
    while (*bufferPt)
    {
        if (*bufferPt != ',' && *bufferPt != '\t')
        {
            ptable->numCols++, bufferPt++;
            // printf("\nnumCol: %d", ptable->numCols);
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

    printf("\nInt col = %d\n\n", ptable->numCols);

    ptable->numRows = INITIAL_NUM_ROWS;
    ptable->columns = calloc(ptable->numCols, sizeof(DataColumn));
    for (int i = 0; i < ptable->numCols; i++)
    {
        ptable->columns[i].data = calloc(INITIAL_NUM_ROWS, sizeof(char *));
    }

    while (fgets(line, LINE_LENGTH, file))
    {
        if (line[0] == '\0')
        {
            continue;
        }

        if (line[strlen(line) - 1] == '\n')
        { // Discard the newline character
            line[strlen(line) - 1] = '\0';
        }

        if (j == ptable->numRows)
        {
            ptable->numRows += ROW_INCRIMENT;
            for (int i = 0; i < ptable->numCols; i++)
            {
                ptable->columns[i].data =
                    realloc(ptable->columns[i].data, ptable->numRows * sizeof(char *));
                for (int r = j; r < ptable->numRows; r++)
                {
                    ptable->columns[i].data[r] = NULL;
                }
            }
        }

        temp = strtok(line, comma);
        for (int i = 0; i < ptable->numCols; i++)
        {
            ptable->columns[i].data[j] = malloc(strlen(temp) + 1);
            strcpy(ptable->columns[i].data[j], temp);

            // printf("%s\n", temp);
            printf("%s  ", ptable->columns[i].data[j]);

            temp = strtok(NULL, comma);

            if (i % ptable->numCols == 0)
            {
                printf("\n");
            }
        }

        j++;
    }

    printf("\nClosing\n");
    fclose(file);

    ptable->numRows = j;
    for (int i = 0; i < ptable->numCols; i++)
    {
        ptable->columns[i].data =
            realloc(ptable->columns[i].data, ptable->numRows * sizeof(char *));
        for (int r = j; r < ptable->numRows; r++)
        {
            ptable->columns[i].data[r] = NULL;
        }
    }

    return true;
}

int main(int argc, char *argv[])
{

    bool end = false;
    char userInput[256];
    DataTable ptable;

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
            readFile("amazonBestsellers.txt", &ptable);
            end = true;
        }
        else if ((strncmp(userInput, "bookInfo", 8) == 0) || (strncmp(userInput, "BookInfo", 8) == 0) || (strncmp(userInput, "BookInfo.txt", 12) == 0) || (strncmp(userInput, "BookInfo.txt", 12) == 0))
        {
            printf("\nReading bookInfo file\n");
            readFile("bookInfo.txt", &ptable);
            end = true;
        }
        else if ((strncmp(userInput, "options", 7) == 0))
        {
            printf("\nReading bookInfo file\n");
            readFile("options.txt", &ptable);
            end = true;
        }
        else
        {
            printf("\nNot valid input\n");
        }
    }
}