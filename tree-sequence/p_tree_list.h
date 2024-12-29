#ifndef P_TREE_H
#define P_TREE_H

#include "../H_global.h"
#include <stdlib.h>
#include <inttypes.h>
#include <stdbool.h>

// PTree Node
typedef struct _PTreeNode {
    bool leaf;
    union {
        // Leaf Node (Contains Data)
        struct {
            DATA data;
        };

        // Non-Leaf Node (Contains Index Segment (phantom/implicit))
        struct {
            struct _PTreeNode* left;
            struct _PTreeNode* right;
        };
    };
} PTreeNode;

// PTree
typedef struct _PTree {
    LENGTH l; // number of leaf nodes
    LENGTH k; // power of two
    PTreeNode* root;
} PTree;

// Doubly-Linked-List Node of PTrees
typedef struct _PTreeListNode {
    PTree* ptree;
    struct _PTreeListNode* next;
    struct _PTreeListNode* prev;
} PTreeListNode;

// Main List
typedef struct _PTreeList {
    LENGTH n; // number of leaf nodes across all trees
    PTreeListNode* head;
    PTreeListNode* tail;
    bool reversed;

    DATA leftmost;
    DATA rightmost;
} PTreeList;


// Helpers
void DEFOREST(void* ptr);

void _getGreatestPowerOfTwo(LENGTH num, LENGTH* powRef, LENGTH* kRef);
PTreeNode* _constructPTreeNodesFromRange(DATA* seq, LENGTH seqIndexOffset, LENGTH lowerBound, LENGTH upperBound);
void _constructPTrees(LENGTH n, DATA* seq, PTreeListNode** headRef, PTreeListNode** tailRef);
PTreeNode* _getLeafNodeAtIndex(PTreeList* list, LENGTH i);

PTree* _constructZeroPTree(DATA v);
void _mergeNonDistinctPTreesToRight(PTreeList* list, PTreeListNode* startNode);
void _mergeNonDistinctPTreesToLeft(PTreeList* list, PTreeListNode* startNode);
void _cascadeRemovalLeft(PTree* ptree, PTreeListNode** subHeadRef, PTreeListNode** subTailRef);
void _cascadeRemovalRight(PTree* ptree, PTreeListNode** subHeadRef, PTreeListNode** subTailRef);
void _peekABoo(PTreeList* list);

void _push_left_base(PTreeList* list, DATA v);
void _push_right_base(PTreeList* list, DATA v);
bool _pop_left_base(PTreeList* list);
bool _pop_right_base(PTreeList* list);


// Init
PTreeList* make(LENGTH n, DATA* seq);

// Flags/List Info
void reverse(PTreeList* list);
LENGTH size(PTreeList* list);
bool empty(PTreeList* list);

// Getters/Setters
DATA get(PTreeList* list, LENGTH i);
void set(PTreeList* list, LENGTH i, DATA v);
DATA peek_left(PTreeList* list);
DATA peek_right(PTreeList* list);

// Insertions/Deletions
void push_left(PTreeList* list, DATA v);
void push_right(PTreeList* list, DATA v);
bool pop_left(PTreeList* list);
bool pop_right(PTreeList* list);


// Test Operations
void TEST_elements(PTreeList* list, LENGTH* nRef, DATA** seqRef);
bool TEST_internal(PTreeList* list);
bool TEST_reversed(PTreeList* list);



#endif