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

// Level that references to the left sentinel and right sentinel
typedef struct _Level {
    struct _Level *up;
    struct _Level *down;
    SkipNode *headSentinel; // head
    SkipNode *tailSentinel; // tail
} Level;

// Vertical doubly-linked List to store the levels
typedef struct _LevelList {
    Level *top; // Top Header
    Level *bottom; // Level 0 Header
} LevelList;

typedef struct _LevelRecord {
    Level *topLevel;
    struct _LevelRecord *next;
    struct _LevelRecord *prev;
} LevelRecord;

typedef struct _LevelRecordsList {
    LevelRecord *head;
    LevelRecord *tail;
} LevelRecordsList;

typedef struct _SkipList {
    LevelList *levels;
    LevelRecordsList *nodeTopLevels;
    
    DATA leftmost;
    DATA rightmost;
    
    LENGTH maxHeight;
    LENGTH currHeight;
    LENGTH size;
    bool is_reversed;
    
} SkipList;

// Helpers
SkipNode *_getNode(SkipList *l, LENGTH i);
void _leftPromoteLevel(SkipList *l, Level *currLevel, SkipNode *sentinel, SkipNode *belowNode, DATA v);
void _rightPromoteLevel(SkipList *l, Level *currLevel, SkipNode *sentinel, SkipNode *belowNode, DATA v);
void _pruneLevel(SkipList *l, Level *currLevel);
bool _leftDemoteLevel(SkipList *l, Level *currLevel, SkipNode *popNode);
bool _rightDemoteLevel(SkipList *l, Level *currLevel, SkipNode *popNode);
bool _flipCoin();

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