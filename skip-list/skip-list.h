#ifndef SKIP_LIST_H
#define SKIP_LIST_H
#include "../H_global.h"
#include <stdbool.h>
#include <stdint.h>

typedef struct _SkipNode {
    struct _SkipNode *right;  // link to the right adjacent nodes
    struct _SkipNode *left; // link to the left adjacent nodes
    struct _SkipNode *below; // link to the each levels; level 0, node is linked to NULL
    bool isSentinel; // flag
    DATA val;  
    LENGTH width;
} SkipNode;

typedef struct _Level {
    struct _Level *up;
    struct _Level *down;
    SkipNode *sentinel; // head
    SkipNode *tail; // tail
} Level;

typedef struct _LevelList {
    Level *top; // header
    Level *bottom; // level 0 zero
} LevelList;

typedef struct _SkipList {
    LevelList *levels;
    
    DATA leftmost;
    DATA rightmost;
    
    LENGTH maxHeight;
    LENGTH currHeight;
    LENGTH size;
    
    bool is_reversed;
} SkipList;

// HELPERS
SkipNode *_getNode(SkipList *l, LENGTH i);

// Init
SkipList *_initSkipList();
SkipList *make(LENGTH n, DATA *seq);

// Flags/SkipList Info
void reverse(SkipList *l);
LENGTH size(SkipList *l);
bool empty(SkipList *l);

// Getters/Setters
DATA get(SkipList *l, LENGTH i);
void set(SkipList *l, LENGTH i, DATA v);
DATA peek_left(SkipList *l);
DATA peek_right(SkipList *l);

// Insertions/Deletions
void push_left(SkipList *l, DATA v);
void push_right(SkipList *l, DATA v);
bool pop_left(SkipList *l);
bool pop_right(SkipList *l);

// Test Operations
void TEST_elements(SkipList* l, LENGTH* nRef, DATA** seqRef);
bool TEST_internal(SkipList* l);
bool TEST_reversed(SkipList* l);


#endif