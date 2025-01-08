/*

-----------------------------------------------
SEQUENCE OF PERFECT BINARY TREES
-----------------------------------------------
Perfect Binary Tree / Phantom Segment Tree
    The Perfect Binary Trees are represented by the struct PTree, which is a modified
    Segment Tree-esque data structure with a special property of having implicit bounds.
    For this reason, I also like to call it a Phantom Segment Tree (Phantom Index-Segment Tree),
    or PTree for short, as the bounds are only revealed once the list and trees are traversed
    with get/set operations.

*/
// --------------------------------------------------------- >>
// --------------------------------------------------------- >>
// --------------------------------------------------------- >>
#include <assert.h>
#include <stdio.h>
#include "p_tree_list.h"

void DEFOREST(void* ptr){
    free(ptr);
}


/*
-----------------------------------------------
HELPERS
-----------------------------------------------
*/

/*
This helper function returns the exponent of the greatest power of 2 at most the given number.
Formally, it returns the largest k where 2^k <= number.
For example, with n=22, its greatest power of two is 2^4=16, because 2^5=32 which exceeds it.
It utilizes bitshifting for quick exponents.
*/
LENGTH _get_greatest_power_of_two(LENGTH num){
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
    return k;
}

/*
This is a recursive function that constructs a Perfect Binary Tree from the root, with the leaves accurately representing a subsequence of the given sequence.
Once it reaches the leaves, it gets the appropriate value from the sequence, along with seqIndexOffset (if it's in a different part of the sequence).

For example, we have a sequence of {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11} (length 12)
If we want to get a PTree of type 3 with the leaves from indices 4-11, then we call the helper function with:
lowerBound=0, upperBound=7 (since 2^3 = length 8), seqIndexOffset=4 (since it starts from 4)
Then after the recursive calls, the resulting leaves would be {4, 5, 6, 7, 8, 9, 10, 11}

Then, the caller receives the root node.
*/
PTreeNode* _construct_ptree_nodes_from_range(DATA* seq, LENGTH seqIndexOffset, LENGTH lowerBound, LENGTH upperBound){
    PTreeNode* newTreeNode = (PTreeNode*) malloc(sizeof(PTreeNode));
    
    if ((upperBound-lowerBound+1) == 1){
        newTreeNode->leaf = true;
        newTreeNode->data = seq[lowerBound+seqIndexOffset];

    } else {
        newTreeNode->leaf = false;

        LENGTH mid = (upperBound+lowerBound)/2;
        newTreeNode->left = _construct_ptree_nodes_from_range(seq, seqIndexOffset, lowerBound, mid);
        newTreeNode->right = _construct_ptree_nodes_from_range(seq, seqIndexOffset, mid+1, upperBound);
    }
    
    return newTreeNode;
}

