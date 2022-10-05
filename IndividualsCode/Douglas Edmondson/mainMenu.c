//Group G
//Douglas Edmondson
//douglas.edmondson@okstate.edu
//09/25
//The logic to enable the server to display the contents of the struct array on the client
//


//DOES NOT COMPILE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readFile.h>
#include <display.h>
#include <saveFile.h>

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
                printf("1. Title  \n");
                printf("2. Book Category\n");
                printf("3. Star Rating \n");
                printf("4. Price\n");
                printf("5. Stock\n");
                printf("6. Quantity\n");
                scanf("%i", &midChoice); //saves choice made from user

                if(midChoice==1)   //if user chooses option 1
                {
                    //display(title)
                } 

                if(midChoice==2)   //if user chooses option 2
                {
                    //display(book category)
                } 

                if(midChoice==3)   //if user chooses option 3
                {
                    //display(star rating)
                } 
                if(midChoice==4)   //if user chooses option 4
                {
                    //display(price)
                } 

                if(midChoice==5)   //if user chooses option 5
                {
                    //display(stock)
                } 

                if(midChoice==6)   //if user chooses option 6
                {
                    //display(quantity)
                } 
            }

            if(choice==2)   //if user chooses to save records
            {
                printf("Which record to save: \n");
                printf("1. Title  \n");
                printf("2. Book Category\n");
                printf("3. Star Rating \n");
                printf("4. Price\n");
                printf("5. Stock\n");
                printf("6. Quantity\n");//saves choice made from user 

                if(midChoice==1)   //if user chooses option
                {
                    //saveFile(title)
                } 

                if(midChoice==2)   //if user chooses option
                {
                    //saveFile(book category)
                } 

                if(midChoice==3)   //if user chooses option
                {
                    //saveFile(star rating)
                } 
                if(midChoice==4)   //if user chooses option
                {
                    //saveFile(price)
                } 

                if(midChoice==5)   //if user chooses option
                {
                    //saveFile(stock)
                } 

                if(midChoice==6)   //if user chooses option
                {
                    //saveFile(quantity)
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
                printf("4. Reviews\n");
                printf("5. Price\n");
                printf("6. Year\n");
                printf("7. Genre\n");
                scanf("%i", &midChoice); //saves choice made from user

                if(midChoice==1)   //if user chooses option
                {
                    //display(name)
                } 

                if(midChoice==2)   //if user chooses option
                {
                    //display(author)
                } 

                if(midChoice==3)   //if user chooses option
                {
                    //display(user rating)
                } 
                if(midChoice==4)   //if user chooses option
                {
                    //display(reviews)
                } 

                if(midChoice==5)   //if user chooses option
                {
                    //display(price)
                } 

                if(midChoice==6)   //if user chooses option
                {
                    //display(year)
                } 
                if(midChoice==7)   //if user chooses option
                {
                    //display(genre)
                } 
            }

            if(choice==2)   //if user chooses to save records
            {
                printf("Which record to save: \n");
                printf("1. Name  \n");
                printf("2. Author\n");
                printf("3. User Rating \n");
                printf("4. Reviews\n");
                printf("5. Price\n");
                printf("6. Year\n");
                printf("7. Genre\n");
                scanf("%i", &midChoice); //saves choice made from user 

                if(midChoice==1)   //if user chooses option
                {
                    //saveFile(name)
                } 

                if(midChoice==2)   //if user chooses option
                {
                    //saveFile(author)
                } 

                if(midChoice==3)   //if user chooses option
                {
                    //saveFile(user rating)
                } 
                if(midChoice==4)   //if user chooses option
                {
                    //saveFile(reviews)
                } 

                if(midChoice==5)   //if user chooses option
                {
                    //saveFile(price)
                } 

                if(midChoice==6)   //if user chooses option
                {
                    //saveFile(year)
                } 
                if(midChoice==7)   //if user chooses option
                {
                    //saveFile(genre)
                } 

            }

            if(choice==3)   //if user chooses to display summary
            {
                //displaySummary(args)
            }
        }
    }   
}