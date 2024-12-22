#include <stdio.h>
#include <stdlib.h>
#include "dynamic_array.h"

list *make(int n, int64_t *seq){
    node *head = malloc(sizeof(node));
    list *ptr = malloc(sizeof(list));
    node *curr = head;
    ptr->head = head;
    ptr->size = n;
    curr->right = NULL;
    if (n == 0){
        ptr = NULL;
        return ptr;
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

void push_left(list *l, int64_t v);
void push_right(list *l, int64_t v);
bool pop_left(list *l);
bool pop_right(list *l);

int64_t peek_left(list *l){
    return l->head->left;
}
int64_t peek_right(list *l){
    return l->head->right;
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

int64_t get(list *l, int i);

void set(list *l, int i, int64_t v);
void reverse(list *l);

void display(list *head){
    list *curr = head;

    while (curr->head){
        printf("%d <-> ", curr->head->val);
        curr->head = curr->head->right;
    }
}

int main(){
    int n = 5;
    int64_t *array = malloc(n * sizeof(int64_t));
    for (int i = 0; i < n; i++){
        array[i] = i;
    }
    list *head = make(n, array);
    display(head);
    int ret = size(head);
    printf("%d ", ret);

}