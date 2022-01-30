#include "structs.h"

//Functions

//Function to set \0 string
void SetZero(char str[], int size);

/********** Functions and help function to split incoming Data ************/
void BasicDataSpliter(struct gate *ptr, char tosplit[]);
void InputOutputSpliter(struct gate *ptr, char tosplit[]);
char * StringSplit(char tosplit[]);
int FindInputOutputs(char stringforsearch[]);
/********** End of functions spliting Data **************/

//Function to Create List1(Initial list without levels)
struct gate *CreateInitialList(char *input, char *output, char *wires);

//Function to find a gate with a name stringforsearch
struct gate *Find(struct gate *ptr, char stringforsearch[], struct gate *value[]);
void counts(struct gate *head);

//Function to find a specific wire and returns the pointer to the found wire. If not found Null is returned
struct wire *FindCheck(struct wire *ptr, char stringforsearch[]);
void StringForWireList(char input[], char output[], char wire[], char str[]);

//Function that initializes the wire list (list2)
struct wire *InitializeWireList(char input[], char output[], char wire[]);
void inputFix(char input[]);

//Connect struct gate with wires
void connect(struct gate *headgate, struct wire *headwire);

//Function to set 0 level at the flip flops
void levelSetToDFlipFlops(struct gate *headGate);

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

//Function that fixes new list Level 0 only
struct gate *rebuildLevelOrderLayer0(struct gate **head); //Fixes only flip flops due to other gates have no level
void restGatesLeveled(struct gate *head, struct wire *wireHead, int level);

//returns the level of a wire
int wireLevelReturn(struct wire *headWire, char *name);
//This Function gives levels at the wires after the gates that got level
void levelingWireAfterGate(struct gate *headGate, struct wire *wireHead, int level);

//Fixing gates levels give the level
void fixGateLevel(struct gate **head, struct gate *list3, int level);

//This function will fix the IC in order.
void buildCircuitLeveled(struct gate **head, struct gate *list3);

//Function to write to a file inputs and output
void dataToFile(char *dataStr);