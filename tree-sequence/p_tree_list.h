#ifndef P_TREE_H
#define P_TREE_H

#include "../H_global.h"
#include <stdlib.h>
#include <inttypes.h>
#include <stdbool.h>

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
    bool reversed;

    DATA leftmost;
    DATA rightmost;
} PTreeList;


void DEFOREST(void* ptr);

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



#endif