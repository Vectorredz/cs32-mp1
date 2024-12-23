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
    LENGTH c; // number of nodes
    LENGTH l; // number of leaf nodes
    LENGTH k; // power of two
    PTreeNode** nodes;
} PTree;

typedef struct _PTreeListNode {
    PTree* ptree;
    struct _PTreeListNode* next;
    struct _PTreeListNode* prev;
} PTreeListNode;
typedef struct _PTreeList {
    LENGTH n; // number of leaf nodes across all trees
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

void _constructPTreeNodesFromRange(DATA* seq, LENGTH seqIndexOffset, PTreeNode** nodes, LENGTH index, LENGTH lower, LENGTH upper){
    PTreeNode* newTreeNode = (PTreeNode*) malloc(sizeof(PTreeNode));
    //printf("%d\n", upper-lower+1);
    if ((upper-lower+1) == 1){
        newTreeNode->leaf = true;
        newTreeNode->index = lower;
        newTreeNode->data = seq[lower+seqIndexOffset];
        // printf("TEST: %d, %d\n", newTreeNode->index, seq[lower+seqIndexOffset]);

    } else {
        newTreeNode->leaf = false;
        newTreeNode->lower = lower;
        newTreeNode->upper = upper;

        LENGTH mid = (upper+lower)/2;
        // printf("Segment: %d, %d\n", lower, upper);
        // printf("%d, MID: %d, %d\n", lower, mid, upper);
        _constructPTreeNodesFromRange(seq, seqIndexOffset, nodes, 2*index + 1, lower, mid);
        _constructPTreeNodesFromRange(seq, seqIndexOffset, nodes, 2*index + 2, mid+1, upper);
    }

    nodes[index] = newTreeNode;
}
void _constructPTreesFromRange(DATA* seq, LENGTH lower, LENGTH upper, PTreeListNode** headRef, PTreeListNode** tailRef){
    PTreeListNode* head = NULL;
    PTreeListNode* tail = NULL;
    
    LENGTH currentStart = lower;
    while (currentStart <= upper){
        //printf("-------------\n");
        LENGTH l, k;
        _getGreatestPowerOfTwo((upper-currentStart) + 1, &l, &k);
        //printf("%d, %d, %d, %d, %d\n", (upper-lower)+1, k, l, currentStart, upper);

        PTree* ptree = (PTree*) malloc(sizeof(PTree));
        ptree->c = (1 << (k+1)) - 1;
        ptree->l = l;
        ptree->k = k;
        ptree->nodes = (PTreeNode**) malloc((ptree->c)*sizeof(PTreeNode*));
        _constructPTreeNodesFromRange(seq, currentStart, ptree->nodes, 0, 0, l-1);

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
    LENGTH offset = 0;
    while (currentListNode != NULL){
        PTree* ptree = currentListNode->ptree;
        PTreeNode** nodes = ptree->nodes;
        
        PTreeNode* root = nodes[0];
        if (root->leaf == true){
            //printf("???: %d, %d\n", root->index, i);
            if (root->index + offset == i){
                return root;
            } else {
                offset++;
                currentListNode = currentListNode->next;
                continue;
            }
        } else {
            if (!(root->lower + offset <= i && i <= root->upper + offset)){
                offset += root->upper + 1;
                currentListNode = currentListNode->next;
                continue;
            }
        }

        LENGTH currentIndex = 0;
        while (currentIndex < ptree->c){
            //printf("Wtf\n");
            PTreeNode* leftChild = nodes[2*currentIndex + 1];
            PTreeNode* rightChild = nodes[2*currentIndex + 2];
            if (leftChild->leaf == true){
                //printf("INDEX: %d\n", leftChild->lower);
                if (leftChild->index + offset == i){
                    return leftChild;
                } else {
                    return rightChild;
                }
            } else {
                if (leftChild->lower + offset <= i && i <= leftChild->upper + offset) {
                    currentIndex = 2*currentIndex + 1;
                } else {
                    currentIndex = 2*currentIndex + 2;
                }
            }
        }

        offset += root->upper + 1;
        currentListNode = currentListNode->next;
    }
    return NULL;
}


PTree* _constructZeroPTree(DATA v){
    PTreeNode* newTreeNode = (PTreeNode*) malloc(sizeof(PTreeNode));
    newTreeNode->leaf = true;
    newTreeNode->index = 0;
    newTreeNode->data = v;

    PTree* ptree = (PTree*) malloc(sizeof(PTree));
    ptree->c = 1;
    ptree->l = 1;
    ptree->k = 0;
    ptree->nodes = (PTreeNode**) malloc((ptree->c)*sizeof(PTreeNode*));
    (ptree->nodes)[0] = newTreeNode;

    return ptree;
}

void _mergeNonDistinctPTrees(PTreeList* list, bool fromRight){
    if (list->n <= 1){
        return;
    }
    PTreeListNode* currentListNode = fromRight == false ? list->head : list->tail;
    PTreeListNode* nextListNode = fromRight == false ? currentListNode->next : currentListNode->prev;
    while (currentListNode != NULL && nextListNode != NULL){
        printf("-----------------------");
        PTree* leftPTree = currentListNode->ptree;
        PTree* rightPTree = nextListNode->ptree;
        
        if (leftPTree->k != rightPTree->k){
            break;
        }

        // Construct PTree with the two roots as children
        PTree* ptree = (PTree*) malloc(sizeof(PTree));
        ptree->c = leftPTree->c + leftPTree->c + 1;
        ptree->l = leftPTree->l + rightPTree->l;
        ptree->k = leftPTree->k + 1;
        ptree->nodes = (PTreeNode**) malloc((ptree->c)*sizeof(PTreeNode*));
        printf("PTREE: %d, %d, %d\n", ptree->c, ptree->l, ptree->k);
        DATA* newSeq = (DATA*) malloc((ptree->l)*sizeof(DATA));
        LENGTH index = 0;
        for (LENGTH i = (leftPTree->c)-(leftPTree->l); i < (leftPTree->c); i++){
            newSeq[index] = ((leftPTree->nodes)[i])->data;
            printf("Test: %d, %d\n", newSeq[index], i);
            index++;
        }
        for (LENGTH i = (rightPTree->c)-(rightPTree->l); i < (rightPTree->c); i++){
            newSeq[index] = ((rightPTree->nodes)[i])->data;
            printf("Test2: %d, %d\n", newSeq[index], i);
            index++;
        }
        printf("The fuck?\n");
        printf("Wtf?!?\n");
        for (LENGTH i = 0; i < ptree->l; i++){
            printf("What %d, %d\n", newSeq[i], ptree->l);
        }
        _constructPTreeNodesFromRange(newSeq, 0, ptree->nodes, 0, 0, ptree->l-1);
        printf("FINAL\n");
        for (LENGTH i = 0; i < ptree->c; i++){
            if ((ptree->nodes)[i]->leaf == false){
                printf("FINAL??: %d, %d\n", (ptree->nodes)[i]->lower, (ptree->nodes)[i]->upper);
            } else {
                printf("FINAL: %d\n", (ptree->nodes)[i]->data);
            }
        }


        // Add to list and link neighbors
        PTreeListNode* newListNode = (PTreeListNode*) malloc(sizeof(PTreeListNode));
        newListNode->ptree = ptree;
        newListNode->prev = fromRight == false ? currentListNode->prev : nextListNode->prev;
        newListNode->next = fromRight == false ? nextListNode->next  : currentListNode->next;
        if (currentListNode == list->head || nextListNode == list->head){
            list->head = newListNode;
            printf("Yeahzz\n");
        }
        if (currentListNode == list->tail || nextListNode == list->tail){
            list->tail = newListNode;
        }
        if (newListNode->prev != NULL){
            newListNode->prev->next = newListNode;
        }
        if (newListNode->next != NULL){
            newListNode->next->prev = newListNode;
        }

        printf("K: %d\n", (nextListNode->ptree->nodes)[0]->lower);
        currentListNode = newListNode;
        if (currentListNode != NULL){
            nextListNode = fromRight == false ? currentListNode->next : currentListNode->prev;
        }
    }
}


void _push_left_base(PTreeList* list, DATA v){
    // Construct a PTree of type 0 with the new data as root (also leaf)
    PTree* ptree = _constructZeroPTree(v);

    // Add to left of list
    PTreeListNode* newListNode = (PTreeListNode*) malloc(sizeof(PTreeListNode));
    newListNode->ptree = ptree;
    newListNode->prev = NULL;
    newListNode->next = list->head;
    list->head = newListNode;
    (list->n)++;

    // Fix non-distinct types
    _mergeNonDistinctPTrees(list, false);
}
void _push_right_base(PTreeList* list, DATA v){
    // Construct a PTree of type 0 with the new data as root (which is also a leaf)
    PTree* ptree = _constructZeroPTree(v);

    // Add to right of list
    PTreeListNode* newListNode = (PTreeListNode*) malloc(sizeof(PTreeListNode));
    newListNode->ptree = ptree;
    newListNode->next = NULL;
    newListNode->prev = list->tail;
    list->tail = newListNode;
    (list->n)++;

    // Fix non-distinct types
    _mergeNonDistinctPTrees(list, true);
}
bool _pop_left_base(PTreeList* list){
    if (list->n == 0){
        return false;
    }

    PTree* leftPTree = list->head->ptree;
    if (leftPTree->k == 0){
        list->head = list->head->next;
        (list->n)--;
        return true;
    }

    DATA* newSeq = (DATA*) malloc((leftPTree->l-1)*sizeof(DATA));
    LENGTH index = 0;
    for (LENGTH i = (leftPTree->c)-(leftPTree->l)+1; i < (leftPTree->c); i++){
        newSeq[index] = ((leftPTree->nodes)[i])->data;
        index++;
    }
}
bool _pop_right_base(PTreeList* list){
    if (list->n == 0){
        return false;
    }
    return true;
}



// --------------------------------------------------------- >>
/* ----------------------------------------- <<

            ||-- MAIN OPERATIONS --||

<< ----------------------------------------- */
// --------------------------------------------------------- >>

// Init
PTreeList* make(LENGTH n, DATA* seq){
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

// Flags/List Info
void reverse(PTreeList* list){
    list->reverse = !(list->reverse);
}
LENGTH size(PTreeList* list){
    return list->n;
}
bool empty(PTreeList* list){
    return size(list) == 0;
}

// Getters/Setters
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

// Insertions/Deletions
void push_left(PTreeList* list, DATA v){
    list->reverse == false ? _push_left_base(list, v) : _push_right_base(list, v);
}
void push_right(PTreeList* list, DATA v){
    list->reverse == false ? _push_right_base(list, v) : _push_left_base(list, v);
}
bool pop_left(PTreeList* list){
    return list->reverse == false ? _pop_left_base(list) : _pop_right_base(list);
}
bool pop_right(PTreeList* list){
    return list->reverse == false ? _pop_right_base(list) : _pop_left_base(list);
}





int main(){
    // TODO: REVERT TYPEDEFS TO INT64_T AND SIZE_T
    // TODO: free memory
    int n = 17;
    DATA seq[17] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17};
    PTreeList* list = make(n, seq);
    printf("Pass.");
    reverse(list);
    for (int i = 0; i < n; i++){
        printf("Yeah: %d, %d\n", get(list, i), _getPTreeNodeAtIndex(list, i)->index);
    }
    printf("LEFT: %d\n", peek_left(list));
    printf("RIGHT: %d\n", peek_right(list));

    // DATA seq1[4] = {1, 2, 3, 4};
    // LENGTH l1, k1;
    // _getGreatestPowerOfTwo(4, &l1, &k1);

    // PTree* ptree1 = (PTree*) malloc(sizeof(PTree));
    // ptree1->c = (1 << (k1+1)) - 1;
    // ptree1->l = l1;
    // ptree1->k = k1;
    // ptree1->nodes = (PTreeNode**) malloc((ptree1->c)*sizeof(PTreeNode*));
    // _constructPTreeNodesFromRange(seq1, 0, ptree1->nodes, 0, 0, 3);

    // DATA seq2[4] = {5, 6, 7, 8};
    // LENGTH l2, k2;
    // _getGreatestPowerOfTwo(4, &l2, &k2);

    // PTree* ptree2 = (PTree*) malloc(sizeof(PTree));
    // ptree2->c = (1 << (k2+1)) - 1;
    // ptree2->l = l2;
    // ptree2->k = k2;
    // ptree2->nodes = (PTreeNode**) malloc((ptree2->c)*sizeof(PTreeNode*));
    // _constructPTreeNodesFromRange(seq2, 0, ptree2->nodes, 0, 0, 3);

    // DATA seq3[8] = {9, 10, 11, 12, 13, 14, 15, 16};
    // LENGTH l3, k3;
    // _getGreatestPowerOfTwo(8, &l3, &k3);

    // PTree* ptree3 = (PTree*) malloc(sizeof(PTree));
    // ptree3->c = (1 << (k3+1)) - 1;
    // ptree3->l = l3;
    // ptree3->k = k3;
    // ptree3->nodes = (PTreeNode**) malloc((ptree3->c)*sizeof(PTreeNode*));
    // _constructPTreeNodesFromRange(seq3, 0, ptree3->nodes, 0, 0, 7);

    // PTreeList* list = (PTreeList*) malloc(sizeof(PTreeList));
    // list->n = 16;
    // list->reverse = false;
    
    // PTreeListNode* a = (PTreeListNode*) malloc(sizeof(PTreeListNode));
    // a->prev = NULL;
    // a->ptree = ptree1;

    // PTreeListNode* b = (PTreeListNode*) malloc(sizeof(PTreeListNode));
    // b->prev = a;
    // a->next = b;
    // b->ptree = ptree2;

    // PTreeListNode* c = (PTreeListNode*) malloc(sizeof(PTreeListNode));
    // c->next = NULL;
    // c->prev = b;
    // b->next = c;
    // c->ptree = ptree3;

    // list->head = a;
    // list->tail = c;

    // _mergeNonDistinctPTrees(list, false);
    // printf("??\n");
    // PTreeListNode* currentListNode = list->head;
    // while (currentListNode != NULL){
    //     PTree* ptree = currentListNode->ptree;
    //     printf("KK: %d\n", ptree->k);
    //     for (LENGTH i = ptree->c-ptree->l; i < ptree->c; i++){
    //         printf("%d\n", (ptree->nodes)[i]->data);
    //     }
    //     currentListNode = currentListNode->next;
    //     printf("-------------------\n");
    // }
}

