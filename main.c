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
int countHighPerformersTraits(int *perm, int start, int end);//to count high performer bonus traits in a team
int searchPerformers(int catIndex, int i); //recursively searches through a cat for traits above or equal to 90
int synergyBonusApplies(int *perm, int start, int end);//to check if a team gets a synergy bonus or not
int searchSynergy(int catIndex, int i); //searches through team to see if any scores are >= 85
int rivalPenaltyApplies(int *perm, int start, int end, int r);//to check if a team gets a rival's penalty or not
int searchRival(int *perm, int start, int r); //checks every rival pair at current team position
int diversityBonusApplies(int *perm, int start, int end); //checks if every cat is different or not
float calculateTeamScore(int *perm, int start, int end, int c, int r); //calculates a team's score within the list using helper functions
void usedArrayPerm(int *perm, int *used, int k, int n, int c, int r); //used array permutation function
void printOutput(int n, int c, int r); //print output of best team permutation, team, and score
void freeMemory(int n, int t); //frees memory of program



int main(){
    int numTeams, numCats, numRivals, totalCats;//n is number of teams, c is number of cats on each team
    int *n = &numTeams;
    int *c = &numCats;
    int *r = &numRivals;
    int *t = &totalCats;
    
    createCats(n, c, t); // create cat teams
    createRivals(r, t); //craete rivals
    createTracker(*n, *c); //allocates memory for tracker

    bestPermScore = 0;
    int perm[*t];
    int used[*t]; //can't initialize with 0 at compile time
    for(int i = 0; i < *t; i++){
        used[i] = 0;
    }

    usedArrayPerm(perm, used, 0, *n, *c, *r);
    printOutput(*n, *c, *r);
    freeMemory(*n, *t);


    return 0;
}

void createCats(int *n, int *c, int *t){ //takes input, then creates cat teams and their attributes
    scanf("%d %d", n, c);
    *t = (*n) * (*c); //number of total cats

    cats = (Cat*)malloc(*t * sizeof(Cat));// allocate memory for cats
    int temp = 0;
    char tempName[MAX_STR];
    char tempBreed[MAX_STR];
    for(int i = 0; i < *t; i++){ //take input for each cat and initialize each field
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

int countHighPerformersTraits(int *perm, int start, int end){ //recursively adds +5 for every trait over 90
    if(start == end)
        return 0;
    int total = searchPerformers(perm[start], MAX_SCORES);
    total += countHighPerformersTraits(perm, start + 1, end);
    return total;
}

int searchPerformers(int catIndex, int i){ //recursively searches each trait in a cat
    if(i-1 == -1)
        return 0;
    int total = searchPerformers(catIndex, i-1);
    if(cats[catIndex].scores[i-1] >= 90)
        total += 5;
    return total;
}

int synergyBonusApplies(int *perm, int start, int end){ //recursivley checks for synergy bonus
    if(start == end)
        return 1;
    if(!searchSynergy(perm[start], MAX_SCORES))
        return 0;
    return synergyBonusApplies(perm, start + 1, end);
}

int searchSynergy(int catIndex, int i){ //searches each trait to check synergy
    if(i-1 == -1)
        return 0;
    if(cats[catIndex].scores[i-1] >= 85)
        return 1;
    return searchSynergy(catIndex, i-1);
}

int rivalPenaltyApplies(int *perm, int start, int end, int r){ //recursively checks for rival penalty (only adjacent cats apply)
    if(start >= end -1)
        return 0;
    int penalty = rivalPenaltyApplies(perm, start + 1, end, r);
    penalty += searchRival(perm, start, r) * 25;
    return penalty;
}

int searchRival(int *perm, int start, int r){ //checks every rival pair at current team endition
    if(r-1 == -1)
        return 0;
    int total = searchRival(perm, start, r-1);
    if((&cats[perm[start]] == rivals[r-1].cat1 && &cats[perm[start + 1]] == rivals[r-1].cat2) ||
       (&cats[perm[start]] == rivals[r-1].cat2 && &cats[perm[start + 1]] == rivals[r-1].cat1))
            total += 1;
    return total;
}

int diversityBonusApplies(int *perm, int start, int end){ //checks if every cat is different or not
    for(int i = start; i < end; i++){
        for(int j = i + 1; j < end; j++){
            if(strcmp(cats[perm[i]].breed, cats[perm[j]].breed) == 0)
                return 0;
        }
    }
    return 1;
}

float calculateTeamScore(int *perm, int start, int end, int c, int r){ //calculates a team's score within the list using helper functions
    float total = 0;

    for(int i = start; i < end; i++){ //add base scores
        total += cats[perm[i]].baseScore;
    }
    total /= c; //take average to get base team score


    total += countHighPerformersTraits(perm, start, end);
    if(synergyBonusApplies(perm, start, end))
        total += 30;
    if(diversityBonusApplies(perm, start, end))
        total += 10;
    else
        total -= 15;

    int temp = 0;
    for(int i = start; i < end; i++){
        total += cats[perm[i]].baseScore * (POSITION_BONUS[temp] / 100.0);
        temp++;
    }
    total -= rivalPenaltyApplies(perm, start, end, r);

    return total;
}

void usedArrayPerm(int *perm, int *used, int k, int n, int c, int r){ //used array permutation function
    if(k == n * c){
        float score = 0;

        for(int i = 0; i < n; i++){
            int start = i *c;
            int end = start + c;
            score += calculateTeamScore(perm, start, end, c, r);
        }

        if(score > bestPermScore){
            bestPermScore = score;
            
            for(int i = 0; i < n; i++){
                for(int j = 0; j < c; j++){
                    tracker[i][j] = perm[i * c + j];
                }
            }
        }
        return;
    }


    for(int i = 0; i < n * c; i++){
        if(!used[i]){
            used[i] = 1;
            perm[k] = i;
            usedArrayPerm(perm, used, k+1, n, c, r);
            used[i] = 0;
        }
    }
}

void printOutput(int n, int c, int r){ //print output of best team permutation, team, and score
    printf("Best Teams Grouping score: %.2f\n", bestPermScore);

    float bestTeamScore = -1;
    int bestTeamIndex = 0;

    for(int i = 0; i < n; i++){
        printf("Team %d: ", i + 1);
        for(int j = 0; j < c; j++){
            printf("%s ", cats[tracker[i][j]].name);
        }

        float teamScore = calculateTeamScore(tracker[i], 0, c, c, r);
        printf("%.2f\n", teamScore);

        if(teamScore > bestTeamScore){
            bestTeamScore = teamScore;
            bestTeamIndex = i;
        }
    }

    printf("Best Candidate: ");
    for(int i = 0; i < c; i++){
        printf("%s ", cats[tracker[bestTeamIndex][i]].name);
    }
}

void freeMemory(int n, int t){ //frees memory of program
    for(int i = 0; i < t; i++){
        free(cats[i].name);
        free(cats[i].breed);
    }
    free(cats);
    free(rivals);
    for(int i = 0; i < n; i++){
        free(tracker[i]);
    }
    free(tracker);
}