// DOUBLY-LINKED LIST IMPLEMENTATION OF DYNAMIC LIST 
#include <stdio.h>
#include <stdlib.h>
#include "d_linked_list.h"

// Following are the OPERATIONS for the implementation

//*-------------------------------------------------------------*//

list *init(){
    list *l = malloc(sizeof(list));
    l->head = l->tail =  NULL;
    l->reversed = false;
    l->size = 0;
    return l;
}

list *make(LENGTH n, DATA *seq){
    // undefined behavior
    if (n < 0) return NULL;

    node *head = (node*)malloc(sizeof(node));
    list *ptr = init();
    node *curr = head;

    if (n == 0){
        curr->val = 0;
        curr->left = curr->right = NULL;
        ptr->head = ptr->tail = NULL;
        ptr->size = 0;
        return ptr;
    }
    ptr->head = head;
    ptr->size = n;
    curr->right = NULL;

    for (LENGTH i = 0; i <= n; i++){
        // if the index = 0, means that it is the first element
        if (i == 0){
            curr->val = seq[i];
            curr->left = NULL;
        }
        else {
            node *new = (node*)malloc(sizeof(node));
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

void push_left(list *l, DATA v){
    node *newNode = (node*)malloc(sizeof(node));
    newNode->val = v;
    newNode->left = newNode->right = NULL;
    node *curr = l->reversed ? l->head : l->tail;
    //empty_list case
    if (!(curr)) l->head = l->tail = newNode;
    else {
        if (!(l->reversed)){
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

void push_right(list *l, DATA v){
    node *newNode = (node*)malloc(sizeof(node));
    newNode->val = v;
    newNode->left = newNode->right = NULL;
    node *curr = l->reversed ? l->head : l->tail;
    //empty_list case
    if (!(curr)) l->head = l->tail = newNode;
    else {
        if (l->reversed){
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
    node *curr = !(l->reversed) ? l->head : l->tail;
    node *store = !(l->reversed) ? curr->right : curr->left; // if left, then store next
    // single_node case
    if ((l->tail) == (l->head)){
        l->head = l->tail = NULL;
        free(curr);
        // reduce the size
        (l->size)--;
        // free the node
        return true; 
    }
    else {
        if (!(l->reversed)){ // reversed toggled ON
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
    node *curr = l->reversed ? l->head : l->tail;
    node *store = l->reversed ? curr->right : curr->left; // if right, then store prev
    // single_node case
    if ((l->tail) == (l->head)){
        l->head = l->tail = NULL;
        free(curr);
        // reduce the size
        (l->size)--;
        // free the node
        return true; 
    }
    else {
        if (l->reversed){ // reversed toggled ON
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

DATA peek_left(list *l){
    // peek from an empty list
    if (!(l->head) && !(l->tail)){
        fprintf(stderr, "\nIndexError: Peek From Empty List\n");
        exit(1);\
    }
    node *curr = !(l->reversed) ? l->head : l->tail;
    return curr->val;
}

DATA peek_right(list *l){
    // peek from an empty list
    if (!(l->head) && !(l->tail)){
        fprintf(stderr, "\nIndexError: Peek From Empty List\n");
        exit(1);\
    }
    node *curr = (l->reversed) ? l->head : l->tail;
    return curr->val;
}

LENGTH size(list *l){
    if (l->size < 0){
        fprintf(stderr, "\nIndexError: invalid size\n");
        exit(1);\
    }
    return l->size;
}

bool empty(list *l){
    return (l->size == 0);
}

DATA get(list *l, LENGTH i){
    node *curr = l->reversed ? l->tail : l->head;
    // raise an exception
    if (i < 0){
        fprintf(stderr, "\nIndexError: list index out of range\n");\
        exit(1);\
    }
    if (i >= size(l)){
        fprintf(stderr, "\nIndexError: list index out of range\n");\
        exit(1);\
    }
    LENGTH j = 0;
    while (j != i){
        if (l->reversed){
            curr = curr->left; 
        }
        else {
            curr = curr->right; 
        }
        j++;
    }
    return curr->val;
}

void set(list *l, LENGTH i, DATA v){
    node *curr = l->reversed ? l->tail : l->head;
    if (i < 0){
        fprintf(stderr, "IndexError: list index out of range\n");\
        exit(1);\
    }
    if (i >= size(l)){
        fprintf(stderr, "IndexError: list index out of range\n");\
        exit(1);\
    }
    LENGTH j = 0;
    while (j != i){
        if (l->reversed){
            curr = curr->left; 
        }
        else {
            curr = curr->right; 
        }
        j++;
    }
    curr->val = v;

}

void reverse(list *l){
    // l->reversed = l->reversed == 0 ? 1 : 0;
    // l->reversed = !(l->reversed);
    if (l->reversed == 0){
        l->reversed = 1;
    }
    else {
        l->reversed = 0;
    }
}

void display(list *l){
    // if (l->size == 0) return;
    node *curr = l->head;
    printf("\n");
    while (curr){
        printf("%ld <-> ", curr->val);
        curr = curr->right;
    }
    printf("NULL \n");
}

// DATA *make_seq(LENGTH size, DATA *array){
//     for (LENGTH i = 0; i < size; i++){
//         array[i] = i;
//     }
//     return array;
// }


// --------------------------------------------------------- >>
/* ----------------------------------------- <<

            ||-- TEST OPERATIONS --||

<< ----------------------------------------- */
// --------------------------------------------------------- >>


void TEST_elements(list* l, LENGTH* nRef, DATA** seqRef){   
    LENGTH i = 0;
    list *dl = l;
    DATA *seq = malloc((dl->size) * sizeof(DATA*));
    node *curr = dl->head;
    while (curr->right){
        seq[i] = curr->val;
        curr = curr->right;
        i++;
    }
    *nRef = l->size;
    *seqRef = seq;
}

bool TEST_internal(list* l){
    return true;
}

DATA *make_seq(LENGTH size){
    DATA *array = malloc(size * sizeof(DATA));
    for (LENGTH i = 0; i < size; i++){
        array[i] = i;
    }
    return array;
}

LENGTH main(){
    LENGTH n = 5;
    DATA *array = make_seq(n);
    DATA *seq = malloc(n * sizeof(DATA*));
    list *l = make(n, array);
    TEST_elements(l,&n,&seq);
}


