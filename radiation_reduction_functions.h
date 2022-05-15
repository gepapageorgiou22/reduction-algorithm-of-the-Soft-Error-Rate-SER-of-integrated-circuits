/**************************************************************
*                                                             *
*  In this file are the functions to reduce soft error rate   *
*                                                             *
**************************************************************/
#include "functions.h"

//Test function does nothing
void testNewHeaderFile(struct gate *head);

//Fixes names of the gates
void fixTheNames(struct gate *head);

//This function will change the circuit topology
void circuitChange(struct gate *head, struct wire *wireHead);
void findAndChangeGate(struct gate *head, struct wire *wireHead, char name[]);
void changeANDGateWithTwoNANDS(struct gate * gate, struct gate *head);
