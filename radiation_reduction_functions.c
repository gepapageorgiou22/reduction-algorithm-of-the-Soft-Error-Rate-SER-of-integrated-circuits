#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "radiation_reduction_functions.h"

#define sizeToChange 20

void testNewHeaderFile(struct gate *head){

    printf("############## Test Mode ##############\n\n");
    printGate(head);

}


//This function will change the circuit topology to improve the masking.
void circuitChange(struct gate *head, struct wire *wireHead){
    char gateToChange[sizeToChange];
    
    
    SetZero(gateToChange, sizeToChange);
    //Take the names of the gates you want to change.
    while(1){
        printf("Enter the name of the gate you want to change: ");
        scanf(" %s", gateToChange);

        if(strcmp(gateToChange, "quit") == 0){
            printf("Exiting upon request...\n");
            return;
        }

        if(gateExists(head, gateToChange) == -1){
            printf("Pfff the gate you entered does not exist.\nExiting...\n");
            return;
        }
        findAndChangeGate(head, wireHead, gateToChange);

    }

    /*   DONE   */     //ALL the bellow must be done in the while loop... SOS
    /*   DONE   */     //Find the gates
    /*   DONE   */     //From the type perfomr the neccessary changes to the gate AND -> NAND + NAND add the new wires to the list
    /*   DONE   */     //At OR gate -> OR + Inverter || NOR + NOR
    
    /*    The bellow steps can be done in the main */
    //Re run the circuit.
    //Export the new verilog file
}

void findAndChangeGate(struct gate *head, struct wire *wireHead, char name[]){
    struct gate * itr;

    itr = head;

    while(itr!=NULL){
        if(strcmp(itr->gate_name,name) == 0){
            if(strcmp(itr->gate_type, "AND") == 0){
                changeANDGateWithTwoNANDS(itr, head);
            }
            if(strcmp(itr->gate_type, "OR") == 0){
                changeORGateWithTwoNORS(itr, head);
            }
            if(strcmp(itr->gate_type, "NAND") == 0){
                changeNandWithAndInverter(itr, head);
            }
            
            break;
        }
        
        itr = itr -> next;
    }

}

void changeANDGateWithTwoNANDS(struct gate * gate, struct gate *head){
struct gate *temp, *itr;
struct wire *newWire, *wireItr;
int counter;

temp = (struct gate *)malloc(sizeof(struct gate));
if(temp == NULL){
    printf("Error allocating memmory for the better circuit!\n");
    return;
}

//Build the pointers on the first list
temp->next = gate->next;
gate->next = temp;

//Copy information for the gate
strcpy(gate->gate_type,"NAND");
strcpy(temp->gate_type,gate->gate_type);
temp->layer = gate->layer;
temp->inputs[0] = gate->outputs[0];
temp->inputs[1] = gate->outputs[0];
strcpy(temp->gate_name, gate->gate_name);
strcat(temp->gate_name, "_New");


//now create new wire that will be the output of the new gate
newWire = (struct wire *)malloc(sizeof(struct wire));
if(newWire == NULL){
    printf("Error allocating memmory for the better circuit!\n");
    return;
}

newWire->next = gate->outputs[0]->next;
gate->outputs[0]->next = newWire;
strcpy(newWire->node_name, gate->outputs[0]->node_name);
strcat(newWire->node_name, "_New");
newWire->layer = gate->outputs[0]->layer;

//find all the gates that have the old wire as input and change it
itr = head;
while(itr!=NULL){
    counter = 0;
    wireItr = temp->inputs[counter];
    while(wireItr != NULL){
        if(wireItr == temp->inputs[0]){
            itr->inputs[counter] = newWire;    
        }
        counter++;
        wireItr = itr->inputs[counter];
    }

    itr = itr->next;
}

//Attach the wire to the new NAND gate
temp->outputs[0] = newWire;

}

