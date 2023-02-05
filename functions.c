#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "functions.h"
#include "readFileData.h"

#define SIZE 512
#define MAX_FANOUT 50
#define wireStringSize 4000
#define gateStringSize 1000
#define outputStringSize 2000
#define inputStringSize 2000
#define lineCount 1000
#define wordLength 4000
#define fileOpen "s27_mapped.v"
//#define fileOpen "s1423_mapped.v"


/************************************************ DO NOT TOUCH WORKING PERFECTLY *********************************************************************/
//Just printing all gates
void printGate(struct gate *head){

    struct gate *curr = head;
    int counter;

    while(curr!=NULL){
       counter=0;
       printf("Gate with name: %s has type: %s and level: %d.\n", curr->gate_name, curr->gate_type,curr->layer);
       curr = curr->next;
    }

}

//Just printing all wires
void printWire(struct wire *head){

    struct wire *curr = head;

    while(curr!=NULL){
       printf("Node wire with name: %s and Value: %d\n", curr->node_name, curr->value);
       printf("Current level is: %d\n", curr->layer);
       curr = curr->next;
    }
}

//Set string to \0
void SetZero(char str[], int size){
    int counter;

    for(counter=0; counter<size; counter++){
        str[counter] = '\0';
    }

}

//Clear memory droping the real head
void FreeMem(struct gate **ptr){
    struct gate *curr;
    struct gate *next;

    curr = *ptr;

    while(curr != NULL){
        next = curr->next;
        free(curr);
        curr = next;
    }

    *ptr = NULL;

    return;
}

void FreeMemWire(struct wire **ptr){
    struct wire *curr;
    struct wire *next;

    curr = *ptr;

    while(curr != NULL){
        next = curr->next;
        free(curr);
        curr = next;
    }

    *ptr = NULL;

    return;
}


struct wire *FindCheck(struct wire *ptr, char stringforsearch[]){
    struct wire *curr;

    curr=ptr;

    while(curr != NULL){

        if(strcmp(curr->node_name, stringforsearch) == 0){
            return curr;
        }
        else{
            curr = curr->next;
        }
    }

    return NULL;
}

void null(struct gate *head){
    struct gate *curr;
    int counter;

    curr = head;

    while(curr!=NULL){
        for(counter=0; counter<50; counter++){
            curr->inputs[counter] = NULL;
        }
        for(counter=0; counter<2; counter++){
            curr->outputs[counter] = NULL;
        }
        curr = curr->next;
    }
}

int checkOccurrences(char *s,char c) {
    int i,count=0;
     for(i=0;s[i];i++)  
    {
    	if(s[i]==c)
    	{
          count++;
		}
 	}
 	return count;  
 }

