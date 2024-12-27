#ifndef DYNAMIC_LIST_H
#define DYNAMIC_LIST_H
#include <stdbool.h>
#include <stdint.h>

// ADT for a skip list node
// **** Assuming that the lanes: express and normal are sorted and is doubly-linked list ***** //

/*
what we actually need for the a skip node are: next, val, level
*/
typedef struct skip_node {
    int level; // current level based on flipped coin
    int key; // acts as the index of the node
    int val;  // integer value of a node
    struct skip_node *right;  // link to the right adjacent nodes
    struct skip_node *left; // link to the left adjacent nodes
    struct skip_node *below; // link to the each levels; level 0, node is linked to NULL
} node;

typedef struct skip_list {
    node *sentinel;
    node *leftmost;
    node *rightmost;
    int maxHeight;
    int size;
    bool is_reversed;
} list;

typedef struct doubly_linked_list { // tailRecord
    node **record;
    struct doubly_linked_list *prev; 
    struct doubly_linked_list *next;
} dlist; 


list *make(int n, int64_t *seq); // done
void push_left(list *l, int64_t v);
void push_right(list *l, int64_t v);
bool pop_left(list *l);
bool pop_right(list *l);
int64_t peek_left(list *l);
int64_t peek_right(list *l);
int size(list *l);
bool empty(list *l);
int64_t get(list *l, int i);
void set(list *l, int i, int64_t v);
void reverse(list *l);

#endif