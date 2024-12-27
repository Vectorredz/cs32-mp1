/*
<< ------ ||
// --------------------------------------------------------- >>
--//======================================================\\--
--|| DYNAMIC ARRAY: SEQUENCE OF PERFECT BINARY TREES
--||              [[ PHANTOM SEGMENT TREE ]]
--\\======================================================//--
// --------------------------------------------------------- >>
<< ------ ||
Perfect Binary Tree / Phantom Segment Tree
    The Perfect Binary Trees are represented by the struct PTree, which is a modified
    Segment Tree-esque data structure with a special property of having implicit bounds.
    For this reason, I also like to call it a Phantom Segment Tree, as the bounds are
    only revealed once the list and trees are traversed with get/set operations.

*/
// --------------------------------------------------------- >>
// --------------------------------------------------------- >>
// --------------------------------------------------------- >>
#include "p_tree_list.h"

void DEFOREST(void* ptr){
    free(ptr);
}


// --------------------------------------------------------- >>
/* ----------------------------------------- <<

            ||-- HELPERS --||

<< ----------------------------------------- */
// --------------------------------------------------------- >>

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
void _mergeNonDistinctPTreesToRight(PTreeList* list, PTreeListNode* startNode){
    if (list->n <= 1){
        return;
    }

    PTreeListNode* currentListNode = startNode;
    PTreeListNode* nextListNode = currentListNode->next;
    while (currentListNode != NULL && nextListNode != NULL){
        
        PTree* currentPTree = currentListNode->ptree;
        PTree* nextPTree = nextListNode->ptree;

        if (currentPTree->k != nextPTree->k){
            break;
        }

        // Construct PTree with the two roots as children
        PTreeNode* root = (PTreeNode*) malloc(sizeof(PTreeNode));
        root->leaf = false;
        root->left = currentPTree->root;
        root->right = nextPTree->root;

        PTree* ptree = (PTree*) malloc(sizeof(PTree));
        ptree->l = currentPTree->l + nextPTree->l;
        ptree->k = currentPTree->k + 1;
        ptree->root = root;

        // Add to list and link neighbors
        PTreeListNode* newListNode = (PTreeListNode*) malloc(sizeof(PTreeListNode));
        newListNode->ptree = ptree;
        newListNode->prev = currentListNode->prev;
        newListNode->next = nextListNode->next;
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
        DEFOREST(currentListNode->ptree);
        DEFOREST(currentListNode);
        DEFOREST(nextListNode->ptree);
        DEFOREST(nextListNode);

        currentListNode = newListNode;
        nextListNode = currentListNode->next;
    }
}
void _mergeNonDistinctPTreesToLeft(PTreeList* list, PTreeListNode* startNode){
    if (list->n <= 1){
        return;
    }

    PTreeListNode* currentListNode = startNode;
    PTreeListNode* nextListNode = currentListNode->prev;
    while (currentListNode != NULL && nextListNode != NULL){
        
        PTree* currentPTree = currentListNode->ptree;
        PTree* nextPTree = nextListNode->ptree;

        if (currentPTree->k != nextPTree->k){
            break;
        }

        // Construct PTree with the two roots as children
        PTreeNode* root = (PTreeNode*) malloc(sizeof(PTreeNode));
        root->leaf = false;
        root->left = nextPTree->root;
        root->right = currentPTree->root;

        PTree* ptree = (PTree*) malloc(sizeof(PTree));
        ptree->l = currentPTree->l + nextPTree->l;
        ptree->k = currentPTree->k + 1;
        ptree->root = root;

        // Add to list and link neighbors
        PTreeListNode* newListNode = (PTreeListNode*) malloc(sizeof(PTreeListNode));
        newListNode->ptree = ptree;
        newListNode->prev = nextListNode->prev;
        newListNode->next = currentListNode->next;
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
        DEFOREST(currentListNode->ptree);
        DEFOREST(currentListNode);
        DEFOREST(nextListNode->ptree);
        DEFOREST(nextListNode);

        currentListNode = newListNode;
        nextListNode = currentListNode->prev;
    }
}
void _cascadeRemovalLeft(PTree* ptree, PTreeListNode** subHeadRef, PTreeListNode** subTailRef){
    PTreeListNode* subHead = NULL;
    PTreeListNode* subTail = NULL;

    LENGTH currentL = ptree->l;
    LENGTH currentK = ptree->k;
    PTreeNode* currentTreeNode = ptree->root;
    while (currentTreeNode != NULL){
        if (currentTreeNode->leaf == true){
            DEFOREST(currentTreeNode);
            currentTreeNode = NULL;

        } else {
            PTree* keptPTree = (PTree*) malloc(sizeof(PTree));
            keptPTree->l = currentL/2;
            keptPTree->k = currentK-1;
            keptPTree->root = currentTreeNode->right;
            
            PTreeListNode* newListNode = (PTreeListNode*) malloc(sizeof(PTreeListNode));
            newListNode->ptree = keptPTree;
            if (subTail == NULL){
                subTail = newListNode;
            }
            if (subHead != NULL){
                subHead->prev = newListNode;
            }
            newListNode->prev = NULL;
            newListNode->next = subHead;
            subHead = newListNode;
            
            PTreeNode* leftover = currentTreeNode;
            currentL = currentL / 2;
            currentK--;
            currentTreeNode = currentTreeNode->left;
            DEFOREST(leftover);
        }
    }
    DEFOREST(ptree);

    *subHeadRef = subHead;
    *subTailRef = subTail;
}
void _cascadeRemovalRight(PTree* ptree, PTreeListNode** subHeadRef, PTreeListNode** subTailRef){
    PTreeListNode* subHead = NULL;
    PTreeListNode* subTail = NULL;

    LENGTH currentL = ptree->l;
    LENGTH currentK = ptree->k;
    PTreeNode* currentTreeNode = ptree->root;
    while (currentTreeNode != NULL){
        if (currentTreeNode->leaf == true){
            DEFOREST(currentTreeNode);
            currentTreeNode = NULL;

        } else {
            PTree* keptPTree = (PTree*) malloc(sizeof(PTree));
            keptPTree->l = currentL/2;
            keptPTree->k = currentK-1;
            keptPTree->root = currentTreeNode->left;
            
            PTreeListNode* newListNode = (PTreeListNode*) malloc(sizeof(PTreeListNode));
            newListNode->ptree = keptPTree;
            if (subHead == NULL){
                subHead = newListNode;
            }
            if (subTail != NULL){
                subTail->next = newListNode;
            }
            newListNode->prev = subTail;
            newListNode->next = NULL;
            subTail = newListNode;
            
            PTreeNode* leftover = currentTreeNode;
            currentL = currentL / 2;
            currentK--;
            currentTreeNode = currentTreeNode->right;
            DEFOREST(leftover);
        }
    }
    DEFOREST(ptree);

    *subHeadRef = subHead;
    *subTailRef = subTail;
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
    _mergeNonDistinctPTreesToRight(list, list->head);
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
    _mergeNonDistinctPTreesToLeft(list, list->tail);
    _peekABoo(list, false, true);
}
bool _pop_left_base(PTreeList* list){
    if (list->n == 0){
        return false;
    }
    (list->n)--;

    PTree* leftPTree = list->head->ptree;
    if (leftPTree->k == 0){
        PTreeListNode* oldHead = list->head;
        if (oldHead == list->tail){
            list->tail = NULL;
        }
        if (oldHead->next != NULL){
            oldHead->next->prev = NULL;
        }
        list->head = oldHead->next;
        DEFOREST(oldHead->ptree->root);
        DEFOREST(oldHead->ptree);
        DEFOREST(oldHead);
        return true;
    }
    
    PTreeListNode* subHead = NULL;
    PTreeListNode* subTail = NULL;
    _cascadeRemovalLeft(leftPTree, &subHead, &subTail);

    PTreeListNode* oldHead = list->head;
    if (oldHead == list->tail){
        list->tail = subTail;
        DEFOREST(oldHead);
    } else {
        subTail->next = oldHead->next;
        if (oldHead->next != NULL){
            oldHead->next->prev = subTail;
        }
    }
    list->head = subHead;

    _mergeNonDistinctPTreesToRight(list, subTail);
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
        PTreeListNode* oldTail = list->tail;
        if (list->head == oldTail){
            list->head = NULL;
        }
        if (oldTail->prev != NULL){
            oldTail->prev->next = NULL;
        }
        list->tail = oldTail->prev;
        DEFOREST(oldTail->ptree->root);
        DEFOREST(oldTail->ptree);
        DEFOREST(oldTail);
        return true;
    }

    PTreeListNode* subHead = NULL;
    PTreeListNode* subTail = NULL;
    _cascadeRemovalRight(rightPTree, &subHead, &subTail);

    PTreeListNode* oldTail = list->tail;
    if (list->head == oldTail){
        list->head = subHead;
        DEFOREST(oldTail);
    } else {
        subHead->prev = oldTail->prev;
        if (oldTail->prev != NULL){
            oldTail->prev->next = subHead;
        }
    }
    list->tail = subTail;

    _mergeNonDistinctPTreesToLeft(list, subHead);
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
    if (!(0 <= i && i < list->n)){
        return 0;
    }
    return _getPTreeNodeAtIndex(list, list->reversed == false ? i : (list->n)-1-i)->data;
}
void set(PTreeList* list, LENGTH i, DATA v){
    if (!(0 <= i && i < list->n)){
        return;
    }
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
    if (list->n == 0){
        return 0;
    }
    return list->reversed == false ? list->leftmost : list->rightmost;
}
DATA peek_right(PTreeList* list){
    if (list->n == 0){
        return 0;
    }
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


// --------------------------------------------------------- >>
/* ----------------------------------------- <<

            ||-- TEST OPERATIONS --||

<< ----------------------------------------- */
// --------------------------------------------------------- >>

void _getLeaves(PTreeNode* node, DATA* seq, LENGTH* iRef){
    if (node->leaf == false){
        _getLeaves(node->left, seq, iRef);
    }
    if (node->leaf == true){
        seq[*iRef] = node->data;
        (*iRef)++;
        return;
    }
    if (node->leaf == false){
        _getLeaves(node->right, seq, iRef);
    }
}
void TEST_elements(PTreeList* list, LENGTH* nRef, DATA** seqRef){
    DATA* seq = (DATA*) malloc((list->n)*sizeof(DATA));
    LENGTH i = 0;
    PTreeListNode* currentListNode = list->head;
    while (currentListNode != NULL){
        _getLeaves(currentListNode->ptree->root, seq, &i);
        currentListNode = currentListNode->next;
    }
    *nRef = list->n;
    *seqRef = seq;
}
bool TEST_internal(PTreeList* list){
    PTreeListNode* currentListNode = list->head;
    if (currentListNode == NULL){
        return true;
    }

    LENGTH prevK = currentListNode->ptree->k;
    bool decreasing = false;
    currentListNode = currentListNode->next;
    while (currentListNode != NULL){
        LENGTH currentK = currentListNode->ptree->k;
        if (decreasing == false){
            if (prevK > currentK){
                decreasing = true;
            }
        } else {
            if (prevK < currentK){
                return false;
            }
        }
        prevK = currentK;
        currentListNode = currentListNode->next;
    }
    return true;
}










// :)