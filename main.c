#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX 1000

struct Receta{
  char name[50];
  char description[300];
  char category[50];
  char profile[100];
 
  
};

int parseFile(FILE *ptrFile) {
	char* line = (char*) malloc(sizeof(char) * MAX);
	

	ptrFile = fopen("Recipes.txt", "r");
  
	if(ptrFile == NULL) {
		printf("Error in fopen \n");
		return -1;
	}
  else{
		printf("File opened successfully \n");
		// Reads file by lines
    while(!feof(ptrFile)) {
			// Recipe
      fgets(line, MAX, ptrFile);
			// Description
			// Category
			// Profile
			printf("%s", line);
			// 
    }
	}
}


int main(void) {
	FILE *ptrFile;

	if(parseFile(ptrFile));
  
  return 0;
}

//line.find("Ingredients") != string::nops 