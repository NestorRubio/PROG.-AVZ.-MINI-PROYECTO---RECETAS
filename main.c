#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#define DEBUG 0
// #include "FileReader.h"
#define MAX 1000

struct RecipeList recipes;

struct IngredientNode {
	char name[MAX];
	int amt;
	struct IngredientNode* next;
};

struct IngredientList {
	struct IngredientNode* head;
	struct IngredientNode* tail;	
	int size;
};

struct Recipe {
  char name[MAX];
  char description[MAX];
  char category[MAX];
	char profile[MAX];
	struct IngredientList ingredientList;
};

struct RecipeNode {
	struct Recipe recipe;
	struct RecipeNode* next;
};

// Dynamic list of recipes
struct RecipeList {
	int size;
	struct RecipeNode* head;
	struct RecipeNode* tail;	
};

void printIngredients(struct IngredientList ingredientList) {
	struct IngredientNode *curr = ingredientList.head;
	while(curr != NULL) {
		printf("%s:", curr->name);
		printf("%d \n", curr->amt);
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
		recipes.tail=recipes.tail->next;
	}
	recipes.size++;
}

void insertIngredient(char* name, int amt, struct Recipe* recipe) {
	struct IngredientNode *ingredientNode;
	ingredientNode = (struct IngredientNode*)malloc(sizeof(struct IngredientNode));
	memset(ingredientNode->name, '\0', sizeof(ingredientNode->name));
	strncpy(ingredientNode->name, name, strlen(name));
	ingredientNode->amt = amt;
	ingredientNode->next = NULL;

	if(recipe->ingredientList.head == NULL) {
		recipe->ingredientList.head = ingredientNode;
		recipe->ingredientList.tail = ingredientNode;
	}
	else {
		recipe->ingredientList.tail->next = ingredientNode;
		recipe->ingredientList.tail = recipe->ingredientList.tail->next;
	}
	recipe->ingredientList.size++;
}

int getCharPos(const char c, const char* line) {
	for(int i = 0; i < strlen(line); i++) {
		if(line[i] == c) return i;
	}
	return -1;
}



//empezar con Recipe->IngredientsList->head
double recipeComparison(struct IngredientList il1, struct IngredientList il2){
  double distancia = 0;
	bool foundDupe;

	struct IngredientNode *curr1 = il1.head, *curr2 = il2.head;


	while(curr1 != NULL) {
		foundDupe = false;
		while(curr2 != NULL) {
      if(strcmp(curr1->name, curr2->name) == 0) {
         	distancia += pow(curr1->amt - curr2->amt, 2);
					foundDupe = true;
					break;
		  }
			curr2 = curr2->next;
		}
		if(!foundDupe) {
			distancia += pow(curr1->amt, 2);
		}
    curr1 = curr1->next;
		curr2 = il2.head;
	}

	curr1 = il1.head;
	curr2 = il2.head;
	
  while(curr2 != NULL) {
		foundDupe = false;
		while(curr1 != NULL) {
			if(strcmp(curr1->name, curr2->name) == 0) {
				foundDupe = true;
				break;
			}
			curr1 = curr1->next;
		}
		if(!foundDupe) {
			distancia += pow(curr2->amt, 2);
		}
		curr2 = curr2->next;
		curr1 = il1.head;
	}
  
  return sqrt(distancia);
}

void printRecipes() {
	struct RecipeNode *curr = recipes.head;
	while(curr != NULL) {
		printf("Name: %s\n", curr->recipe.name);
		printf("Description: %s", curr->recipe.description);
		printf("Category: %s", curr->recipe.category);
		printf("Profile: %s", curr->recipe.profile);
		printf("# ingredients: %d\n", curr->recipe.ingredientList.size);
		printIngredients(curr->recipe.ingredientList);
		printf("\n");
		curr = curr->next;
	}
}

