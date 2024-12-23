// DOUBLY-LINKED LIST IMPLEMENTATION OF DYNAMIC LIST 

#include <stdio.h>
#include <stdlib.h>
#include "dynamic_list.h"

// Following are the OPERATIONS for the implementation

//*-------------------------------------------------------------*//

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
    if (!(l->is_reversed)){
        node *curr = malloc(sizeof(node));
        curr->val = v;
        curr->right = curr->left = NULL;
        if (l->head == NULL){
            l->head = l->tail = curr;
        }
        else {
            curr->right = l->head;
            l->head->left = curr;
            l->head = curr;
        }
    }
    else { // When is_reversed is ON -> push_left operation is done instead
        node *curr = malloc(sizeof(node));
        curr->val = v;
        curr->left = curr->right = NULL;
        if (l->tail == NULL){ // out-of-bounds check
            l->head = l->tail = curr;
        }
        else {
            curr->left = l->tail;
            l->tail->right = curr;
            l->tail = curr;
        }
    }
   
}

void push_right(list *l, int64_t v){
    if (!(l->is_reversed)){
        node *curr = malloc(sizeof(node));
        curr->val = v;
        curr->left = curr->right = NULL;
        if (l->tail == NULL){ // out-of-bounds check
            l->head = l->tail = curr;
        }
        else {
            curr->left = l->tail;
            l->tail->right = curr;
            l->tail = curr;
        }
    }
    else { // When is_reversed is ON -> push_left operation is done instead
        node *curr = malloc(sizeof(node));
        curr->val = v;
        curr->right = curr->left = NULL;
        if (l->head == NULL){
            l->head = l->tail = curr;
        }
        else {
            curr->right = l->head;
            l->head->left = curr;
            l->head = curr;
        }
    }
   
}

bool pop_left(list *l){
    if (!(l->is_reversed)){
        node *curr = l->head;
        node *next = curr->right;
        if (l->head == NULL && l->tail == NULL){ // out-of-bounds check
            return false;
        }
        if (l->head->right == NULL && l->head->left == NULL && l->tail->right == NULL && l->tail->left == NULL){ 
            l->tail = l->head = NULL;
            return false;
        }
        else{
            curr->right = NULL;
            next->left = NULL;
            l->head = next;
            return true;
        }
    }
    else { // When is_reversed is ON -> pop_right operation is done instead
        node *curr = l->tail;
        node *prev = curr->left;
        if (l->head == NULL && l->tail == NULL){
            return false;
        }
        if (l->head->right == NULL && l->head->left == NULL && l->tail->right == NULL && l->tail->left == NULL){
            l->tail = l->head = NULL;
            return false;
        }
        else{
            curr->left = NULL;
            prev->right = NULL;
            l->tail = prev;
            return true;
        }
    }
}

bool pop_right(list *l){
    if (!(l->is_reversed)){
        node *curr = l->tail;
        node *prev = curr->left;
        if (l->head == NULL && l->tail == NULL){
            return false;
        }
        if (l->head->right == NULL && l->head->left == NULL && l->tail->right == NULL && l->tail->left == NULL){
            l->tail = l->head = NULL;
            return false;
        }
        else{
            curr->left = NULL;
            prev->right = NULL;
            l->tail = prev;
            return true;
        }
    }
    else { // When is_reversed is ON -> pop_left operation is done instead
        node *curr = l->head;
        node *next = curr->right;
        if (l->head == NULL && l->tail == NULL){ // out-of-bounds check
            return false;
        }
        if (l->head->right == NULL && l->head->left == NULL && l->tail->right == NULL && l->tail->left == NULL){ 
            l->tail = l->head = NULL;
            return false;
        }
        else{
            curr->right = NULL;
            next->left = NULL;
            l->head = next;
            return true;
        }
    }
}

int64_t peek_left(list *l){
    if (!(l->is_reversed)){
        node *curr = l->head;
        return curr->val;
    }
    else { // When is_reversed is ON -> peek_right operation is done instead
        node *curr = l->tail;
        return curr->val;
    }
    
}
int64_t peek_right(list *l){
    if (!(l->is_reversed)){
        node *curr = l->tail;
        return curr->val;
    }
    else { // When is_reversed is ON -> peek_left operation is done instead
        node *curr = l->tail;
        return curr->val;
    }

}

int size(list *l){
    return l->size;
}

bool empty(list *l){
    if (!(size(l))){
        return true;
    }
    else {
        return false;
    }
}

int64_t get(list *l, int i){
    node *curr = l->head;
    if (i < 0){
        return 0;
    }
    if (i >= size(l)){
        return 0;
    }
    int j = 0;
    while (j != i-1){
        curr = curr->right;
        j++;
    }
    return curr->val;
}

void set(list *l, int i, int64_t v){
    node *curr = l->head;
    if (i < 0){
        return;
    }
    if (i >= size(l)){
        return;
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
    if (l->is_reversed == 0){
        l->is_reversed = 1;
    }
    else {
        l->is_reversed = 0;
    }
}

void display(list *l){
    node *curr = l->head;
    while (curr){
        printf("%ld <-> ", curr->val);
        curr = curr->right;
    }
    printf("NULL\n");
}

int main(){
    int n = 1;
    int64_t *array = malloc(n * sizeof(int64_t));
    for (int i = 0; i < n; i++){
        array[i] = i;
    }
    list *head = make(n, array);
}