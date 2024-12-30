#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>
#include "skip-list.h"

// --------------------------------------------------------- >>
/* ----------------------------------------- <<

            ||-- INITIALIZERS --||

<< ----------------------------------------- */
// --------------------------------------------------------- >>

LENGTH _capHeight(LENGTH n){
    return (LENGTH) ceil(log2(n));
}

SkipNode *_makeNode(DATA v){
    SkipNode *newNode = (SkipNode*)malloc(sizeof(SkipNode));
    newNode->below = newNode->right = newNode->left = NULL; // initialize the pointers to NULL
    newNode->isSentinel = false;
    newNode->val = v;
    newNode->width = 0;
    return newNode; 
}

Level *_makeLevel(){
    Level *newLevel = (Level*)malloc(sizeof(Level));
    newLevel->up = newLevel->down = NULL;

    // initialize sentinels
    SkipNode *headSentinel = (SkipNode*)malloc(sizeof(SkipNode));
    headSentinel->left = headSentinel->right = headSentinel->below = NULL;

    SkipNode *tailSentinel = (SkipNode*)malloc(sizeof(SkipNode));
    tailSentinel->left = tailSentinel->right = tailSentinel->below = NULL;

    headSentinel->isSentinel = true;
    headSentinel->val = 0;
    headSentinel->width = 0;

    tailSentinel->isSentinel = true;
    tailSentinel->val = 0;
    tailSentinel->width = 0;

    // link the sentinels 
    headSentinel->right = tailSentinel;
    tailSentinel->left = headSentinel;

    newLevel->headSentinel = headSentinel;
    newLevel->tailSentinel = tailSentinel;
    

    return newLevel;
} 

SkipList *_initSkipList(){
    SkipList *skip_list = malloc(sizeof(SkipList));

    // initalize level-related members
    skip_list->levels = (LevelList*)malloc(sizeof(LevelList));
    LevelList *levelList = skip_list->levels;
    levelList->bottom = levelList->top = _makeLevel();

    skip_list->nodeTopLevels = (LevelRecordsList*)malloc(sizeof(LevelRecordsList));
    LevelRecordsList *levelRecords = skip_list->nodeTopLevels;
    levelRecords->head = levelRecords->tail = NULL;
    
    // initialize values
    skip_list->leftmost = skip_list->rightmost = 0;
    skip_list->maxHeight = 1;
    skip_list->currHeight = 0;
    skip_list->size = 0;
    skip_list->is_reversed = false; // default : toggled off

    return skip_list;
}


// --------------------------------------------------------- >>
/* ----------------------------------------- <<

            ||-- HELPERS --||

<< ----------------------------------------- */
// --------------------------------------------------------- >>

Level *_promoteLevel(SkipList *l, SkipNode *sentinel, SkipNode *belowNode, DATA v, bool fromRight){
    Level *currLevel = l->levels->bottom;
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
        currLevel = currLevel->up;

        SkipNode *newPrevNode = _makeNode(v);
        SkipNode *sentinel = fromRight ? currLevel->tailSentinel : currLevel->headSentinel;

        // below
        newPrevNode->below = belowNode;
        belowNode = newPrevNode;

        if (!fromRight){
            
            // left
            newPrevNode->right = sentinel->right;
            sentinel->right->left = newPrevNode;
            sentinel->right = newPrevNode;
            newPrevNode->left = sentinel;
        }
        else {

            // right
            newPrevNode->left = sentinel->left;
            sentinel->left->right = newPrevNode;
            sentinel->left = newPrevNode;
            newPrevNode->right = sentinel;
        }
    }

    return currLevel;
}

void _demoteLevel(SkipList *l, Level *currLevel, bool fromRight){
    // update the height
    
    while (currLevel != NULL){
        SkipNode *sentinel = fromRight ? currLevel->tailSentinel : currLevel->headSentinel;
        SkipNode *currNode = fromRight ? sentinel->left : sentinel->right;
        SkipNode *currPrevNode = fromRight ? currNode->left : currNode->right;

        // Remove currNode from the level
        if (!fromRight){
            sentinel->right = currPrevNode;
            currPrevNode->left = sentinel;

        } else {
            sentinel->left = currPrevNode;
            currPrevNode->right = sentinel;
        }
        free(currNode);

        // If level is empty and is NOT Level 0, then remove it from the list
        if (currLevel != l->levels->bottom && (currLevel->headSentinel->right == currLevel->tailSentinel)){
            if (l->levels->top == currLevel){
                l->levels->top = currLevel->down;
            }
            // Update sentinel pointers
            Level *upLevel = currLevel->up;
            Level *downLevel = currLevel->down;

            downLevel->up = upLevel;
            if (upLevel != NULL){
                upLevel->headSentinel->below = downLevel->headSentinel;
                upLevel->tailSentinel->below = downLevel->tailSentinel;
                upLevel->down = currLevel->down;
            }

            free(currLevel->headSentinel);
            free(currLevel->tailSentinel);
            free(currLevel);

            l->currHeight--;
        }

        currLevel = currLevel->down;
    }
}