void changeORGateWithTwoNORS(struct gate * gate, struct gate *head){

struct gate *temp, *itr;
struct wire *newWire, *wireItr;
int counter;

temp = (struct gate *)malloc(sizeof(struct gate));
if(temp == NULL){
    printf("Error allocating memmory for the better circuit!\n");
    return;
}

//Build the pointers on the first list
temp->next = gate->next;
gate->next = temp;

//Copy information for the gate
strcpy(gate->gate_type,"NOR");
strcpy(temp->gate_type,gate->gate_type);
temp->layer = gate->layer;
temp->inputs[0] = gate->outputs[0];
temp->inputs[1] = gate->outputs[0];
strcpy(temp->gate_name, gate->gate_name);
strcat(temp->gate_name, "_New");


//now create new wire that will be the output of the new gate
newWire = (struct wire *)malloc(sizeof(struct wire));
if(newWire == NULL){
    printf("Error allocating memmory for the better circuit!\n");
    return;
}

newWire->next = gate->outputs[0]->next;
gate->outputs[0]->next = newWire;
strcpy(newWire->node_name, gate->outputs[0]->node_name);
strcat(newWire->node_name, "_New");
newWire->layer = gate->outputs[0]->layer;

//find all the gates that have the old wire as input and change it
itr = head;
while(itr!=NULL){
    counter = 0;
    wireItr = temp->inputs[counter];
    while(wireItr != NULL){
        if(wireItr == temp->inputs[0]){
            itr->inputs[counter] = newWire;    
        }
        counter++;
        wireItr = itr->inputs[counter];
    }

    itr = itr->next;
}

//Attach the wire to the new NOR gate
temp->outputs[0] = newWire;
}

void changeNandWithAndInverter(struct gate * gate, struct gate *head){
    struct gate *temp, *itr;
    struct wire *newWire, *wireItr;
    int counter;

    temp = (struct gate *)malloc(sizeof(struct gate));
    if(temp == NULL){
        printf("Error allocating memmory for the better circuit!\n");
        return;
    }

    //Build the pointers on the first list
    temp->next = gate->next;
    gate->next = temp;

    //Copy information for the gate
    strcpy(gate->gate_type,"AND");
    strcpy(temp->gate_type, "Inverter");
    temp->layer = gate->layer;
    temp->inputs[0] = gate->outputs[0];
    strcpy(temp->gate_name, gate->gate_name);
    strcat(temp->gate_name, "_New");
    
    //now create new wire that will be the output of the new gate   
    newWire = (struct wire *)malloc(sizeof(struct wire));
    if(newWire == NULL){
        printf("Error allocating memmory for the better circuit!\n");
        return;
    }

    newWire->next = gate->outputs[0]->next;
    gate->outputs[0]->next = newWire;
    strcpy(newWire->node_name, gate->outputs[0]->node_name);
    strcat(newWire->node_name, "_New");
    newWire->layer = gate->outputs[0]->layer;

    //find all the gates that have the old wire as input and change it
    itr = head;
    while(itr!=NULL){
        counter = 0;
        wireItr = temp->inputs[counter];
        while(wireItr != NULL){
            if(wireItr == temp->inputs[0]){
                itr->inputs[counter] = newWire;    
            }
            counter++;
            wireItr = itr->inputs[counter];
        }

        itr = itr->next;
    }

//Attach the wire to the new NOR gate
temp->outputs[0] = newWire;

}

void fixTheNames(struct gate *head){
    struct gate *itr;
    char updatedName[50];
    int counter, counterNewName;

    itr = head;
    
    while(itr != NULL){
        SetZero(updatedName, 50);
        counter =0;
        counterNewName = 0;
        while(itr->gate_name[counter] != ' '){
            counter++;      
        }
        
        counter++;
        while(itr->gate_name[counter] != ' ' && counter < strlen(itr->gate_name)){
            updatedName[counterNewName] = itr->gate_name[counter];
            counter++;
            counterNewName++;
        }
//         printf("New name: %s\n", updatedName);
        SetZero(itr->gate_name, 50);
        strcpy(itr->gate_name, updatedName);
//         printf("Name passed: %s\n", itr->gate_name);
        itr = itr->next;
    }
    printf("Exiting\n");
}
