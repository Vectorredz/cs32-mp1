#ifndef DLINKED_LIST_H
#define DLINKED_LIST_H

#include "../H_global.h"
#include <stdlib.h>
#include <inttypes.h>
#include <stdbool.h>

// ADT for a doubly-linked list node
typedef struct node{
    DATA val;
    struct node *left;
    struct node *right;
} node;

// ADT for a doubly-linked list 
typedef struct list{
    struct node *head;
    struct node *tail;
    LENGTH size;
    bool reversed;
} list;

// Helpers
DATA _get_node(list *l, node* current_node, LENGTH i, bool from_set, DATA v);
void push_left_base(list *l, node *current_node, DATA v);
void push_right_base(list *l, node *current_node, DATA v);
bool pop_left_base(list *l, node *current_node, node *store_node);
bool pop_right_base(list *l, node *current_node, node *store_node);
DATA _peek(list *l, bool from_right);

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