bool _flipCoin(){
    return rand() > RAND_MAX / 2;
}

SkipNode *_getNode(SkipList *l, LENGTH i){
    SkipList *skipList = l;
    SkipNode *header = skipList->levels->top->headSentinel; // Topmost headSentinel; Header
    LENGTH sumOffset = header->right->width; // initialize the offset for the topmost
    LENGTH target = i;   
    LENGTH sumRight = header->right->right ? header->right->right->width : 0;
    
    if (l->size > 0 && 0 <= target && target < l->size) return NULL; // bounds check
    
    while (header->isSentinel == true){
        if (header->right == NULL){
            header = header->below; // still a sentinel
            continue;
        }
        if (sumOffset <= target){  
            header = header->right; // not sentinel anymore
            break;
        }
        else { // if the leftmost SkipNode width >= target go down the headSentinel
            header = header->below;
        }
    }

    // can go down as long as header->below != NULL
    while (header != NULL){
        if (sumOffset == target){ //if matching index
            header = header->below;
            continue;
        }
        if (sumOffset + sumRight <= target){
            header = header->right;
            sumOffset += sumRight; // update the current offset
        }
        else {
            header = header->below; // exhaust go down until the same offset is found
        }
        sumRight = header->right->right ? header->right->right->width : 0;
    }

    return header; 

}

// --------------------------------------------------------- >>
/* ----------------------------------------- <<

            ||-- MAIN OPERATIONS --||

<< ----------------------------------------- */
// --------------------------------------------------------- >>


SkipList *make(LENGTH n, DATA *seq){
    // undefined behavior
    if (n < 0) return NULL;

    SkipList *sl = _initSkipList();
    if (n == 0) return sl;
    
    // traverse through the sequence
    for (LENGTH i = 0; i < n; i++){
        push_left(sl, seq[i]);
    }
    return sl;
}

void _push_left_base(SkipList *l, DATA v){
    // update leftmost widths
    Level* currLevel = l->levels->bottom; // start from level 0 
    while (currLevel != NULL){
        SkipNode* prevNode = currLevel->headSentinel->right;
        if (prevNode->isSentinel == false){
            prevNode->width++;
        }
        currLevel = currLevel->up;
    }

    // restart at level 0
    currLevel = l->levels->bottom;
    SkipNode *belowNode = _makeNode(v);
    SkipNode *sentinel = currLevel->headSentinel; // header

    // update left connection
    belowNode->right = sentinel->right;
    sentinel->right->left = belowNode;
    sentinel->right = belowNode;
    belowNode->left = sentinel;
    
    // update values
    l->size++;
    l->leftmost = v;
    if (l->size == 1) l->rightmost = v;
    l->maxHeight = _capHeight(l->size);
    
    // promote
    Level *maxLevelReached = _promoteLevel(l, sentinel, belowNode, v, false);
    LevelRecord *newRecord = (LevelRecord*) (malloc(sizeof(LevelRecord)));
    newRecord->prev = NULL;
    newRecord->next = l->nodeTopLevels->head;
    l->nodeTopLevels->head = newRecord;
    if (l->nodeTopLevels->tail == NULL){
        l->nodeTopLevels->tail = newRecord;
    }
    newRecord->topLevel = maxLevelReached;
}

void _push_right_base(SkipList *l, DATA v){
    // update rightmost widths
    Level* currLevel = l->levels->bottom; // start from level 0 
    while (currLevel != NULL){
        SkipNode* prevNode = currLevel->tailSentinel->left;
        if (prevNode->isSentinel == false){
            prevNode->width++;
        }
        currLevel = currLevel->up;
    }

    // restart at level 0
    currLevel = l->levels->bottom;
    SkipNode *belowNode = _makeNode(v);
    SkipNode *sentinel = currLevel->tailSentinel; // header

    // update right connection
    belowNode->left = sentinel->left;
    sentinel->left->right = belowNode;
    sentinel->left = belowNode;
    belowNode->right = sentinel;
    
    // update values
    l->size++;
    l->rightmost = v;
    if (l->size == 1) l->leftmost = v;
    l->maxHeight = _capHeight(l->size);

    Level *maxLevelReached = _promoteLevel(l, sentinel, belowNode, v, true);
    LevelRecord *newRecord = (LevelRecord*) (malloc(sizeof(LevelRecord)));
    newRecord->prev = l->nodeTopLevels->head;
    newRecord->next = NULL;
    l->nodeTopLevels->tail = newRecord;
    if (l->nodeTopLevels->head == NULL){
        l->nodeTopLevels->head = newRecord;
    }
    newRecord->topLevel = maxLevelReached;
}

