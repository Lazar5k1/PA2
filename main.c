/* COP 3502C PA2
This program is written by: Nicholas Gill */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"

int main(){
    int numTeams, numCats;//n is number of teams, c is number of cats on each team
    int *n = &numTeams;
    int *c = &numCats;
    Cat *cats;

    
    createTeams(n, c, cats);

    return 0;
}

Cat *createTeams(int *n, int *c, Cat *cats){
    scanf("%d %d", n, c);\
    int numCats = (*n) * (*c);
    cats = (Cat*)malloc(numCats * sizeof(Cat));
    cats->name = (char*)malloc(numCats * sizeof(char));
    cats->breed = (char*)malloc(numCats * sizeof(char));

    for(int i = 0; i < numCats; i++){
        scnaf("%s", cats->name);
        scanf("%s", cats->breed);
        for(int j = 0; j < MAX_SCORES; j++){
            scanf("%d", cats->scores[j]);
        }
    }
    // write rival part next
}