void connect(struct gate *headgate, struct wire *headwire){
    struct gate *currgate;
    struct wire *currwire;
    int counter, i=0, j=0, flag=0;
    char temp[100] = {'\0'};
    int variablesCount;
     int testsdasta, testsdasta1=0;

    //Loop to go throw gate list and connect it times as inputs to the correct wire list
    for(currgate = headgate; currgate != NULL; currgate = currgate->next){

        variablesCount = checkOccurrences(currgate->gate_inputs, ',');

        if(variablesCount == 0) { // Only 1 input
            strcpy(temp, currgate->gate_inputs);
            currwire = FindCheck(headwire, temp);
            if(currwire == NULL){
                printWire(headwire);
                printGate(headgate);
                printf("Something went wrong finding wire: %s!\n", temp);
                exit(1);
            }
            currgate->inputs[0] = currwire;
        }

        //More than 1 inputs
        else{
            i=0;
            for(counter=0; counter<variablesCount; counter++){
                j=0;
                SetZero(temp,100);
                while(currgate->gate_inputs[i] != ',' && flag == 0){
                    if(i == strlen(currgate->gate_inputs)){
                        flag = 1;
                    }
                    temp[j] = currgate->gate_inputs[i];
                    i++;
                    j++;
                }
            
                currwire = FindCheck(headwire, temp);
                if(currwire == NULL){
                    printWire(headwire);
                    printf("Something went wrong finding wire: %s!\n", temp);
                    exit(1);
                }
                i++;

                //Build pointers from here on
                currgate->inputs[counter] = currwire;
            }

            SetZero(temp, 100);
           testsdasta1=0;

            for(testsdasta=i; testsdasta<strlen(currgate->gate_inputs); testsdasta++) {
                temp[testsdasta1] = currgate->gate_inputs[testsdasta];
                testsdasta1++;
            }

                currwire = FindCheck(headwire, temp);
                if(currwire == NULL){
                    printWire(headwire);
                    printf("Something went wrong finding wire: %s!\n", temp);
                    exit(1);
                }
                currgate->inputs[counter] = currwire;
        }

        i=0;
    }

    //Set zero to temp string
    SetZero(temp,100);

    //Loop to go throw gate list and connect it times as outputs to the correct wire list
    for(currgate = headgate; currgate != NULL; currgate = currgate->next){
        flag=0;
    
        variablesCount = checkOccurrences(currgate->gate_output, ',');
        if(variablesCount == 0) { // Only 1 output
            strcpy(temp, currgate->gate_output);
            currwire = FindCheck(headwire, temp);
            if(currwire == NULL){
                printWire(headwire);
                printf("Something went wrong finding wire: %s!!!!\n", temp);
                exit(1);
            }
            currgate->outputs[0] = currwire;
        }
        else {
            for(counter=0; counter<variablesCount; counter++){
                SetZero(temp,100);
                j = 0;
                while(currgate->gate_output[i] != ',' && flag == 0){
                    if(i == strlen(currgate->gate_output)){
                        flag = 1;
                    }
                    temp[j] = currgate->gate_output[i];
                    i++;
                    j++;
                }
            
                currwire = FindCheck(headwire, temp);
                if(currwire == NULL){
                    printf("Something went wrong!!\n");
                    exit(1);
                }
                SetZero(temp,100);
                j=0;
                i++;

                //Build pointers from here on
                currgate->outputs[0] = currwire;
            }

            SetZero(temp, 100);
            testsdasta1=0;

            for(testsdasta=i; testsdasta<strlen(currgate->gate_output); testsdasta++) {
                temp[testsdasta1] = currgate->gate_output[testsdasta];
                testsdasta1++;
            }

                currwire = FindCheck(headwire, temp);
                if(currwire == NULL){
                    printWire(headwire);
                    printf("Something went wrong finding wire: %s!\n", temp);
                    exit(1);
                }
                currgate->outputs[1] = currwire;
        }
        i=0;
    }

}
/******************************************************************************************************************************************************/


struct gate *rebuildLevelOrderLayer0(struct gate **head){
    struct gate *newHead;
    struct gate *newListTemp;
    struct gate *temp, *ptrReFix;

    temp = *head;

    //Find node with level 0 to make it head to the new list
    while(temp->layer != 0){
        ptrReFix = temp;
        temp = temp->next;
    }
    
    //Node must been found
    newHead = temp;

    //Check if that node was head and build head if so
    if(temp == *head){
        *head = temp->next;
    }
    else{
        //Fix pointers for next, and the old list
        ptrReFix->next = temp->next;
    }  
    
    newHead->next = NULL;
    
    temp = *head;
    newListTemp = newHead; 

    //Build evrything in level 0
    while(temp != NULL){

        if(temp->layer == 0){
            if(temp == *head){
                *head = temp->next;
                newListTemp->next = temp;
                newListTemp = newListTemp->next;
                temp = temp->next;
                newListTemp->next = NULL;
                continue;
            }
            else{
                newListTemp->next = temp;
                newListTemp = newListTemp->next;
                newListTemp->next = NULL;
                ptrReFix->next = temp->next;
            }
        }
        ptrReFix = temp;
        temp = temp->next;
    }
    return newHead;
}

void restGatesLeveled(struct gate *head, struct wire *headWire, int level){

    struct gate *iterator;
    int counter, inputCount;
    int pos;
    int flag;

    iterator = head;

    //Set levels
    while(iterator != NULL){
        if(strcmp(iterator->gate_type,"D_Flif_Flop") == 0 ){
            iterator = iterator->next;
            continue;
        }

        //initialize count
        counter = 0;
        inputCount = 0;
        //Finding number of inputs of a gate so later to check the layer of the wire
        while(iterator->inputs[counter] != NULL){
            inputCount++;
            counter++;
        }
        iterator->input = inputCount;
        flag = 0;
        for(pos=0; pos<inputCount; pos++){
            if(iterator->inputs[pos]->layer <= level && iterator->inputs[pos]->layer >= 0){
                //Do nothing
            }
            else{
                flag=1;
            }
        }
        if(flag == 0){
            iterator->layer = level;
        }
        else{
            iterator->layer = -1;
        }
        iterator = iterator->next;
    }

}

//Curently not in Use
int wireLevelReturn(struct wire *headWire, char *name){
    struct wire *tmp;

    tmp = headWire;

    while(tmp != NULL){
        if(strcmp(tmp->node_name, name) == 0){
            return tmp->layer;
        }
        tmp = tmp->next;
    }
    return -1;
}

