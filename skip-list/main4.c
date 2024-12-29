#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>
#include "skip-list.h"

enum MODE{
    LEFT,
    RIGHT
};

enum MODE mode = LEFT;
// Initializes a newNode
SkipNode *_makeNode(DATA v){
    SkipNode *newNode = (SkipNode*)malloc(sizeof(SkipNode));
    newNode->below = newNode->right = newNode->left = NULL; // initialize the pointers to NULL
    newNode->isSentinel = false;
    newNode->val = v;
    newNode->width = 0;
    return newNode; 
}

// Initializes a newLevel 
Level *_makeLevel(){
    Level *newLevel = (Level*)malloc(sizeof(Level));
    newLevel->up = newLevel->down = NULL;

    // initialize sentinels
    SkipNode *headSentinel = (SkipNode*)malloc(sizeof(SkipNode));
    headSentinel->left = headSentinel->right = headSentinel->below = NULL;

    SkipNode *tailSentinel = (SkipNode*)malloc(sizeof(SkipNode));
    tailSentinel ->left = tailSentinel ->right = tailSentinel ->below = NULL;

    headSentinel->isSentinel = true;
    headSentinel->val = INT_MIN;
    headSentinel->width = 0;

    tailSentinel->isSentinel = true;
    tailSentinel->val = INT_MAX;
    tailSentinel->width = 0;

    // link the sentinels 
    headSentinel->right = tailSentinel;
    tailSentinel->left = headSentinel;

    newLevel->headSentinel = headSentinel;
    newLevel->tailSentinel = tailSentinel;
    

    return newLevel;
} 

bool _flipCoin(){
    return rand() > RAND_MAX / 2;
}

// initialize the SkipList
SkipList *_initSkipList(LENGTH n){
    SkipList *skip_list = malloc(sizeof(SkipList));

    // initalize levelList
    skip_list->levels = (LevelList*)malloc(sizeof(LevelList));
    LevelList *levelList = skip_list->levels;
    levelList->bottom = levelList->top = _makeLevel();
    
    skip_list->leftmost = skip_list->rightmost = 0;
    skip_list->maxHeight = (LENGTH) ceil(log2(n));
    skip_list->currHeight = 0;

    skip_list->size = n;
    skip_list->is_reversed = false; // default : toggled off

    return skip_list;
}

