#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>
#include "skip-list.h"

// initialize the nodes
node *makeNode(LENGTH key){
    node *newNode = malloc(sizeof(node));
    newNode->below = newNode->right = newNode->left = NULL; // initialize the pointers to NULL
    newNode->key = key; // set the key 
    return newNode; // return the created node
}

// initialize the skip-list
list *initSkipList(){
    list *skip_list = malloc(sizeof(list));
    node *sentinel_left = skip_list->leftmost = INT_MIN; // initially set the leftmost to the sentinel 
    node *sentinel_right = skip_list->rightmost = INT_MAX; // initially set the righmost to the sentinel 
    
    // link the sentinels 
    sentinel_left->right = sentinel_right;
    sentinel_right->left = sentinel_left;

    skip_list->is_reversed = false; // default : toggled off
    skip_list->maxHeight = 0; 
    skip_list->size = 0;
}

// HELPER

// Search
node *search(list *sl, LENGTH key){
    // start with the header
    node *header = sl->leftmost;
    
    // traverse through the header until key found
    while (header->below && key > header->key){ // when current traversal is not NULL 
        // seearch one level down
        header = header->below;
        // search one level right 
        while (header->right->key < key){
            header = header->right;
        }
        // if key is found 
        if (key == header->key){
            return header; // return the node
        }
    }
    return header;
}

node *insert(int key){
    
}



LENGTH main(){

}