#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>
#include "skip-list.h"

// INITIALIZE A NEWNODE 
node *makeNode(int key, int val){ // O(1)
    node *newNode = malloc(sizeof(node));
    newNode->key = key;
    newNode->val = val;
    newNode->left = newNode->right = newNode->below = NULL;
    return newNode;
}

// INITIALIZE A SKIP-LIST
list *initList() { // O(1)
    list *sl = malloc(sizeof(list));
    
    sl->leftmost = sl->rightmost = sl->sentinel = NULL;
    sl->is_reversed = false;
    sl->maxHeight = 0;

    return sl;
}

// INITIALIZE DLL
dlist *initDll(int n) { // O(1)
    dlist *dl = malloc(sizeof(dlist));
    
    dl->next = dl->prev = NULL;
    dl->record = malloc(n * sizeof(node)); // allocate n cuz its the max possible height ever;

    return dl;
}

list *make(int n, int64_t *seq){ // O(n * 1/2)
    if (n == 0) return NULL;
    dlist *recorder = initDll(n);

    list *sl0 = initList(); // initialize the level 0: skip-list 
    node *sentinelZero = makeNode(INT_MIN, INT_MIN); // to keep tract of the head of the level 0
    sl0->sentinel = sentinelZero;
    recorder->record[0] = sentinelZero;

    // initialize the maxLVl as 0
    int maxLvl = 0;
    sl0->maxHeight = maxLvl;

    // traverse each element in the seq 
    for (int i = 0; i < n; i++){ // O(n)
        // create node based off the elements in the seq
        node *curr = makeNode(i, seq[i]);
        
        // track the every prev element in the level 0 
        recorder->record[0]->right = curr;
        curr->left = recorder->record[0];
        recorder->record[0] = curr;

        // inf -> 0 ->
        // inf -> 0 -> 
        int currLvl = 1; // start at level 1

        while (rand() < RAND_MAX / 2){ // Exhaust on how many levels per current node // O(1/2)
            if (currLvl > maxLvl){
                maxLvl = currLvl;
                sl0->maxHeight = maxLvl;
                node *newLevel = makeNode(INT_MIN, INT_MIN);// sentinel for the new level
                newLevel->below = sl0->sentinel;
                sl0->sentinel = newLevel;
                recorder->record[currLvl] = sl0->sentinel;
            }
            
            node *newNode = makeNode(i, seq[i]);
            newNode->below = curr;
           
            if (recorder->record[currLvl] != NULL){
                recorder->record[currLvl]->right = newNode;
                newNode->left = recorder->record[currLvl];
            }
            recorder->record[currLvl] = newNode;
            curr = newNode;
            currLvl++;
        } 
    }
    
    return sl0;
}


void display(list *sl) {
    node *currLevel = sl->sentinel;
    int level = sl->maxHeight;
    printf("H: (%d, %d) \n", currLevel->key, currLevel->val);
    while (currLevel) {
        printf("Level %d: ", level);
        node *curr = currLevel->right;
        while (curr) {
            printf("(%d, %d) <->", curr->key, curr->val);
            curr = curr->right;
        }
        printf("\n");
        currLevel = currLevel->below;
        level--;
    }
}

int main() {
    srand(time(NULL)); 
    int n = 5;
    int64_t *array = malloc(n * sizeof(int64_t));
    for (int i = 0; i < n; i++){
        array[i] = i;
    }
    list *sl = make(n, array);
    display(sl);
    
}
