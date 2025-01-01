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
    return (LENGTH)ceil(log2(n)); // ceil up
}

bool _flipCoin(){
    return rand() > RAND_MAX / 2;
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

    newLevel->cachedRightWidth = 0;

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
    SkipList *skip_list = (SkipList*)malloc(sizeof(SkipList));

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

Level *_promoteLevel(SkipList *l, bool fromRight){
    Level *currLevel = l->levels->bottom;
    SkipNode *belowNode = fromRight ? currLevel->tailSentinel->left : currLevel->headSentinel->right;
    DATA v = belowNode->val;
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

            // if from right and there's an element to its left
            if (fromRight == true && l->size > 1){
                // update cached with to match entire list
                newLevel->cachedRightWidth = l->size-1;
            }
            
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
            newPrevNode->right = sentinel->right; // curr leftmost
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

            newPrevNode->width = currLevel->cachedRightWidth; // update the newPrevNode width
            currLevel->cachedRightWidth = 0; // reset cached with
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

SkipNode *_getNode(SkipList *l, LENGTH target, bool fromSet, DATA v){
    // bounds check
    if (l->size == 0) return NULL;
    if (!(0 <= target && target < l->size)) return NULL; // index out of range and negative index
   
    SkipNode *header = l->levels->top->headSentinel; // Topmost headSentinel; Header
    LENGTH sumOffset = 0; // initialize the offset for the topmost
    if (header == NULL) return NULL;
    if (header->right == NULL) return NULL;
    // sentinel phase
    while (header && header->isSentinel == true){
        // don't go right if it's tailSentinel
        if (header->right->isSentinel == true){
            header = header->below;
            continue;
        }
        LENGTH sumRight = header->right->width;
        // within index, can go right
        if (sumOffset + sumRight <= target){  
            sumOffset += sumRight;
            header = header->right; // not sentinel anymore
            break;
        }
        else { // if the leftmost SkipNode width >= target go down the headSentinel
            header = header->below;
        }
    }

    // can go down as long as header->below != NULL
    // Main traversal
    while (header) {
        if (sumOffset == target) {
            if (fromSet){
                header->val = v;
            }
            if (header->below == NULL) {
                return header;
            }
            header = header->below;
            continue;
        }

        if (header->right->isSentinel == false && header->right && sumOffset + header->right->width <= target) {
            sumOffset += header->right->width;
            header = header->right;
        } else {
            header = header->below;
        }
    }

    
    return header; 

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
    Level *maxLevelReached = _promoteLevel(l, false);
    LevelRecord *newRecord = (LevelRecord*) (malloc(sizeof(LevelRecord)));
    
    newRecord->prev = NULL;
    newRecord->next = l->nodeTopLevels->head;
    if (l->nodeTopLevels->head != NULL){
        l->nodeTopLevels->head->prev = newRecord;
    }
    l->nodeTopLevels->head = newRecord;
    if (l->nodeTopLevels->tail == NULL){
        l->nodeTopLevels->tail = newRecord;
    }
    newRecord->topLevel = maxLevelReached;

}

void _push_right_base(SkipList *l, DATA v){

    // start at level 0
    Level* currLevel = l->levels->bottom;
    SkipNode *belowNode = _makeNode(v);
    SkipNode *sentinel = currLevel->tailSentinel; // header
    sentinel->isSentinel = true;

    // update right connection
    belowNode->left = sentinel->left;
    sentinel->left->right = belowNode;
    sentinel->left = belowNode;
    belowNode->right = sentinel;
    
    // update values
    belowNode->width = currLevel->cachedRightWidth;
    currLevel->cachedRightWidth = 0;

    l->size++;
    l->rightmost = v;
    if (l->size == 1) l->leftmost = v;
    l->maxHeight = _capHeight(l->size);

    // Important: call after size++
    Level *maxLevelReached = _promoteLevel(l, true);
    LevelRecord *newRecord = (LevelRecord*) (malloc(sizeof(LevelRecord)));
    
    newRecord->next = NULL;
    newRecord->prev = l->nodeTopLevels->tail;
    if (l->nodeTopLevels->tail != NULL){
        l->nodeTopLevels->tail->next = newRecord;
    }
    l->nodeTopLevels->tail = newRecord;
    if (l->nodeTopLevels->head == NULL){
        l->nodeTopLevels->head = newRecord;
    }
    newRecord->topLevel = maxLevelReached;

    // update rightmost widths
    currLevel = l->levels->bottom; // start from level 0 
    while (currLevel != NULL){
        currLevel->cachedRightWidth++;
        currLevel = currLevel->up;
    }
}

bool _pop_left_base(SkipList *l){
    // cannot pop an empty list
    if (l->size == 0) return false;

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

    // update widths
    Level *currLevel = l->levels->bottom; // start from level 0
    while (currLevel != NULL){
        SkipNode* prevNode = currLevel->headSentinel->right;
        prevNode->width--; // decrement widths
        currLevel = currLevel->up;
    }

    // update leftmost/rightmost
    if (l->size > 0){
        DATA leftmostVal = l->levels->bottom->headSentinel->right->val;
        l->leftmost = leftmostVal;
        if (l->size == 1){
            l->rightmost = leftmostVal;
        }
    }

    // decrease the size
    l->size--;
    l->maxHeight = _capHeight(l->size);


    return true;
}

bool _pop_right_base(SkipList *l){
    // cannot pop an empty list
    if (l->size == 0) return false;

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

    // update widths
    Level* currLevel = l->levels->bottom; // start from level 0 
    while (currLevel != NULL){
        currLevel->cachedRightWidth = currLevel->tailSentinel->left->width;
        currLevel->cachedRightWidth--;
        currLevel = currLevel->up;
    }

    // Demote that node
    _demoteLevel(l, maxLevelReached, true);

    // update leftmost/rightmost
    if (l->size > 0){
        DATA rightmostVal = l->levels->bottom->tailSentinel->left->val;
        l->rightmost = rightmostVal;
        if (l->size == 1){
            l->rightmost = rightmostVal;
        }
    }

    // decrease the size
    l->size--;
    l->maxHeight = _capHeight(l->size);


    return true;
}

// --------------------------------------------------------- >>
/* ----------------------------------------- <<

            ||-- MAIN OPERATIONS --||

<< ----------------------------------------- */
// --------------------------------------------------------- >>


SkipList *make(LENGTH n, DATA *seq){
    // undefined behavior
    srand(time(NULL));
    SkipList *sl = _initSkipList();
    if (n <= 0) return sl;
    
    // traverse through the sequence
    for (LENGTH i = 0; i < n; i++){
        push_right(sl, seq[i]);
    }
    return sl;
}

void reverse(SkipList *sl){
    sl->is_reversed = !(sl->is_reversed);
}

LENGTH size(SkipList *l){
    return l->size;
}

bool empty(SkipList *l){
    return (l->size == 0);
}

DATA get(SkipList *l, LENGTH i){
    if (l->size == 0) return 0;
    // if (i == l->size-1) return peek_right(l);
    SkipNode *retNode = _getNode(l, l->is_reversed == false ? i : l->size-1-i, false, 0);
    // if (retNode == NULL) return 0;

    return retNode->val;
}

void set(SkipList *l, LENGTH i, DATA v){
    if (l->size == 0) return;
    if (!(0 <= i && i < l->size)) return; // SkipNode doesn't exist, error
    LENGTH index = l->is_reversed == false ? i : l->size-1-i;
    SkipNode *retNode = _getNode(l, index, true, v);
    retNode->val = v;
    if (index == 0){
        l->leftmost = v;
    }
    if (index == l->size-1){
        l->rightmost = v;
    }
}

DATA peek_left(SkipList *l){
    if (l->size == 0) return 0;
    return l->is_reversed ? l->rightmost : l->leftmost;
}

DATA peek_right(SkipList *l){
    if (l->size == 0) return 0;
    return l->is_reversed ? l->leftmost : l->rightmost;
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

void displayWidth(SkipList *sl) {

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
                        printf("----- ");
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

void displayNodes(SkipList *sl) {
    Level *currLevel = sl->levels->top;
    LENGTH height = sl->currHeight;
    while(currLevel){
        printf("Level %ld: ", height--);
        SkipNode *curr = currLevel->headSentinel;
        while (curr){
            printf("(%lld) <-> ", curr->val);
            curr = curr->right;
        }
        printf("\n");
        currLevel = currLevel->down;
    }

}

// void displayNodes(SkipList *sl) {
//     if (sl == NULL || sl->levels == NULL || sl->levels->top == NULL) {
//         printf("Empty skip list\n");
//         return;
//     }

//     Level *currLevel = sl->levels->top;
//     int currHeight = sl->currHeight;  // Use actual current height instead of maxHeight
    
//     while (currLevel != NULL) {
//         printf("Level %d: ", currHeight--);
//         SkipNode *curr = currLevel->headSentinel;
        
//         while (curr != NULL) {
//             if (curr->isSentinel) {
//                 printf("[%s]", curr == currLevel->headSentinel ? "HEAD" : "TAIL");
//             } else {
//                 printf("(%ld)", curr->val);
//             }
            
//             if (curr->right != NULL) {
//                 printf(" <-> ");
//             }
//             curr = curr->right;
//         }
//         printf("\n");
//         currLevel = currLevel->down;
//     }
// }

// --------------------------------------------------------- >>
/* ----------------------------------------- <<

            ||-- TEST OPERATIONS --||

<< ----------------------------------------- */
// --------------------------------------------------------- >>


void TEST_elements(SkipList* l, LENGTH* nRef, DATA** seqRef){   
    LENGTH i = 0;
    SkipNode *header = l->levels->bottom->headSentinel->right;
    DATA *seq = (DATA*)malloc((l->size) * sizeof(DATA));
    while (header != l->levels->bottom->tailSentinel){
        seq[i] = header->val;
        header = header->right;
        i++;
    }
    *nRef = l->size;
    *seqRef = seq;
}

bool TEST_internal(SkipList* l){
    return true;
}

bool TEST_reversed(SkipList* l){
  return (l->is_reversed);
}

// int main(){
//     srand(time(NULL));
//     int n = 4;
//     DATA *array = (DATA*) malloc(n * sizeof(DATA));
//     array[0] = -1;
//     array[1] = -2;
//     array[2] = -3;
//     array[3] = -4;
//     SkipList *skip_list = make(n, array);
//     displayWidth(skip_list);
//     _getNode(skip_list, 3, false, 0);
//     // printf("%lld : %lld : %lld ", skip_list->size, _getNode(skip_list, 3, false, 0), peek_right(skip_list))5;
//     // peek_left(skip_list);
//     // printf("%d ", get(skip_list, 6));
//     // get(skip_list, 0);
// //     displayWidth(skip_list);
// }