//This function fixes the level of the wires after a gate in the final list, starting from given level
void levelingWireAfterGate(struct gate *headGate, struct wire *wireHead, int level){
    struct gate *iterator;

    iterator = headGate;

    while(iterator != NULL){
        if(iterator->layer == level){
            iterator->outputs[0]->layer = iterator->layer + 1;
            if(iterator->outputs[1] != NULL){
                iterator->outputs[1]->layer = iterator->layer + 1;
            }
        }
       iterator = iterator->next; 
    }
}

//Working Fine to give a gate a level based on wire inputs given level.
void fixGateLevel(struct gate **head, struct gate *list3, int level){
    struct gate *iterator, *finalList, *tmp, *tmp1;
    int pos, flag, maxLayer;
    int inputCount;
    int times=0;
    int stored=0;

    iterator = *head;
    tmp1 = *head;

    while(tmp1 != NULL){
        stored++;
        tmp1 = tmp1->next;
    }

    finalList=list3;

    //Go to the end of the final list
    while(finalList->next != NULL){
        finalList = finalList->next;
    }
    
    int newtemp;

    while(iterator != NULL){
        times++;
        pos = 0;
        inputCount = 0;
        maxLayer = 0;
        flag = 0;

        //Count gate inputs
        while(iterator->inputs[pos] != NULL){
            inputCount++;
            pos++;
        }

        for(newtemp=0; newtemp<inputCount; newtemp++){
            //Check if wires are valid
            if(iterator->inputs[newtemp]->layer > -1 && iterator->inputs[newtemp]->layer <= level){
                if(iterator->inputs[newtemp]->layer > maxLayer){
                    maxLayer = iterator->inputs[newtemp]->layer;
                }
            }
            else{
                flag=1;
                break;
            }
        }

        if(flag == 0){
            //check if head
            if(iterator == *head){
                *head = (*head)->next;
                finalList->next = iterator;
                finalList = finalList->next;
                finalList->next = NULL;
                finalList->layer = maxLayer;
                
                //Fix now wire level after the inserted gate
                finalList->outputs[0]->layer = maxLayer + 1;
                if(finalList->outputs[1] != NULL){
                    finalList->outputs[1]->layer = maxLayer + 1;
                }

                iterator = *head;
                continue;
            }
            //else add and fix ptrs
            else{
                tmp->next = iterator->next;
                finalList->next = iterator;
                finalList = finalList->next;
                finalList->next = NULL;
                finalList->layer = maxLayer;
                
                //Fix now wire level after the inserted gate
                finalList->outputs[0]->layer = maxLayer + 1;
                if(finalList->outputs[1] != NULL){
                    finalList->outputs[1]->layer = maxLayer + 1;
                }
                iterator = tmp->next;
                continue;
            }
        }
        else{
            tmp=iterator;
            iterator = iterator->next;
            continue;
        }
        iterator = iterator->next;
    }
}

void buildCircuitLeveled(struct gate **head, struct gate *list3){

    int levelToBuild; //Remember level 0 is already built

    levelToBuild = 1; //Start from level 1 since level 0 is ready
    while(*head != NULL){
        fixGateLevel(head, list3, levelToBuild);
        levelToBuild++;
    }

}

void dataToFile(char *dataStr){
    
    FILE *fp;

    fp = fopen("data/inputs_outputs_logs.txt", "a");


    /* fopen() return NULL if last operation was unsuccessful */
    if(fp == NULL)
    {
        /* File not created hence exit */
        printf("Unable to create or open file.\n");
        exit(EXIT_FAILURE);
    }

    fprintf(fp, "%s\n", dataStr);

    fclose(fp);
}


char *getType(struct gate *node){
    return (node->gate_type);
}

void calculateValues(struct gate *gateToCalculate) {
        
        //Based on gate type, perform actions numbers
        //place it on the wire
        if(strcmp(gateToCalculate->gate_type,"OR") == 0){
            gateToCalculate->outputs[0]->value = valueGateOR(gateToCalculate);
        }
        else if(strcmp(gateToCalculate->gate_type,"NOR") == 0){
            gateToCalculate->outputs[0]->value = valueGateNOR(gateToCalculate);
        }
        else if(strcmp(gateToCalculate->gate_type,"AND") == 0){
            gateToCalculate->outputs[0]->value = valueGateAND(gateToCalculate);
        }
        else if(strcmp(gateToCalculate->gate_type,"NAND") == 0){
            gateToCalculate->outputs[0]->value = valueGateNAND(gateToCalculate);
        }
        else if(strcmp(gateToCalculate->gate_type,"Inverter") == 0){
            gateToCalculate->outputs[0]->value = valueGateInverter(gateToCalculate);
        }
        else if(strcmp(gateToCalculate->gate_type,"D_Flif_Flop") == 0){
            gateToCalculate->outputs[0]->value = valueGateDFlipFlop(gateToCalculate);
        }

}