void readFile(FILE *ptrFile, char* fileName) {
	ptrFile = fopen(fileName, "r");

	char line[MAX], name[MAX], amt[MAX];
	struct Recipe* recipe = (struct Recipe*) malloc(sizeof(struct Recipe));
	int accum, colonPos, iamt;

	if(ptrFile == NULL) {
		printf("Error in fopen \n");
	}
  else{
		recipes.size = 0;
		printf("File opened successfully \n");
		do {

			// reseting variables
			memset(recipe->name, '\0', sizeof(recipe->name));
			memset(recipe->description , '\0', sizeof(recipe->description));
			memset(recipe->profile , '\0', sizeof(recipe->profile));
			memset(recipe->category , '\0', sizeof(recipe->category));
			memset(&recipe->ingredientList, 0, sizeof(struct IngredientList));

			//recipeHeader
			fgets(line, MAX, ptrFile);
			colonPos = getCharPos(':', line);
			if(colonPos < 0) {
				printf("ERROR IN FORMAT");
				return;
			}
			strncpy(recipe->name, line + colonPos + 1, strlen(line) - colonPos - 2);

			fgets(line, MAX, ptrFile);
			colonPos = getCharPos(':', line);
			if(colonPos < 0) {
				printf("ERROR IN FORMAT");
				return;
			}
			strncpy(recipe->description, line + colonPos + 1, strlen(line) - colonPos + 1);

			fgets(line, MAX, ptrFile);
			colonPos = getCharPos(':', line);
			if(colonPos < 0) {
				printf("ERROR IN FORMAT");
				return;
			}
			strncpy(recipe->category, line + colonPos + 1, strlen(line) - colonPos + 1);

			fgets(line, MAX, ptrFile);
			colonPos = getCharPos(':', line);
			if(colonPos < 0) {
				printf("ERROR IN FORMAT");
				return;
			}
			strncpy(recipe->profile, line + colonPos + 1, strlen(line) - colonPos + 1);

// #if DEBUG == 1
// 			printf("%s", recipe->name);
// 			printf("%s", recipe->description);
// 			printf("%s", recipe->category);
// 			printf("%s", recipe->profile);
// #endif

			fgets(line, MAX, ptrFile);
			accum = 0;
			recipe->ingredientList.size = 0;
			do {
				memset(amt, '\0', sizeof(amt));
				memset(name, '\0', sizeof(name));
				fgets(line, MAX, ptrFile);
				colonPos = getCharPos(':', line);
				if(colonPos < 0) {
					printf("Expected to find : in ingredient pair. Found none. Please check that the document is in a valid format.");
					return;
				}
			
				// capturing ingredient name
				strncpy(name, line, colonPos);
				strncpy(amt, line + colonPos + 1, strlen(line) - colonPos + 1);

				iamt = atoi(amt);
				accum += iamt;
				insertIngredient(name, iamt, recipe);
			} while(accum < 100 && !feof(ptrFile));
			insertRecipe(recipe);
		} while(!feof(ptrFile));
	}
}
void DispG(FILE *ptrG, char* fileNameG){
  int i,j;
  ptrG = fopen(fileNameG, "w");
  
  if(ptrG == NULL){
    printf("Error al abrir el visualizador \n");
  }
  else{
		struct RecipeNode *curri = recipes.head, *currj = recipes.head;
		int i = 0, j = 0;
    fprintf(ptrG, "digraph G  {\n");
		while(curri != NULL){
			j = 0;
			while(currj != NULL) {
				if(i != j) {
					fprintf(ptrG, "\"%s\" -> \"%s\" [label = \"%f\"];\n", curri->recipe.name, currj->recipe.name, recipeComparison(curri->recipe.ingredientList, currj->recipe.ingredientList));
				}
				currj = currj->next;
				j++;	
			}
			curri = curri->next;
			currj = recipes.head;
			i++;
      
		}
    fprintf(ptrG, "}\n");
  }
}

int main(void) {
	FILE *ptrFile;
	char* fileName = "Recipes.txt";
	readFile(ptrFile, fileName);
  FILE *ptrG;
  char* filenameG = "graph.gv";

	double distMat[MAX][MAX];
	struct RecipeNode *curri = recipes.head, *currj = recipes.head;
	int i = 0, j = 0;
	while(curri != NULL){
		j = 0;
		while(currj != NULL) {
			distMat[i][j] = recipeComparison(curri->recipe.ingredientList,currj->recipe.ingredientList);
			currj = currj->next;
			j++;	
		}
		curri = curri->next;
		currj = recipes.head;
		i++;
	}

// print the matrix
	for(int x = 0; x < recipes.size; x++) {
		for(int y = 0; y < recipes.size; y++) {
			printf("%f ", distMat[x][y]);
		}
		printf("\n");
	}
  
  DispG(ptrG,filenameG);
  printRecipes();
  
  
  return 0;
}

//line.find("Ingredients") != string::nops   printRecipes();
