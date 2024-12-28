#ifndef SKIP_LIST_H
#define SKIP_LIST_H
#include <../H_global.h>
#include <stdbool.h>
#include <stdint.h>

// ADT for a skip list node
// **** Assuming that the lanes: express and normal are sorted and is doubly-linked list ***** //

/*
what we actually need for the a skip node are: next, val, level
*/
typedef struct skip_node {
    struct skip_node *right;  // link to the right adjacent nodes
    struct skip_node *left; // link to the left adjacent nodes
    struct skip_node *below; // link to the each levels; level 0, node is linked to NULL
    // LENGTH level; // current level based on flipped coin
    LENGTH key; // acts as the index of the node
    // LENGTH val;  // integer value of a node
} node;

typedef struct skip_list {
    node *leftmost; // sentinel : INT_MIN
    node *rightmost; // seintinel : INT_MAX
    LENGTH maxHeight; 
    LENGTH size;
    bool is_reversed;
} list;

typedef struct doubly_linked_list { // tailRecord
    node **record;
    struct doubly_linked_list *prev; 
    struct doubly_linked_list *next;
} dlist; 


// Init
list *init();
list *make(LENGTH n, DATA *seq);

// Flags/List Info
void reverse(list *l);
LENGTH size(list *l);
bool empty(list *l);

// Getters/Setters
DATA get(list *l, LENGTH i);
void set(list *l, LENGTH i, DATA v);
DATA peek_left(list *l);
DATA peek_right(list *l);

// Insertions/Deletions
void push_left(list *l, DATA v);
void push_right(list *l, DATA v);
bool pop_left(list *l);
bool pop_right(list *l);

// Test Operations
void TEST_elements(list* l, LENGTH* nRef, DATA** seqRef);
bool TEST_internal(list* l);


#endif