int valueGateOR(struct gate *node){
    int counter;
    struct gate *nodeItr;

    counter=0;
    
    nodeItr = node;
    
    while (nodeItr->inputs[counter] != NULL){
        counter++;
    }

    for(int inpunt_count=0; inpunt_count<counter; inpunt_count++){
        // In OR gate if 1 is found as one input output is 1
        if(nodeItr->inputs[inpunt_count]->value == 1){
            nodeItr->value = 1;
            return 1;
        }
        nodeItr->value = 0;
    }
    
    return 0;
}

int valueGateNOR(struct gate *node){
    int counter;
    struct gate *nodeItr;

    counter=0;
    nodeItr = node;
    
    while (nodeItr->inputs[counter] != NULL){
        counter++;
    }

    for(int inpunt_count=0; inpunt_count<counter; inpunt_count++){
        // In OR gate if 1 is found as one input output is 1
        if(nodeItr->inputs[inpunt_count]->value == 1){
            nodeItr->value = 0;
            return 0;
        }
    }
    nodeItr->value = 1;

    return 1;
}

int valueGateAND(struct gate *node){
    int counter, flag;
    struct gate *nodeItr;

    counter=0;
    nodeItr = node;
    
    while (nodeItr->inputs[counter] != NULL){
        counter++;
    }

    //flag represents if value of 1 has been found
    flag=0;

    for(int inpunt_count=0; inpunt_count<counter; inpunt_count++){
        // In OR gate if 1 is found as one input output is 1
        if(nodeItr->inputs[inpunt_count]->value == 1){
            flag=1;
        }
        if(nodeItr->inputs[inpunt_count]->value == 0 && flag == 1){
            nodeItr->value = 0;
            return 0;
        }
    }
    nodeItr->value = 1;

    return 1;
}

int valueGateNAND(struct gate *node){
    int counter, flag;
    struct gate *nodeItr;

    counter=0;
    nodeItr = node;
    
    while (nodeItr->inputs[counter] != NULL){
        counter++;
    }

    //flag represents if value of 1 has been found
    flag=0;

    for(int inpunt_count=0; inpunt_count<counter; inpunt_count++){
        // In OR gate if 1 is found as one input output is 1
        if(nodeItr->inputs[inpunt_count]->value == 1){
            flag=1;
        }
        if(nodeItr->inputs[inpunt_count]->value == 0 && flag == 1){
            nodeItr->value = 1;
            return 1;
        }
    }
    nodeItr->value = 0;
    return 0;
}

int valueGateInverter(struct gate *node){
    int returnValue = node->inputs[0]->value;;
    if (returnValue == 1){
        node->value = 0;
        return 0;
    }
    else{
        node->value = 0;
        return 1;
    }
}

int valueGateDFlipFlop(struct gate *node){
    if(node->inputs[1] == NULL) {
        node->value = node->inputs[0]->value;
        return node->inputs[0]->value;
    }
    else {
        node->value = node->inputs[1]->value;
        return node->inputs[1]->value;
    }
}

//Function that prints the gates values at each run to the file
void printGateToFile(struct mapping *head){

    struct mapping *curr = head;
    int counter;
    FILE *pFile;

    pFile=fopen("data/inputs_outputs_logs.txt", "a");
    if(pFile==NULL) {
        perror("Error opening file.");
        return;
    }

    while(curr != NULL) {
        counter=0;
        while (curr->gatesLevel[counter] != NULL) {
            fprintf(pFile, "Gate with name: %s has value of: %d\n", curr->gatesLevel[counter]->gate_name, curr->gatesLevel[counter]->value);
            counter++;
        }
        curr = curr->mappingNext;
    }

    fclose(pFile);
}


struct gate *create(struct wire *headwire, char *input) {
    char array[1000][4000]; //This array contains each line from the verilog file
    struct gate *head, *curr, *tempHead;
    int counter;

    prepareData(fileOpen, array);

