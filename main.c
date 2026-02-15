/* COP 3502C PA2
This program is written by: Nicholas Gill */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SCORES 5 // size of a cat scores array representing 5 traits
#define MAX_STR 25 // maximum name length of a cat
const float POSITION_BONUS[10] = {3.0, 5.0, 4.0, 6.0, 7.0, 2.0, 8.0, 1.0, 9.0, 1.5};
// bonus percentages that apply to each position
Cat *cats;
Rivals *rivals;
float bestPermScore;
int **tracker;

typedef struct Cat{
char *name; // dynamically allocated cat name
char *breed; // dynamically allocated breed
int scores[MAX_SCORES]; // array of size 5 representing trait scores
int baseScore; // sum of scores array
} Cat;

typedef struct Rivals{
Cat *cat1; // pointer to cat1
Cat *cat2; // pointer to cat2
} Rivals;

void createCats(int *n, int *c,int *t); //create cats
void createRivals(int *r, int *t); //create rivals
void createTracker(int n, int c); //allocate memory to tracker
Cat *findCatByName(char *name, int numCats); //searches each cat and returns cat if name's are the same
int countHighPerformersTraits(int c, int stop);//to count high performer bonus traits in a team
int searchPerformers(int c, int i); //recursively searches through a cat for traits above or equal to 90
int synergyBonusApplies(int c, int stop);//to check if a team gets a synergy bonus or not
int searchSynergy(int c, int i); //searches through team to see if any scores are >= 85
int rivalPenaltyApplies();//to check if a team gets a rival's penalty or not
void usedArrayPerm(); //used array permutation function



int main(){
    int numTeams, numCats, numRivals, totalCats;//n is number of teams, c is number of cats on each team
    int *n = &numTeams;
    int *c = &numCats;
    int *r = &numRivals;
    int *t = &totalCats;
    
    createCats(n, c, t); // create cat teams
    createRivals(r, t); //craete rivals
    createTracker(*n, *c); //allocates memory for tracker

    return 0;
}

void createCats(int *n, int *c,int *t){ //takes input, then creates cat teams and their attributes
    scanf("%d %d", n, c);
    *t = (*n) * (*c); //number of total cats

    cats = (Cat*)malloc(*t * sizeof(Cat));// allocate memory for cats
    int temp = 0;
    char tempName[MAX_STR];
    char tempBreed[MAX_STR];
    for(int i = 0; i < t; i++){ //take input for each cat and initialize each field
        scanf("%s", tempName);
        cats[i].name = (char*)malloc((strlen(tempName) + 1 )* sizeof(char));
        strcpy(cats[i].name, tempName); //create perfectly sized name
        scanf("%s", tempBreed);
        cats[i].breed = (char*)malloc((strlen(tempBreed) + 1 )* sizeof(char));
        strcpy(cats[i].breed, tempBreed); //create perfectly sized breed
        for(int j = 0; j < MAX_SCORES; j++){
            scanf("%d", &cats[i].scores[j]);
            temp += cats[i].scores[j];
        }
        cats[i].baseScore = temp;
        temp = 0;
    }
}

void createRivals(int *r, int *t){ //takes input, and searches for the right cat to initialize rivals
    char tempName[MAX_STR];
    scanf("%d", r); //input number of rivals
    rivals = (Rivals*)malloc(*r * sizeof(Rivals)); //allocate memory for rivals
    for(int i = 0; i < *r; i++){ //input, search, and initialize each rival
        scanf("%s", tempName);
        rivals[i].cat1 = findCatByName(tempName, *t);
        scanf("%s", tempName);
        rivals[i].cat2 = findCatByName(tempName, *t);
    }
}

void createTracker(int n, int c){ //allocates memory to tracker based on number of teams and cats per team
    tracker = (int**)malloc(n * sizeof(int*));
    for(int i = 0; i < n; i++){
        tracker[i] = (int*)malloc(c * sizeof(int));
    }
}

Cat *findCatByName(char *name, int t){ //searches each cat and compares input name to cat name
    for(int i = 0; i < t; i++){
        if(strcmp(name, cats[i].name) == 0){
            return &cats[i];
        }
    }
    printf("\n\ncat not found\n\n"); //no cat found error
    return NULL;
}

int countHighPerformersTraits(int c, int stop){ //recursively adds +5 for every trait over 90
    if(c-1 == stop)
        return 0;
    int total = 0;
    total += countHighPerformersTraits(c-1, stop);
    total += searchPerformers(c, MAX_SCORES);
    return total;
}

int searchPerformers(int c, int i){ //recursively searches each trait in a cat
    if(i-1 == -1)
        return 0;
    int total = searchPerformers(c, i-1);
    if(cats[c].scores[i-1] >= 90)
        total += 5;
    return total;
}

int synergyBonusApplies(int c, int stop){
    if(c-1 == stop)
        return 1;
    if(!searchSynergy(c, MAX_SCORES))
        return 0;
    return synergyBonusApplies(c-1, stop);
}

int searchSynergy(int c, int i){
    if(i-1 == -1)
        return 0;
    int total = searchSynergy(c, i-1);
    if(cats[c].scores[i-1] >= 85)
        return 1;
    return searchSynergy(c, i-1);
}

int rivalPenaltyApplies(int c, int stop, int r){
    if(c-2 == stop)
        return 0;
    int penalty = 0;
    penalty -= rivalPenaltyApplies(c-1, stop, r);
    penalty = searchRival(c, r) * -25;
    return penalty;
}

int searchRival(int c, int r){
    if(r-1 == -1)
        return 0;
    int total = 0;
    searchRival(c, r-1);
    if(&cats[c-2] == rivals[r].cat1 || &cats[c-2] == rivals[r].cat2)
        if(&cats[c-1] == rivals[r].cat1 || &cats[c-1] == rivals[r].cat2)
            total += 1;
    return total;
}