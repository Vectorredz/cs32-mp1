#include <stdio.h>


#include <stdlib.h>
#include <inttypes.h>
#include <stdbool.h>
#include <math.h>

/*
<< ------ ||
// --------------------------------------------------------- >>
            --//======================================================\\--
            --|| DYNAMIC ARRAY: SEQUENCE OF PERFECT BINARY TREES
            --\\======================================================//--
// --------------------------------------------------------- >>
<< ------ ||
*/

/*
Perfect Binary Tree / Phantom Segment Tree
    The Perfect Binary Trees are represented by the struct PTree, which is a modified
    Segment Tree-esque data structure with a special property of having implicit bounds.
    For this reason, I also like to call it a Phantom Segment Tree, as the bounds are
    only revealed once the list and trees are traversed with get/set operations.

*/





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
            DATA data;
        };

        // Non-Leaf Node (Contains Index Segment)
        struct {
            struct _PTreeNode* left;
            struct _PTreeNode* right;
        };
    };
} PTreeNode;
typedef struct _PTree {
    LENGTH l; // number of leaf nodes
    LENGTH k; // power of two
    PTreeNode* root;
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

void _printLeaves(PTreeNode* node){
    if (node->leaf == false){
        _printLeaves(node->left);
    }
    if (node->leaf == true){
        printf("%d\n", node->data);
        return;
    }
    if (node->leaf == false){
        _printLeaves(node->right);
    }
}

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

PTreeNode* _constructPTreeNodesFromRange(DATA* seq, LENGTH seqIndexOffset, LENGTH lowerBound, LENGTH upperBound){
    PTreeNode* newTreeNode = (PTreeNode*) malloc(sizeof(PTreeNode));
    
    if ((upperBound-lowerBound+1) == 1){
        newTreeNode->leaf = true;
        newTreeNode->data = seq[lowerBound+seqIndexOffset];

    } else {
        newTreeNode->leaf = false;

        LENGTH mid = (upperBound+lowerBound)/2;
        newTreeNode->left = _constructPTreeNodesFromRange(seq, seqIndexOffset, lowerBound, mid);
        newTreeNode->right = _constructPTreeNodesFromRange(seq, seqIndexOffset, mid+1, upperBound);
    }
    
    return newTreeNode;
}


void _constructPTreesFromRange(DATA* seq, LENGTH lowerBound, LENGTH upperBound, PTreeListNode** headRef, PTreeListNode** tailRef){
    PTreeListNode* head = NULL;
    PTreeListNode* tail = NULL;
    
    LENGTH currentStart = lowerBound;
    while (currentStart <= upperBound){
        LENGTH l, k;
        _getGreatestPowerOfTwo((upperBound-currentStart) + 1, &l, &k);

        PTree* ptree = (PTree*) malloc(sizeof(PTree));
        ptree->l = l;
        ptree->k = k;
        ptree->root = _constructPTreeNodesFromRange(seq, currentStart, 0, l-1);

        PTreeListNode* newListNode = (PTreeListNode*) malloc(sizeof(PTreeListNode));
        newListNode->ptree = ptree;
        if (head == NULL){
            head = newListNode;
        }
        if (tail != NULL){
            tail->next = newListNode;
        }
        newListNode->prev = tail;
        newListNode->next = NULL;
        tail = newListNode;

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
    printf("----------------------------------\nDesired Index: %d\n", i);
    while (currentListNode != NULL){
        printf("----------------\n");

        PTree* ptree = currentListNode->ptree;
        PTreeNode* currentTreeNode = ptree->root;

        LENGTH lowerBound = offset;
        LENGTH upperBound = offset + ptree->l - 1;

        if (currentTreeNode->leaf == true){
            if (lowerBound == i){
                return currentTreeNode;
            } else {
                offset++;
                currentListNode = currentListNode->next;
                continue;
            }
            
        } else {
            if (!(lowerBound <= i && i <= upperBound)){
                offset += ptree->l;
                currentListNode = currentListNode->next;
                continue;
            }
        }

        while (currentTreeNode->leaf == false){
            PTreeNode* leftChild = currentTreeNode->left;
            PTreeNode* rightChild = currentTreeNode->right;

            if (leftChild->leaf == true){
                if (lowerBound == i){
                    return leftChild;
                } else {
                    return rightChild;
                }

            } else {
                printf("%d, %d\n", lowerBound, upperBound);
                LENGTH mid = (upperBound+lowerBound)/2;
                if (lowerBound <= i && i <= mid) {
                    upperBound = mid;
                    currentTreeNode = leftChild;
                } else {
                    lowerBound = mid + 1;
                    currentTreeNode = rightChild;
                }
            }
        }

        offset += ptree->l;
        currentListNode = currentListNode->next;
    }
    return NULL;
}

PTree* _constructZeroPTree(DATA v){
    PTreeNode* newTreeNode = (PTreeNode*) malloc(sizeof(PTreeNode));
    newTreeNode->leaf = true;
    newTreeNode->data = v;

    PTree* ptree = (PTree*) malloc(sizeof(PTree));
    ptree->l = 1;
    ptree->k = 0;
    ptree->root = newTreeNode;

    return ptree;
}

void _mergeNonDistinctPTrees(PTreeList* list, PTreeListNode* startNode, bool traverseLeft){
    if (list->n <= 1){
        return;
    }

    PTreeListNode* currentListNode = startNode;
    PTreeListNode* nextListNode = traverseLeft == false ? currentListNode->next : currentListNode->prev;
    while (currentListNode != NULL && nextListNode != NULL){
        printf("--------------------------\n");
        PTree* leftPTree = currentListNode->ptree;
        PTree* rightPTree = nextListNode->ptree;

        if (leftPTree->k != rightPTree->k){
            currentListNode = nextListNode;
            nextListNode = traverseLeft == false ? currentListNode->next : currentListNode->prev;
            continue;
        }

        // Construct PTree with the two roots as children
        PTreeNode* root = (PTreeNode*) malloc(sizeof(PTreeNode));
        root->leaf = false;
        root->left = leftPTree->root;
        root->right = rightPTree->root;
        _printLeaves(leftPTree->root);
        printf("----\n");
        _printLeaves(rightPTree->root);

        PTree* ptree = (PTree*) malloc(sizeof(PTree));
        ptree->l = leftPTree->l + rightPTree->l;
        ptree->k = leftPTree->k + 1;
        ptree->root = root;

        // Add to list and link neighbors
        PTreeListNode* newListNode = (PTreeListNode*) malloc(sizeof(PTreeListNode));
        newListNode->ptree = ptree;
        newListNode->prev = traverseLeft == false ? currentListNode->prev : nextListNode->prev;
        newListNode->next = traverseLeft == false ? nextListNode->next  : currentListNode->next;
        if (currentListNode == list->head || nextListNode == list->head){
            list->head = newListNode;
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

        currentListNode = newListNode;
        if (currentListNode != NULL){
            nextListNode = traverseLeft == false ? currentListNode->next : currentListNode->prev;
        }
    }
}

void _cascadeRemoval(PTree* ptree, PTreeListNode** headRef, PTreeListNode** tailRef){
    PTreeListNode* head = NULL;
    PTreeListNode* tail = NULL;

    LENGTH currentL = ptree->l;
    LENGTH currentK = ptree->k;
    PTreeNode* currentTreeNode = ptree->root;
    while (currentTreeNode != NULL){
        if (currentTreeNode->leaf == true){
            currentTreeNode = NULL;

        } else {
            PTree* rightPTree = (PTree*) malloc(sizeof(PTree));
            rightPTree->l = currentL/2;
            rightPTree->k = currentK-1;
            rightPTree->root = currentTreeNode->right;
            
            PTreeListNode* newListNode = (PTreeListNode*) malloc(sizeof(PTreeListNode));
            newListNode->ptree = rightPTree;
            if (tail == NULL){
                tail = newListNode;
            }
            if (head != NULL){
                head->prev = newListNode;
            }
            newListNode->prev = NULL;
            newListNode->next = head;
            head = newListNode;

            currentL = currentL / 2;
            currentK--;
            currentTreeNode = currentTreeNode->left;
        }
    }

    *headRef = head;
    *tailRef = tail;
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
    _mergeNonDistinctPTrees(list, list->head, false);
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
    _mergeNonDistinctPTrees(list, list->tail, true);
}
bool _pop_left_base(PTreeList* list){
    if (list->n == 0){
        return false;
    }
    (list->n)--;

    PTree* leftPTree = list->head->ptree;
    if (leftPTree->k == 0){
        list->head = list->head->next;
        return true;
    }

    PTreeListNode* head;
    PTreeListNode* tail;
    _cascadeRemoval(leftPTree, &head, &tail);
    tail->next = list->head->next;
    if (tail->next != NULL){
        tail->next->prev = tail;
    }
    list->head = head;
    _mergeNonDistinctPTrees(list, list->head, false);

    return true;
}
bool _pop_right_base(PTreeList* list){
    if (list->n == 0){
        return false;
    }
    (list->n)--;

    PTree* rightPTree = list->head->ptree;
    if (rightPTree->k == 0){
        list->tail = list->tail->prev;
        return true;
    }

    PTreeListNode* head;
    PTreeListNode* tail;
    _cascadeRemoval(rightPTree, &head, &tail);
    head->prev = list->tail->prev;
    if (head->prev != NULL){
        head->prev->next = head;
    }
    list->tail = tail;
    _mergeNonDistinctPTrees(list, list->tail, true);

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
    PTreeList* list0 = make(n, seq);
    printf("Pass.");
    reverse(list0);
    for (int i = 0; i < n; i++){
        printf("Yeah: %d\n", get(list0, i));
    }
    printf("LEFT: %d\n", peek_left(list0));
    printf("RIGHT: %d\n", peek_right(list0));

    DATA seq1[4] = {1, 2, 3, 4};
    LENGTH l1, k1;
    _getGreatestPowerOfTwo(4, &l1, &k1);

    PTree* ptree1 = (PTree*) malloc(sizeof(PTree));
    ptree1->l = l1;
    ptree1->k = k1;
    ptree1->root = _constructPTreeNodesFromRange(seq1, 0, 0, 3);

    DATA seq2[4] = {5, 6, 7, 8};
    LENGTH l2, k2;
    _getGreatestPowerOfTwo(4, &l2, &k2);

    PTree* ptree2 = (PTree*) malloc(sizeof(PTree));
    ptree2->l = l2;
    ptree2->k = k2;
    ptree2->root = _constructPTreeNodesFromRange(seq2, 0, 0, 3);

    DATA seq3[8] = {9, 10, 11, 12, 13, 14, 15, 16};
    LENGTH l3, k3;
    _getGreatestPowerOfTwo(8, &l3, &k3);

    PTree* ptree3 = (PTree*) malloc(sizeof(PTree));
    ptree3->l = l3;
    ptree3->k = k3;
    ptree3->root = _constructPTreeNodesFromRange(seq3, 0, 0, 7);

    PTreeList* list = (PTreeList*) malloc(sizeof(PTreeList));
    list->n = 16;
    list->reverse = false;
    
    PTreeListNode* a = (PTreeListNode*) malloc(sizeof(PTreeListNode));
    a->prev = NULL;
    a->ptree = ptree1;

    PTreeListNode* b = (PTreeListNode*) malloc(sizeof(PTreeListNode));
    b->prev = a;
    a->next = b;
    b->ptree = ptree2;

    PTreeListNode* c = (PTreeListNode*) malloc(sizeof(PTreeListNode));
    c->next = NULL;
    c->prev = b;
    b->next = c;
    c->ptree = ptree3;

    list->head = a;
    list->tail = c;

    _mergeNonDistinctPTrees(list, list->head, false);
    printf("??\n");
    PTreeListNode* currentListNode = list->head;
    while (currentListNode != NULL){
        PTree* ptree = currentListNode->ptree;
        printf("KK: %d\n", ptree->k);
        _printLeaves(ptree->root);
        currentListNode = currentListNode->next;
        printf("-------------------\n");
    }

    printf("POP:\n");
    pop_left(list);
    currentListNode = list->head;
    while (currentListNode != NULL){
        PTree* ptree = currentListNode->ptree;
        printf("KK: %d\n", ptree->k);
        _printLeaves(ptree->root);
        currentListNode = currentListNode->next;
        printf("-------------------\n");
    }
}
