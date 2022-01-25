struct gate{
    char gate_name[50];
    char gate_inputs[200];
    char gate_output[100];
    char gate_type[50];
    int input, output;
    int layer;
    struct gate *next;
    //struct wire *next[10] = NULL;
    //I will point to node n
    struct wire *inputs[50]; //Remember to Initialize to NULL
    struct wire *outputs[2];
};

//For wires
struct wire{
    char node_name[50];
    unsigned short int value;
    int layer;
    struct wire *next;
};
