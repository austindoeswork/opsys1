// parser.cpp
#include "process.h"
#include "parser.h"
#include <string.h> //for c string functions
#include <vector> //for vector stuff

#define MAXSTRING 2048 //max length of a string

std::vector<struct Process *> parseFile(char const * filename);

void Parse(char const * filename) {
	std::vector<struct Process *> pv = parseFile(filename);
	int i = 0;
	for (; i < pv.size(); i++){
		pv[i]->pprint();
	}
}

// PARSE HELPERS

int is_empty(const char *s) {
	while (*s != '\0') {
  		if (*s == '#') //TODO better comment checking
  			return 1;
		if (!isspace(*s))
  			return 0;
		s++;
		}
	return 1;
}

std::vector<struct Process *> parseFile(char const *filename){

	std::vector<struct Process *> procVec;


    FILE *file_p; //file pointer
    file_p = fopen(filename, "r"); //open the file

    //ERROR CHECKING ON FILE
    if (file_p == NULL){
        fprintf(stderr, "INVALID INPUT FILE %s\n", filename);
        exit(1);
    }

    //READ FILE LINE BY LINE
    char lineBuffer[MAXSTRING]; //temp buffer to hold line
    while(fgets(lineBuffer, MAXSTRING, file_p)){ //while lines exist
    	if (is_empty(lineBuffer))
    		continue;

        //READ LINE WORD BY WORD INTO valArray
        char *word_p = strtok(lineBuffer, "|"); //tokenize the line
        
    	std::vector<char *> tokenVec;

        for(; word_p != NULL; word_p = strtok(NULL, "|")){
        	char *copy = (char *) malloc(sizeof(char) * (strlen(word_p)+1));
        	strcpy(copy, word_p);
        	tokenVec.push_back(copy);
        }

	    if (tokenVec.size() == 5){
			Process * proc = new Process(tokenVec[0],atoi(tokenVec[1]),atoi(tokenVec[2]),atoi(tokenVec[3]),atoi(tokenVec[4]));
			procVec.push_back(proc);
			// proc.pprint();
	    }
	    
	    int i = 0;
	    for (; i < tokenVec.size(); i++){
	    	free(tokenVec[i]);
	    }
    }

    //some cleanup
    fclose(file_p); //close the file
    return procVec;
}