    //Gates information will be at position 4. In case not, increase lenth from 3000 to a higher value
    head = (struct gate *)malloc(sizeof(struct gate));
    if(head == NULL){
        printf("Error allocating memmory for the head of the gate list!\n");
        return head;
    }
    tempHead = head;
    initGateInstance(head, array[4]);
    counter = 5;
    while (array[counter][0] != 'e'){
        curr = (struct gate *)malloc(sizeof(struct gate));
        if(curr == NULL){
            printf("Error allocating memmory for the head of the gate list!\n");
            return curr;
        }
        initGateInstance(curr, array[counter]);
        tempHead -> next = curr;
        tempHead = curr;
        counter++;
    }

    createWireList(array[1], array[2], array[3], headwire);
    strcpy(input, array[1]);

    return head;
}

void createWireList(char inputString[], char outputString[], char wireString[], struct wire *head) {
    struct wire *curr, *temp;
    char wireName[20];
    int counter=5, i=0, flag=0;

    while(inputString[counter]!= ','){
        wireName[i] = inputString[counter];
        i++;
        counter++;
    }

    initWireInstance(head, wireName);
    head->layer = 0; //Level is 0 because it is input to the circuit
    temp = head;
    counter++; //go after comma

    //create inputWires
    while(inputString[counter] != ';' && flag != 1){
        SetZero(wireName, 20);
        i=0;
        while(inputString[counter] != ',' && flag != 1){
            if(inputString[counter+1] == ';'){
                flag = 1;
            }
            wireName[i] = inputString[counter];
            i++;
            counter++;
        }
        curr = (struct wire *)malloc(sizeof(struct wire));
        if(curr == NULL){
            printf("Error allocating memmory for wire list\n");
            return;
        }
        initWireInstance(curr, wireName);
        curr->layer = 0;
        temp -> next = curr;
        temp = curr;
        counter++;
    }
    
    counter = 6; //Position ending output
    flag = 0;

    //Create output wires
    while(outputString[counter] != ';' && flag != 1){
        SetZero(wireName, 20);
        i=0;
        while(outputString[counter]!= ','){
            if(outputString[counter+1] == ';'){
                wireName[i] = outputString[counter];
                i++;
                counter++;
                flag = 1;
                break;
            }
            wireName[i] = outputString[counter];
            i++;
            counter++;
        }
        if(wireName[strlen(wireName)] == ';') {
            wireName[strlen(wireName)] = '\0';
        }

        curr = (struct wire *)malloc(sizeof(struct wire));
        if(curr == NULL){
            printf("Error allocating memmory for wire list\n");
            return ;
        }
        initWireInstance(curr, wireName);
        temp -> next = curr;
        temp = curr;
        counter++;
    }

    counter = 4; //Position ending wire word
    flag = 0;
    //Create wire wires
    while(wireString[counter] != ';' && flag != 1){
        SetZero(wireName, 20);
        i=0;
        while(wireString[counter]!= ','){
            if(wireString[counter+1] == ';'){
                wireName[i] = wireString[counter];
                i++;
                counter++;
                flag = 1;
                break;
            }
            wireName[i] = wireString[counter];
            i++;
            counter++;
        }
        curr = (struct wire *)malloc(sizeof(struct wire));
        if(curr == NULL){
            printf("Error allocating memmory for wire list\n");
            return;
        }
        initWireInstance(curr, wireName);
        temp -> next = curr;
        temp = curr;
        counter++;
    }
}

void initWireInstance(struct wire *ptr, char *data){
    ptr->next = NULL;
    ptr->layer = -1;
    ptr->value = 0;
    strcpy(ptr->node_name, data);
}

void initGateInstance(struct gate *ptr, char *data) {
    ptr->next = NULL;
    ptr->layer = -1;

    //Set type
    setType(ptr, data);

    //Set name
    setName(ptr, data);

    //Set input - output of the gate
    setInutsOutputs(ptr, data);

    initLevelOfDFlipFlops(ptr);
}

void initLevelOfDFlipFlops(struct gate *ptr) {
    struct gate *iterator;

    iterator = ptr;

    while(iterator != NULL){
        if(strcmp(iterator->gate_type, "D_Flif_Flop") == 0) {
            iterator->layer = 0;
        }
        iterator = iterator->next;
    }
}