/*
This helper function is used exclusively for make().
The point is to represent n as the sum of distinct powers of two, from highest to lowest.
It achieves this by repetitively using _get_greatest_power_of_two and constructing a PTree for each remaining subsequence, until it exhausts the entire sequence.

For example, we have a sequence of n=7
{0, 1, 2, 3, 4, 5, 6}

We can represent n as the sum of distinct powers of two:
n = 2^2 + 2^1 + 2^0 = 4 + 2 + 1

So, the sequence of k's would be {2, 1, 0}, and there would be three PTrees.
The leaves of these three PTrees would contain the numbers:
{0, 1, 2, 3}, {4, 5}, {6}.
So, it satisfies the increasing and then decreasing sequence of k's requirement.

After everything, it gives the caller the appropriate doubly-linked-list from head to tail.
*/
void _construct_ptrees(LENGTH n, DATA* seq, PTreeListNode** headRef, PTreeListNode** tailRef){
    assert(n > 0);

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
        LENGTH k = _get_greatest_power_of_two((upperBound-currentStart) + 1);
        LENGTH l = (LENGTH) 1 << k;

        // Construct a PTree for the appropriate subsequence
        PTree* ptree = (PTree*) malloc(sizeof(PTree));
        ptree->l = l;
        ptree->k = k;
        ptree->root = _construct_ptree_nodes_from_range(seq, currentStart, 0, l-1);

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
The star of the show -- these helper functions' goal is to return the appropriate leaf node for a given index
Because the nodes themselves don't store their index, the function must calculate for the "phantom" index.
That is, the function has to manually calculate the "offsets" and bounds themselves.
The calculations of these offsets and bounds are only done on the needed nodes instead of every single node in every single tree,
so search is brought down to O(logn).
It utilizes the l member of the PTree to calculate for offsets along the doubly-linked-list (horizontal) as it traverses from left to right.
Then, it shifts the current recorded lowerBound and upperBound as it traverses down the tree (vertical).
*/

// This is for the vertical search (the correct PTree was already found)
PTreeNode* _get_leaf_node(PTreeNode* node, LENGTH i, LENGTH lowerBound, LENGTH upperBound){
    if (node->leaf == true){
        assert(lowerBound == upperBound);
        return node;
    }

    /*
    The appropriate bounds for the LeftChild and RightChild are:
    [lowerBound, mid] OR [mid+1, upperBound]
    So we check if the index is in either one, and update the bounds appropriately.
    */
    LENGTH mid = (upperBound+lowerBound)/2;
    if (lowerBound <= i && i <= mid) {
        return _get_leaf_node(node->left, i, lowerBound, mid);
    } else {
        return _get_leaf_node(node->right, i, mid + 1, upperBound);
    }
}

// This is for the horizontal search (it aims to find the correct PTree first before vertical search)
PTreeNode* _get_leaf_node_at_index(PTreeList* list, LENGTH i){
    PTreeListNode* currentListNode = list->head;
    LENGTH offset = 0;

    while (currentListNode != NULL){
        PTree* ptree = currentListNode->ptree;

        /*
        Get the appropriate bounds for our current PTree
        We base it off of our corrent offset for lower bound,
        and use the PTree's "l" property (the number of leaf nodes it has) for the upper bound.
        */
        LENGTH lowerBound = offset;
        LENGTH upperBound = offset + ptree->l - 1;

        // Found correct tree, start vertical search
        if (lowerBound <= i && i <= upperBound){
            return _get_leaf_node(ptree->root, i, lowerBound, upperBound);
        }

        // Did not find correct tree
        offset += ptree->l;
        currentListNode = currentListNode->next;
    }
    assert(false);
    return NULL;
}

/*
The purpose of this helper function is to construct a PTree of type 0,
with the given DATA value as its root (leaf)'s value.
*/
PTree* _construct_zero_ptree(DATA v){
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
It is inspired by binomial heap's merge operation.

It aims to correct the list into satisfying the concatenation of a strictly increasing and strictly decreasing
sequence of k's. After a push/pop operation, there may be some PTrees that are non-distinct (that is, their k's (types) are the same.)
This means that we can combine these two trees into one bigger tree, with type k+1.

We are given a sequence of PTrees as a doubly-linked-list. We start at some node in this list and proceed to the right (left).
If our current PTree's type is the same as the next PTree, then we construct a third PTree containing their two roots as children.
(We know that this is of type k+1, and contains l*2 leaf nodes.)
By assumption, we know that all the PTrees further than the current and next PTree already have
distinct k's (distinct types) when this function was called.
By "merging" these two PTrees together, one of two conditions can happen:
- this new larger PTree has a different k from the next one.
- this new larger PTree has the same k as the next one, so we can combine them both into one even bigger PTree.
The function will keep merging to the right (left) until it falls into the first condition, which means it finished correcting the list's sequence of k's,
since any PTree further than the two being checked are already distinct by assumption.
*/
void _merge_non_distinct_ptrees_to_right(PTreeList* list, PTreeListNode* startNode){
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
// Exactly the same as _merge_non_distinct_ptrees_to_right, but the traversal is mirrored.
void _merge_non_distinct_ptrees_to_left(PTreeList* list, PTreeListNode* startNode){
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

Note that we assume the given PTree for this helper is of type k > 0, since if the PTree is of type 0, then it can just
be immediately removed from the list without any repercussions (it's a single leaf, which is already our target for removal.)
After everything, it gives the caller the appropriate sublist of right-child (left-child) PTrees.
*/
void _cascade_remove_left(PTree* ptree, PTreeListNode** subHeadRef, PTreeListNode** subTailRef){
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
// Exactly the same as _cascade_remove_left, but the traversal is mirrored.
void _cascade_remove_right(PTree* ptree, PTreeListNode** subHeadRef, PTreeListNode** subTailRef){
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
void _peek_a_boo(PTreeList* list){
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
    PTree* ptree = _construct_zero_ptree(v);

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
    _merge_non_distinct_ptrees_to_right(list, list->head);

    // Update left (update right too if new tail)
    list->leftmost = v;
    if (list->n == 1){
        list->rightmost = v;
    }
}
// Exactly the same as _push_left_base, but operations are mirrored.
void _push_right_base(PTreeList* list, DATA v){
    // Construct a PTree of type 0 with the new data as root (which is also a leaf)
    PTree* ptree = _construct_zero_ptree(v);
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

    _merge_non_distinct_ptrees_to_left(list, list->tail);
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
        _peek_a_boo(list);
        return true;
    }
    
    // If its of type k > 0, then we must cascade
    PTreeListNode* subHead = NULL;
    PTreeListNode* subTail = NULL;
    _cascade_remove_left(leftPTree, &subHead, &subTail);

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
    _merge_non_distinct_ptrees_to_right(list, subTail);

    // Update both just in case
    _peek_a_boo(list);
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

        _peek_a_boo(list);
        return true;
    }

    PTreeListNode* subHead = NULL;
    PTreeListNode* subTail = NULL;
    _cascade_remove_right(rightPTree, &subHead, &subTail);
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

    _merge_non_distinct_ptrees_to_left(list, subHead);
    _peek_a_boo(list);
    return true;
}


/*
-----------------------------------------------
MAIN OPERATIONS
-----------------------------------------------
*/

// Init
PTreeList* make(LENGTH n, DATA* seq){
    PTreeList* list = (PTreeList*) malloc(sizeof(PTreeList));

    PTreeListNode* head = NULL;
    PTreeListNode* tail = NULL;
    if (n > 0){
        _construct_ptrees(n, seq, &head, &tail);
        list->leftmost = seq[0];
        list->rightmost = seq[n-1];
    }

    list->n = n;
    list->head = head;
    list->tail = tail;
    list->reversed = false;
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
        fprintf(stderr, "\nError: index is out of bounds!\n");
        exit(1);
    }
    return _get_leaf_node_at_index(list, list->reversed == false ? i : (list->n)-1-i)->data;
}
void set(PTreeList* list, LENGTH i, DATA v){
    if (!(0 <= i && i < list->n)){
        fprintf(stderr, "\nError: index is out of bounds!\n");
        exit(1);
    }
    
    LENGTH trueIndex = list->reversed == false ? i : (list->n)-1-i;
    _get_leaf_node_at_index(list, trueIndex)->data = v;
    if (trueIndex == 0){
        list->leftmost = v;
    }
    if (trueIndex == list->n - 1){
        list->rightmost = v;
    }
}
DATA peek_left(PTreeList* list){
    if (list->n == 0){
        fprintf(stderr, "\nError: peek is not allowed if size is 0!\n");
        exit(1);
    }
    return list->reversed == false ? list->leftmost : list->rightmost;
}
DATA peek_right(PTreeList* list){
    if (list->n == 0){
        fprintf(stderr, "\nError: peek is not allowed if size is 0!\n");
        exit(1);
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


/*
-----------------------------------------------
TEST OPERATIONS
-----------------------------------------------
*/

void _get_leaf_data(PTreeNode* node, DATA* seq, LENGTH* iRef){
    if (node->leaf == false){
        _get_leaf_data(node->left, seq, iRef);
    }
    if (node->leaf == true){
        seq[*iRef] = node->data;
        (*iRef)++;
        return;
    }
    if (node->leaf == false){
        _get_leaf_data(node->right, seq, iRef);
    }
}
void TEST_elements(PTreeList* list, LENGTH* nRef, DATA** seqRef){
    DATA* seq = (DATA*) malloc((list->n)*sizeof(DATA));
    LENGTH i = 0;
    PTreeListNode* currentListNode = list->head;
    while (currentListNode != NULL){
        _get_leaf_data(currentListNode->ptree->root, seq, &i);
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