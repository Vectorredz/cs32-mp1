#ifndef DYNAMIC_LIST_H
#define DYNAMIC_LIST_H
#include <stdbool.h>
#include <stdint.h>

// ADT for a skip list node
// **** Assuming that the lanes: express and normal are sorted and is doubly-linked list ***** //

/*
what we actually need for the a skip node are: next, val, level
*/
typedef struct skip_node{
    int val; // integer value of a node
    struct skip_node *left; // link to the left adjacent node
    struct skip_node *right; // link to the right adjacent node
    struct skip_node *below; // link to the down level node
} node;
/*
what we actually need for the a skip list are: height, length and head
*/
typedef struct skip_list{
    int length; // horizontal size of the skip_list 
    int height; // vertical size of the skip_list 
    struct skip_node *head; // points to the head of each list
    bool is_reversed;
} list;


list init(void);
list *make(int n, int64_t *seq);
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