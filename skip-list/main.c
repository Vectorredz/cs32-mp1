#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>

node *makeNode(int key, int val){
    node *newNode = malloc(sizeof(node));
    newNode->key = key;
    newNode->val = val;
    newNode->left = newNode->right = newNode->below = NULL;
    return newNode;
}

static int randLevel(list *sl){ // O(h)
    float p = 1.0/2.0;
    int level = 0;
    float flip = ((float)rand()) / RAND_MAX; // random value between [0, 1);
    while (rand() < RAND_MAX /2 && level < sl->height){ // flipped coin = [0, 1/2) ? tails : heads 
        level += 1;
    }

    return level;
}

list *init() { // O(n)
    list *sl = malloc(sizeof(list));
    
    node *header = makeNode(INT_MIN, INT_MIN); // Sentinel value
    node *footer = makeNode(INT_MAX, INT_MAX); // Sentinel value

    header->level = 0;
    footer->below = 0;

    sl->header = header;
    sl->footer = footer;
    sl->height = 0;
    sl->length = 0;
    sl->is_reversed = false;

    return sl;
}

// Generate level 0: Doubly-linked list 

list *generateLevelZero(int n, int64_t *seq){ // O(n)

    node *head = (node*)malloc(sizeof(node)); // generate node for the linked-list
    list *sl0 = init(); // initialize the skip-list 
    sl0->height = (int)ceil(log2(n)); // init the max vertical length of the skip-list

    node *curr = head;
    curr->right = NULL;
    if (n == 0) return NULL;
    for (int i = 0; i < n; i++){
        if (i == 0){
            curr->key = i;
            curr->val = seq[i] * 10;
            // connect to the header
            curr->left = sl0->header;
            sl0->header->right = curr;
            // connect below to NULL
            curr->below = NULL;
        }
        else {
            node *new = (node*)malloc(sizeof(node));
            new->key = i;
            new->val = seq[i] * 10;
            curr->right = new;
            new->left = curr;
            new->right = NULL;
            // connect below to NULL
            curr->below = NULL;
            curr = new;
        }
    }
    curr->right = NULL;
    // connect to footer
    sl0->footer->left = curr;
    curr->right = sl0->footer;

    return sl0;
}

// General Levels > 0

list *generateSkipList(list *l0){ // O(n)
    list *sl = l0; // Level 0
    node *curr = sl->header->right; // first node of level 0; skipping the sentinel node

    // traverse the level 0 and generate whether current node will promote level
    while (curr != sl->footer){
        // flip coin on each node
        int new_level = randLevel(sl);
        curr->level = new_level; // holds the levels for that node;
        printf("(%d, %d) -> ", curr->key, curr->level);
        curr = curr->right;
    }

    curr = sl->header->right;
    int maxHeight = sl->height;
    int currHeight = 1;


    while (currHeight <=  maxHeight){ // O(h) = O(logn)
        list *newList = init();

        newList->header->below = sl->header;
        sl->header = newList->header;
        newList->height = maxHeight;
        node *prevHeader = newList->header;
        prevHeader->level = currHeight;
        node *curr = sl->header->below->right; // initally level 0
        while (curr != sl->footer){ // O(n)
            if (rand() < RAND_MAX / 2){ 
                node *newNode = makeNode(curr->key, curr->val);
                newNode->below = curr;
                prevHeader->right = newNode;
                newNode->left = prevHeader;
                prevHeader = newNode;
            }
            curr = curr->right;
        }
        /*
        >>>>>> Help: I cannot troubleshoot this, why doesn't this work? 

        while (curr != sl->footer){
            if (curr->level > 0){ 
                node *newNode = makeNode(curr->key, curr->val);
                newNode->below = curr;
                prevHeader->right = newNode;
                newNode->left = prevHeader;
                prevHeader = newNode;
            }
            (curr->level)--;
            curr = curr->right;
        }
        */
        prevHeader->right = newList->footer;
        newList->footer->left = prevHeader;
        sl = newList;
        currHeight++;
    }
    return sl;
}

list *make(int n, int64_t *seq){ // O(n log(n))
    list *sl0 = generateLevelZero(n, seq);
    list *sl = generateSkipList(sl0);
    return sl;
}

int64_t peek_left(list *l){
    node *curr = l->is_reversed ? l->footer :l->header;
    return l->header->right->val;
}
int64_t peek_right(list *l){
    return l->footer->left->val;
}

void display(list *sl) {
    node *currLevel = sl->header;

    for (int level = 0; level <= sl->height; level++){
        printf("Level %d: ", currLevel->level);
        node *curr = currLevel;
        while (curr && curr != sl->footer){
            printf("(%d, %d) <-> ", curr->key, curr->val);
            curr = curr->right;
        }
        printf("\n");
        currLevel = currLevel->below;
    }
}

int main() {
    srand(time(NULL)); 
    int n = 4;
    int64_t *array = malloc(n * sizeof(int64_t));
    for (int i = 0; i < n; i++){
        array[i] = i;
    }
    list *sl = make(n, array);
    display(sl);

    int64_t val = peek_right(sl);
    printf("%ld ", val);
}
