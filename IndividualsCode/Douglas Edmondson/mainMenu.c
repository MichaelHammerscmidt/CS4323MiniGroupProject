//Group G
//Douglas Edmondson
//rebecca.wasserott@okstate.edu
//09/25
//The logic to enable the server to display the contents of the struct array on the client
//Not finished. This is for the progress report.

//DOES NOT COMPILE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readFile.h>

int main(char** fileFromReadFile)
{
    if(strcmp("bookInfo.txt",fileFromReadFile) != 0)
    {
        int choice;
        int midChoice;
        while (choice!=4)   //while choice isn't exit, program runs
        {
            printf("Enter a selection: \n");
            printf("1.Display Records  \n");
            printf("2.Save Records \n");
            printf("3.Display Summary \n");
            printf("4. Exit \n");
            scanf("%i", &choice); //saves choice made from user 

            if(choice==1)   //if user chooses to display records
            {
                printf("Which record to display: \n");
                printf("1.Title  \n");
                printf("2. Book Category\n");
                printf("3.Star Rating \n");
                printf("4. Price");
                printf("5. Stock");
                printf("6. Quantity");
                scanf("%i", &midChoice); //saves choice made from user

                if(midChoice==1)   //if user chooses option
                {
                    //display()
                } 

                if(midChoice==2)   //if user chooses option
                {
                    //display()
                } 

                if(midChoice==3)   //if user chooses option
                {
                    //display()
                } 
                if(midChoice==4)   //if user chooses option
                {
                    //display()
                } 

                if(midChoice==5)   //if user chooses option
                {
                    //display()
                } 

                if(midChoice==6)   //if user chooses option
                {
                    //display()
                } 
            }

            if(choice==2)   //if user chooses to save records
            {
                printf("Which record to save: \n");
                printf("1.Title  \n");
                printf("2. Book Category\n");
                printf("3.Star Rating \n");
                printf("4. Price");
                printf("5. Stock");
                printf("6. Quantity");//saves choice made from user 

                if(midChoice==1)   //if user chooses option
                {
                    //saveFile()
                } 

                if(midChoice==2)   //if user chooses option
                {
                    //saveFile()
                } 

                if(midChoice==3)   //if user chooses option
                {
                    //saveFile()
                } 
                if(midChoice==4)   //if user chooses option
                {
                    //saveFile()
                } 

                if(midChoice==5)   //if user chooses option
                {
                    //saveFile()
                } 

                if(midChoice==6)   //if user chooses option
                {
                    //saveFile()
                } 

            }

            if(choice==3)   //if user chooses to display summary
            {
            //displaySummary function
            }
        }
    }

    else
    {
        int choice;
        int midChoice;
        while (choice!=4)   //while choice isn't exit, program runs
        {
            printf("Enter a selection: \n");
            printf("1.Display Records  \n");
            printf("2.Save Records \n");
            printf("3.Display Summary \n");
            printf("4. Exit \n");
            scanf("%i", &choice); //saves choice made from user 

            if(choice==1)   //if user chooses to display records
            {
                printf("Which record to display: \n");
                printf("1. Name  \n");
                printf("2. Author\n");
                printf("3. User Rating \n");
                printf("4. Reviews");
                printf("5. Price");
                printf("6. Year");
                printf("7. Genre");
                scanf("%i", &midChoice); //saves choice made from user

                if(midChoice==1)   //if user chooses option
                {
                    //display()
                } 

                if(midChoice==2)   //if user chooses option
                {
                    //display()
                } 

                if(midChoice==3)   //if user chooses option
                {
                    //display()
                } 
                if(midChoice==4)   //if user chooses option
                {
                    //display()
                } 

                if(midChoice==5)   //if user chooses option
                {
                    //display()
                } 

                if(midChoice==6)   //if user chooses option
                {
                    //display()
                } 
                if(midChoice==7)   //if user chooses option
                {
                    //display()
                } 
            }

            if(choice==2)   //if user chooses to save records
            {
                printf("Which record to save: \n");
                printf("1. Name  \n");
                printf("2. Author\n");
                printf("3. User Rating \n");
                printf("4. Reviews");
                printf("5. Price");
                printf("6. Year");
                printf("7. Genre");
                scanf("%i", &midChoice); //saves choice made from user 

                if(midChoice==1)   //if user chooses option
                {
                    //saveFile()
                } 

                if(midChoice==2)   //if user chooses option
                {
                    //saveFile()
                } 

                if(midChoice==3)   //if user chooses option
                {
                    //saveFile()
                } 
                if(midChoice==4)   //if user chooses option
                {
                    //saveFile()
                } 

                if(midChoice==5)   //if user chooses option
                {
                    //saveFile()
                } 

                if(midChoice==6)   //if user chooses option
                {
                    //saveFile()
                } 
                if(midChoice==7)   //if user chooses option
                {
                    //saveFile()
                } 

            }

            if(choice==3)   //if user chooses to display summary
            {
            //displaySummary function
            }
        }
    }   
}