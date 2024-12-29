#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>
#include "skip-list.h"

#define UPPERBOUND 64


// initialize the nodes
node *makeNode(LENGTH key){
    node *newNode = (node*)malloc(sizeof(node));
    newNode->below = newNode->right = newNode->left = NULL; // initialize the pointers to NULL
    newNode->key = key; // set the key 
    return newNode; // return the created node
}

// initialize the skip-list
list *initSkipList(){
    list *skip_list = malloc(sizeof(list));

    skip_list->leftmost = skip_list->rightmost = NULL;

    // initialize the sentinels
    node *front = skip_list->front = malloc(sizeof(node));
    // node *rear = skip_list->rear = malloc(sizeof(node));
    front->key = INT_MIN;
    front->left = NULL;
    // rear->key = INT_MAX;
    // rear->right = NULL;

    // link the sentinels 
    front->right = NULL;
    front->below = NULL;
 
    skip_list->is_reversed = false; // default : toggled off
    skip_list->maxHeight = 0; 
    skip_list->size = 0;

    return skip_list;
}

// HELPERS

// Search
node *search(list *sl, LENGTH key){
    // start with the header
    node *header = sl->leftmost;
    
    // traverse through the header until key found
    while (header && key > header->key){ // when current traversal is not NULL 
        // search one level right 
        while (header->key < key){
            header = header->right;
        }
        // if key is found 
        if (key == header->key){
            return header; // return the node
        }
        // search one level down
        header = header->below;
    }
    return NULL; // key is not found
}

node *searchBottomMost(list *sl, LENGTH key, node **seqRef, LENGTH *sizeRef){
    node *header = sl->front; // topmost sentinel node
    *sizeRef = 0;
    while (header){
        while(header->right){
            header = header->right;
        }
        // track each levels and store the nodes
        seqRef[(*sizeRef)++] = header;
        header = header->below;
    }
    
}

// bool promoteLevel(){
//     return (rand() < (RAND_MAX / 2));
// }

void push_left(list *l, DATA v) {
    node *seqRef[64];
    LENGTH sizeRef = 0;
    searchBottomMost(l, v, seqRef, &sizeRef);

    node *belowNode = NULL;
    bool promote = true;

    while (promote && sizeRef > 0) {
        node *sentinel = seqRef[--sizeRef];
        node *newNode = makeNode(v);

        // Link horizontally
        newNode->right = sentinel->right;
        if (sentinel->right) {
            sentinel->right->left = newNode;
        }
        sentinel->right = newNode;
        newNode->left = sentinel;

        // Link vertically
        newNode->below = belowNode;
        belowNode = newNode;

        // Update leftmost if necessary
        if (l->leftmost == NULL || v < l->leftmost->key) {
            l->leftmost = newNode;
        }

        promote = ((rand() % 2) == 0);
    }

    // Add new level if needed
    if (promote) {
        node *newFront = malloc(sizeof(node));
        newFront->key = INT_MIN;
        node *newNode = makeNode(v);

        newFront->below = l->front;
        newFront->right = newNode;
        newFront->left = NULL;

        newNode->left = newFront;
        newNode->right = NULL;
        newNode->below = belowNode;

        l->front = newFront;
        l->maxHeight++;
    }

    l->size++;
}


void display(list* skiplist) {
    node* level = skiplist->front;
    while (level) {
        node* temp = level;
        while (temp) {
            printf("%d -> ", temp->key);
            temp = temp->right;
        }
        printf("\n");
        level = level->below;
    }
}

LENGTH main(){
    srand(time(NULL));

    // Initialize the skip list
    list *sl = initSkipList();

    push_left(sl, 0);
    push_left(sl, 1);
    // push_left(sl, 3);
    display(sl);

}