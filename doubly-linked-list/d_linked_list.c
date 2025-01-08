// DOUBLY-LINKED LIST IMPLEMENTATION OF DYNAMIC LIST 
#include <stdio.h>
#include <stdlib.h>
#include "d_linked_list.h"


/*
-----------------------------------------------
INITIALIZERS
-----------------------------------------------
*/

list *init(){
    list *l = (list*)malloc(sizeof(list));
    l->head = l->tail =  NULL;
    l->reversed = false;
    l->size = 0;
    return l;
}

/*
-----------------------------------------------
HELPERS
-----------------------------------------------
*/

DATA _get_node(list *l, node* current_node, LENGTH i, bool from_set, DATA v){
    LENGTH j = 0;
    while (j != i){
        if (l->reversed){
            current_node = current_node->left; 
        }
        else {
            current_node = current_node->right; 
        }
        j++;
    }
    if (from_set) current_node->val = v;
    return current_node->val;
}

void push_left_base(list *l, node *current_node, DATA v){
    node *new_node = (node*)malloc(sizeof(node));
    new_node->val = v;
    new_node->left = new_node->right = NULL;

    // empty-list case
    if (!(current_node)) l->head = l->tail = new_node;
    else{
        new_node->right = l->head;
        l->head->left = new_node;
        l->head = new_node;
    }
    // update size
    (l->size)++;

}

void push_right_base(list *l, node *current_node, DATA v){
    node *new_node = (node*)malloc(sizeof(node));
    new_node->val = v;
    new_node->left = new_node->right = NULL;

    // empty-list case
    if (!(current_node)) l->head = l->tail = new_node;
    else{
        new_node->left = l->tail;
        l->tail->right = new_node;
        l->tail = new_node;
    }
    // update size
    (l->size)++;

}

bool pop_left_base(list *l, node *current_node, node *store_node){
    // single-element case
    if ((l->tail) == (l->head)){
        l->head = l->tail = NULL;
    }
    else {
        store_node->left = NULL;
        l->head = store_node;
    }
    (l->size)--;
    free(current_node);
    return true;
}

bool pop_right_base(list *l, node *current_node, node *store_node){
    // single-element case
    if ((l->tail) == (l->head)){
        l->head = l->tail = NULL;
    }
    else {
        store_node->right = NULL;
        l->tail = store_node;
    }
    (l->size)--;
    free(current_node);
    return true;
}

DATA _peek(list *l, bool from_right){
    if (!(from_right)){
        return l->head->val;
    }
    else {
        return l->tail->val;
    }
}

/*
-----------------------------------------------
MAIN OPERATIONS
-----------------------------------------------
*/


list *make(LENGTH n, DATA *seq){
    // undefined behavior
    if (n < 0) return NULL;

    node *head = (node*)malloc(sizeof(node));
    list *ptr = init();
    node *current_node = head;

    // if empty, return an empty list
    if (n == 0){
        current_node->val = 0;
        current_node->left = current_node->right = NULL;
        ptr->head = ptr->tail = NULL;
        ptr->size = 0;
        return ptr;
    }

    ptr->head = head;
    ptr->size = n;
    current_node->right = NULL;

    // traverse through the sequence
    for (LENGTH i = 0; i < n; i++){
        // get the first element
        if (i == 0){
            current_node->val = seq[i];
            current_node->left = NULL;
        }
        else {
            node *new_node = (node*)malloc(sizeof(node));
            new_node->val = seq[i];
            current_node->right = new_node;
            new_node->left = current_node;
            new_node->right = NULL;
            current_node = new_node;
        }
    }
    current_node->right = NULL;
    ptr->tail = current_node;
    return ptr;
}

// Flags/List Info
void reverse(list *l){
    l->reversed = !(l->reversed);
}

LENGTH size(list *l){
    return l->size;
}

bool empty(list *l){
    return (l->size == 0);
}

// Getters/Setters
DATA get(list *l, LENGTH i){
    node *current_node = l->reversed ? l->tail : l->head;
    // raise an exception
    if (i >= size(l)){
        fprintf(stderr, "\nIndexError: list index out of range\n");
        exit(1);
    }
    return _get_node(l, current_node, i, false, 0);
}

void set(list *l, LENGTH i, DATA v){
    node *current_node = l->reversed ? l->tail : l->head;
    _get_node(l, current_node, i, true, v);

}

DATA peek_left(list *l){
    // empty-list
    if (!(l->head) && !(l->tail)){
        fprintf(stderr, "\nIndexError: Peek From Empty List\n");
        exit(1);
    }
    return l->reversed ? _peek(l, true) : _peek(l, false);
}

DATA peek_right(list *l){
    // empty-list
    if (!(l->head) && !(l->tail)){
        fprintf(stderr, "\nIndexError: Peek From Empty List\n");
        exit(1);
    }
    return l->reversed ? _peek(l, false) : _peek(l, true);
}

// Insertions/Deletions
void push_left(list *l, DATA v){
    node *current_node = l->reversed ? l->tail : l->head;
    l->reversed ? push_right_base(l, current_node, v) : push_left_base(l, current_node, v);
}

void push_right(list *l, DATA v){
    node *current_node = l->reversed ? l->head : l->tail;
    l->reversed ? push_left_base(l, current_node, v) : push_right_base(l, current_node, v);
}

bool pop_left(list *l){
    // empty-list
    if (!(l->head) && !(l->tail)) return false;
    node *current_node = l->reversed ? l->tail : l->head;
    node *store_node = l->reversed ? current_node->left : current_node->right;
    return l->reversed ? pop_right_base(l, current_node, store_node) : pop_left_base(l, current_node, store_node);
}

bool pop_right(list *l){
    // empty_list
    if (!(l->head) && !(l->tail)) return false;
    node *current_node = l->reversed ? l->head : l->tail;
    node *store_node = l->reversed ? current_node->right : current_node->left;
    return l->reversed ? pop_left_base(l, current_node, store_node) : pop_right_base(l, current_node, store_node);
}

// Print
void display(list *l){
    if (l->size == 0) return;
    node *current_node = l->head;
    printf("\n");
    while (current_node){
        printf("%ld <-> ", current_node->val);
        current_node = current_node->right;
    }
    printf("NULL \n");
}


/*
-----------------------------------------------
TEST OPERATIONS
-----------------------------------------------
*/


void TEST_elements(list* l, LENGTH* nRef, DATA** seqRef){   
    LENGTH i = 0;
    list *dl = l;
    DATA *seq = (DATA*)malloc((dl->size) * sizeof(DATA));

    node *current_node = dl->head;
    while (current_node){
        seq[i] = current_node->val;
        current_node = current_node->right;
        i++;
    }
    *nRef = l->size;
    *seqRef = seq;
}

bool TEST_internal(list* l){
    return true;
}

bool TEST_reversed(list* l){
  return (l->reversed);
}


