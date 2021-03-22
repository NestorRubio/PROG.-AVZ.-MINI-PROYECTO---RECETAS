#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
// #include "FileReader.h"
#define MAX 1000

struct RecipeList recipes;


struct IngredientNode {
	char* name;
	int amt;
	struct IngredientNode* next;
};

struct IngredientList {
	struct IngredientNode* head;
	struct IngredientNode* tail;	
};

struct Recipe {
  char* name;
  char* description;
  char* category;
	char* profile;
	struct IngredientList ingredientList;
};

struct RecipeNode {
	struct Recipe recipe;
	struct RecipeNode* next;
};

// Dynamic list of recipes
struct RecipeList {
	struct RecipeNode* head;
	struct RecipeNode* tail;	
};

void printIngredients(struct IngredientList ingredientList) {
	struct IngredientNode *curr = ingredientList.head;
	while(curr != NULL) {
		printf("%s:", curr->name);
		printf("%d\n", curr->amt);
		curr = curr->next;
	}
}
void printRecipes() {
	struct RecipeNode *curr = recipes.head;
	while(curr != NULL) {
		printf("Name: %s\n", curr->recipe.name);
		printf("Description: %s\n", curr->recipe.description);
		printf("Category: %s\n", curr->recipe.category);
		printf("Profile: %s\n", curr->recipe.profile);
		printIngredients(curr->recipe.ingredientList);
		curr = curr->next;
	}
}
void insertRecipe(struct Recipe* recipe) {
	struct RecipeNode *recipeNode;

	recipeNode = (struct RecipeNode*)malloc(sizeof(struct RecipeNode));
	recipeNode->recipe = *recipe;
	recipeNode->next = NULL;
	

	if(recipes.head == NULL) {
		recipes.head = recipeNode;
		recipes.tail = recipeNode;
	}
	else {
		recipes.tail->next = recipeNode;
		recipes.tail= recipes.tail->next;
	}
}

void insertIngredient(char* name, int amt, struct Recipe* recipe) {
	struct IngredientNode *ingredientNode;
	ingredientNode = (struct IngredientNode*)malloc(sizeof(struct IngredientNode));
	ingredientNode->name = (char *)malloc(sizeof(char) * MAX);
	strcpy(ingredientNode->name, name);
	ingredientNode->amt = amt;
	ingredientNode->next = NULL;

	if(recipes.head == NULL) {
		recipe->ingredientList.head = ingredientNode;
		recipe->ingredientList.tail = ingredientNode;
	}
	else {
		recipe->ingredientList.tail->next = ingredientNode;
		recipe->ingredientList.tail = recipe->ingredientList.tail->next;
	}
}

char* findVal(char* stringToFind, char* line) {
	char* matchPtr;
	matchPtr = strstr(line, stringToFind);
	if(matchPtr == NULL) {
		return NULL;
	}
	return matchPtr + strlen(stringToFind);
}

void readRecipeHeader(FILE *ptrFile, struct Recipe* recipe) {
	char* line = (char*) malloc(sizeof(char) * MAX);

	printf("%s", recipe->name);
	fgets(line, MAX, ptrFile);
	strcpy(recipe->description , findVal("Description:", line));
	printf("%s", recipe->description);
	fgets(line, MAX, ptrFile);
	strcpy(recipe->category , findVal("Categories:", line));
	printf("%s", recipe->category);
	fgets(line, MAX, ptrFile);
	strcpy(recipe->profile , findVal("Profile:", line));
	printf("%s", recipe->profile);
	fgets(line, MAX, ptrFile);
	findVal("Ingredients:", line);
}

int getCharPos(const char c, const char* line) {
	for(int i = 0; i < strlen(line); i++) {
		if(line[i] == c) return i;
	}
	return -1;
}