void setInutsOutputs(struct gate *ptr, char *data) {
    int counter, i = 0;
    char inputs[300];
    char outputs[300];
    char newData[lineCount];


    SetZero(inputs, 300);
    SetZero(outputs, 300);
    SetZero(newData, lineCount);

    while (data[i] != '('){
        i++;
    }
    i++; //Now we have passed the first '(' -> meaning we arrived at input output

    if(data[i] == '.'){
        i++;
    }

    for(counter = 0; counter< strlen(data); counter++) {
        newData[counter] = data[i];
        i++;
    }

    if(strcmp(ptr->gate_type, "D_Flif_Flop") == 0){
        setInputsFlipFlop(ptr, newData);
    }
    else if(strcmp(ptr->gate_type, "INVERTER") == 0){ //Not flip flop
        setInputsInverter(ptr, newData);
    }
    else{
        setInputsAndNorOr(ptr, newData);
    }

}

void setInputsFlipFlop(struct gate *ptr, char *data) {
        char outputs[300];
        char inputs[300];
        int i, j, counter;
        int flag = 0 ;

        SetZero(outputs, 300);
        SetZero(inputs, 300);
        j=0;
        i=0;
        counter = 0;
        while(i<strlen(data)) {
            if(data[i] == ','){
                i++;
            }
            if(data[i] == '.'){
                i++;
            }
            if(data[i] == 'D'){
                i = i+2;
                while(data[i] != ')'){
                    inputs[j] = data[i];
                    i++;
                    j++;
                }
            }
            else if(data[i] == 'C'){
                i++; //We do not use CLK at the momment
            }
            else if(data[i] == 'Q') {
                i++;
                if(data[i] == '('){
                    i++;
                    while(data[i] != ')'){
                        outputs[counter] = data[i];
                        counter++;
                        i++;
                        flag = 1;
                    }
                }
                else if (data[i] == 'N'){
                    if(flag == 1) {
                        outputs[counter] = ',';
                        counter++;
                    }
                    i = i + 2;
                    while (data[i] != ')'){
                        outputs[counter] = data[i];
                        counter++;
                        i++;
                    }
                }
            }
            else{
                i++;
            }
        }
        strcpy(ptr->gate_inputs, inputs);
        strcpy(ptr->gate_output, outputs);
}

void setInputsInverter(struct gate *ptr, char *data) {
        
        char outputs[300];
        char inputs[300];
        int i, j, counter;

        SetZero(outputs, 300);
        SetZero(inputs, 300);
        counter=0;
        i=0;
        j=0;
        while(i<strlen(data)) {
            if(data[i] == ','){
                i++;
                continue;
            }
            if(data[i] == '.'){
                i++;
                continue;
            }
            if(data[i] == 'Z'){
                while(data[i] != '('){ 
                    i++;
                }
                i++;
                
                while(data[i] != ')'){
                    outputs[counter] = data[i];
                    i++;
                    counter++;
                }
                strcpy(ptr->gate_output, outputs);
            }
            else if(data[i] == 'A') {
                i++;
                while(data[i] != '(') {
                    i++;
                }
                i++;
                while(data[i] != ')'){
                    inputs[j] = data[i];
                    j++;
                    i++;
                }
                strcpy(ptr->gate_inputs, inputs);
            }
            i++;
        }
}

void setInputsAndNorOr(struct gate *ptr, char *data) {
        char outputs[300];
        char inputs[300];
        int i, j, count;

        SetZero(outputs, 300);
        SetZero(inputs, 300);
        j=0;
        i = 0;
        count = 0;
        while(i<strlen(data)) {
            if(data[i] == ','){
                i++;
            }
            if(data[i] == '.'){
                i++;
            }
            if(data[i] == 'A'){
                i++;
                while(data[i] != '('){
                    i++;
                }
                i++;
                if(count>0){
                    inputs[j] = ',';
                    j++;
                }
                while(data[i] != ')'){
                    inputs[j] = data[i];
                    i++;
                    j++;
                }
                count++;
            }
            else if(data[i] == 'Z') {
                j=0;
                i++;
                if(data[i] == 'N'){
                    i++;
                }
                
                if(data[i] == '('){
                    i++;
                    while(data[i] != ')'){
                        outputs[j] = data[i];
                        j++;
                        i++;
                    }
                }
            }
            i++;
        }

        strcpy(ptr->gate_inputs, inputs);
        strcpy(ptr->gate_output, outputs);
}

void setName(struct gate *ptr, char *data) {

int i=0, j=0;
char name[30];

SetZero(name, 30);

    if(strcmp(ptr->gate_type, "D_Flif_Flop") == 0) {
        i = 2;
        while(data[i] != 'D'){
            // data++;
            i++;
        } 
    }
    else {
        while(data[i] != 'U'){
            i++;
        }
    }
    while (data[i] != '('){
        name[j] = data[i];
        j++;
        i++;
    }
    SetZero(ptr->gate_name, 50);
    strcpy(ptr->gate_name, name);
}

