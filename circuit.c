/*****************************************************/
/*                                                   */
/* Created By George Papageorgiou                    */
/* AEM: 2600                                         */
/*                                                   */
/*****************************************************/ 

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

//#include "structs.h"
#include "functions.h"

#define SIZE 512
#define logSize 500
#define MAX_FANOUT 50

int main(int argc, char *argv[]){
    char input[500];
    char output[500];
    char wires[3000];
    char data[logSize];
    char numberToString[20];
    
    struct gate *listhead;
    struct wire *headwire;

    //Setting Variables to 0 or \0
    SetZero(input, 500);
    SetZero(output, 500);
    SetZero(wires, 3000);
    SetZero(data, logSize);

    struct stat st = {0};
    if (stat("data", &st) == -1) {
        mkdir("data", 0700);
    }
    
    //Delete file in the folder in order to be fresh every run
    if (remove("data/inputs_outputs_logs.txt") == 0) {
    } else {
        printf("The file is not deleted.");
    }

    //Create List to work with
    listhead = CreateInitialList(input, output, wires);
    if(listhead == NULL){
        printf("Oups, spaceship is launching!\n");
        return -1;
    }

    //Setting pointers pointing to wire list to NULL
    //Both input array and output
    null(listhead);

    //Now i will count and save count of inputs and outputs
    counts(listhead);
    
    //Creating wire list
    headwire = InitializeWireList(input, output, wires);
    if(headwire == NULL){
        printf("Going to Space!!!!!!!!!\n");
        return -1;
    }

    ////////////////////////////////////
    //                                //
    //                                //
    //   CONNECT LIST WIRE-GATE NOW   //
    connect(listhead, headwire);
    //                                //
    //                                //
    ////////////////////////////////////

    //Enter time of repeats
    //Default will start with 0 for D Flip flop
    int repeat;

    printf("Enter how many passes you want to perform: ");
    scanf(" %d", &repeat);


    //Count inputs and ask user to enter them

    //In a loop perform iterations for the circuit.
    //Parts that will get value after the first iteration
    //Will start with 0 as default
    char search[20];
    SetZero(search, 20);
    int counterforinput=0;
    int pos=0;
    int valuetopass;
    struct wire *tmp;

    inputFix(input);

   struct gate *gateList3Head;
    restGatesLeveled(listhead, headwire, 0);
    gateList3Head = rebuildLevelOrderLayer0(&listhead);
    
    //setting levels to the next wires.
    levelingWireAfterGate(gateList3Head, headwire, 0);

    //This function does the connection between the lists
    //It builds a new list without more memmory allocated
    //The new list is in levels.
    buildCircuitLeveled(&listhead, gateList3Head);


    //Not correct need to do that after setting levels
    //Running circuit
    for(int counter=0; counter<repeat; counter++){
        strcat(data, "Run #"); //Fixing string to store input-output into a file
        sprintf(numberToString, "%d\n", counter);
        strcat(data, numberToString);

        while(strlen(input) >= counterforinput ){
            if(input[counterforinput] == ' ' || counterforinput == strlen(input)){
                printf("Please enter Value for input wire (%s): ", search);
                //Need a scanf here with search wire list and store the value
                scanf(" %d", &valuetopass);
                
                //All these strcats are building the string to go to output
                strcat(data, "\tWire "); 
                strcat(data, search);
                strcat(data, ": ");
                sprintf(numberToString, "%d\n", valuetopass);
                strcat(data, numberToString);
                
                tmp = FindCheck(headwire, search);
                if(tmp == NULL ){
                    printf("Something went wrong!\n");
                    return -1;
                }
                tmp->value = valuetopass;
                SetZero(search, 20);
                counterforinput++;
                pos=0;
            }
            else{
                search[pos] = input[counterforinput];
                pos++;
                counterforinput++;
            }
        }

        //Calculate new values of the circuit
        //make function void update circuit status
        run(gateList3Head, headwire);
        // Update output value on file
        //Write the data in to the file to have metrics 
        dataToFile(data);
        printGateToFile(gateList3Head);
        SetZero(data, logSize);
        counterforinput=0;
    }

    
    printf("\n############################ Printing Level Order List ############################\n\n");
    printGate(gateList3Head);

    //Free the list
    FreeMem(&listhead);
    FreeMemWire(&headwire);
    FreeMem(&gateList3Head);
    printf("\n");
    
    return 0;
}

