#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "functions.h"

#define SIZE 512
#define MAX_FANOUT 50
#define fileOpen "s27_mapped.v"


//Creating all inputs that i will ask for a run
void inputFix(char input[]){
    int pos, counter;
    char temp[500];

    SetZero(temp,500);

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

    SetZero(input, 500);
    strcpy(input, temp);

    return;
}
/************************************************ DO NOT TOUCH WORKING PERFECTLY *********************************************************************/
//Just printing all gates
void printGate(struct gate *head){

    struct gate *curr = head;

    while(curr!=NULL){
       printf("%s\n%s\n%s\n%s\n", curr->gate_name, curr->gate_inputs, curr->gate_output, curr->gate_type);
       printf("\n");
       curr = curr->next;
    }

}

//Just printing all wires
void printWire(struct wire *head){

    struct wire *curr = head;

    while(curr!=NULL){
       printf("Node wire with name: %s and Value: %d\n", curr->node_name, curr->value);
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
    char temp[500];
    char splited[500];
    int counter = 0;
    int counter1 = 0;

    SetZero(temp, 500);
    SetZero(splited, 500);

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
            SetZero(temp, 500);
        }
        counter++;
    }

    SetZero(tosplit, 500);
    strcpy(tosplit, splited);

    return tosplit;
}

void InputOutputSpliter(struct gate *ptr, char tosplit[]){
    int counter1=0;
    int counter=0, inp;
    char Q = 'Q';
    char Z = 'Z';
    char temp[500];

    SetZero(temp,500);

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
    char output[200];
    SetZero(output, 200);
    strcpy(output, temp);
    strcat(output, ";");
    strcpy(ptr->gate_output, StringSplit(output));

    SetZero(temp, 100);
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
    if(type[0] == 'D'){
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
    char temp[100];
    int counter=0, counter1=0;

    SetZero(temp,100);

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
    SetZero(temp, 100);

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
    char str[SIZE]; //String to read from file

    //String for the circuit input - output - wire
    char inputs[200];
    char outputs[50];
    char wire[3000];
    char temp[50];
    char readtemp[3000];
    char read[3000];

    //Which line from file i read starting from input(0)
    int counter, lineread = 0;
    int flag=0;

    //Head of temp linked list
    struct gate *head = NULL;
    struct gate *tnode;
    struct gate *curr;

    //Initialize everything to \0
    SetZero(str, SIZE);
    SetZero(inputs, 200);
    SetZero(outputs, 50);
    SetZero(wire, 200);
    SetZero(temp,50);
    SetZero(readtemp, 600);
    SetZero(read, 3000);

    // opening file for reading
    fp = fopen(fileOpen , "r");
    if(fp == NULL) {
       perror("Error opening file");

      return NULL;
    }

   while(fgets(str, SIZE, fp)!= NULL){

       strcpy(readtemp,str);

       if(readtemp[0] == '/'){
            SetZero(str, SIZE);
            SetZero(readtemp, 3000);
            continue;
        }
        //Start code here for structure!!!!!

        //Throw first line saying module
        //Check first only for m only for speed
        if(readtemp[0] == 'm'){
            SetZero(temp,10);
            for(counter=0; counter<6; counter++){
                temp[counter] = readtemp[counter];
            }
            if(!strcmp(temp,"module")){
                lineread = 0;
                continue;
            }
        }

        //Throw last line saying endmodule
        if(readtemp[0] == 'e'){
            SetZero(readtemp,10);
            for(counter=0; counter<9; counter++){
                temp[counter] = readtemp[counter];
            }
            if(!strcmp(temp,"endmodule")){
                break;
            }
        }

        //do it until \n so we can track ,
        counter=0;
        while(readtemp[counter] != '\n'){
            if(readtemp[counter] == ';'){break;}
            counter++;
        }
        //SetZero(readtemp, 300);
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
            SetZero(str, SIZE);
            SetZero(readtemp, 600);
            SetZero(read, 600);
            lineread++;
            continue;
        }
        //Read output
        if(lineread == 1){
            strcpy(outputs, read);
            SetZero(str, SIZE);
            SetZero(readtemp, 600);
            SetZero(read, 600);
            lineread++;
            continue;
        }
        //Read wire
        if(lineread == 2){
            strcpy(wire, read);
            SetZero(str, SIZE);
            SetZero(readtemp, 600);
            SetZero(read, 600);
            lineread++;
            continue;
        }

        //Drop empty lines no line will have length less than 2
        if(strlen(read) < 2){
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
            lineread++;
            SetZero(str, SIZE);
            SetZero(readtemp, 600);
            SetZero(read, 600);
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
        curr->next = NULL;
        BasicDataSpliter(curr, read);
        lineread++;
        tnode = curr;

        flag = 0;

        SetZero(str, SIZE);
        SetZero(readtemp, 600);
        SetZero(read, 600);

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
            printf("Found %s\n", ret);
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
    char inputtemp[500]={'\0'};
    char outputtemp[500] ={'\0'};
    char wiretemp[3000]= {'\0'};

    strcpy(inputtemp, input);
    strcpy(outputtemp, output);
    strcpy(wiretemp, wire);

    for(counter=0; counter<strlen(inputtemp); counter++){
        if(inputtemp[counter] == ';'){
            inputtemp[counter] = '\0';
        }

        if(counter + 8 > 500 ){
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
        if(counter + 12 > 500 ){
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
        if(counter + 12 > 3000 ){
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
    char temporary[20];
    char str[5000] = {'\0'};

    StringForWireList(input, output, wire, str);

    head = (struct wire *)malloc(sizeof(struct wire));
    if(head == NULL){
        printf("Error allocating memmory for wirelist\n");
        return NULL;
    }

    counter=0;
    SetZero(temporary, 20);

    while(1){
        if(str[counter] == ' '){
            break;
        }
        temporary[counter] = str[counter];
        counter++;
    }
    strcpy(head->node_name, temporary);

    curr = head;

    counter++;
    i=counter;
    while( i < strlen(str)){
        i++;
        SetZero(temporary, 20);

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
    char temp[30] = {'\0'};

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
            SetZero(temp,30);
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
    SetZero(temp,30);

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
                printf("Something went wrong!!\n");
                exit(1);
            }
            SetZero(temp,30);
            j=0;
            i++;

            //Build pointers from here on
            currgate->outputs[counter] = currwire;
        }
        i=0;
        //Remove delimiter now
        currgate->gate_output[strlen(currgate->gate_output)-1] = '\0';
    }


}
/******************************************************************************************************************************************************/
