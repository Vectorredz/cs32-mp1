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
list *initList(int n) { // O(1)
    list *sl = malloc(sizeof(list));
    
    sl->leftmost = sl->rightmost = sl->sentinel = NULL;
    sl->is_reversed = false;
    sl->maxHeight = 0;
    sl->size = n;

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

    list *sl0 = initList(n); // initialize the level 0: skip-list 
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
        
        // get the first element in the level 0
        if (i == 0){
            recorder->record[0]->right = curr;
            curr->left = recorder->record[0];
            recorder->record[0] = curr;
            sl0->leftmost = recorder->record[0];
        }
        // track the every prev element in the level 0 
        else {
             recorder->record[0]->right = curr;
            curr->left = recorder->record[0];
            recorder->record[0] = curr;
            sl0->rightmost = recorder->record[0];
        }

        // printf(" %d ", recorder->record[0]->val);
        
        // set the first element as the leftmost 

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
 
int64_t peek_left(list *l){
    node *curr = l->is_reversed ? l->rightmost : l->leftmost;
    return curr->val;
}

int64_t peek_right(list *l){
    node *curr = l->is_reversed ? l->leftmost : l->rightmost;
    return curr->val;
}

int size(list *l){
    return l->size;
}

bool empty(list *l){
    return (l->size == 0);
}

void display(list *sl) {
    node *currLevel = sl->sentinel;
    int level = sl->maxHeight;
    printf("H: (%d, %d) \n", currLevel->key, currLevel->val);
    while (currLevel) {
        printf("Level %d: ", level);
        node *curr = currLevel;
        while (curr) {
            printf("(%d, %d) <->", curr->key, curr->val);
            curr = curr->right;
        }
        printf("\n");
        currLevel = currLevel->below;
        level--;
    }
}

void reverse(list *l){
    l->is_reversed = !(l->is_reversed);
}

void push_left(list *l, int64_t v){
    list *sl = l;
    // if is_reversed is toggled 'ON' get the leftmost element else rightmost
    node *curr =  sl->is_reversed ? l->rightmost : l->leftmost; 
    node *sentinel = l->leftmost->left; // sentinel node

    if (!(sl->is_reversed)){
        // disconnect the sentinel to the curr
        sentinel->right = NULL;
        curr->left = NULL;

        // insert the newNode
        node *newNode = makeNode(v,v);
        newNode->right = curr; // link the new element to the leftmost element
        newNode->left = sentinel; // connect the newNode to the sentinel 
        sentinel->right = newNode;
        curr->left = newNode;
        curr = newNode; // set the latest inserted node as the current elemet / leftmost element

        //update the leftmost element
        sl->leftmost = curr;
    }
    else {
        // in rightmost there is no sentinel only NULL
        node *newNode = makeNode(v,v);
        newNode->left = curr;
        newNode->right = NULL;
        curr->right = newNode;
        curr = newNode;

        //update the rightmost element
        sl->rightmost = newNode;
    }
}

void push_right(list *l, int64_t v){
    list *sl = l;
    // if is_reversed is toggled 'ON' get the rightmost element else leftmost
    node *curr =  sl->is_reversed ? l->leftmost : l->rightmost; 
    node *sentinel = l->leftmost->left; // sentinel node

    if ((sl->is_reversed)){
        // disconnect the sentinel to the curr
        sentinel->right = NULL;
        curr->left = NULL;

        // insert the newNode
        node *newNode = makeNode(v,v);
        newNode->right = curr; // link the new element to the leftmost element
        newNode->left = sentinel; // connect the newNode to the sentinel 
        sentinel->right = newNode;
        curr->left = newNode;
        curr = newNode; // set the latest inserted node as the current elemet / leftmost element

        //update the leftmost element
        sl->leftmost = curr;
    }
    else {
        // in rightmost there is no sentinel only NULL
        node *newNode = makeNode(v,v);
        newNode->left = curr;
        newNode->right = NULL;
        curr->right = newNode;
        curr = newNode;

        sl->rightmost = newNode;
    }
}

int main() {
    srand(time(NULL)); 
    int n = 3;
    int64_t *array = malloc(n * sizeof(int64_t));
    for (int i = 0; i < n; i++){
        array[i] = i*10;
    }
    list *sl = make(n, array);
    display(sl);
}
