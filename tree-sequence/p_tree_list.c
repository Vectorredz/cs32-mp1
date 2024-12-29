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

// Get the greatest power of two obtainable for a given number without exceeding it.
// pow is the power of two, and k is the exponent
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

/*
This constructs a perfect binary tree from the root, with the leaves accurately representing a subsequence of the given sequence.
Once it reaches the leaves, it gets the appropriate value from the sequence, along with seqIndexOffset (if it's in a different part of the sequence).

For example, we have a sequence of {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11} (length 12)
If we want to get a PTree of type 3 with the leaves from indices 4-11, then we call the helper function with:
lowerBound=0, upperBound=7 (since 2^3 = length 8), seqIndexOffset=4 (since it starts from 4)
Then after the recursive calls, the resulting leaves would be {4, 5, 6, 7, 8, 9, 10, 11}
*/
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

/*
This helper function is used exclusively for make().
The point is to represent n as distinct powers of two, from highest to lowest.
It achieves this by repetitively using _getGreatestPowerOfTwo and constructing a PTree for each one, until it exhausts the entire sequence.
After everything, it gives the caller the appropriate doubly-linked-list from head to tail.
*/
void _constructPTrees(LENGTH n, DATA* seq, PTreeListNode** headRef, PTreeListNode** tailRef){
    PTreeListNode* head = NULL;
    PTreeListNode* tail = NULL;
    
    LENGTH currentStart = 0;
    LENGTH upperBound = n-1;
    while (currentStart <= upperBound){
        /*
        Get greatest power of two for our current remaining subsequence without a PTree yet
        It is possible that this PTree's bounds do not exhaust the entire subsequence at once,
        so we have a while loop to keep exhausting it until the very last element
        */
        LENGTH l, k;
        _getGreatestPowerOfTwo((upperBound-currentStart) + 1, &l, &k);

        // Construct a PTree for the appropriate subsequence
        PTree* ptree = (PTree*) malloc(sizeof(PTree));
        ptree->l = l;
        ptree->k = k;
        ptree->root = _constructPTreeNodesFromRange(seq, currentStart, 0, l-1);

        // Usual doubly-linked-list linking of neighbors
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

/*
The star of the show -- this helper function's goal is to return the appropriate leaf node for a given index
Because the nodes themselves don't store their index, the function must calculate for the "phantom" index.
That is, the function has to manually calculate the "offsets" and bounds themselves.
The calculations of these offsets and bounds are only done on the needed nodes instead of every single node in every single tree,
so search is brought down to O(logn).
*/
PTreeNode* _getLeafNodeAtIndex(PTreeList* list, LENGTH i){
    if (list->n == 0){
        return NULL;
    }
    PTreeListNode* currentListNode = list->head;
    LENGTH offset = 0;

    while (currentListNode != NULL){
        
        PTree* ptree = currentListNode->ptree;
        PTreeNode* currentTreeNode = ptree->root;

        /*
        Get the appropriate bounds for our current PTree
        We base it off of our corrent offset for lower bound,
        and use the PTree's "l" property (the number of leaf nodes it has) for the upper bound.
        */
        LENGTH lowerBound = offset;
        LENGTH upperBound = offset + ptree->l - 1;

        // Base case: the PTree itself is of type 0, so the root node *is* the leaf
        if (currentTreeNode->leaf == true){
            // If it's the appropriate index, then we have found our leaf node.
            if (lowerBound == i){
                return currentTreeNode;

            // If not, then move to the next tree, updating the offset in the process.
            } else {
                offset++;
                currentListNode = currentListNode->next;
                continue;
            }
        
        /*
        If the index we're trying to find isn't part of the appropriate bounds,
        then we know for sure that it isn't part of this PTree.
        */
        } else {
            if (!(lowerBound <= i && i <= upperBound)){
                offset += ptree->l;
                currentListNode = currentListNode->next;
                continue;
            }
        }

        /*
        Now we know that the leaf *must* be in this PTree, and that it's of type k > 0.
        We know proceed with our search akin to a normal Segment Tree, manually updating the bounds in the process.
        */

        /*
        This while loop's expression is actually useless because in the loop, it already checks
        if the children are leaf nodes and immediately returns,
        but I put it here just in case.
        */
        while (currentTreeNode->leaf == false){
            PTreeNode* leftChild = currentTreeNode->left;
            PTreeNode* rightChild = currentTreeNode->right;

            /*
            Since it's a perfect binary tree,
            we know for sure that if the LeftChild is a leaf, then so is the RightChild.
            */
            if (leftChild->leaf == true){
                /*
                These two children are adjacent indices, and since we know the index is within either one of these two,
                then it's either one or the other.
                And voila, we have found the correct leaf.
                */
                if (lowerBound == i){
                    return leftChild;
                } else {
                    return rightChild;
                }

            
            // If it's not a leaf node, then we keep searching for the appropriate child to traverse to.
            } else {
                /*
                The appropriate bounds for the LeftChild and RightChild are:
                [lowerBound, mid] OR [mid+1, upperBound]
                So we check if the index is in either one, and update the bounds appropriately.
                */
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
    }
    return NULL;
}

/*
The purpose of this helper function is to construct a PTree of type 0,
with the given DATA value as its root (leaf)'s value.
*/
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

/*
Here we have helper functions for merging non-distinct PTrees together, split into two: one for merging to the left, and one for merging to the right.
PTrees are non-distinct if their type is the same, which means they can be combined into one greater power of two,
improving the running time of the get/set operations, satisfying the concatenation of a strictly increasing and strictly decreasing
sequence of k's, and overall preventing the degenerate representation of n PTrees of type 0.

It is inspired by binomial heap's merge operation.
Given a starting doubly-linked-list node, we proceed to the right (left),
and if our current PTree's type is the same as the next PTree, then we construct a third PTree containing their two roots as children.
We know that this is of type k+1, and contains l*2 leaf nodes.
*/
void _mergeNonDistinctPTreesToRight(PTreeList* list, PTreeListNode* startNode){
    // Base case: there's 0 or 1 PTrees, so we don't need to do anything.
    if (list->n <= 1){
        return;
    }

    PTreeListNode* currentListNode = startNode;
    PTreeListNode* nextListNode = currentListNode->next;

    // If either the current node or the next node is NULL, then we reached the end.
    while (currentListNode != NULL && nextListNode != NULL){
        
        PTree* currentPTree = currentListNode->ptree;
        PTree* nextPTree = nextListNode->ptree;
        
        // If the next PTree is distinct from our current one, then there's no point in going further.
        if (currentPTree->k != nextPTree->k){
            break;
        }

        // Construct PTree with the two roots as children, and update k and l respectively.
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
        // If either one is a head/tail, then the combination of them will become the head/tail as well.
        if (currentListNode == list->head || nextListNode == list->head){
            list->head = newListNode;
        }
        if (currentListNode == list->tail || nextListNode == list->tail){
            list->tail = newListNode;
        }
        // Neighbor backwards-linking.
        if (newListNode->prev != NULL){
            newListNode->prev->next = newListNode;
        }
        if (newListNode->next != NULL){
            newListNode->next->prev = newListNode;
        }
        // Free dem trees.
        DEFOREST(currentListNode->ptree);
        DEFOREST(currentListNode);
        DEFOREST(nextListNode->ptree);
        DEFOREST(nextListNode);

        // Advance.
        currentListNode = newListNode;
        nextListNode = currentListNode->next;
    }
}
// Exactly the same as _mergeNonDistinctPTreesToRight, but the traversal is mirrored.
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

        PTreeNode* root = (PTreeNode*) malloc(sizeof(PTreeNode));
        root->leaf = false;
        root->left = nextPTree->root;
        root->right = currentPTree->root;

        PTree* ptree = (PTree*) malloc(sizeof(PTree));
        ptree->l = currentPTree->l + nextPTree->l;
        ptree->k = currentPTree->k + 1;
        ptree->root = root;

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

/*
These helper functions for pop operations.
They are pretty much the opposite of the merge operations, split into two: one for cascading the right of the PTree, and one for cascading the left.
We know that only the leftmost (rightmost) leaf of the target PTree is the one that needs to be removed.
So, its purpose is to "wrap open" the PTree back into a sequence of PTrees.
It does this by snipping off the right (left) subtree and making it its own doubly-linked-list node sub-list.
Then the other leftover left (right) child is deallocated and removed from the list entirely.
Each successive right (left) subtree is of type k-1 and l/2.
After everything, it gives the caller the appropriate sublist of right-child (left-child) PTrees.
*/
void _cascadeRemovalLeft(PTree* ptree, PTreeListNode** subHeadRef, PTreeListNode** subTailRef){
    PTreeListNode* subHead = NULL;
    PTreeListNode* subTail = NULL;

    LENGTH currentL = ptree->l;
    LENGTH currentK = ptree->k;
    PTreeNode* currentTreeNode = ptree->root;
    while (currentTreeNode != NULL){
        // If it's the desired leaf node we want to remove then we are done.
        if (currentTreeNode->leaf == true){
            DEFOREST(currentTreeNode);
            currentTreeNode = NULL;

        // If not, then keep the right child and remove the left child.
        } else {
            PTree* keptPTree = (PTree*) malloc(sizeof(PTree));
            keptPTree->l = currentL/2;
            keptPTree->k = currentK-1;
            keptPTree->root = currentTreeNode->right;
            
            // Doubly-linked-list linking.
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
            
            // Free that child.
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
// Exactly the same as _cascadeRemovalLeft, but the traversal is mirrored.
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

/*
A useful helper function for updating the leftmost/rightmost value after a modification of the list.
It is useful for peek_left/peek_right operations.
It does this by getting the leftmost (rightmost) PTree and traversing all the way to the leftmost (rightmost) leaf node,
and finally updating the value.
*/
void _peekABoo(PTreeList* list){
    if (list->n == 0){
        return;
    }

    // Update left
    PTreeNode* currentTreeNode = currentTreeNode = list->head->ptree->root;
    while (currentTreeNode->leaf == false){
        currentTreeNode = currentTreeNode->left;
    }
    list->leftmost = currentTreeNode->data;

    // Update right
    currentTreeNode = list->tail->ptree->root;
    while (currentTreeNode->leaf == false){
        currentTreeNode = currentTreeNode->right;
    }
    list->rightmost = currentTreeNode->data;
}

/*
A set of 4 helper functions to make doing the correct "true" operation whether the list is reversed or not.
*/
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

    // Update left (update right too if new tail)
    list->leftmost = v;
    if (list->n == 1){
        list->rightmost = v;
    }
}
// Exactly the same as _push_left_base, but operations are mirrored.
void _push_right_base(PTreeList* list, DATA v){
    // Construct a PTree of type 0 with the new data as root (which is also a leaf)
    PTree* ptree = _constructZeroPTree(v);
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

    _mergeNonDistinctPTreesToLeft(list, list->tail);
    list->rightmost = v;
    if (list->n == 1){
        list->leftmost = v;
    }
}
bool _pop_left_base(PTreeList* list){
    if (list->n == 0){
        return false;
    }
    (list->n)--;

    PTree* leftPTree = list->head->ptree;
    
    // Base case: leftmost PTree is of type 0, so we can just remove it immediately without cascading
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

        // Update both just in case
        _peekABoo(list);
        return true;
    }
    
    // If its of type k > 0, then we must cascade
    PTreeListNode* subHead = NULL;
    PTreeListNode* subTail = NULL;
    _cascadeRemovalLeft(leftPTree, &subHead, &subTail);

    // Update the list appropriately with the new sublist
    PTreeListNode* oldHead = list->head;
    if (oldHead == list->tail){
        list->tail = subTail;
    } else {
        subTail->next = oldHead->next;
        if (oldHead->next != NULL){
            oldHead->next->prev = subTail;
        }
    }
    list->head = subHead;
    DEFOREST(oldHead);

    // Fix non-distinct types (start from furthest in the list, since we know all the cascaded right sub-trees are distinct)
    _mergeNonDistinctPTreesToRight(list, subTail);

    // Update both just in case
    _peekABoo(list);
    return true;
}
// Exactly the same as _pop_left_base, but operations are mirrored.
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

        _peekABoo(list);
        return true;
    }

    PTreeListNode* subHead = NULL;
    PTreeListNode* subTail = NULL;
    _cascadeRemovalRight(rightPTree, &subHead, &subTail);
    PTreeListNode* oldTail = list->tail;
    if (list->head == oldTail){
        list->head = subHead;
    } else {
        subHead->prev = oldTail->prev;
        if (oldTail->prev != NULL){
            oldTail->prev->next = subHead;
        }
    }
    list->tail = subTail;
    DEFOREST(oldTail);

    _mergeNonDistinctPTreesToLeft(list, subHead);
    _peekABoo(list);
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
        _constructPTrees(n, seq, &head, &tail);
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
    return _getLeafNodeAtIndex(list, list->reversed == false ? i : (list->n)-1-i)->data;
}
void set(PTreeList* list, LENGTH i, DATA v){
    if (!(0 <= i && i < list->n)){
        return;
    }
    LENGTH trueIndex = list->reversed == false ? i : (list->n)-1-i;
    _getLeafNodeAtIndex(list, trueIndex)->data = v;
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

    // Tests concat of strictly increasing and strictly decreasing k-values
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
bool TEST_reversed(PTreeList* list){
    return list->reversed;
}













// :)