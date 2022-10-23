#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define maxWordSize 3000
#define buffToRead 3000
#define lineCount 500
#define wordLength 3000

void parse_line(char dataBuffer[lineCount][wordLength], char *line){

int i, j=0;
char tempString[maxWordSize];

for (i = 0; i < maxWordSize; i++){
    tempString[i] = '\0';
}


   for(i=0; i<=(strlen(line)); i++){
       if(line[i]==' ' || line[i]=='\0' || line[i]=='{' || line[i]==':' ||  line[i]=='"' || line[i]=='}'){      
            
            if(line[i] == ';' ){
                break;
            }
	    }
        else{
           tempString[j]=line[i];
           j++;
        }
   }

i = 0;
while(dataBuffer[i][0] != '\0') {
    i++;
}

strcpy(dataBuffer[i], tempString);

}

void initArray(char dataBuffer[lineCount][wordLength]){
    int i, j;
    for (i = 0; i < 500; i++) {
        for (j = 0; j < 3000; j++) {
            dataBuffer[i][j] = '\0';
        }
        
    }
}

void prepareData(char *filePathToVerilogFile, char dataBuffer[lineCount][wordLength]) {
    initArray(dataBuffer);
    FILE *fp;
    char str[buffToRead];

    fp = fopen(filePathToVerilogFile , "r");
    if(fp == NULL) {
       perror("Error opening file");
      return;
    }
    int counter=0;
    while(fgets(str, buffToRead, fp) != NULL) {
        if(str[0] != '\n' && str[0] != '/'){
            parse_line(dataBuffer, str);
        }
        counter++;
    }

    fclose(fp);
}