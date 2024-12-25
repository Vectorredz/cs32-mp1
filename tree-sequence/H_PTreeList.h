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
} PTreeList;




#endif