bool _pop_left_base(SkipList *l){
    // cannot pop an empty list
    if (l->size == 0) return false;

    // update widths
    Level *currLevel = l->levels->bottom; // start from level 0
    while (currLevel != NULL){
        SkipNode* prevNode = currLevel->headSentinel->right;
        prevNode->width--; // decrement widths
        currLevel = currLevel->up;
    }

    // decrease the size
    l->size--;

    // remove leftmost node from list
    LevelRecord *currRecord = l->nodeTopLevels->head;
    LevelRecord *prevRecord = currRecord->next;
    Level *maxLevelReached = currRecord->topLevel;

    l->nodeTopLevels->head = prevRecord;
    if (prevRecord != NULL){
        prevRecord->prev = NULL;
    }
    if (l->nodeTopLevels->tail == currRecord){
        l->nodeTopLevels->tail = NULL;
    }
    free(currRecord);
    
    _demoteLevel(l, maxLevelReached, false);

    // single case
    if (l->size > 0){
        DATA leftmostVal = l->levels->bottom->headSentinel->right->val;
        l->leftmost = leftmostVal;
        if (l->size == 1){
            l->rightmost = leftmostVal;
        }
    }

    return true;
}

bool _pop_right_base(SkipList *l){
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

    
    // remove rightmost node from list
    LevelRecord *currRecord = l->nodeTopLevels->tail;
    LevelRecord *prevRecord = currRecord->prev;
    Level *maxLevelReached = currRecord->topLevel;

    l->nodeTopLevels->tail = prevRecord;
    if (prevRecord != NULL){
        prevRecord->next = NULL;
    }
    if (l->nodeTopLevels->head == currRecord){
        l->nodeTopLevels->head = NULL;
    }
    free(currRecord);
    
    _demoteLevel(l, maxLevelReached, true);

    // single case
    if (l->size > 0){
        DATA rightmostVal = l->levels->bottom->tailSentinel->left->val;
        l->rightmost = rightmostVal;
        if (l->size == 1){
            l->leftmost = rightmostVal;
        }
    }

    return true;
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

void push_left(SkipList *l, DATA v){
    l->is_reversed == false ? _push_left_base(l, v) : _push_right_base(l, v);
}
void push_right(SkipList *l, DATA v){
    l->is_reversed == false ? _push_right_base(l, v) : _push_left_base(l, v);
}
bool pop_left(SkipList *l){
    return l->is_reversed == false ? _pop_left_base(l) : _pop_right_base(l);
}
bool pop_right(SkipList *l){
    return l->is_reversed == false ? _pop_right_base(l) : _pop_left_base(l);
}

void display(SkipList *sl) {

    Level *currLevel = sl->levels->top;
    printf("Max Level: %d \n", sl->currHeight);
    printf("N: %d\n", sl->size);
    int currHeight = sl->currHeight;
    while (currLevel != NULL){
        // traverse right
        printf("Level %d: ", currHeight);
        SkipNode *curr = currLevel->headSentinel; // leftmost
        int currWidth = 0;
        while (curr != NULL){
            currWidth += curr->width;
            if (curr->isSentinel == false){
                int o = 0;
                while (o < curr->width){
                    if (o > 0){
                        printf("----- ");
                    }
                    printf("-> ");
                    o++;
                }
            } else if (curr == currLevel->tailSentinel){
                int o = currWidth;
                while (o < sl->size){
                    if (o > currWidth){
                        printf("----- ");
                    }
                    printf("-> ");
                    o++;
                }
            }
            if (curr == currLevel->headSentinel){
                printf("H -> ");
                if (curr->right->isSentinel == false){
                    if (curr->right->width != 0){
                        printf("| _ | ");
                    }
                }
            } else if (curr == currLevel->tailSentinel){
                printf("T");
            } else {
                printf("| %d | ", curr->width);
            }
            curr = curr->right;
        }
        currLevel = currLevel->down;
        currHeight--;
        printf("\n");
    }
    printf("LEVELS: ");
    LevelRecord *current = sl->nodeTopLevels->head;
    while (current != NULL){
        int count = -1;
        Level *curr2 = current->topLevel;
        while (curr2 != NULL){
            count++;
            curr2 = curr2->down;
        }
        printf("%d ", count);
        current = current->next;
    }
    printf("\n");
    
}

void reverse(SkipList *sl){
    sl->is_reversed = !(sl->is_reversed);
}

int main(){
    srand(time(NULL));
    int n = 10;
    DATA *array = (DATA*) malloc(n * sizeof(DATA));
    for (int i = 0; i < n; i++){
        array[i] = i;
    }
    SkipList *skip_list = make(n, array);
    display(skip_list);
    pop_left(skip_list);
    display(skip_list);
    pop_left(skip_list);
    display(skip_list);
    printf("%d ", get(skip_list, 2));

}