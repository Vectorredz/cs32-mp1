#include <stdbool.h>
#include <stdint.h>
#include <stdio.h> 
#include <stdlib.h>
#include "dynamic_list.h"

/*
Algorithm for deciding nodes level. Snippet from geeks-for-geeks

randomLevel()
lvl := 1
//random() that returns a random value in [0...1)
while random() < p and lvl < MaxLevel do
lvl := lvl + 1
return lvl

 */

// generate random level based on the probability and passed max height

void randomLevel(int height){
    float p = 1/2;
    int level = 0;
    float flip = rand() % 1; // random value between [0, 1);
    while (flip < p && level < height){ // flipped coin = [0, 1/2) ? tails : heads 
        level += 1;
    }
    return level;
}

// initialize an empty skip_list, set param as 0/NULL

list init(){
    list sl1 = {0,0,NULL};
    return sl1;
}

// create a single node 
node *createNode(int val){
    node *newNode = malloc(sizeof(node));
    newNode->val = val;
    // initialize the skip nodes as a disconnected node
    newNode->below = newNode->left = newNode->right = NULL;
    return newNode;
}

list *make(int n, int64_t *seq){
    // TODO
}


// display the skip_list from each levels
void display(list *sl1){
    node *curr = sl1->head;
    // for loop to traverse the depth of skip_list
    for (int i = 0; i < sl1->height; i++){
        curr = curr->below;
        printf(" | ");
        while (curr){
            printf(" %d <-> ", curr->val);
            curr = curr->right;
        }
    }
    // while loop to traverse the length of skip_list
}


int main(){

}