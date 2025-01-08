#ifndef SKIP_LIST_H
#define SKIP_LIST_H
#include "../H_global.h"
#include <stdbool.h>
#include <stdint.h>


typedef struct _SkipNode {
    struct _SkipNode *right;  // link to the right adjacent nodes
    struct _SkipNode *left; // link to the left adjacent nodes
    struct _SkipNode *below; // link to the each levels; level 0, node is linked to NULL
    bool is_sentinel; // flag
    DATA val;  
    LENGTH width;
} SkipNode;

// Level that references to the left sentinel and right sentinel
typedef struct _Level {
    struct _Level *up;
    struct _Level *down;
    SkipNode *head_sentinel; // head
    SkipNode *tail_sentinel; // tail
    LENGTH cached_right_width;
} Level;

// Vertical doubly-linked List to store the levels
typedef struct _LevelList {
    Level *top; // Top Header
    Level *bottom; // Level 0 Header
} LevelList;

typedef struct _LevelRecord {
    Level *top_level;
    struct _LevelRecord *next;
    struct _LevelRecord *prev;
} LevelRecord;

typedef struct _LevelRecordsList {
    LevelRecord *head;
    LevelRecord *tail;
} LevelRecordsList;

typedef struct _SkipList {
    LevelList *levels;
    LevelRecordsList *node_top_levels;
    
    DATA leftmost;
    DATA rightmost;
    
    LENGTH max_height;
    LENGTH curr_height;
    LENGTH size;
    bool is_reversed;
} SkipList;

// Helpers

LENGTH ceil_lg(LENGTH n);
LENGTH _cap_height(LENGTH n);
bool _flip_coin();
SkipNode *_get_node(SkipList *l, LENGTH i, bool from_set, DATA v);
Level *_promote_level(SkipList *l, bool from_right);
void _demote_level(SkipList *l, Level *curr_level, bool from_right);
void _push_left_base(SkipList *l, DATA v);
void _push_right_base(SkipList *l, DATA v);
bool _pop_left_base(SkipList *l);
bool _pop_right_base(SkipList *l);

// Init
SkipNode *_make_node(DATA v);
Level *_make_level();
SkipList *_init();
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