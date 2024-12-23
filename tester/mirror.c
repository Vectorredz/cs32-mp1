#include "H_mirror.h"
#include <stdlib.h>
#include <inttypes.h>
#include <stdbool.h>

// Init
Mirror* make(LENGTH n, DATA* seq){
    Mirror* list = (Mirror*) malloc(sizeof(Mirror));
    list->n = n;

    Node* currentNode = NULL;
    for (LENGTH i = 0; i < n; i++){
        Node* newNode = (Node*) malloc(sizeof(Node));
        newNode->data = seq[i];
        newNode->next = NULL;

        if (list->head == NULL){
            list->head = newNode;
        } else {
            currentNode->next = newNode;
        }
        currentNode = newNode;
    }

    return list;
}

// Flags/List Info
void reverse(Mirror* list){
    list->reverse = !(list->reverse);
}
LENGTH size(Mirror* list){
    return list->n;
}
bool empty(Mirror* list){
    return size(list) == 0;
}

// Getters/Setters
DATA get(Mirror* list, LENGTH i){
    Node* currentNode = list->head;
    LENGTH index = 0;
    while (currentNode != NULL){
        if (index == i){
            return currentNode->data;
        }
        index++;
        currentNode = currentNode->next;
    }
    return NULL;
}
void set(Mirror* list, LENGTH i, DATA v){
    Node* currentNode = list->head;
    LENGTH index = 0;
    while (currentNode != NULL){
        if (index == i){
            currentNode->data = v;
            return;
        }
        index++;
        currentNode = currentNode->next;
    }
    return;
}
DATA peek_left(Mirror* list){
    if (list->n == 0){
        return;
    }
    return list->head->data;
}
DATA peek_right(Mirror* list){
    if (list->n == 0){
        return;
    }
    Node* currentNode = list->head;
    while (currentNode->next != NULL){
        currentNode = currentNode->next;
    }
    return currentNode->data;
}

// Insertions/Deletions
void push_left(Mirror* list, DATA v){
    (list->n)++;

    Node* newNode = (Node*) malloc(sizeof(Node));
    newNode->data = v;
    newNode->next = list->head;
    list->head = newNode;
}
void push_right(Mirror* list, DATA v){
    (list->n)++;

    Node* newNode = (Node*) malloc(sizeof(Node));
    newNode->data = v;
    newNode->next = NULL;
    
    if (list->head == NULL){
        list->head = newNode;
    } else {
        Node* currentNode = list->head;
        while (currentNode->next != NULL){
            currentNode = currentNode->next;
        }
        currentNode->next = newNode;
    }
}
bool pop_left(Mirror* list){
    if (list->n == 0){
        return false;
    }
    list->head = list->head->next;
    (list->n)--;

    return true;
}
bool pop_right(Mirror* list){
    if (list->n == 0){
        return false;
    }
    (list->n)--;

    if (list->head->next == NULL){
        list->head = NULL;
    } else {
        Node* prevNode = list->head;
        Node* currentNode = prevNode->next;
        while (currentNode->next != NULL){
            prevNode = currentNode;
            currentNode = currentNode->next;
        }
        prevNode->next = NULL;
    }

    return true;
}


