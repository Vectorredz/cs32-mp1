#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>
#include "skip-list.h"


SkipNode *_makeNode(DATA v){
    SkipNode *newNode = (SkipNode*)malloc(sizeof(SkipNode));
    newNode->below = newNode->right = newNode->left = NULL; // initialize the pointers to NULL
    newNode->isSentinel = false;
    newNode->val = v;
    newNode->width = 0;
    return newNode; // return the created SkipNode
}

Level *_makeLevel(){
    Level *newLevel = (Level*)malloc(sizeof(Level));
    newLevel->up = NULL;
    newLevel->down = NULL;

    SkipNode *sentinel = (SkipNode*)malloc(sizeof(SkipNode));
    sentinel->left = NULL;
    sentinel->right = NULL;
    sentinel->below = NULL;

    sentinel->isSentinel = true;
    sentinel->val = INT_MIN;
    sentinel->width = 0;

    newLevel->sentinel = sentinel;
    newLevel->tail = sentinel;

    return newLevel;
} 

bool _flipCoin(){
    return rand() > RAND_MAX / 2;
}

// initialize the skip-SkipList
SkipList *_initSkipList(LENGTH n){
    SkipList *skip_list = malloc(sizeof(SkipList));

    // initalize levelList
    skip_list->levels = (LevelList*)malloc(sizeof(LevelList));
    LevelList *levelList = skip_list->levels;
    levelList->bottom = levelList->top = _makeLevel();
    
    skip_list->leftmost = 0;
    skip_list->rightmost = 0;
    skip_list->maxHeight = (LENGTH) ceil(log2(n));
    skip_list->currHeight = 0;

    skip_list->size = n;
    skip_list->is_reversed = false; // default : toggled off

    return skip_list;
}


SkipNode *_getNode(SkipList *l, LENGTH i){
    SkipList *skipList = l;
    SkipNode *header = skipList->levels->top->sentinel;
    LENGTH sumOffset = header->right->width; // initialize the offset for the topmost
    LENGTH target = i;
    // bounds check
    if (i >= l->size) return NULL;

    // sentinel search
    while (header->isSentinel == true){
        if (header->right == NULL){
            header = header->below;
            continue;
        }
        if (sumOffset <= target){ // if the leftmost SkipNode width is 
            header = header->right;
            sumOffset = header->right->width; // update the current offset
        }
        else { // if the leftmost SkipNode width >= target go down the sentinel
            header = header->below;
        }
    }

    // non-sentinel search
    // can go down as long as header->below != NULL
    while (header != NULL){
        if (header->right == NULL){
            header = header->below;
            continue;
        }
        if (sumOffset == target){ //if matching index
            header = header->below;
            continue;
        }
        if (sumOffset + header->right->width <= target){
            header = header->right;
            sumOffset += header->right->width; // update the current offset
        }
        else {
            header = header->below; // exhaust go down until the same offset is found
        }
    }
    return header; 

}

void push_left(SkipList *l, DATA v){

// update values
    l->size++;
    l->leftmost = v;
    if (l->size == 1){
        l->rightmost = v;
    }
    l->maxHeight = (LENGTH) ceil(log2(l->size));
    
    // update leftmost widths

    Level* currLevel = l->levels->bottom;
    while (currLevel != NULL){
        SkipNode* leftmostNode = currLevel->sentinel->right;
        if (leftmostNode != NULL){
            leftmostNode->width++;
        }
        currLevel = currLevel->up;
    }

    // restart at level 0
    currLevel = l->levels->bottom;
    SkipNode *belowNode = _makeNode(v);
    SkipNode *sentinel = currLevel->sentinel;

    // update right connection
    if (sentinel->right != NULL){
        sentinel->right->left = belowNode;
        belowNode->right = sentinel->right;
    } else {
        currLevel->tail = belowNode;
    }
    // update left connection
    sentinel->right = belowNode;
    belowNode->left = sentinel;

    // update values
    belowNode->width = 0;

    // coin flips
    while (l->currHeight < l->maxHeight && _flipCoin() == true){ // promote the level
        // move up a level (create a new one if topmost)
        if (currLevel->up == NULL){
            Level *newLevel = _makeLevel(l->currHeight);

            // update level connections
            newLevel->down = currLevel;
            currLevel->up = newLevel;
            l->levels->top = newLevel;

            // update sentinel connections
            newLevel->sentinel->below = currLevel->sentinel;
            
            l->currHeight++;
        }
        
        if (l->currHeight >= l->maxHeight){
            l->maxHeight = l->currHeight;
        }

        currLevel = currLevel->up;

        // traverse up through sentinels, and connect left/right/below
        SkipNode *newLeftNode = _makeNode(v);
        sentinel = currLevel->sentinel;
        SkipNode* prevLeftNode = sentinel->right;

        // below
        newLeftNode->below = belowNode;
        belowNode = newLeftNode;

        // right
        if (prevLeftNode != NULL){
            prevLeftNode->left = newLeftNode;
            newLeftNode->right = prevLeftNode;
        }
        else {
            currLevel->tail = newLeftNode;
        }        newLeftNode->right = prevLeftNode;

        // left
        sentinel->right = newLeftNode;
        newLeftNode->left = sentinel;
        
        // update values
        newLeftNode->width = 0;
    }

    
}

DATA get(SkipList *l, LENGTH i){ 
    SkipNode *retNode = _getNode(l, i);
    if (retNode == NULL){
        return 0; // SkipNode doesn't exist, error
    }
    return retNode->val;
}

void set(SkipList *l, LENGTH i, DATA v){ // set(l, 6, 60)
    SkipNode *retNode = _getNode(l, i);
    if (retNode == NULL){
        return; // SkipNode doesn't exist, error
    }
    retNode->val = v;
}

void display(SkipList *sl) {

    Level *currLevel = sl->levels->top;
    printf("Max Level: %d \n", sl->currHeight);
    while (currLevel != NULL){
        // traverse right
        printf("Level %d: ", sl->currHeight);
        SkipNode *curr = currLevel->sentinel; // leftmost
        bool first = true;
        while (curr != NULL){
            printf("%d -> ", curr->val);
            if (curr->right != NULL){
                int o = 0;
                while (o < curr->right->width){
                    printf("| -> ");
                    o++;
                }
            }
            curr = curr->right;
        }
        currLevel = currLevel->down;
        sl->currHeight--;
        printf("\n");
    }
    
}

int main(){
    srand(time(NULL));
    int n = 0;
    SkipList *skip_list = _initSkipList(0);

    push_left(skip_list, 0);
    push_left(skip_list, 1);
    push_left(skip_list, 2);
    push_left(skip_list, 3);
    push_left(skip_list, 4);
    push_left(skip_list, 5);
    push_left(skip_list, 6);
    push_left(skip_list, 7);
    display(skip_list);
}