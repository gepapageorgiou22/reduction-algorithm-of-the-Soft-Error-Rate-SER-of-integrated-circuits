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

#include "functions.h"
#include "readFileData.h"

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

    headwire = (struct wire *)malloc(sizeof(struct wire));
    if(headwire == NULL){
        printf("Error allocating memmory for wire list\n");
        return -1;
    }

    //Setting Variables to 0 or \0
    SetZero(input, 500);
    SetZero(output, 500);
    SetZero(wires, 3000);
    SetZero(data, logSize);

    struct stat st = {0};
    if (stat("data", &st) == -1) {
        mkdir("data", 0755);
    }

    //Delete file in the folder in order to be fresh every run
    if (remove("data/inputs_outputs_logs.txt") == 0) {
    } else {
        printf("The file is not deleted.");
    }

    //Create List to work with
    listhead = create(headwire);
    if(listhead == NULL){
        printf("Oups, spaceship is launching!\n");
        return -1;
    }
    printf("Got Here!\n");
    //Setting pointers pointing to wire list to NULL
    //Both input array and output
    null(listhead);

    if(headwire == NULL){
        printf("Going to Space!!!!!!!!!\n");
        return -1;
    }

    ////////////////////////////////////
    //                                //
    //   CONNECT LIST WIRE-GATE NOW   //
    connect(listhead, headwire);
    //                                //
    ////////////////////////////////////
  
    //Enter time of repeats
    //Default will start with 0 for D Flip flop
    int repeat;

    printf("Enter how many passes you want to perform: ");
    scanf(" %d", &repeat);

    //In a loop perform iterations for the circuit.
    //Parts that will get value after the first iteration
    //Will start with 0 as default
    char search[20];
    SetZero(search, 20);
    int counterforinput=0;
    int pos=0;
    int valuetopass;
    struct wire *tmp;
    int flag = 1;
    printGate(listhead);
    printf("before restGatesLeveled!\n");
    printf("before restGatesLeveled!\n");
    // printGate(listhead);

    //Issue from here
    // struct gate *gateList3Head = NULL;
    // gateList3Head = (struct gate *)malloc(sizeof(struct gate));
    // if(gateList3Head == NULL){
    //     printf("Error allocating memmory for wire list\n");
    //     return -1;
    // }
    printWire(headwire);

    levelGatesInitialList(listhead);
    
    printWire(headwire);
    printGate(listhead);

    // if(flag ==1 ) {
    //     return 0;
    // }

    // restGatesLeveled(listhead, headwire, 0);
    // // printf("After restGatesLeveled!\n");

    // gateList3Head = rebuildLevelOrderLayer0(&listhead);
    // // printf("After rebuildLevelOrderLayer0!\n");
    
    // printGate(gateList3Head);
    // printGate(listhead);

    // //setting levels to the next wires.
    // levelingWireAfterGate(gateList3Head, headwire, 0);
    // printf("After levelingWireAfterGate!\n");

    // //This function does the connection between the lists
    // //It builds a new list without more memmory allocated
    // //The new list is in levels.
    // buildCircuitLeveled(&listhead, gateList3Head);
    // printf("After buildCircuitLeveled!\n");

    // printf("Before createNewLeveledList\n");
    // createNewLeveledList(listhead, gateList3Head);
    // printf("After createNewLeveledList\n");
    struct mapping *test;

    test = leveled(listhead);
    // int i;
    // struct mapping * temp;
    // temp = test;
    
    // while(temp != NULL){
    //     i = 0;
    //     while(temp->gatesLevel[i] != NULL){
    //         printf("Gate name: %s and level: %d\n", temp->gatesLevel[i]->gate_name, temp->gatesLevel[i]->layer);
    //         i++;
    //     }
    //     temp = temp->mappingNext;
    // }
    // int flag1 =0;
    // if(flag1 == 0) {
    //     return 0;
    // }

    // printGate(gateList3Head);

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
        // run(gateList3Head, headwire);
        // Update output value on file
        //Write the data in to the file to have metrics 
        dataToFile(data);
        // printGateToFile(gateList3Head);
        SetZero(data, logSize);
        counterforinput=0;
    }

    
    printf("\n############################ Printing Level Order List ############################\n\n");
    // printGate(gateList3Head);

    //Free the list
    FreeMem(&listhead);
    FreeMemWire(&headwire);
    // FreeMem(&gateList3Head);
    printf("\n");
    
    return 0;
}

