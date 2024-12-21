#include <stdio.h>
#include <stdlib.h>
#include "dynamic_array.h"

list *build123(){
    list *node1 = malloc(sizeof(list));
    list *node2 = malloc(sizeof(list));

    node1->val = 0;
    node1->next = node2;
    node2->val = 1;
    node2->next = NULL;

    return node1;
}

void display(list *head){
    list *curr = malloc(sizeof(list));
    curr = head;
    while (curr){
        printf("%d -> ", curr->val);
        curr = curr->next;
    }
}


int main(){
    list *head = build123();
    display(head);
}