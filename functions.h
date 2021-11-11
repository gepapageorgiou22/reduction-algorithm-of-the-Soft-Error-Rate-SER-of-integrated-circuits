#include "structs.h"

//Functions
//Function to set \0 string
void SetZero(char str[], int size);
void BasicDataSpliter(struct gate *ptr, char tosplit[]);
void InputOutputSpliter(struct gate *ptr, char tosplit[]);
char * StringSplit(char tosplit[]);
int FindInputOutputs(char stringforsearch[]);
struct gate *CreateInitialList(char *input, char *output, char *wires);
struct gate *Find(struct gate *ptr, char stringforsearch[], struct gate *value[]);
void counts(struct gate *head);
struct wire *FindCheck(struct wire *ptr, char stringforsearch[]);
void StringForWireList(char input[], char output[], char wire[], char str[]);
struct wire *InitializeWireList(char input[], char output[], char wire[]);
void inputFix(char input[]);

//Connect struct gate with wires
void connect(struct gate *headgate, struct wire *headwire);

//Set array pointers to null
void null(struct gate *head);


//Print Functions
void printGate(struct gate *head);
void printWire(struct wire *head);

//Free pointers
void FreeMem(struct gate **ptr);
void FreeMemWire(struct wire **ptr);

//Function to do iterations to calculate new values of the circuit
void circuitRun(struct wire *headwire, struct gate *headGate);
