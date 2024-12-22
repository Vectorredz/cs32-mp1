#include <stdio.h>


#include <stdlib.h>
#include <inttypes.h>
#include <stdbool.h>
#include <math.h>


// --------------------------------------------------------- >>
/* ----------------------------------------- <<

            ||-- TYPEDEFS --||

<< ----------------------------------------- */
// --------------------------------------------------------- >>

typedef int DATA;
typedef int LENGTH;

typedef struct _PTreeNode {
    bool leaf;
    union {
        // Leaf Node (Contains Data)
        struct {
            LENGTH index;
            DATA data;
        };

        // Non-Leaf Node (Contains Index Segment)
        struct {
            LENGTH lower;
            LENGTH upper;
        };
    };
} PTreeNode;
typedef struct _PTree {
    LENGTH c;
    LENGTH l;
    LENGTH k;
    PTreeNode** nodes;
} PTree;

typedef struct _PTreeListNode {
    struct _PTreeListNode* next;
    struct _PTreeListNode* prev;
    PTree* ptree;
} PTreeListNode;
typedef struct _PTreeList {
    LENGTH n;
    PTreeListNode* head;
    PTreeListNode* tail;
    bool reverse;
} PTreeList;

// --------------------------------------------------------- >>
/* ----------------------------------------- <<

            ||-- HELPERS --||

<< ----------------------------------------- */
// --------------------------------------------------------- >>

void _getGreatestPowerOfTwo(LENGTH num, LENGTH* powRef, LENGTH* kRef){
    int k = 0;
    int pow = 1;
    while (pow < num){
        int new = pow << 1;
        if (new > num){
            break;
        }
        k++;
        pow = new;
    }
    *powRef = pow;
    *kRef = k;
}

void _constructPTreeNode(DATA* seq, PTreeNode** nodes, LENGTH index, LENGTH lower, LENGTH upper){
    PTreeNode* newTreeNode = (PTreeNode*) malloc(sizeof(PTreeNode));
    printf("%d\n", upper-lower+1);
    if ((upper-lower+1) == 1){
        newTreeNode->leaf = true;
        newTreeNode->index = lower;
        newTreeNode->data = seq[lower];
        printf("TEST: %d, %d\n", newTreeNode->index, seq[lower]);

    } else {
        newTreeNode->leaf = false;
        newTreeNode->lower = lower;
        newTreeNode->upper = upper;

        LENGTH mid = (upper+lower)/2;
        printf("Segment: %d, %d\n", lower, upper);
        printf("%d, MID: %d, %d\n", lower, mid, upper);
        _constructPTreeNode(seq, nodes, 2*index + 1, lower, mid);
        _constructPTreeNode(seq, nodes, 2*index + 2, mid+1, upper);
    }

    nodes[index] = newTreeNode;
}
void _constructPTreesFromRange(DATA* seq, LENGTH lower, LENGTH upper, PTreeListNode** headRef, PTreeListNode** tailRef){
    PTreeListNode* head = NULL;
    PTreeListNode* tail = NULL;
    
    LENGTH currentStart = lower;
    while (currentStart <= upper){
        printf("-------------\n");
        LENGTH l, k;
        _getGreatestPowerOfTwo((upper-currentStart) + 1, &l, &k);
        printf("%d, %d, %d, %d, %d\n", (upper-lower)+1, k, l, currentStart, upper);

        PTree* ptree = (PTree*) malloc(sizeof(PTree));
        ptree->c = (1 << (k+1)) - 1;
        ptree->l = l;
        ptree->k = k;
        ptree->nodes = (PTreeNode**) malloc((ptree->c)*sizeof(PTreeNode*));
        _constructPTreeNode(seq, ptree->nodes, 0, currentStart, currentStart+l-1);

        PTreeListNode* newPTreeListNode = (PTreeListNode*) malloc(sizeof(PTreeListNode));
        newPTreeListNode->ptree = ptree;
        if (head == NULL){
            newPTreeListNode->prev = NULL;
            head = newPTreeListNode;
        } else {
            tail->next = newPTreeListNode;
            newPTreeListNode->prev = tail;
        }
        newPTreeListNode->next = NULL;
        tail = newPTreeListNode;

        currentStart += l;
    }

    *headRef = head;
    *tailRef = tail;
}
PTreeNode* _getPTreeNodeAtIndex(PTreeList* list, LENGTH i){
    if (list->n == 0){
        return NULL;
    }
    PTreeListNode* currentListNode = list->head;
    while (currentListNode != NULL){
        PTree* ptree = currentListNode->ptree;
        PTreeNode** nodes = ptree->nodes;
        
        PTreeNode* root = nodes[0];
        if (root->leaf == true){
            printf("???: %d, %d\n", root->index, i);
            if (root->index == i){
                return root;
            } else {
                currentListNode = currentListNode->next;
                continue;
            }
        } else {
            if (!(root->lower <= i && i <= root->upper)){
                currentListNode = currentListNode->next;
                continue;
            }
        }

        LENGTH currentIndex = 0;
        while (currentIndex < ptree->c){
            printf("Wtf\n");
            PTreeNode* leftChild = nodes[2*currentIndex + 1];
            PTreeNode* rightChild = nodes[2*currentIndex + 2];
            if (leftChild->leaf == true){
                printf("INDEX: %d\n", leftChild->lower);
                if (leftChild->index == i){
                    return leftChild;
                } else {
                    return rightChild;
                }
            } else {
                if (leftChild->lower <= i && i <= leftChild->upper) {
                    currentIndex = 2*currentIndex + 1;
                } else {
                    currentIndex = 2*currentIndex + 2;
                }
            }
        }

        currentListNode = currentListNode->next;
    }
    return NULL;
}

// --------------------------------------------------------- >>
/* ----------------------------------------- <<

            ||-- MAIN OPERATIONS --||

<< ----------------------------------------- */
// --------------------------------------------------------- >>


PTreeList* make(DATA* seq, LENGTH n){
    PTreeList* list = (PTreeList*) malloc(sizeof(PTreeList));

    PTreeListNode* head = NULL;
    PTreeListNode* tail = NULL;
    _constructPTreesFromRange(seq, 0, n-1, &head, &tail);

    list->n = n;
    list->head = head;
    list->tail = tail;
    list->reverse = false;
    return list;
}


LENGTH size(PTreeList* list){
    return list->n;
}
bool empty(PTreeList* list){
    return size(list) == 0;
}

DATA get(PTreeList* list, LENGTH i){
    return _getPTreeNodeAtIndex(list, list->reverse == false ? i : (list->n)-1-i)->data;
}
void set(PTreeList* list, LENGTH i, DATA v){
    _getPTreeNodeAtIndex(list, list->reverse == false ? i : (list->n)-1-i)->data = v;
}
DATA peek_left(PTreeList* list){
    return get(list, 0);
}
DATA peek_right(PTreeList* list){
    return get(list, (list->n)-1);
}

void reverse(PTreeList* list){
    list->reverse = !(list->reverse);
}


int main(){
    // TODO: REVERT TYPEDEFS TO INT64_T AND SIZE_T
    int n = 17;
    DATA seq[17] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17};
    PTreeList* list = make(seq, n);
    printf("Pass.");
    reverse(list);
    for (int i = 0; i < n; i++){
        printf("Yeah: %d, %d\n", get(list, i), _getPTreeNodeAtIndex(list, i)->index);
    }
    printf("LEFT: %d\n", peek_left(list));
    printf("RIGHT: %d\n", peek_right(list));
}