void setType(struct gate *ptr, char *data) {
    if(data[0] == 'N') {
        if(data[1] == 'A') {
            strcpy(ptr->gate_type, "NAND");
        }
        else{
            strcpy(ptr->gate_type, "NOR");
        }
    }
    if(data[0] == 'A') {
        strcpy(ptr->gate_type, "AND");
    }
    if(data[0] == 'O') {
        strcpy(ptr->gate_type, "OR");
    }
    if(data[0] == 'D') {
        strcpy(ptr->gate_type, "D_Flif_Flop");
    }
    if(data[0] == 'I') {
        strcpy(ptr->gate_type, "INVERTER");
    }
}

void fixFlipFlopWires(struct gate *listhead) {
    struct gate *temp;

    temp = listhead;
    while (temp != NULL){
        if (strcmp(temp->gate_type, "D_Flif_Flop") == 0){
            temp->outputs[0]->layer = 1;
            if (temp->outputs[1] != NULL){
                temp->outputs[1]->layer = 1;
            }
            
        }
        
        temp = temp->next;
    }
    
}

void levelGatesInitialList(struct gate *listhead) {
    struct gate *temp;

    while(allGatesHaveLevels(listhead) != 0) {
        temp = listhead;
        while (temp != NULL){
            //Fix gate layer
            gateFix(temp);
            temp = temp->next;
        }
    }
}

void gateFix(struct gate *gateToLevel) {
    int max = 0;
    int flag = 0;
    int counter = 0;

    if(strcmp(gateToLevel->gate_type, "D_Flif_Flop") == 0) {
        gateToLevel->outputs[0]->layer = 1;
        if(gateToLevel->outputs[1] != NULL) {
            gateToLevel->outputs[1]->layer = 1;
        }
        return;
    }

    while (gateToLevel->inputs[counter] != NULL) {
        if(gateToLevel->inputs[counter]-> layer == -1) {
            flag = 1;
            break;
        }
        if(max < gateToLevel->inputs[counter]-> layer){
            max = gateToLevel->inputs[counter]-> layer;
        }
        counter++;
    }

    if(flag != 1) {
        gateToLevel->layer = max;

        gateToLevel->outputs[0]->layer = max + 1;
    }
    else{
        gateToLevel->layer = -1;
        gateToLevel->outputs[0]->layer = -1;
    }
    
}

int allGatesHaveLevels(struct gate *listhead) {
    struct gate *temp;

    temp = listhead;
    while (temp!=NULL){
        if (temp->layer < 0){
            return 1;
        }
        
        temp = temp->next;
    }

    return 0;
}

int getMaxLevel(struct gate *head) {
    struct gate *itterator;
    int max = 0;

    itterator = head;

    while (itterator != NULL){
        if(itterator->layer > max){
            max = itterator->layer;
        }
        itterator = itterator->next;
    } 
    return max;
}

void clearPtrs(struct gate *gatesLevel[]) {
    int counter = 0;

    for(counter=0; counter < 50; counter++){
        gatesLevel[counter] = NULL;
    }
}

struct mapping * leveled(struct gate *head) {
    struct gate *itterator;
    struct mapping *headMapping;
    struct mapping *temp;
    struct mapping *prev;
    struct mapping *itteratorThirdList;
    int level = 0;
    int positionToAdd = 0;
    int nodesCreated = 0;
    int nrOfNodesCreated = 0;

    headMapping = (struct mapping *)malloc(sizeof(struct mapping));
    if(headMapping == NULL){
        printf("Error creating new list\n");
        return NULL;
    }
    nrOfNodesCreated++;
    headMapping->mappingNext = NULL;
    clearPtrs(headMapping->gatesLevel);
    prev = headMapping;

    for(nodesCreated=1; nodesCreated <= getMaxLevel(head); nodesCreated++) {
        temp = (struct mapping *)malloc(sizeof(struct mapping));
        nrOfNodesCreated++;
        if(temp == NULL){
            printf("Error creating new list\n");
            return NULL;
        }
        temp->mappingNext = NULL;
        clearPtrs(temp->gatesLevel);
        prev->mappingNext = temp;
        prev = prev->mappingNext;
    }
    printGate(head);
    printf("Max level spoted: %d and created in total: %d\n", getMaxLevel(head), nodesCreated);
    
    itteratorThirdList = headMapping;
    
