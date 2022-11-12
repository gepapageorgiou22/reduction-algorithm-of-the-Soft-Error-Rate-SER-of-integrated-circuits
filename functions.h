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
struct gate *createCircuitInOrder(struct gate *headGate, struct wire *headWire);

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

// This Function returns the type of the given gate 
char *getType(struct gate *node);

// This function runs the circuit.
void run(struct gate *gateHead, struct wire *wireHead);

//Do actions for OR gate
int valueGateOR(struct gate *node);
//Do actions for NOR gate
int valueGateNOR(struct gate *node);
//Do actions for AND gate
int valueGateAND(struct gate *node);
//Do actions for NAND gate
int valueGateNAND(struct gate *node);
//Do actions for the inverter;
int valueGateInverter(struct gate *node);
//Do actions for the DFlip Flop;
int valueGateDFlipFlop(struct gate *node);

//Function to write results to a file
void printGateToFile(struct gate *head);

//New function to read from file
struct gate *create(struct wire *headwire);
void initGateInstance(struct gate *ptr, char *data);
void setName(struct gate *ptr, char *data);
void setType(struct gate *ptr, char *data);
void setInutsOutputs(struct gate *ptr, char *data);
void initWireInstance(struct wire *ptr, char *data);
void createWireList(char inputString[], char outputString[], char wireString[], struct wire *head);
void initLevelOfDFlipFlops(struct gate *ptr);
int checkOccurrences(char *s, char c);
void setInputsFlipFlop(struct gate *ptr, char *data);
void setInputsInverter(struct gate *ptr, char *data);
void setInputsAndNorOr(struct gate *ptr, char *data);
void levelGatesInitialList(struct gate *listhead);
int allGatesHaveLevels(struct gate *listhead);
void fixFlipFlopWires(struct gate *listhead);
void gateFix(struct gate *gateToLevel);
void addNode(struct gate *node, struct gate **head, struct gate *newList);
void createNewLeveledList(struct gate *listhead, struct gate *gateList3Head);
void copyNode(struct gate *newList, struct gate *node);
int getMaxLevel(struct gate *head);
struct mapping * leveled(struct gate *head);
void clearPtrs(struct gate *gatesLevel[]);