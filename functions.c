#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "functions.h"

#define SIZE 512
#define MAX_FANOUT 50
#define wireStringSize 3000
#define gateStringSize 1000
#define outputStringSize 2000
#define inputStringSize 2000
#define fileOpen "s27_mapped.v"


//Creating all inputs that i will ask for a run
void inputFix(char input[]){
    int pos, counter;
    char temp[inputStringSize];

    SetZero(temp,inputStringSize);

    pos=0;
    while(pos + 8 <strlen(input)){
        input[pos] = input[pos+8];
        pos++;
    }

    pos=0;
    counter=0;

    while(input[pos] != ';'){
        if(input[pos] == ','){
            temp[counter] = input[pos+1];
            pos += 2;
            counter++;
        }
        temp[counter] = input[pos];
        counter++;
        pos++;
    }

    SetZero(input, inputStringSize);
    strcpy(input, temp);

    return;
}
/************************************************ DO NOT TOUCH WORKING PERFECTLY *********************************************************************/
//Just printing all gates
void printGate(struct gate *head){

    struct gate *curr = head;

    while(curr!=NULL){
       printf("Gate with name: %s has type: %s and level: %d\n", curr->gate_name, curr->gate_type,curr->layer);
       curr = curr->next;
    }

    curr = head;

    while(curr!=NULL){
       printf("Gate with name: %s has value of: %d\n", curr->gate_name, curr->value);
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

char * StringSplit(char tosplit[]){
    char temp[outputStringSize];
    char splited[outputStringSize];
    int counter = 0;
    int counter1 = 0;

    SetZero(temp, outputStringSize);
    SetZero(splited, outputStringSize);

    while(tosplit[counter] != ';'){
        if(tosplit[counter] == '('){
            counter++;
            counter1=0;
            while(tosplit[counter] != ')'){
                temp[counter1] = tosplit[counter];
                counter++;
                counter1++;
            }
            strcat(splited, temp);
            strcat(splited, " ");
            SetZero(temp, outputStringSize);
        }
        counter++;
    }

    SetZero(tosplit, outputStringSize);
    strcpy(tosplit, splited);

    return tosplit;
}

void InputOutputSpliter(struct gate *ptr, char tosplit[]){
    int counter1=0;
    int counter=0, inp;
    char Q = 'Q';
    char Z = 'Z';
    char temp[inputStringSize];

    SetZero(temp,inputStringSize);

    while(1){
        if(tosplit[counter] == Q){
            break;
        }else if(tosplit[counter] == Z){
            break;
        }
        counter++;
    }

    inp=counter;
    counter1 = 0;

    while(tosplit[counter-1] != ';'){
        temp[counter1] = tosplit[counter-1];
        counter++;
        counter1++;
    }

    //Drop space);
    temp[counter1-3] = '\0';

    for(counter=0; counter<counter1; counter++){
        temp[counter] = temp[counter+1];
    }
    char output[outputStringSize];
    SetZero(output, outputStringSize);
    strcpy(output, temp);
    strcat(output, ";");
    strcpy(ptr->gate_output, StringSplit(output));

    SetZero(temp, outputStringSize);
    for(counter=0; counter<inp; counter++){
        temp[counter] = tosplit[counter];
    }
    //Drop space
    for(counter=0; counter<inp; counter++){
        temp[counter] = temp[counter+1];
    }
    temp[counter] = '\0';
    strcat(temp,";");

    strcpy(ptr->gate_inputs, StringSplit(temp));

    //Setting type
    char type[30];
    strcpy(type, ptr->gate_name);
    //You have 2 spaces before a letter
    if(type[0] == 'D' || type[1] == 'D'){
        strcpy(ptr->gate_type ,"D_Flif_Flop");
    }
    else if(type[0] == 'O'){
        strcpy(ptr->gate_type, "OR");
    }
    else if(type[0] == 'I'){
        strcpy(ptr->gate_type, "Inverter");
    }
    else if(type[0] == 'A'){
        strcpy(ptr->gate_type, "AND");
    }
    else if(type[0] == 'N'){
        if(type[1] == 'A'){
            strcpy(ptr->gate_type, "NAND");
        }
        else if(type[1] == 'O'){
            strcpy(ptr->gate_type, "NOR");
        }
    }
}

void BasicDataSpliter(struct gate *ptr, char tosplit[]){
    char temp[inputStringSize];
    int counter=0, counter1=0;

    SetZero(temp,inputStringSize);

    //First get the name
    while(tosplit[counter] != '('){
        temp[counter] = tosplit[counter];
        counter++;
    }
    //Delete last space from temp_gate name
    temp[counter] = '\0';
    while(counter1 < counter){
        temp[counter1] = temp[counter1+2];
        counter1++;
    }

    //Set it to the temp_gate_name
    strcpy(ptr->gate_name, temp);

    //Clear temp string
    SetZero(temp, inputStringSize);

    //Got the Substring
    if(tosplit[counter] == ' '){
        counter++;
    }

    counter1 = 0;
    while(tosplit[counter] != ';'){
        temp[counter1] = tosplit[counter];
        counter++;
        counter1++;
    }
    //Drop space);
    temp[counter1+2] = ';';

    //Split input-output and type
    InputOutputSpliter(ptr, temp);
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


int FindInputOutputs(char stringforsearch[]){
    int counter=0, space=0;

    while(counter<stringforsearch[counter]){
        if(stringforsearch[counter] == ' '){
            space++;
        }
        counter++;
    }

    return space;
}

struct gate *CreateInitialList(char *input, char *output, char *wires){

    //File pointer
    FILE *fp;
    char str[inputStringSize]; //String to read from file

    //String for the circuit input - output - wire
    char inputs[inputStringSize];
    char outputs[outputStringSize];
    char wire[wireStringSize];
    char readtemp[wireStringSize];
    char read[wireStringSize];

    //String for universal use for small calculations. Does not take date from wires-gates etc
    char temp[50]; 

    //Which line from file i read starting from input(0)
    int counter, lineread = 0;
    int flag=0;

    //Head of temp linked list
    struct gate *head = NULL;
    struct gate *tnode;
    struct gate *curr;

    //Initialize everything to \0
    SetZero(str, inputStringSize);
    SetZero(inputs, inputStringSize);
    SetZero(outputs, outputStringSize);
    SetZero(wire, wireStringSize);
    SetZero(readtemp, wireStringSize);
    SetZero(read, wireStringSize);
    SetZero(temp,50);

    // opening file for reading
    fp = fopen(fileOpen , "r");
    if(fp == NULL) {
       perror("Error opening file");

      return NULL;
    }

   while(fgets(str, inputStringSize, fp)!= NULL){

       strcpy(readtemp,str);

       if(readtemp[0] == '/'){
            SetZero(str, inputStringSize);
            SetZero(readtemp, wireStringSize);
            continue;
        }

        //Start code here for structure!!!!!

        //Throw first line saying module
        //Check first only for m only for speed
        if(readtemp[0] == 'm'){
            SetZero(temp,50);
            for(counter=0; counter<6; counter++){
                temp[counter] = readtemp[counter];
            }
            if(!strcmp(temp,"module")){
                lineread = 0;
                continue;
            }
            else{
                return NULL;
            }
        }

        //Throw last line saying endmodule
        if(readtemp[0] == 'e'){
            //SetZero(readtemp,10);
            for(counter=0; counter<9; counter++){
                temp[counter] = readtemp[counter];
            }
            if(!strcmp(temp,"endmodule")){
                break;
            }
            else{
                printf("Temp is %s\n", temp);
                return NULL;
            }
        }

        //do it until \n so we can track ,
        counter=0;
        while(readtemp[counter] != '\n'){
            if(readtemp[counter] == ';'){break;}
            counter++;
        }
        
        if(readtemp[counter-1] == ','){
            strcat(read, readtemp);
            flag = 1;
            continue;
        }
        if(flag == 1){
            strcat(read, readtemp);
        }

        if(flag == 0){
            strcpy(read, readtemp);
        }


        //Read inputs
        if(lineread == 0){
            strcpy(inputs, read);
            SetZero(str, inputStringSize);
            SetZero(readtemp, wireStringSize);
            SetZero(read, wireStringSize);
            lineread++;
            continue;
        }
        //Read output
        if(lineread == 1){
            strcpy(outputs, read);
            SetZero(str, inputStringSize);
            SetZero(readtemp, wireStringSize);
            SetZero(read, wireStringSize);
            lineread++;
            continue;
        }
        //Read wire
        if(lineread == 2){
            strcpy(wire, read);
            SetZero(str, inputStringSize);
            SetZero(readtemp, wireStringSize);
            SetZero(read, wireStringSize);
            lineread++;
            continue;
        }

        //Drop empty lines no line will have length less than 2
        if(strlen(read) < 3){
            lineread++;
            continue;
        }

        //lineread is 4 means i just started reading the temp_gates
        if(lineread == 4){
            head = (struct gate *)malloc(sizeof(struct gate));
            if(head == NULL){
                printf("Error alocating memmory for the temp_gate!!!\nExiting...\n");
                return NULL;
            }
            BasicDataSpliter(head, read);
            head->next = NULL;
            head->layer = -1;
            lineread++;
            SetZero(str, inputStringSize);
            SetZero(readtemp, wireStringSize);
            SetZero(read, wireStringSize);
            tnode = head;

            continue;
        }
        curr = (struct gate *)malloc(sizeof(struct gate));
        if(curr == NULL){
            printf("Error alocating memmory for the temp_gate!!!\nExiting...\n");
            return NULL;
        }

        //Fix first list
        tnode->next = curr;
        curr->layer = -1;
        curr->next = NULL;
        BasicDataSpliter(curr, read);
        lineread++;
        tnode = curr;

        flag = 0;

        SetZero(str, inputStringSize);
        SetZero(readtemp, wireStringSize);
        SetZero(read, wireStringSize);

    }
    strcpy(input,inputs);
    strcpy(output, outputs);
    strcpy(wires, wire);

    //Close pointer to file
    fclose(fp);

    return head;
}

struct gate *Find(struct gate *ptr, char stringforsearch[], struct gate *value[]){
    struct gate *curr;
    int counter, i=0;

    char *ret;
    for(counter=0; counter<40; counter++){
        value[counter] = NULL;
    }

    curr=ptr;

    while(curr != NULL){
        ret = strstr(curr->gate_inputs, stringforsearch);
        if(ret != NULL){
            value[i] = curr;
            i++;
        }
        curr = curr->next;
    }

    return NULL;
}

void counts(struct gate *head){
    struct gate *curr;

    curr = head;

    while(curr != NULL){
        curr->input = FindInputOutputs(curr->gate_inputs);
        curr->output = FindInputOutputs(curr->gate_output);
        curr = curr->next;
    }
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

void StringForWireList(char input[], char output[], char wire[], char str[]){
    int counter = 0;
    char inputtemp[inputStringSize]={'\0'};
    char outputtemp[outputStringSize] ={'\0'};
    char wiretemp[wireStringSize]= {'\0'};

    strcpy(inputtemp, input);
    strcpy(outputtemp, output);
    strcpy(wiretemp, wire);

    for(counter=0; counter<strlen(inputtemp); counter++){
        if(inputtemp[counter] == ';'){
            inputtemp[counter] = '\0';
        }

        if(counter + 8 > inputStringSize ){
            inputtemp[counter] = '\0';
        }
        else{
            inputtemp[counter] = inputtemp[counter + 8];
        }
    }

    for(counter=0; counter<strlen(outputtemp); counter++){
        if(outputtemp[counter] == ';'){
            outputtemp[counter] = '\0';
        }
        if(counter + 12 > outputStringSize ){
            outputtemp[counter] = '\0';
        }
        else{
            outputtemp[counter] = outputtemp[counter + 8];
        }
    }

    for(counter=0; counter<strlen(wiretemp); counter++){
        if(wiretemp[counter] == ';'){
            wiretemp[counter] = '\0';
        }
        if(counter + 12 > wireStringSize ){
            wiretemp[counter] = '\0';
        }
        else{
            wiretemp[counter] = wiretemp[counter + 8];
        }
    }

    strcat(str, inputtemp);
    strcat(str, outputtemp);
    strcat(str, wiretemp);

    counter=0;

    while(counter < strlen(str)){
        if(str[counter] == '\n'){
            str[counter] = ' ';
        }
        if(str[counter] == ','){
            str[counter] = ' ';
        }
        if(str[counter] == ';'){
            str[counter] = ' ';
        }
        counter++;
    }

    int space=0;

    counter=0;
    int counter1;
    while(counter < strlen(str)){
        if(str[counter] == ' '){
            space++;
        }
        if(space > 1){
            for(counter1 = counter; counter1 < strlen(str); counter1++){
                str[counter1] = str[counter1+1];
            }
            counter = counter-2;
            space=0;
        }
        counter++;
    }

}

struct wire *InitializeWireList(char input[], char output[], char wire[]){
    struct wire *head, *temp, *curr;

    int counter, i, counter1=0;
    char temporary[100];
    char str[inputStringSize + outputStringSize +wireStringSize] = {'\0'};

    StringForWireList(input, output, wire, str);

    head = (struct wire *)malloc(sizeof(struct wire));
    if(head == NULL){
        printf("Error allocating memmory for wirelist\n");
        return NULL;
    }

    counter=0;
    SetZero(temporary, 100);

    while(1){
        if(str[counter] == ' '){
            break;
        }
        temporary[counter] = str[counter];
        counter++;
    }
    strcpy(head->node_name, temporary);
    if(strstr(input, temporary) != NULL){
        head->layer = 0;
    }
    else{
        head->layer = -1;
    }

    curr = head;

    counter++;
    i=counter;
    while( i < strlen(str)){
        i++;
        SetZero(temporary, 100);

        temp = (struct wire *)malloc(sizeof(struct wire));
        if(temp == NULL){
            printf("Error at memory!\n");
            return NULL;
        }

        counter1 = 0;
        while(1){
            if(str[counter] == ' '){
                counter++;
                break;
            }
            temporary[counter1] = str[counter];
            counter++;
            counter1++;
            i++;
        }
        if(strstr(input, temporary) != NULL){
            temp->layer = 0;
        }
        else{
            temp->layer = -1;
        }
        strcpy(temp->node_name, temporary);
        temp->value = 0;
        curr->next = temp;
        curr = temp;
    }

    return head;

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

void connect(struct gate *headgate, struct wire *headwire){
    struct gate *currgate;
    struct wire *currwire;
    int counter, i=0, j=0;
    char temp[100] = {'\0'};

    //Loop to go throw gate list and connect it times as inputs to the correct wire list
    for(currgate = headgate; currgate != NULL; currgate = currgate->next){

        //Adding delimiter for spliting the strings in the string
        currgate->gate_inputs[strlen(currgate->gate_inputs)-1] = '/';
        for(counter=0; counter<currgate->input; counter++){
            while(currgate->gate_inputs[i] != ' '){
                if(currgate->gate_inputs[i] == '/'){
                    break;
                }
                temp[j] = currgate->gate_inputs[i];
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
            currgate->inputs[counter] = currwire;
        }
        i=0;

        //Removing delimiter now
        currgate->gate_inputs[strlen(currgate->gate_inputs)-1] = '\0';
    }

    //Set zero to temp string
    SetZero(temp,100);

    //Loop to go throw gate list and connect it times as outputs to the correct wire list
    for(currgate = headgate; currgate != NULL; currgate = currgate->next){
        //Adding delimiter for spliting the strings in the string
        currgate->gate_output[strlen(currgate->gate_output)-1] = '/';
        for(counter=0; counter<currgate->output; counter++){
            while(currgate->gate_output[i] != ' '){
                if(currgate->gate_output[i] == '/'){
                    break;
                }
                temp[j] = currgate->gate_output[i];
                i++;
                j++;
            }

            currwire = FindCheck(headwire, temp);
            if(currwire == NULL){
                printf("Something went wrong1!!\n");
                exit(1);
            }
            SetZero(temp,100);
            j=0;
            i++;

            //Build pointers from here on
            currgate->outputs[counter] = currwire;
        }
        i=0;
        //Remove delimiter now
        currgate->gate_output[strlen(currgate->gate_output)-1] = '\0';
    }

    //Setting level to the flip flops after connecting gates with wires
    levelSetToDFlipFlops(headgate);

}
/******************************************************************************************************************************************************/

//Function used to virtualy run the circuit
void circuitRun(struct wire *headWire, struct gate *headGate){
    // struct wire *wireIterator, *wireTemp;
    // struct gate *gateIterator, *gateTemp;

    // wireIterator = headWire;
    // gateIterator = headGate;

    // while(gateIterator != NULL){

    // }
}

// struct gate *nodeFound(struct gate *head, )

void listDel(struct gate *head, struct gate *todel){
    struct gate *iterator, *temp;

    iterator = head;

    while(iterator != todel){
        temp = iterator;
        iterator = iterator->next;
    }

    temp->next = iterator->next;
    free(iterator); //Should i free it or at the end of the list???????
}

struct gate *createCircuitInOrder(struct gate *headGate, struct wire *headWire){
//     struct gate *tempHead; //Temporary listhead to fix the circuit in layers
//     struct gate *temp, *tempgate;
//     struct gate *newHead; //new gate list gate
//     struct gate *newTemp;

//     struct wire *tempHeadWire;
//     struct wire *tempwire;

//     tempgate = headGate;
//     tempHeadWire = headWire;

// //     newHead = (struct gate*)malloc(sizeof(struct gate));
// //     if(newHead == NULL){
// //         printf("Error creating level order list\n");
// //         exit -1;
// //     }
// //
// //     newHead->next = NULL;
// //     newHead->layer = 0;

//     //Set 0 layer for every flip flop
//     while(tempgate != NULL){
//         if(strcmp(tempgate->gate_type, "D_Flif_Flop")==0){
//             tempgate->outputs[0]->layer = 0;
//             if(tempgate->outputs[1] != NULL){
//                 tempgate->outputs[1]->layer = 0;
//             }
//         }
//         tempgate = tempgate->next;
//      }

     return NULL;

}

void levelSetToDFlipFlops(struct gate *headGate){

    struct gate *curr = headGate;

    while(curr != NULL){
        if(strcmp(curr->gate_type, "D_Flif_Flop") == 0){ //Found Flip FLop
            curr->layer = 0;
        }
        curr = curr->next;
    }
}


//Set levels to the wires
 void levelSet(struct wire *headwire, struct gate *headGate){

//     struct wire *wireTemp = headwire;
//     struct gate *gateTemp;

 }

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

// This function runs the circuit
void run(struct gate *gateHead, struct wire *wireHead){

    struct gate *gateIter;

    gateIter = gateHead;

    //Loop to iterate the list3 and create new values
    while(gateIter != NULL){

        //Based on gate type, perform actions numbers
        //place it on the wire

        //OR GATE HERE
        if(strcmp(gateIter->gate_type,"OR") == 0){
            gateIter->outputs[0]->value = valueGateOR(gateIter);
        }
        else if(strcmp(gateIter->gate_type,"NOR") == 0){
            gateIter->outputs[0]->value = valueGateNOR(gateIter);
        }
        else if(strcmp(gateIter->gate_type,"AND") == 0){
            gateIter->outputs[0]->value = valueGateAND(gateIter);
        }
        else if(strcmp(gateIter->gate_type,"NAND") == 0){
            gateIter->outputs[0]->value = valueGateNAND(gateIter);
        }
        else if(strcmp(gateIter->gate_type,"Inverter") == 0){
            gateIter->outputs[0]->value = valueGateInverter(gateIter);
        }
        else if(strcmp(gateIter->gate_type,"D_Flif_Flop") == 0){
            gateIter->outputs[0]->value = valueGateDFlipFlop(gateIter);
        }
        
        gateIter = gateIter->next;
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
        printf("Gate %s with input %s and value %d\n", nodeItr->gate_name, nodeItr->inputs[inpunt_count]->node_name, nodeItr->inputs[inpunt_count]->value);
        // In OR gate if 1 is found as one input output is 1
        if(nodeItr->inputs[inpunt_count]->value == 1){
            printf("Found 1 exiting...\n");
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
    printf("Here is a flip flop. Value is %d\n", node->inputs[1]->value);
    node->value = node->inputs[1]->value;
    return node->inputs[1]->value;
}

//Function that prints the gates values at each run to the file
void printGateToFile(struct gate *head){

    struct gate *curr = head;
    FILE *pFile;

    pFile=fopen("data/inputs_outputs_logs.txt", "a");
    if(pFile==NULL) {
        perror("Error opening file.");
        return;
    }

    while(curr!=NULL){
       fprintf(pFile, "Gate with name: %s has value of: %d\n", curr->gate_name, curr->value);
       curr = curr->next;
    }
    fclose(pFile);
}