    for(level = 0; level <= getMaxLevel(head); level++){
        itterator = head;
        printf("\n\n");
        printGate(itterator);
        printf("\n\n");

        positionToAdd = 0;

        while (itterator != NULL){
            if(itterator->layer == level) {
                itteratorThirdList->gatesLevel[positionToAdd] = itterator;
                positionToAdd++;
            }
            itterator = itterator->next;
        }

        itteratorThirdList = itteratorThirdList->mappingNext;
    }


printf("\n\n");
printf("\n\n");
        printf("Node 1: %s, Node 2: %s, Node 3: %s, Node 4: %s, Node 5: %s\n", headMapping->gatesLevel[0]->gate_name, 
        headMapping->mappingNext->gatesLevel[0]->gate_name, headMapping->mappingNext->mappingNext->gatesLevel[0]->gate_name,
        headMapping->mappingNext->mappingNext->mappingNext->gatesLevel[0]->gate_name,
        headMapping->mappingNext->mappingNext->mappingNext->mappingNext->gatesLevel[0]->gate_name);
printf("\n\n");
printf("\n\n");
    return headMapping;
}

void createNewLeveledList(struct gate *listhead, struct gate *gateList3Head) {

    int level, maxLevel=0;
    struct gate *temp;
    struct gate *helper;

    temp = listhead;

    //Get the max level
    while (temp != NULL){
        if(maxLevel<temp->layer){
            maxLevel = temp->layer;
        }
        temp = temp->next;
    }

    temp = listhead;

    level = 0;
    while (level<=maxLevel){
        while (temp != NULL){
            if(temp->layer == level){
                helper = temp->next;
                addNode(temp, &listhead, gateList3Head);
                temp = helper;
            }
            else{
                temp = temp->next;
            }
            
        }
        level++;
    }
}

void addNode(struct gate *node, struct gate **head, struct gate *newList) {
    struct gate *temp;
    struct gate *tempOldList;
    struct gate *tempPrevious;

    //Case node is the head of the list
    if (*head == node){
        printf("Head of old list\n");
        temp = newList;
        *head = node->next;
        node->next = NULL;
        
        // Case newList is empty
        if(newList == NULL){
            copyNode(newList, node);
            return;
        }
        else{ //New list is not empty
            while(temp != NULL) {
                if(temp->next == NULL) {
                    temp->next = node;
                    return;
                }
                temp = temp->next;
            }
        }
    }

    //Random on the list
    temp = newList;
    tempOldList = *head;
    tempOldList = tempOldList->next;
    tempPrevious = *head;

    while (tempOldList != node){
        tempOldList = tempOldList->next;
        tempPrevious = tempPrevious->next;
    }
    tempPrevious->next = tempOldList->next;
    node->next = NULL;

    //Case newList is empty
    if(newList == NULL){
        copyNode(newList, node);
        return;
    }
    //Find position to place on the new list
     while(temp != NULL) {
        if(temp->next == NULL) {
            temp->next = node;
            return;
        }
        temp = temp->next;
    }
}

void copyNode(struct gate *newList, struct gate *node) {
    int counter = 0;

    strcpy(newList->gate_name, node->gate_name);
    strcpy(newList->gate_inputs, node->gate_inputs);
    strcpy(newList->gate_output, node->gate_output);
    strcpy(newList->gate_type, node->gate_type);
    newList->layer = node->layer;
    newList->outputs[0] = node->outputs[0];

    if(node->outputs[1] != NULL){
        newList->outputs[1] = node->outputs[1];
    }
    while(node->inputs[counter] != NULL){
        newList->inputs[counter] = node->inputs[counter];
    }
     
}

void adaptToOldCodeInputsLoop(char str[]) {
    int counter = 5;
    int i = 0;
    char temp[4000];
    SetZero(temp, 4000);

    while(str[counter] != ';') {
        if(str[counter] == ','){
            temp[i] = ' ';
        }
        else{
            temp[i] = str[counter];
        }
        counter++;
        i++;
    }

    SetZero(str, 4000);
    strcpy(str, temp);

}

void printMapping(struct mapping *head) {
    while (head != NULL) {
        for (int i = 0; i < 100; i++) {
            if (head->gatesLevel[i] != NULL) {
                printf("Gate name: %s\n", head->gatesLevel[i]->gate_name);
            }
        }
        head = head->mappingNext;
    }
}

// This function runs the circuit
void run(struct mapping *listOrder){

    struct mapping *iterator;
    int counter;

    iterator = listOrder;
    while(iterator != NULL) {
        counter = 0;
        while (iterator->gatesLevel[counter] != NULL) {
            calculateValues(iterator->gatesLevel[counter]);
            counter++;
        }
        iterator = iterator->mappingNext;
    }
}