SkipNode *_getNode(SkipList *l, LENGTH i){
    SkipList *skipList = l;
    SkipNode *header = skipList->levels->top->headSentinel; // Topmost headSentinel; Header
    LENGTH sumOffset = header->right->width; // initialize the offset for the topmost
    LENGTH target = i;
    // bounds check
    if (i >= l->size) return NULL;

    // headSentinel search
    while (header->isSentinel == true){
        if (header->right == NULL){
            header = header->below;
            continue;
        }
        if (sumOffset <= target){ // if the leftmost SkipNode width is 
            header = header->right;
            sumOffset = header->right->width; // update the current offset
        }
        else { // if the leftmost SkipNode width >= target go down the headSentinel
            header = header->below;
        }
    }

    // non-headSentinel search
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

void _promoteLevel(SkipList *l, Level *currLevel, SkipNode *sentinel, SkipNode *belowNode, DATA v){
    
    while (l->currHeight < l->maxHeight && _flipCoin() == true){ // promote the level
        // move up a level (create a new one if topmost)
        if (currLevel->up == NULL){
            Level *newLevel = _makeLevel();

            // update level connections
            newLevel->down = currLevel;
            currLevel->up = newLevel;
            l->levels->top = newLevel;

            // update sentinel connections
            newLevel->headSentinel->below = currLevel->headSentinel;
            newLevel->tailSentinel->below = currLevel->tailSentinel;
           
            
            l->currHeight++;
        }
        
        if (l->currHeight >= l->maxHeight){
            l->maxHeight = l->currHeight;
        }

        currLevel = currLevel->up;

        // right

        if (mode == LEFT){
            // traverse up through sentinels, and connect left/right/below
            SkipNode *newPrevNode = _makeNode(v);
            sentinel = l->is_reversed ? currLevel->tailSentinel : currLevel->headSentinel;
            SkipNode* prevNode = l->is_reversed ? sentinel->left :sentinel->right;

            // below
            newPrevNode->below = belowNode;
            belowNode = newPrevNode;

            if (!l->is_reversed){
                if (prevNode != NULL){
                    prevNode->left = newPrevNode;
                    newPrevNode->right = prevNode;
                }
                else {
                    currLevel->tailSentinel->left = newPrevNode;
                    newPrevNode->right = currLevel->tailSentinel;
                }

                // left
                sentinel->right = newPrevNode;
                newPrevNode->left = sentinel;
            }
            else {
                if (prevNode != NULL){
                    prevNode->right = newPrevNode;
                    newPrevNode->left = prevNode;
                }
                else {
                    currLevel->headSentinel->right = newPrevNode;
                    newPrevNode->left = currLevel->headSentinel;
                }

                // left
                sentinel->left = newPrevNode;
                newPrevNode->right = sentinel;
            }
            newPrevNode->width = 0;
        }
        else if (mode == RIGHT){
             // traverse up through sentinels, and connect left/right/below
            SkipNode *newPrevNode = _makeNode(v);
            sentinel = l->is_reversed ? currLevel->headSentinel : currLevel->tailSentinel;
            SkipNode* prevNode = l->is_reversed ? sentinel->right :sentinel->left;

            // below
            newPrevNode->below = belowNode;
            belowNode = newPrevNode;


            if (l->is_reversed){
                if (prevNode != NULL){
                    prevNode->left = newPrevNode;
                    newPrevNode->right = prevNode;
                }
                else {
                    currLevel->tailSentinel->left = newPrevNode;
                    newPrevNode->right = currLevel->tailSentinel;
                }

            // left
            sentinel->right = newPrevNode;
            newPrevNode->left = sentinel;
            }
            else {
                if (prevNode != NULL){
                    prevNode->right = newPrevNode;
                    newPrevNode->left = prevNode;
                }
                else {
                    currLevel->headSentinel->right = newPrevNode;
                    newPrevNode->left = currLevel->headSentinel;
                }

                // left
                sentinel->left = newPrevNode;
                newPrevNode->right = sentinel;
            }
            newPrevNode->width = 0;
        }
        
        // update values

    }

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

    Level* currLevel = l->levels->bottom; // start from level 0 
    while (currLevel != NULL){
        SkipNode* prevNode = l->is_reversed ? currLevel->tailSentinel->left : currLevel->headSentinel->right;
        if (prevNode != NULL && currLevel){
            prevNode->width++;
        }
        currLevel = currLevel->up;
    }

    // restart at level 0
    currLevel = l->levels->bottom;
    SkipNode *belowNode = _makeNode(v);
    SkipNode *sentinel = l->is_reversed ? currLevel->tailSentinel : currLevel->headSentinel; // header

    // update right connection
    if (!l->is_reversed){
        if (sentinel->right != NULL){
            sentinel->right->left = belowNode; // prevNode
            belowNode->right = sentinel->right;
        } else {
            currLevel->tailSentinel->left = belowNode;
            belowNode->right = currLevel->tailSentinel;
        }
        // update left connection
        sentinel->right = belowNode;
        belowNode->left = sentinel;

    }
    else {
        if (sentinel->left != NULL){
            sentinel->left->right = belowNode;
            belowNode->left = sentinel->left;
        } else {
            currLevel->headSentinel->right = belowNode;
            belowNode->left = currLevel->headSentinel;
        }
        // update right connection
        sentinel->left = belowNode;
        belowNode->right = sentinel;
    }
    
    // update values
    belowNode->width = 0;
    // change mode
    mode = LEFT;
    // coin flips
    _promoteLevel(l, currLevel, sentinel, belowNode, v);
}

void push_right(SkipList *l, DATA v){
       // update values
    l->size++;
    l->leftmost = v;
    if (l->size == 1){
        l->rightmost = v;
    }
    l->maxHeight = (LENGTH) ceil(log2(l->size));
    
    // update leftmost widths

    Level* currLevel = l->levels->bottom; // start from level 0 
    while (currLevel != NULL){
        SkipNode* prevNode = l->is_reversed ? currLevel->headSentinel->right : currLevel->tailSentinel->left;
        if (prevNode != NULL && currLevel){
            prevNode->width++;
        }
        currLevel = currLevel->up;
    }

    // restart at level 0
    currLevel = l->levels->bottom;
    SkipNode *belowNode = _makeNode(v);
    SkipNode *sentinel = l->is_reversed ? currLevel->headSentinel : currLevel->tailSentinel; // header

    // update right connection
    if (l->is_reversed){
        if (sentinel->right != NULL){
            sentinel->right->left = belowNode; // prevNode
            belowNode->right = sentinel->right;
        } else {
            currLevel->tailSentinel->left = belowNode;
            belowNode->right = currLevel->tailSentinel;
        }
        // update left connection
        sentinel->right = belowNode;
        belowNode->left = sentinel;

    }
    else {
        if (sentinel->left != NULL){
            sentinel->left->right = belowNode;
            belowNode->left = sentinel->left;
        } else {
            currLevel->headSentinel->right = belowNode;
            belowNode->left = currLevel->headSentinel;
        }
        // update right connection
        sentinel->left = belowNode;
        belowNode->right = sentinel;
    }
    
    // update values
    belowNode->width = 0;

    // change mode
    mode = RIGHT;
    // coin flips
    _promoteLevel(l, currLevel, sentinel, belowNode, v);

}

SkipList *make(LENGTH n, DATA *seq){
    // undefined behavior
    if (n < 0) return NULL;
    if (n == 0) return _initSkipList(n);

    SkipList *sl = _initSkipList(n);
    
    // traverse through the sequence
    for (int i = 0; i < n; i++){
        push_left(sl, seq[i]);
    }

    return sl;
}

void _pruneLevel(SkipList *l, Level *currLevel){
    // get the below LevelSentinels
    Level *prevLevel = currLevel->up;
    Level *nextLevel = currLevel->down;

    if (nextLevel){
        nextLevel->up = prevLevel;
    }
    else {
        l->levels->top = NULL;
    }

    if (prevLevel){
        prevLevel->down = nextLevel;
    }
    else {
        l->levels->top = nextLevel;
    }
    currLevel->headSentinel->below = NULL;
    currLevel->tailSentinel->below = NULL;
    currLevel = NULL;

    l->currHeight--;
    l->maxHeight--;
}

bool _leftDemoteLevel(SkipList *l, Level *currLevel, SkipNode *popNode){
    // update the height
    while (currLevel && currLevel->headSentinel->right->val == popNode->val){
        SkipNode *currHeadSentinel = currLevel->headSentinel;
        SkipNode *currTailSentinel = currLevel->tailSentinel;

        SkipNode *currNode = currHeadSentinel->right;
        SkipNode *currPrevNode = currNode->right;

        // detach the node 
        currNode->left = currNode->right = NULL;
        currNode->below = NULL;
        free(currNode);

        // relink the sentinel to the curr level previous node
        if (currPrevNode){
            currHeadSentinel->right = currPrevNode;
            currPrevNode->left = currHeadSentinel;
        }
        else {
            currHeadSentinel->right = currTailSentinel;
            currTailSentinel->left = currHeadSentinel;
        }
        
        // if the level has no elements prune the level
        // if the sentinels are linked to eachother call prune
        if ((currHeadSentinel->right == currTailSentinel) && (currTailSentinel->left == currHeadSentinel)){
            _pruneLevel(l, currLevel);
        }
        currLevel = currLevel->up;
    }
    return true;
}

bool _rightDemoteLevel(SkipList *l, Level *currLevel, SkipNode *popNode){
     // update the height
    while (currLevel && currLevel->tailSentinel->left->val == popNode->val){
        SkipNode *currHeadSentinel = currLevel->headSentinel;
        SkipNode *currTailSentinel = currLevel->tailSentinel;

        SkipNode *currNode = currTailSentinel->left;
        SkipNode *currPrevNode = currNode->left;

        // detach the node 
        currNode->left = currNode->right = NULL;
        currNode->below = NULL;
        free(currNode);

        // relink the sentinel to the curr level previous node
        if (currPrevNode){
            currTailSentinel->left = currPrevNode;
            currPrevNode->right = currTailSentinel;
        }
        else {
            currHeadSentinel->right = currTailSentinel;
            currTailSentinel->left = currHeadSentinel;
        }
        
        // if the level has no elements prune the level
        // if the sentinels are linked to eachother call prune
        if ((currHeadSentinel->right == currTailSentinel) && (currTailSentinel->left == currHeadSentinel)){
            _pruneLevel(l, currLevel);
        }
        currLevel = currLevel->up;
    }
    return true;
}

bool pop_left(SkipList *l){
    // cannot pop an empty list
    if (l->size == 0) return false;

    Level *currLevel = l->levels->bottom; // start from level 0
    SkipNode *popNode = currLevel->headSentinel->right; 
    while (currLevel != NULL){
        popNode = currLevel->headSentinel->right;
        SkipNode* prevNode = popNode->right;
        if (prevNode != NULL && currLevel){
            prevNode->width--; // decrement widths
        }
        currLevel = currLevel->up;
    }

    currLevel = l->levels->bottom;
    popNode = l->is_reversed ? currLevel->tailSentinel->left : currLevel->headSentinel->right;

    // single case
    l->leftmost = (popNode->right && popNode->right != currLevel->tailSentinel) ? popNode->right->val : 0; 

    // decrease the size
    l->size--;
    
    return l->is_reversed ? _rightDemoteLevel(l, currLevel, popNode) : _leftDemoteLevel(l, currLevel, popNode);
}

bool pop_right(SkipList *l){
    // cannot pop an empty list
    if (l->size == 0) return false;

    Level *currLevel = l->levels->bottom; // start from level 0
    SkipNode *popNode = currLevel->headSentinel->right; 
    while (currLevel != NULL){
        popNode = currLevel->headSentinel->right;
        SkipNode* prevNode = popNode->right;
        if (prevNode != NULL && currLevel){
            prevNode->width--; // decrement widths
        }
        currLevel = currLevel->up;
    }

    currLevel = l->levels->bottom;
    popNode = l->is_reversed ? currLevel->headSentinel->right : currLevel->tailSentinel->left;

    // single case
    l->rightmost = (popNode->left && popNode->left != currLevel->headSentinel) ? popNode->left->val : 0; 

    // decrease the size
    l->size--;
    
    return l->is_reversed ? _leftDemoteLevel(l, currLevel, popNode) : _rightDemoteLevel(l, currLevel, popNode);
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

DATA peek_left(SkipList *l){
    if (l->size == 0) return 0;
    return l->leftmost;
}

DATA peek_right(SkipList *l){
    if (l->size == 0) return 0;
    return l->rightmost;
}

void display(SkipList *sl) {

    Level *currLevel = sl->levels->top;
    printf("Max Level: %d \n", sl->currHeight);
    int currHeight = sl->currHeight;
    while (currLevel != NULL){
        // traverse right
        printf("Level %d: ", currHeight);
        SkipNode *curr = currLevel->headSentinel; // leftmost
        bool first = true;
        while (curr != NULL){
            printf("%d <-> ", curr->val);
            // if (curr->right != NULL){
            //     int o = 0;
            //     while (o < curr->right->width){
            //         printf("| -> ");
            //         o++;
            //     }
            // }
            curr = curr->right;
        }
        currLevel = currLevel->down;
        currHeight--;
        printf("\n");
    }
    
}

void reverse(SkipList *sl){
    sl->is_reversed = !(sl->is_reversed);
}

int main(){
    srand(time(NULL));
    int n = 3;
    DATA *array = malloc(n * sizeof(DATA*));
    for (int i = 0; i < n; i++){
        array[i] = i;
    }
    SkipList *skip_list = make(n, array);
    display(skip_list);
    // get(skip_list, 2);
    printf("%d ", get(skip_list, 1));

}