//empezar con Recipe->IngredientsList->head
int recipeComparison(struct IngredientNode *receta1, struct IngredientNode *receta2){
  double ph;
  int counter = 0;
  double distancia = 0;
  char char_Arr[15][30];

  struct IngredientNode *temp1;
  struct IngredientNode *temp2;

  temp1 = receta1;
  temp2 = receta2;

  while(temp1 && temp2){
    do{
      if(strcmp(temp1->name, temp2->name)){
        distancia += pow(temp1->amt-temp2->amt, 2);
        char_Arr[counter][0] = *temp1->name;
        counter++;
      }
      temp1 = temp1->next;
    }while(temp1);
    temp1 = receta1;
    temp2 = temp2->next;
  };

  temp1 = receta1;
  temp2 = receta2;
  int counter2 = 0;
  int counter3 = 0;

  while(temp1){
    if(!strcmp(temp1->name, char_Arr[counter2])){
      distancia += pow(temp1->amt, 2);
    }
    counter2++;
  }
  while(temp2){
    if(!strcmp(temp2->name, char_Arr[counter3])){
      distancia += pow(temp2->amt, 2);
    }
    counter3++;
  }

  return sqrt(distancia);

  return 0;
}



void readRecipeIngredients(FILE *ptrFile, struct Recipe* recipe) {
	char* line = (char*) malloc(sizeof(char) * MAX);
	char* name = (char *) malloc(sizeof(sizeof(char) * MAX));
	char* amt = (char *) malloc(sizeof(sizeof(char) * MAX));
	int iamt;

	fgets(line, MAX, ptrFile);
	// Read pairs until recipe text is found
	while(findVal("Recipe:", line) == NULL && !feof(ptrFile)) {
		int colonPos = getCharPos(':', line);
		if(colonPos < 0) {
			printf("Expected to find : in ingredient pair. Found none. Please check that the document is in a valid format.");
			return;
		}

		// capturing ingredient name
		memcpy(name, line, colonPos);
		memcpy(amt, line + colonPos + 1, strlen(line) - colonPos + 1);
		printf("%s: ", name);
		printf("%s", amt);
		iamt = atoi(amt);
		insertIngredient(name, iamt, recipe);
	}
}


void readFile(FILE *ptrFile, char* fileName) {
	ptrFile = fopen(fileName, "r");

	char* line = (char*) malloc(sizeof(char) * MAX);
	struct Recipe* recipe;
	int accum, colonPos, iamt;
	char *name, *amt;

	if(ptrFile == NULL) {
		printf("Error in fopen \n");
	}
  else{
		printf("File opened successfully \n");
		do {
			fgets(line, MAX, ptrFile);
			recipe = (struct Recipe*) malloc(sizeof(struct Recipe));
			recipe->name = (char *) malloc(sizeof(char) * MAX);
			recipe->description = (char *) malloc(sizeof(char) * MAX);
			recipe->category = (char *) malloc(sizeof(char) * MAX);
			recipe->profile = (char *) malloc(sizeof(char) * MAX);

			//recipeHeader
			strcpy(recipe->name , findVal("Recipe:", line));
			fgets(line, MAX, ptrFile);
			strcpy(recipe->description , findVal("Description:", line));
			fgets(line, MAX, ptrFile);
			strcpy(recipe->category , findVal("Categories:", line));
			fgets(line, MAX, ptrFile);
			strcpy(recipe->profile , findVal("Profile:", line));

			printf("%s", recipe->name);
			printf("%s", recipe->description);
			printf("%s", recipe->category);
			printf("%s", recipe->profile);

			fgets(line, MAX, ptrFile);
			findVal("Ingredients:", line);

			accum = 0;
			do {
				fgets(line, MAX, ptrFile);

				colonPos = getCharPos(':', line);
				name = (char *) malloc(sizeof(sizeof(char) * MAX));
				amt = (char *) malloc(sizeof(sizeof(char) * MAX));

				if(colonPos < 0) {
					printf("Expected to find : in ingredient pair. Found none. Please check that the document is in a valid format.");
					return;
				}
			
				// capturing ingredient name
				printf("%s: ", line);
				memcpy(name, line, colonPos);
				memcpy(amt, line + colonPos + 1, strlen(line) - colonPos + 1);
				printf("%s: ", name);

				iamt = atoi(amt);
				accum += iamt;
				printf("%d\n", iamt);
				insertIngredient(name, iamt, recipe);
				free(name);
				free(amt);
				printf("%d", accum);
			} while(accum < 100 && !feof(ptrFile));

			free(recipe);
			insertRecipe(recipe);
		} while(!feof(ptrFile));
		printRecipes();
	}
}


int main(void) {
	FILE *ptrFile;
	char* fileName = "Recipes.txt";
	readFile(ptrFile, fileName);
  return 0;
}

//line.find("Ingredients") != string::nops 