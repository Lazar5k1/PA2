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

void countHighPerformersTraits();//to count high performer bonus traits in a team
void synergyBonusApplies();//to check if a team gets a synergy bonus or not
void rivalPenaltyApplies();//to check if a team gets a rival's penalty or not
void usedArrayPerm(); //used array permutation function
Cat *createTeams(int *n, int *c, Cat *cats); //create cat list
