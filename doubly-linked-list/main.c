// DOUBLY-LINKED LIST IMPLEMENTATION OF DYNAMIC LIST 
#include <stdio.h>
#include <stdlib.h>
#include "dynamic_list.h"

// Following are the OPERATIONS for the implementation

//*-------------------------------------------------------------*//

list init(void){
    list l = {NULL, NULL, 0,0};
    return l;
}

list *make(int n, int64_t *seq){
    node *head = malloc(sizeof(node));
    list *ptr = malloc(sizeof(list));
    node *curr = head;
    ptr->head = head;
    ptr->size = n;
    curr->right = NULL;
    if (n == 0){
        return NULL;
    }
    else {
        for (int i = 0; i < n; i++){
            if (i == 0){
                curr->val = seq[i];
                curr->left = NULL;
            }
            else {
                node *new = malloc(sizeof(node));
                new->val = seq[i];
                curr->right = new;
                new->left = curr;
                new->right = NULL;
                curr = new;
            }
        }
        curr->right = NULL;
        ptr->tail = curr;
        return ptr;
    }
}

void push_left(list *l, int64_t v){
    node *newNode = malloc(sizeof(node));
    newNode->val = v;
    newNode->left = newNode->right = NULL;
    node *curr = l->is_reversed ? l->head : l->tail;
    //empty_list case
    if (!(curr)) l->head = l->tail = newNode;
    else {
        if (!(l->is_reversed)){
            newNode->right = l->head;
            l->head->left = newNode;
            l->head = newNode;
        }
        else {
            newNode->left = l->tail;
            l->tail->right = newNode;
            l->tail = newNode;
        }
    }
    (l->size)++;
}

void push_right(list *l, int64_t v){
    node *newNode = malloc(sizeof(node));
    newNode->val = v;
    newNode->left = newNode->right = NULL;
    node *curr = l->is_reversed ? l->head : l->tail;
    //empty_list case
    if (!(curr)) l->head = l->tail = newNode;
    else {
        if (l->is_reversed){
            newNode->right = l->head;
            l->head->left = newNode;
            l->head = newNode;
        }
        else {
            newNode->left = l->tail;
            l->tail->right = newNode;
            l->tail = newNode;
        }
    }
    (l->size)++;
}

bool pop_left(list *l){
    // empty_list
    if (!(l->head) && !(l->tail)){
        fprintf(stderr, "\nIndexError: Pop From Empty List\n");
        return false;
    };
    node *curr = !(l->is_reversed) ? l->head : l->tail;
    node *store = !(l->is_reversed) ? curr->right : curr->left;
    // single_node case
    if ((l->tail) == (l->head)){
        free(curr);
        l->head = l->tail = NULL;
        // reduce the size
        (l->size)--;
        // free the node
        return true; 
    }
    else {
        if (!(l->is_reversed)){ // is_reversed ON
            store->left = NULL;
            l->head = store;
        }
        else {
            store->right = NULL;
            l->tail = store;
        }
    }
    (l->size)--;
    free(curr);
    return true;
}

bool pop_right(list *l){
    // empty_list
    if (!(l->head) && !(l->tail)) return false;
    node *curr = l->is_reversed ? l->head : l->tail;
    node *store = l->is_reversed ? curr->right : curr->left;
    // single_node case
    if ((l->tail) == (l->head)){
        free(curr);
        l->head = l->tail = NULL;
        // reduce the size
        (l->size)--;
        // free the node
        return true; 
    }
    else {
        if (l->is_reversed){ // is_reversed ON
            store->left = NULL;
            l->head = store;
        }
        else {
            store->right = NULL;
            l->tail = store;
        }
    }
    (l->size)--;
    free(curr);
    return true;
}


int64_t peek_left(list *l){
    // peek from an empty list
    if (!(l->head) && !(l->tail)){
        fprintf(stderr, "\nIndexError: Peek From Empty List\n");
        return -1;
    }
    node *curr = !(l->is_reversed) ? l->head : l->tail;
    return curr->val;
}

int64_t peek_right(list *l){
    // peek from an empty list
    if (!(l->head) && !(l->tail)){
        fprintf(stderr, "\nIndexError: Peek From Empty List\n");
        return -1;
    }
    node *curr = (l->is_reversed) ? l->head : l->tail;
    return curr->val;
}

int size(list *l){
    return l->size;
}

bool empty(list *l){
    return (l->size == 0);
}

int64_t get(list *l, int i){
    node *curr = l->head;
    // raise an exception
    if (i < 0){
        fprintf(stderr, "IndexError: list index out of range\n");\
        exit(1);\
    }
    if (i >= size(l)){
        fprintf(stderr, "IndexError: list index out of range\n");\
        exit(1);\
    }
    int j = 0;
    while (j != i){
        curr = curr->right;
        j++;
    }
    return curr->val;
}

void set(list *l, int i, int64_t v){
    node *curr = l->head;
    if (i < 0){
        fprintf(stderr, "IndexError: list index out of range\n");\
        exit(1);\
    }
    if (i >= size(l)){
        fprintf(stderr, "IndexError: list index out of range\n");\
        exit(1);\
    }
    int j = 0;
    while (j != i){
        curr = curr->right;
        j++;
    }
    curr->val = v;

}

void reverse(list *l){
    // l->is_reversed = l->is_reversed == 0 ? 1 : 0;
    // l->is_reversed = !(l->is_reversed);
    if (l->is_reversed == 0){
        l->is_reversed = 1;
    }
    else {
        l->is_reversed = 0;
    }
}

void display(list *l){
    node *curr = l->head;
    printf("\n");
    while (curr){
        printf("%ld <-> ", curr->val);
        curr = curr->right;
    }
    printf("NULL \n");
}
