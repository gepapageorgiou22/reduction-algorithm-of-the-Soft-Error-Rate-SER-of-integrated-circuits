#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define maxWordSize 4000
#define buffToRead 4000
#define lineCount 1000
#define wordLength 4000

void parse_line(char dataBuffer[lineCount][wordLength], char *line){

int i, j=0;
char tempString[maxWordSize];

for (i = 0; i < maxWordSize; i++){
    tempString[i] = '\0';
}


   for(i=0; i<=(strlen(line)); i++){
       if(line[i]==' ' || line[i]=='\0' || line[i]=='{' || line[i]==':' ||  line[i]=='"' || line[i]=='}' || line[i]=='\n'){      
            
            if(line[i] == ';' || line[0]=='\n'){
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

void replaceFalseNewLines(char dataBuffer[lineCount][wordLength]) {
    int counterLine;
    int counterWord;

    for(counterLine=0; counterLine<lineCount; counterLine++) {
        for(counterWord=0; counterWord<wordLength; counterWord++) {
            if(dataBuffer[counterLine][counterWord] == '\n') {
                dataBuffer[counterLine][counterWord] = '\0';
            } 
        }
    }
}

void initArray(char dataBuffer[lineCount][wordLength]){
    int i, j;
    for (i = 0; i < 1000; i++) {
        for (j = 0; j < 4000; j++) {
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

    replaceFalseNewLines(dataBuffer);

    fclose(fp);
}