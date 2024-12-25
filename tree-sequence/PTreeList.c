#include <stdio.h>

#include "../H_global.h"
#include "H_PTreeList.h"
#include <stdlib.h>
#include <inttypes.h>
#include <stdbool.h>


/*
<< ------ ||
// --------------------------------------------------------- >>
--//======================================================\\--
--|| DYNAMIC ARRAY: SEQUENCE OF PERFECT BINARY TREES
--||              [[ PHANTOM SEGMENT TREE ]]
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
    LENGTH k = 0;
    LENGTH pow = 1;
    while (pow < num){
        LENGTH new = pow << 1;
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

    while (currentListNode != NULL){
        
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
        root->left = traverseLeft == false ? leftPTree->root : rightPTree->root;
        root->right = traverseLeft == false ? rightPTree->root : leftPTree->root;

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

void _cascadeRemoval(PTree* ptree, bool fromRight, PTreeListNode** headRef, PTreeListNode** tailRef){
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
            rightPTree->root = fromRight == false ? currentTreeNode->right : currentTreeNode->left;
            
            PTreeListNode* newListNode = (PTreeListNode*) malloc(sizeof(PTreeListNode));
            newListNode->ptree = rightPTree;
            if (fromRight == false){
                if (tail == NULL){
                    tail = newListNode;
                }
                if (head != NULL){
                    head->prev = newListNode;
                }
                newListNode->prev = NULL;
                newListNode->next = head;
                head = newListNode;
            } else {
                if (head == NULL){
                    head = newListNode;
                }
                if (tail != NULL){
                    tail->next = newListNode;
                }
                newListNode->prev = tail;
                newListNode->next = NULL;
                tail = newListNode;
            }

            currentL = currentL / 2;
            currentK--;
            currentTreeNode = fromRight == false ? currentTreeNode->left : currentTreeNode->right;
        }
    }

    *headRef = head;
    *tailRef = tail;
}

void _peekABoo(PTreeList* list, bool updateLeft, bool updateRight){
    if (list->n == 0){
        return;
    }

    PTreeNode* currentTreeNode = NULL;
    if (updateLeft == true){
        currentTreeNode = list->head->ptree->root;
        while (currentTreeNode->leaf == false){
            currentTreeNode = currentTreeNode->left;
        }
        list->leftmost = currentTreeNode->data;
    }
    if (updateRight == true){
        currentTreeNode = list->tail->ptree->root;
        while (currentTreeNode->leaf == false){
            currentTreeNode = currentTreeNode->right;
        }
        list->rightmost = currentTreeNode->data;
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
    if (list->head != NULL){
        list->head->prev = newListNode;
    }
    list->head = newListNode;
    if (list->n == 0){
        list->tail = newListNode;
    }
    (list->n)++;

    // Fix non-distinct types
    _mergeNonDistinctPTrees(list, list->head, false);

    _peekABoo(list, true, false);
}
void _push_right_base(PTreeList* list, DATA v){
    // Construct a PTree of type 0 with the new data as root (which is also a leaf)
    PTree* ptree = _constructZeroPTree(v);

    // Add to right of list
    PTreeListNode* newListNode = (PTreeListNode*) malloc(sizeof(PTreeListNode));
    newListNode->ptree = ptree;
    newListNode->next = NULL;
    newListNode->prev = list->tail;
    if (list->tail != NULL){
        list->tail->next = newListNode;
    }
    list->tail = newListNode;
    if (list->n == 0){
        list->head = newListNode;
    }
    (list->n)++;

    // Fix non-distinct types
    _mergeNonDistinctPTrees(list, list->tail, true);

    _peekABoo(list, false, true);
}
bool _pop_left_base(PTreeList* list){
    if (list->n == 0){
        return false;
    }
    (list->n)--;

    PTree* leftPTree = list->head->ptree;
    if (leftPTree->k == 0){
        if (list->head == list->tail){
            list->tail = NULL;
        }
        if (list->head->next != NULL){
            list->head->next->prev = NULL;
        }
        list->head = list->head->next;
        return true;
    }
    
    PTreeListNode* head = NULL;
    PTreeListNode* tail = NULL;
    _cascadeRemoval(leftPTree, false, &head, &tail);
    if (list->head == list->tail){
        list->tail = tail;
    } else {
        tail->next = list->head->next;
        if (list->head->next != NULL){
            list->head->next->prev = tail;
        }
    }
    list->head = head;
    _mergeNonDistinctPTrees(list, list->head, false);
    _peekABoo(list, true, true);
    return true;
}
bool _pop_right_base(PTreeList* list){
    if (list->n == 0){
        return false;
    }
    (list->n)--;

    PTree* rightPTree = list->tail->ptree;
    if (rightPTree->k == 0){
        if (list->head == list->tail){
            list->head = NULL;
        }
        if (list->tail->prev != NULL){
            list->tail->prev->next = NULL;
        }
        list->tail = list->tail->prev;
        return true;
    }

    PTreeListNode* head = NULL;
    PTreeListNode* tail = NULL;
    _cascadeRemoval(rightPTree, true, &head, &tail);
    if (list->head == list->tail){
        list->head = head;
    } else {
        head->prev = list->tail->prev;
        if (list->tail->prev != NULL){
            list->tail->prev->next = head;
        }
    }
    list->tail = tail;
    _mergeNonDistinctPTrees(list, list->tail, true);
    _peekABoo(list, true, true);
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
    if (n > 0){
        _constructPTreesFromRange(seq, 0, n-1, &head, &tail);
    }

    list->n = n;
    list->head = head;
    list->tail = tail;
    list->reversed = false;
    list->leftmost = seq[0];
    list->rightmost = seq[n-1];
    return list;
}

// Flags/List Info
void reverse(PTreeList* list){
    list->reversed = !(list->reversed);
}
LENGTH size(PTreeList* list){
    return list->n;
}
bool empty(PTreeList* list){
    return list->n == 0;
}

// Getters/Setters
DATA get(PTreeList* list, LENGTH i){
    return _getPTreeNodeAtIndex(list, list->reversed == false ? i : (list->n)-1-i)->data;
}
void set(PTreeList* list, LENGTH i, DATA v){
    LENGTH trueIndex = list->reversed == false ? i : (list->n)-1-i;
    _getPTreeNodeAtIndex(list, trueIndex)->data = v;
    if (trueIndex == 0){
        list->leftmost = v;
    }
    if (trueIndex == list->n - 1){
        list->rightmost = v;
    }
}
DATA peek_left(PTreeList* list){
    return list->reversed == false ? list->leftmost : list->rightmost;
}
DATA peek_right(PTreeList* list){
    return list->reversed == false ? list->rightmost : list->leftmost;
}

// Insertions/Deletions
void push_left(PTreeList* list, DATA v){
    list->reversed == false ? _push_left_base(list, v) : _push_right_base(list, v);
}
void push_right(PTreeList* list, DATA v){
    list->reversed == false ? _push_right_base(list, v) : _push_left_base(list, v);
}
bool pop_left(PTreeList* list){
    return list->reversed == false ? _pop_left_base(list) : _pop_right_base(list);
}
bool pop_right(PTreeList* list){
    return list->reversed == false ? _pop_right_base(list) : _pop_left_base(list);
}





























































































































































































// :)