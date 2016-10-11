// parser.cpp
#include "parser.h"

#define MAXSTRING 2048 //max length of a string

std::vector<class Process *> Parse(char const * filename);

//PRETTYPRINT
void PrintFile(char const * filename) {
	std::vector<class Process *> pv = Parse(filename);
	int i = 0;
	for (; i < pv.size(); i++){
		printf("====================\n");
		pv[i]->pprint();
	}
	printf("====================\n");
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

// PARSE
std::vector<class Process *> Parse(char const *filename){

	std::vector<class Process *> procVec; //vector of procs

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

	    if (tokenVec.size() == 5){ //if right size, add to the vector
			Process * proc = new Process(tokenVec[0],atoi(tokenVec[1]),atoi(tokenVec[2]),atoi(tokenVec[3]),atoi(tokenVec[4]));
			procVec.push_back(proc);
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
