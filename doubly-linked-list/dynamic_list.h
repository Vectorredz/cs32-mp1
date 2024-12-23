#ifndef DYNAMIC_LIST_H
#define DYNAMIC_LIST_H
#include <stdbool.h>
#include <stdint.h>

// ADT for a doubly-linked list node
typedef struct node{
    int64_t val;
    struct node*left;
    struct node *right;
} node;

// ADT for a doubly-linked list 
typedef struct list{
    struct node *head;
    struct node *tail;
    int size;
    bool reversed;
} list;

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